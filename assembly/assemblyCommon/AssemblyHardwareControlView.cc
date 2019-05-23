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

#include <AssemblyHardwareControlView.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

AssemblyHardwareControlView::AssemblyHardwareControlView(const LStepExpressMotionManager* const manager, QWidget* parent)
 : QWidget(parent)
 , manager_(manager)

 , w_move_  (nullptr)
 , w_vacuum_(nullptr)

 , cb_lockMotionSettings_(nullptr)

 , w_lStep_        (nullptr)
 , w_lStepJoystick_(nullptr)
 , w_lStepPosition_(nullptr)
{
  if(manager_ == nullptr)
  {
    NQLog("AssemblyHardwareControlView", NQLog::Critical) << "initialization error"
      << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }

  if(manager_->model() == nullptr)
  {
    NQLog("AssemblyHardwareControlView", NQLog::Critical) << "initialization error"
      << ": null pointer to LStepExpressModel object from LStepExpressMotionManager::model(), exiting constructor";

    return;
  }

  //// Main Layout -----------------------------
  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  //// Motion and Vacuum Widgets
  QHBoxLayout* g1 = new QHBoxLayout;

  /// Motion Widgets
  QGroupBox* box_move = new QGroupBox(tr("Motion Stage"));
  box_move->setStyleSheet("QGroupBox { font-weight: bold; } ");

  QVBoxLayout* l_move = new QVBoxLayout;
  box_move->setLayout(l_move);

  w_move_ = new AssemblyMoveWidget(manager_);
  w_move_->setToolTip("Single-Motion interface");
  w_move_->setEnabled(false);

  l_move->addWidget(w_move_);
  // -----

  this->stateChanged(manager_->model()->getDeviceState());

  connect(manager_->model(), SIGNAL(deviceStateChanged(State)), this, SLOT(stateChanged(State)));

//  connect(manager_->model(), SIGNAL(motionStarted ()), this, SLOT(disableMotionTools()));
//  connect(manager_->model(), SIGNAL(motionFinished()), this, SLOT( enableMotionTools()));

  g1->addWidget(box_move, 60);
  /// -------------

  /// Vacuum Widget
  QGroupBox* box_vacuum = new QGroupBox(tr("Vacuum"));
  box_vacuum->setStyleSheet("QGroupBox { font-weight: bold; } ");

  w_vacuum_ = new AssemblyVacuumWidget("Toggle Vacuum");
  w_vacuum_->setToolTip("(3) Controls vacuum valves");

  box_vacuum->setLayout(w_vacuum_->layout());

  g1->addWidget(box_vacuum, 40);
  /// -------------

  layout->addLayout(g1);

  //// ------------------

  cb_lockMotionSettings_ = new QCheckBox("Lock Motion Stage Settings");
  layout->addWidget(cb_lockMotionSettings_);

  //// LStepExpressWidget
  w_lStep_ = new LStepExpressWidget(manager_->model());
  layout->addWidget(w_lStep_);

  connect(w_lStep_, SIGNAL(clearQueue_request()), manager_, SLOT(clear_motion_queue()));
  //// ------------------

  //// LStepExpressJoystickWidget
  w_lStepJoystick_ = new LStepExpressJoystickWidget(manager_->model());
  layout->addWidget(w_lStepJoystick_);
  //// ------------------

//  // LStepExpressPositionWidget
//  w_lStepPosition_ = new LStepExpressPositionWidget(manager_, manager_->model());
//  layout->addWidget(w_lStepPosition_);
//  //// ------------------

  connect(cb_lockMotionSettings_, SIGNAL(toggled(bool)), w_lStep_, SLOT(lockMotionSettings(bool)));

  // -------------------------------------------

  NQLog("AssemblyHardwareControlView", NQLog::Debug) << "constructed";
}

AssemblyHardwareControlView::~AssemblyHardwareControlView()
{
  if(w_lStep_){ delete w_lStep_; w_lStep_ = nullptr; }

  if(w_move_){ delete w_move_; w_move_ = nullptr; }

  if(w_lStepJoystick_){ delete w_lStepJoystick_; w_lStepJoystick_ = nullptr; }
  if(w_lStepPosition_){ delete w_lStepPosition_; w_lStepPosition_ = nullptr; }
}

void AssemblyHardwareControlView::stateChanged(const State& newState)
{
  if(w_move_){ w_move_->setEnabled(newState == READY); }
}

void AssemblyHardwareControlView::enableMotionTools(const bool enable)
{
  if(w_move_){ w_move_->setEnabled(enable); }

  if(w_lStep_        ){ w_lStep_        ->enableMotionTools(enable); }
  if(w_lStepJoystick_){ w_lStepJoystick_->enableMotionTools(enable); }
  if(w_lStepPosition_){ w_lStepPosition_->enableMotionTools(enable); }
}

void AssemblyHardwareControlView::disableMotionTools(const bool disable)
{
  this->enableMotionTools(!disable);
}
