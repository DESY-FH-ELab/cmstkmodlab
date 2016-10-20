#include <iostream>

#include <QCoreApplication>
#include <QtCore>
#include <QtNetwork>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "Controller.h"

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  QStringList arguments = QCoreApplication::arguments();
  arguments.removeAt(0);

  if (arguments.contains("--debug")) {
    NQLogger::instance()->addActiveModule("*");
    NQLogger::instance()->addDestiniation(stdout, NQLog::Debug);

    arguments.removeOne("--debug");
  }

  if (arguments.contains("--web")) {
    ApplicationConfig::instance("pumpstation.cfg");
  } else {
    ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/pumpstation/pumpstation.cfg");
  }

  Controller controller(arguments);

  return app.exec();
}
