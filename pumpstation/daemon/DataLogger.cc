#include <iostream>

#include <QMutexLocker>
#include <QProcess>
#include <QXmlStreamWriter>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "DataLogger.h"

DataLogger::DataLogger(PumpStationModel* model,
                       QObject *parent)
 : QObject(parent),
   model_(model),
   isStreaming_(false),
   ofile_(0),
   stream_(0)
{
  connect(model_, SIGNAL(switchStateChanged(int, State)),
          this, SLOT(switchStateChanged(int, State)));

  connect(model_, SIGNAL(pressureChanged(int,double)),
          this, SLOT(pressureChanged(int,double)));

  restartTimer_ = new QTimer();
  connect(restartTimer_, SIGNAL(timeout()),
          this, SLOT(checkRestart()));
}

void DataLogger::start()
{
  if (isStreaming_) return;

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

  stream_ = new QTextStream(ofile_);

  isStreaming_ = true;

  fileDateTime_ = dt;

  writeStatus();

  restartTimer_->start(60000);
}

void DataLogger::stop()
{
  if (!isStreaming_) return;

  restartTimer_->stop();

  writeStatus();

  stream_->device()->close();
  delete ofile_;
  delete stream_;

  isStreaming_ = false;
}

void DataLogger::checkRestart()
{
  if (!isStreaming_) return;

  QDateTime utime = QDateTime::currentDateTime();

  if (fileDateTime_.secsTo(utime)<60*60) return;

  stop();
  start();
}

void DataLogger::writeStatus()
{
  QMutexLocker locker(&mutex_);

  QDateTime utime = QDateTime::currentDateTime();

  QString buffer;
  QXmlStreamWriter xml(&buffer);
  xml.setAutoFormatting(true);

  for (int i=0;i<5;++i) {
    State state = model_->getSwitchState(i);

    xml.writeStartElement("ConradSwitch");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("id", QString::number(i));
    xml.writeAttribute("state", QString::number((int)state));
    xml.writeEndElement();
  }

  for (int i=0;i<3;++i) {
    double p = model_->getPressure(i);

    xml.writeStartElement("LeyboldGraphixThree");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("id", QString::number(i));
    xml.writeAttribute("p", QString::number(p, 'e', 6));
    xml.writeEndElement();
  }

  writeToStream(buffer);
}

void DataLogger::switchStateChanged(int device, State newState)
{
  if (newState!=OFF && newState!=READY) return;

  QMutexLocker locker(&mutex_);

  NQLogMessage("logger") << "void DataLogger::switchStateChanged(" << device << ", " << (int)newState << ")";

  QDateTime utime = QDateTime::currentDateTime();

  QString buffer;
  QXmlStreamWriter xml(&buffer);
  xml.setAutoFormatting(true);

  xml.writeStartElement("ConradSwitch");
  xml.writeAttribute("time", utime.toString(Qt::ISODate));
  xml.writeAttribute("id", QString::number(device));
  xml.writeAttribute("state", QString::number((int)newState));
  xml.writeEndElement();

  writeToStream(buffer);
}

void DataLogger::pressureChanged(int channel, double p)
{
  QMutexLocker locker(&mutex_);

  NQLogMessage("logger") << "void DataLogger::pressureChanged(" << channel << ", " << p << ")";

  QDateTime utime = QDateTime::currentDateTime();

  QString buffer;
  QXmlStreamWriter xml(&buffer);
  xml.setAutoFormatting(true);

  xml.writeStartElement("LeyboldGraphixThree");
  xml.writeAttribute("time", utime.toString(Qt::ISODate));
  xml.writeAttribute("id", QString::number(channel));
  xml.writeAttribute("p", QString::number(p, 'e', 6));
  xml.writeEndElement();

  writeToStream(buffer);
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
