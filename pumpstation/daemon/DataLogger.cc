/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <iostream>

#include <QApplication>
#include <QMutexLocker>
#include <QProcess>
#include <QXmlStreamWriter>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "DataLogger.h"

int DataLogger::sighupFd[2];
int DataLogger::sigintFd[2];
int DataLogger::sigtermFd[2];

DataLogger::DataLogger(PumpStationModel* model,
		                   CommunicationThread* thread,
                       QObject *parent)
 : QObject(parent),
   model_(model),
   thread_(thread),
   isStreaming_(false),
   ofile_(0),
   stream_(0)
{
  connect(model_, SIGNAL(dataValid()),
          this, SLOT(initialize()));
}

void DataLogger::initialize()
{
  NQLog("DataLogger") << "initialize";

  if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sighupFd))
    qFatal("Couldn't create HUP socketpair");

  if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigintFd))
    qFatal("Couldn't create HUP socketpair");

  if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigtermFd))
    qFatal("Couldn't create TERM socketpair");

  snHup = new QSocketNotifier(sighupFd[1], QSocketNotifier::Read, this);
  connect(snHup, SIGNAL(activated(int)), this, SLOT(handleSigHup()));
  snInt = new QSocketNotifier(sigintFd[1], QSocketNotifier::Read, this);
  connect(snInt, SIGNAL(activated(int)), this, SLOT(handleSigInt()));
  snTerm = new QSocketNotifier(sigtermFd[1], QSocketNotifier::Read, this);
  connect(snTerm, SIGNAL(activated(int)), this, SLOT(handleSigTerm()));

  connect(model_, SIGNAL(switchStateChanged(int, State)),
          this, SLOT(switchStateChanged(int, State)));

  connect(model_, SIGNAL(pressureChanged(int,double)),
          this, SLOT(pressureChanged(int,double)));

  restartTimer_ = new QTimer();
  connect(restartTimer_, SIGNAL(timeout()),
          this, SLOT(checkRestart()));

  statusTimer_ = new QTimer();
  connect(statusTimer_, SIGNAL(timeout()),
          this, SLOT(writeStatus()));
  
  start();
}

void DataLogger::start()
{
  if (isStreaming_) return;

  NQLog("DataLogger") << "start";
 
  QDateTime dt = QDateTime::currentDateTime();

  ApplicationConfig* config = ApplicationConfig::instance();
  QString dataPath(config->getValue<std::string>("DataPath").c_str());

  currentDir_.setPath(dataPath);

  QString filename("pumpstation_%1_%2.xml");
  filename = filename.arg(dt.toString("yyyyMMdd"));
  int i = 1;
  while (currentDir_.exists(filename.arg(i, 2, 10, QChar('0')))) ++i;
  filename = filename.arg(i, 2, 10, QChar('0'));

  ofilename_ = currentDir_.absoluteFilePath(filename);
  ofile_ = new QFile(ofilename_);

  NQLog("logger") << "streaming to file " << ofilename_.toStdString();

  if (ofile_->open(QFile::WriteOnly | QFile::Truncate)) {
    /*
    QProcess process;
    QStringList args;
    args << "fhlabs";
    args << ofilename_;
    if (process.execute("chgrp", args)!=0) {
      NQLog("logger", NQLog::Fatal) << "could not change group of output file to 'fhlabs'";
    } else {
      args.clear();
      args << "g+rw";
      args << ofilename_;
      if (process.execute("chmod", args)!=0) {
        NQLog("logger", NQLog::Fatal) << "could not change write permission of output file to 'fhlabs'";
    }
    */
  }

  //stream_ = new QTextStream(ofile_);

  xml_ = new QXmlStreamWriter(ofile_);
  xml_->setAutoFormatting(true);

  isStreaming_ = true;

  fileDateTime_ = dt;

  QString buffer;
  xml_->writeStartDocument();

  xml_->writeStartElement("PumpStationLog");
  xml_->writeAttribute("time", dt.toString(Qt::ISODate));

  writeStatus();

  restartTimer_->start(60000);
  statusTimer_->start(30000);
}

void DataLogger::stop()
{
  if (!isStreaming_) return;

  NQLog("DataLogger") << "stop";
 
  statusTimer_->stop();
  restartTimer_->stop();

  writeStatus();

  xml_->writeEndElement();
  xml_->writeEndDocument();

  //stream_->device()->close();
  delete ofile_;
  delete xml_;
  //delete stream_;

  isStreaming_ = false;
}

