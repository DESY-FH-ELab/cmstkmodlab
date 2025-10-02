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

#include <QApplication>
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

#include "ThermoMainWindow.h"
#include "TestWindow.h"

static const char* thermoDAQGUID = "{2F9BC7D7-44A2-4625-A7C6-2EBE3C27C7F5}";
//#define SINGLETON 1
//#define AUTOSTARTTHERMODISPLAY 1

int main( int argc, char** argv )
{
    NQLogger::instance()->addActiveModule("*");
    NQLogger::instance()->addDestiniation(stdout, NQLog::Spam);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString logdir = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
    QString logdir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
    QDir dir(logdir);
    if (!dir.exists()) dir.mkpath(".");
    QString logfilename = logdir + "/thermoDAQ.log";

    NQLog("thermoDAQ") << "version " << APPLICATIONVERSIONSTR;

    NQLog("thermoDAQ") << "using " << logfilename << " for logging";

    QFile * logfile = new QFile(logfilename);
    if (logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        NQLogger::instance()->addDestiniation(logfile, NQLog::Message);
    }

    qRegisterMetaType<State>("State");

#ifdef SINGLETON
    SingletonApplication app(argc, argv, thermoDAQGUID);
    if(!app.lock()){
        NQLog("thermoDAQ") << "Application instance already running!";
        exit(1);
    }
#else
    QApplication app( argc, argv );
#endif

  app.setStyle("cleanlooks");

#ifdef AUTOSTARTTHERMODISPLAY
  std::string thermoDisplay = std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo1/thermoDisplay/thermoDisplay";
  QProcess myProcess(&app);
  myProcess.start(thermoDisplay.c_str());
#endif

  ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo1/thermo.cfg");

  ThermoMainWindow mainWindow;
  //TestWindow mainWindow;

  mainWindow.show();

  return app.exec();
}
