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

#include <iostream>

#include <nqlogger.h>
#include <ApplicationConfig.h>
#include <ApplicationConfigItemModel.h>

#include <RohdeSchwarzNGE103BModel.h>
#include <RohdeSchwarzNGE103BWidget.h>

#include "CommonTestMainWindow.h"

CommonTestMainWindow::CommonTestMainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  NQLog("CommonTestMainWindow", NQLog::Debug) << "CommonTestMainWindow(QWidget *parent)";

  ApplicationConfig* config = ApplicationConfig::instance();

  /*
  RohdeSchwarzNGE103BModel *nge103BModel_ = new RohdeSchwarzNGE103BModel(config->getValue<std::string>("RohdeSchwarzNGE103BDevice").c_str(),
									 10, this);
  RohdeSchwarzNGE103BWidget* nge103BWidget_ = new RohdeSchwarzNGE103BWidget(nge103BModel_);
  setCentralWidget(nge103BWidget_);
  */
  
  /*
  viewer_ = new ApplicationConfigViewer(false, this);
  setCentralWidget(viewer_);
  */
  
  updateGeometry();

  connect(config, SIGNAL(valueChanged()),
      this, SLOT(configChanged()));
}

void CommonTestMainWindow::quit()
{
  NQLog("CommonTestMainWindow", NQLog::Debug) << "quit()";
}

void CommonTestMainWindow::configChanged()
{
  NQLog("CommonTestMainWindow", NQLog::Debug) << "configChanged()";

  std::cout << "config changed" << std::endl;
}