void DataLogger::checkRestart()
{
  if (!isStreaming_) return;

  QDateTime utime = QDateTime::currentDateTime();

  if (fileDateTime_.secsTo(utime)<60*60) return;

  ApplicationConfig * config = ApplicationConfig::instance();
  config->safe(std::string(Config::CMSTkModLabBasePath) + "/pumpstation/pumpstation.cfg");

  stop();
  start();
}

void DataLogger::writeStatus()
{
  QMutexLocker locker(&mutex_);

  QDateTime utime = QDateTime::currentDateTime();

  xml_->writeStartElement("Status");
  xml_->writeAttribute("time", utime.toString(Qt::ISODate));

  for (int i=0;i<5;++i) {
    xml_->writeStartElement("ConradSwitch");
    xml_->writeAttribute("id", QString::number(i));
    xml_->writeAttribute("state", QString::number((int)model_->getSwitchState(i)));
    xml_->writeEndElement();
  }

  for (int i=1;i<4;++i) {
    xml_->writeStartElement("LeyboldGraphixThree");
    xml_->writeAttribute("id", QString::number(i));
    xml_->writeAttribute("status", QString::number(model_->getSensorStatus(i)));
    xml_->writeAttribute("p", QString::number(model_->getPressure(i), 'e', 6));
    xml_->writeEndElement();
  }

  xml_->writeEndElement();

  ofile_->flush();
}

void DataLogger::switchStateChanged(int device, State newState)
{
  if (newState!=OFF && newState!=READY) return;

  QMutexLocker locker(&mutex_);

  NQLogDebug("logger") << "void DataLogger::switchStateChanged(" << device << ", " << (int)newState << ")";

  QDateTime utime = QDateTime::currentDateTime();

  xml_->writeStartElement("ConradSwitch");
  xml_->writeAttribute("time", utime.toString(Qt::ISODate));
  xml_->writeAttribute("id", QString::number(device));
  xml_->writeAttribute("state", QString::number((int)newState));
  xml_->writeEndElement();

  ofile_->flush();
}

void DataLogger::pressureChanged(int sensor, double p)
{
  QMutexLocker locker(&mutex_);

  NQLogDebug("logger") << "void DataLogger::pressureChanged(" << sensor << ", " << p << ")";

  QDateTime utime = QDateTime::currentDateTime();

  xml_->writeStartElement("LeyboldGraphixThree");
  xml_->writeAttribute("time", utime.toString(Qt::ISODate));
  xml_->writeAttribute("id", QString::number(sensor));
  xml_->writeAttribute("status", QString::number(model_->getSensorStatus(sensor)));
  xml_->writeAttribute("p", QString::number(p, 'e', 6));
  xml_->writeEndElement();

  ofile_->flush();
}

void DataLogger::writeToStream(QString& buffer)
{
  if (buffer.startsWith("\n")) {
      buffer.remove(0, 1);
  }

  if (isStreaming_ && buffer.length()>0) {
    *stream_ << buffer << "\n";
    stream_->flush();
  }
}

void DataLogger::hupSignalHandler(int)
{
    char a = 1;
    ::write(sighupFd[0], &a, sizeof(a));
}

void DataLogger::intSignalHandler(int)
{
    char a = 1;
    ::write(sigintFd[0], &a, sizeof(a));
}

void DataLogger::termSignalHandler(int)
{
    char a = 1;
    ::write(sigtermFd[0], &a, sizeof(a));
}

void DataLogger::handleSigHup()
{
    snHup->setEnabled(false);
    char tmp;
    ::read(sighupFd[1], &tmp, sizeof(tmp));

    // std::cout << "handleSigHup()" << std::endl;

    snHup->setEnabled(true);
}

void DataLogger::handleSigInt()
{
    snInt->setEnabled(false);
    char tmp;
    ::read(sigintFd[1], &tmp, sizeof(tmp));

    // std::cout << "handleSigInt()" << std::endl;

    stop();

    thread_->quit();
    thread_->wait();

    ApplicationConfig * config = ApplicationConfig::instance();
    config->safe(std::string(Config::CMSTkModLabBasePath) + "/pumpstation/pumpstation.cfg");

    QCoreApplication::exit(0);

    snInt->setEnabled(true);
}

void DataLogger::handleSigTerm()
{
    snTerm->setEnabled(false);
    char tmp;
    ::read(sigtermFd[1], &tmp, sizeof(tmp));

    // std::cout << "handleSigTerm()" << std::endl;

    snTerm->setEnabled(true);
}
