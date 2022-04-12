/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <QDateTime>
#include <QProcess>

#include <nqlogger.h>

#include "ApplicationConfig.h"
#include "Thermo2DAQStreamer.h"
#include "Thermo2DAQModel.h"

Thermo2DAQStreamer::Thermo2DAQStreamer(Thermo2DAQModel* model,
                                       QObject* parent)
  : QObject(parent),
    model_(model),
    isStreaming_(false),
    ofile_(0),
    stream_(0)
{
  connect(model_, SIGNAL(daqStateChanged(bool)),
          this, SLOT(daqStateChanged(bool)));
  connect(model_, SIGNAL(daqMessage(QString)),
          this, SLOT(handleDAQMessage(QString)));
}

void Thermo2DAQStreamer::handleDAQMessage(const QString& message)
{
  QString buffer = message;
  if (buffer.startsWith("\n")) {
    buffer.remove(0, 1);
  }

  if (isStreaming_ && buffer.length()>0) {
    *stream_ << buffer << "\n";
    stream_->flush();
  }
}

void Thermo2DAQStreamer::daqStateChanged(bool state)
{
  if (state==true) {
    QDateTime dt = QDateTime::currentDateTime().toUTC();

    ApplicationConfig* config = ApplicationConfig::instance();
    QString dataPath(config->getValue<std::string>("main", "DataPath").c_str());
    QString dataGroup(config->getValue<std::string>("main", "DataGroup").c_str());

    QString measurementDirPath(dataPath + "/%1");
    currentDir_.setPath(measurementDirPath.arg(dt.toString("yyyyMMdd")));
    if (!currentDir_.exists()) {
      currentDir_.mkpath(currentDir_.absolutePath());
      QProcess process;
      QStringList args;
      args << dataGroup;
      args << currentDir_.absolutePath();
      if (process.execute("chgrp", args)!=0) {
        NQLogFatal("ThermoDAQStreamer") << "could not change group of output directory to " << dataGroup;
      } else {
        args.clear();
        args << "g+rwx";
        args << currentDir_.absolutePath();
        if (process.execute("chmod", args)!=0) {
          NQLogFatal("ThermoDAQStreamer") << "could not change write permission of output directory to 'g+rwx'";
        }
      }
    }

    QString filename("%1-%2.xml");
    filename = filename.arg(dt.toString("yyyyMMdd"));
    int i = 1;
    while (currentDir_.exists(filename.arg(i))) ++i;
    filename = filename.arg(i);

    ofilename_ = currentDir_.absoluteFilePath(filename);
    ofile_ = new QFile(ofilename_);
    if (ofile_->open(QFile::WriteOnly | QFile::Truncate)) {
      QProcess process;
      QStringList args;
      args << dataGroup;
      args << ofilename_;
      if (process.execute("chgrp", args)!=0) {
        NQLogFatal("ThermoDAQStreamer") << "could not change group of output file to " << dataGroup;
      } else {
        args.clear();
        args << "g+rw";
        args << ofilename_;
        if (process.execute("chmod", args)!=0) {
          NQLogFatal("ThermoDAQStreamer") << "could not change write permission of output file to 'g+rw'";
        }
      }
      stream_ = new QTextStream(ofile_);
    }
  } else {
    stream_->device()->close();
    delete ofile_;
    delete stream_;
  }

  isStreaming_ = state;
}
