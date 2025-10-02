#include <iostream>

#include <QApplication>
#include <QProcess>
#include <QFile>
#include <QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

//#include <TROOT.h>

#include <nqlogger.h>

#include "SingletonApplication.h"
#include "ApplicationConfig.h"

#include "DefoMainWindow.h"
#include "TestWindow.h"

static const char* defoDAQGUID = "{94E678B7-AD8C-4027-971D-5F49E0FFC647}";
//#define SINGLETON 1

int main( int argc, char** argv )
{
  //std::cout << TROOT::Initialized() << std::endl;

  qRegisterMetaType<DefoConradModel::DeviceSwitch>("DefoConradModel::DeviceSwitch");
  qRegisterMetaType<State>("State");

  NQLogger::instance()->addActiveModule("*");
  NQLogger::instance()->addDestiniation(stdout, NQLog::Spam);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QString logdir = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
  QString logdir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
  QDir dir(logdir);
  if (!dir.exists()) dir.mkpath(".");
  QString logfilename = logdir + "/defoDAQ.log";

  NQLog("thermoDAQ") << "version " << APPLICATIONVERSIONSTR;

  NQLog("thermoDAQ") << "using " << logfilename << " for logging";

  QFile * logfile = new QFile(logfilename);
  if (logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
      NQLogger::instance()->addDestiniation(logfile, NQLog::Message);
  }

#ifdef SINGLETON
    SingletonApplication app(argc, argv, defoDAQGUID);
    if(!app.lock()){
        std::cout << "Application instance already running!" << std::endl;
        exit(1);
    }
#else
    QApplication app( argc, argv );
#endif

  app.setStyle("cleanlooks");

  ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/defo/defo.cfg", "main");

  DefoMainWindow mainWindow;
  //TestWindow mainWindow;
  //ScriptWindow mainWindow;

  mainWindow.show();

  return app.exec();
}
