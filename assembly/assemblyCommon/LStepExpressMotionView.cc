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

#include <nqlogger.h>

#include <LStepExpressMotionView.h>

#include <QHBoxLayout>
#include <QVBoxLayout>

//-- Obsolete class, not used anymore
LStepExpressMotionView::LStepExpressMotionView(LStepExpressModel* model, LStepExpressMotionManager* manager, QWidget* parent) :
  QWidget(parent),
  model_(model),
  manager_(manager),
  w_lStep_(nullptr),
  w_lStepJoystick_(nullptr),
  w_lStepPosition_(nullptr)
{
  // MAIN LAYOUT -------------------------------
  QHBoxLayout* layout = new QHBoxLayout(this);
  this->setLayout(layout);

  // layout #1
  QVBoxLayout* layout_1 = new QVBoxLayout();

  w_lStep_ = new LStepExpressWidget(model_, this);
  layout_1->addWidget(w_lStep_);

  connect(w_lStep_, SIGNAL(clearQueue_request()), manager, SLOT(clear_motion_queue()));

  w_lStepJoystick_ = new LStepExpressJoystickWidget(model_, this);
  layout_1->addWidget(w_lStepJoystick_);

  layout->addLayout(layout_1);

//  // layout #2
//  QVBoxLayout* layout_2 = new QVBoxLayout();
//
//  w_lStepPosition_ = new LStepExpressPositionWidget(manager_, model_, this);
//  layout_2->addWidget(w_lStepPosition_);
//
//  layout->addLayout(layout_2);
  // -------------------------------------------

  NQLog("LStepExpressMotionView", NQLog::Debug) << "constructed";
}

LStepExpressMotionView::~LStepExpressMotionView()
{
  if(w_lStep_){ delete w_lStep_; w_lStep_ = nullptr; }

  if(w_lStepJoystick_){ delete w_lStepJoystick_; w_lStepJoystick_ = nullptr; }
  if(w_lStepPosition_){ delete w_lStepPosition_; w_lStepPosition_ = nullptr; }
}
