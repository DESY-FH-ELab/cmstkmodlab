/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <QDebug>
#include <QApplication>

#include "PumpStationSingletonApplication.h"

#include "PumpStationMainWindow.h"
#include "PumpStationHTTPModel.h"

static const char* CMSPumpStationControlGUID = "{DDF04062-A7ED-4B47-B5E0-AEB2A86F6CA9}";

int main( int argc, char** argv )
{
  PumpStationSingletonApplication app(argc, argv, CMSPumpStationControlGUID);
  if (!app.lock()) {
    qDebug() << "Application instance already running!";
    exit(1);
  }

  app.setStyle("cleanlooks");

  PumpStationHTTPModel * model = new PumpStationHTTPModel(10);

  PumpStationMainWindow mainWindow(model);

  mainWindow.setWindowTitle("CMSPumpStationControl");
  mainWindow.show();

  model->updateInformation();

  return app.exec();
}
