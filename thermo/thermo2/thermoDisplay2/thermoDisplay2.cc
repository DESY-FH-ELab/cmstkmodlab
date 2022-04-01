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

#include <QApplication>
#include <QCommandLineParser>
#include <QTcpSocket>
#include <QProcess>
#include <QFile>
#include <QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include <nqlogger.h>

#include <SingletonApplication.h>
#include <ApplicationConfig.h>

#include "ThermoDisplay2MainWindow.h"
#include "ThermoDAQ2NetworkReader.h"
#include "ThermoDAQ2Client.h"

// static const char* thermoDisplay2GUID = "{413A4B5E-EEAD-44E5-8195-4106D8C1A2DD}";
// #define SINGLETON 1

int main( int argc, char** argv )
{
#ifdef SINGLETON
  SingletonApplication app(argc, argv, thermoDisplay2GUID);
  if(!app.lock()){
    NQLogMessage("thermoDisplay2") << "Application instance already running!";
    exit(1);
  }
#else
  QApplication app( argc, argv );
#endif

  QApplication::setApplicationName("thermoDisplay2");
  QCoreApplication::setApplicationVersion(APPLICATIONVERSIONSTR);

  QCommandLineParser parser;
  parser.addHelpOption();

  parser.addOption(QCommandLineOption("spam" , "Set log level to spam."));
  parser.addOption(QCommandLineOption("debug" , "Set log level to debug."));
  parser.addOption(QCommandLineOption("nogui" ,
      "Don't startup display application\n"
      "Just read status over network."));

  parser.process(app);

  bool spamLogLevel = parser.isSet("spam");
  bool debugLogLevel = parser.isSet("debug");

  // make 'spam' log level the default for now
  if (!debugLogLevel) spamLogLevel = true;
  
  NQLogger::instance()->addActiveModule("*");
  if (debugLogLevel) {
    NQLogger::instance()->addDestiniation(stdout, NQLog::Debug);
  } else if (spamLogLevel) {
    NQLogger::instance()->addDestiniation(stdout, NQLog::Spam);
  } else {
    NQLogger::instance()->addDestiniation(stdout, NQLog::Message);
  }

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QString logdir = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
  QString logdir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
  QDir dir(logdir);
  if (!dir.exists()) dir.mkpath(".");

  QDateTime dt = QDateTime::currentDateTime().toUTC();
  QString logfilename("thermoDisplay2_%1-%2.log");
  logfilename = logfilename.arg(dt.toString("yyyyMMdd"));

  int i = 1;
  while (dir.exists(logfilename.arg(i))) ++i;
  logfilename = logfilename.arg(i);

  logfilename = dir.absoluteFilePath(logfilename);

  NQLogMessage("thermoDisplay2") << "version " << APPLICATIONVERSIONSTR;

  NQLogMessage("thermoDisplay2") << "using " << logfilename << " for logging";

  QFile * logfile = new QFile(logfilename);
  if (logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
    if (parser.isSet("debug")) {
      NQLogger::instance()->addDestiniation(logfile, NQLog::Debug);
    } else if (parser.isSet("spam")) {
      NQLogger::instance()->addDestiniation(logfile, NQLog::Spam);
    } else {
      NQLogger::instance()->addDestiniation(logfile, NQLog::Message);
    }
  }

  app.setStyle("cleanlooks");

  ApplicationConfig::instance();
  ApplicationConfig::instance()->append(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo2/thermo2.cfg", "main");

  if (parser.isSet("nogui")) {
    ThermoDAQ2Client * client = new ThermoDAQ2Client(ApplicationConfig::instance()->getValue<unsigned int>("main", "ServerPort"));
    ThermoDAQ2NetworkReader * reader = new ThermoDAQ2NetworkReader(&app);

    QObject::connect(client, SIGNAL(handleMessage(QString&)),
        reader, SLOT(run(QString&)));
    QObject::connect(reader, SIGNAL(finished()),
        &app, SLOT(quit()));

    client->readDAQStatus();
  } else {
    ThermoDisplay2MainWindow * mainWindow = new ThermoDisplay2MainWindow();
    mainWindow->show();
  }

  return app.exec();
}
