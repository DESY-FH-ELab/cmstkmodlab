/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <QApplication>
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

#include "SingletonApplication.h"
#include "ApplicationConfig.h"

#include "ThermoDisplay2MainWindow.h"
#include "ThermoDAQ2NetworkReader.h"
#include "ThermoDAQ2Client.h"

// static const char* thermoDisplay2GUID = "{413A4B5E-EEAD-44E5-8195-4106D8C1A2DD}";
// #define SINGLETON 1

int main( int argc, char** argv )
{
  NQLogger::instance()->addActiveModule("*");
  NQLogger::instance()->addDestiniation(stdout, NQLog::Debug);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QString logdir = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
  QString logdir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
  QDir dir(logdir);
  if (!dir.exists()) dir.mkpath(".");
  QString logfilename = logdir + "/thermoDisplay2.log";

  NQLogMessage("thermoDisplay2") << "version " << APPLICATIONVERSIONSTR;

  NQLogMessage("thermoDisplay2") << "using " << logfilename << " for logging";

  QFile * logfile = new QFile(logfilename);
  if (logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
    NQLogger::instance()->addDestiniation(logfile, NQLog::Debug);
  }

#ifdef SINGLETON
  SingletonApplication app(argc, argv, thermoDisplay2GUID);
  if(!app.lock()){
    NQLogMessage("thermoDisplay2") << "Application instance already running!";
    exit(1);
  }
#else
  QApplication app( argc, argv );
#endif

  app.setStyle("cleanlooks");

  ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo2/thermo2.cfg");

  if (app.arguments().size()==2 &&
      app.arguments().at(1)=="--nogui") {

    ThermoDAQ2Client * client = new ThermoDAQ2Client(ApplicationConfig::instance()->getValue<unsigned int>("ServerPort"));
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
