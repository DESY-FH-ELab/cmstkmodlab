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

#include <LStepExpressMotionView.h>
#include <LStepExpressWidget.h>
#include <LStepExpressJoystickWidget.h>
#include <LStepExpressPositionWidget.h>
#include <nqlogger.h>

#include <QHBoxLayout>
#include <QVBoxLayout>

LStepExpressMotionView::LStepExpressMotionView(LStepExpressModel* model, LStepExpressMotionManager* manager, QWidget* parent) :
  QWidget(parent),
  model_(model),
  manager_(manager)
{
  // MAIN LAYOUT -------------------------------
  QHBoxLayout* layout = new QHBoxLayout(this);
  this->setLayout(layout);

  // layout #1
  QVBoxLayout* layout_1 = new QVBoxLayout();

  LStepExpressWidget* motionWidget = new LStepExpressWidget(model_, this);
  layout_1->addWidget(motionWidget);

  connect(motionWidget, SIGNAL(clearQueue_request()), manager, SLOT(clear_motion_queue()));

  LStepExpressJoystickWidget* lStepJoystick = new LStepExpressJoystickWidget(model_, this);
  layout_1->addWidget(lStepJoystick);

  layout->addLayout(layout_1);

//  // layout #2
//  QVBoxLayout* layout_2 = new QVBoxLayout();
//
//  LStepExpressPositionWidget* lStepPosition = new LStepExpressPositionWidget(manager_, model_, this);
//  layout_2->addWidget(lStepPosition);
//
//  layout->addLayout(layout_2);
  // -------------------------------------------

  NQLog("LStepExpressMotionView", NQLog::Debug) << "constructed";
}
