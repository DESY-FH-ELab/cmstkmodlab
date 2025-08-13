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
    arguments.removeOne("--web");
  } else {
    ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/pumpstation/pumpstation.cfg");
  }

  Controller controller(arguments);

  if (arguments.contains("--help")) {
    QTimer::singleShot(0, &controller, SLOT(printHelp()));
  } else {
    QTimer::singleShot(0, &controller, SLOT(connectToServer()));
  }

  return app.exec();
}
