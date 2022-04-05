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

#include <ApplicationConfig.h>
#include <RohdeSchwarzNGE103BWidget.h>

#include "TestWindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent)
{
  QHBoxLayout *layout = new QHBoxLayout();
  QWidget *central = new QWidget();
  central->setLayout(layout);

  ApplicationConfig* config = ApplicationConfig::instance();

  // RohdeSchwarzNGE103B Model
  nge103BModel_ = new RohdeSchwarzNGE103BModel(config->getValue<std::string>("main", "RohdeSchwarzNGE103BDevice").c_str(),
					       10, this);
  RohdeSchwarzNGE103BWidget* nge103BWidget = new RohdeSchwarzNGE103BWidget(nge103BModel_, central);
  nge103BWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  layout->addWidget(nge103BWidget);

  setCentralWidget(central);
}
