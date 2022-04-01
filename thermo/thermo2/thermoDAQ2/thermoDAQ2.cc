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

#include "Thermo2MainWindow.h"
#include "TestWindow.h"

//static const char* thermoDAQ2GUID = "{2F9BC7D7-44A2-4625-A7C6-2EBE3C27C7F5}";
//#define SINGLETON 1

int main( int argc, char** argv )
{
#ifdef SINGLETON
  SingletonApplication app(argc, argv, thermoDAQ2GUID);
  if(!app.lock()){
    NQLogMessage("thermoDAQ2") << "Application instance already running!";
    exit(1);
  }
#else
  QApplication app( argc, argv );
#endif

  QApplication::setApplicationName("thermoDAQ2");
  QCoreApplication::setApplicationVersion(APPLICATIONVERSIONSTR);

  QCommandLineParser parser;
  parser.addHelpOption();

  parser.addOption(QCommandLineOption("spam" , "Set log level to spam."));
  parser.addOption(QCommandLineOption("debug" , "Set log level to debug."));
  parser.addOption(QCommandLineOption("display" , "Autostart display application."));

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
  QString logfilename("thermoDAQ2_%1-%2.log");
  logfilename = logfilename.arg(dt.toString("yyyyMMdd"));

  int i = 1;
  while (dir.exists(logfilename.arg(i))) ++i;
  logfilename = logfilename.arg(i);

  logfilename = dir.absoluteFilePath(logfilename);

  NQLogMessage("thermoDAQ2") << "version " << APPLICATIONVERSIONSTR;
 
  NQLogMessage("thermoDAQ2") << "using " << logfilename << " for logging";

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

  qRegisterMetaType<State>("State");

  app.setStyle("cleanlooks");

  if (parser.isSet("display")) {
    QStringList arguments;
    if (parser.isSet("spam")) arguments << "--spam";
    if (parser.isSet("debug")) arguments << "--debug";
    std::string thermoDisplay = std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo2/thermoDisplay2/thermoDisplay2";
    QProcess myProcess(&app);
    myProcess.start(thermoDisplay.c_str(), arguments);
  }

  ApplicationConfig::instance();
  ApplicationConfig::instance()->append(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo2/thermo2.cfg", "main");

  //TestWindow mainWindow;
  Thermo2MainWindow mainWindow;

  mainWindow.show();

  return app.exec();
}
