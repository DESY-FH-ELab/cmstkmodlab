#include <iostream>

#include <QCoreApplication>
#include <QtCore>
#include <QtNetwork>
#include <QFile>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include <nqlogger.h>
#include <ApplicationConfig.h>
#include <DeviceState.h>

#include <ConradModel.h>
#include <DataLogger.h>

#include "CommunicationThread.h"

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  NQLogger::instance()->addActiveModule("*");
  NQLogger::instance()->addDestiniation(stdout, NQLog::Debug);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QString logdir = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
  QString logdir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
  QDir dir(logdir);
  if (!dir.exists()) dir.mkpath(".");
  QString logfilename = logdir + "/pumpstation.log";

  NQLog("pumpstation") << "version " << APPLICATIONVERSIONSTR;

  NQLog("pumpstation") << "using " << logfilename << " for logging";

  QFile * logfile = new QFile(logfilename);
  if (logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
    NQLogger::instance()->addDestiniation(logfile, NQLog::Message);
  }

  qRegisterMetaType<State>("State");

  ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/pumpstation/pumpstation.cfg");

  ConradModel conrad(&app);

  DataLogger logger(&conrad, &app);

  CommunicationThread commthread(&conrad, &app);

  commthread.start();

  return app.exec();
}
