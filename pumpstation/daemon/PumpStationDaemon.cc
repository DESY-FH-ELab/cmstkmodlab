#include <signal.h>

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
#include <LeyboldGraphixThreeModel.h>
#include <DataLogger.h>
#include <WatchDog.h>

#include "PumpStationModel.h"
#include "CommunicationThread.h"

static int setup_unix_signal_handlers()
{
  /*
    struct sigaction sighup;
    sighup.sa_handler = DataLogger::hupSignalHandler;
    sigemptyset(&sighup.sa_mask);
    sighup.sa_flags = 0;
    sighup.sa_flags |= SA_RESTART;

    if (sigaction(SIGHUP, &sighup, 0))
    return 1;
  */

  struct sigaction sigint;
  sigint.sa_handler = DataLogger::intSignalHandler;
  sigemptyset(&sigint.sa_mask);
  sigint.sa_flags = 0;
  sigint.sa_flags |= SA_RESTART;

  if (sigaction(SIGINT, &sigint, 0))
    return 2;

  /*
    struct sigaction sigterm;
    sigterm.sa_handler = DataLogger::termSignalHandler;
    sigemptyset(&sigterm.sa_mask);
    sigterm.sa_flags |= SA_RESTART;

    if (sigaction(SIGTERM, &sigterm, 0))
    return 3;
  */

  return 0;
}

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  setup_unix_signal_handlers();

  if (app.arguments().contains("--nodaemon")) {

    NQLogger::instance()->addActiveModule("*");
    NQLogger::instance()->addDestiniation(stdout, NQLog::Spam);

  } else if (app.arguments().contains("--pidfile")) {

  	int idx = app.arguments().indexOf("--pidfile");
    if (app.arguments().count()>idx+1) {
      QString pidfile = app.arguments().at(idx+1);
    }

    NQLogger::instance()->addActiveModule("*");
    NQLogger::instance()->addDestiniation(stdout, NQLog::Spam);
  }

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

  ApplicationConfig * config = ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/pumpstation/pumpstation.cfg");
  
  std::string conradPort = config->getValue("ConradPort");
  ConradModel conrad(conradPort.c_str(), &app);

  std::string leyboldPort = config->getValue("LeyboldPort");
  LeyboldGraphixThreeModel leybold(leyboldPort.c_str(), config->getValue<int>("LeyboldUpdateInterval"), &app);

  /*
  if (leybold.getSensorDetectionMode(1)!=VLeyboldGraphixThree::SensorDetectionAuto) {
    leybold.setSensorDetectionMode(1, VLeyboldGraphixThree::SensorDetectionAuto);
  }
  if (leybold.getSensorType(1)==VLeyboldGraphixThree::SensorType_TTRx) {
    leybold.setSensorType(1, VLeyboldGraphixThree::SensorType_TTR91N);
  }

  if (leybold.getSensorDetectionMode(2)!=VLeyboldGraphixThree::SensorDetectionAuto) {
    leybold.setSensorDetectionMode(2, VLeyboldGraphixThree::SensorDetectionAuto);
  }
  if (leybold.getSensorType(2)==VLeyboldGraphixThree::SensorType_TTRx) {
    leybold.setSensorType(2, VLeyboldGraphixThree::SensorType_TTR91N);
  }

  if (leybold.getSensorDetectionMode(3)!=VLeyboldGraphixThree::SensorDetectionAuto) {
    leybold.setSensorDetectionMode(3, VLeyboldGraphixThree::SensorDetectionAuto);
  }
  if (leybold.getSensorType(3)==VLeyboldGraphixThree::SensorType_TTRx) {
    leybold.setSensorType(3, VLeyboldGraphixThree::SensorType_TTR91N);
  }

  leybold.setSensorName(1, "SYS");
  leybold.setSensorName(2, "P1");
  leybold.setSensorName(3, "P2");
  */

  leybold.setDisplayUnit(VLeyboldGraphixThree::DisplayUnit_mbar);

  leybold.updateInformation();

  PumpStationModel model(&conrad, &leybold, 5, &app);

  CommunicationThread commthread(&model, &app);
  commthread.start();

  DataLogger logger(&model, &commthread, &app);
  logger.initialize();
  
  WatchDog watchdog(&model, 300/5, &app);
  watchdog.initialize();

  return app.exec();
}
