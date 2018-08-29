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

 , w_moveabs_(nullptr)
 , w_moverel_(nullptr)
 , w_vacuum_ (nullptr)

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
  QVBoxLayout* layout = new QVBoxLayout(this);
  this->setLayout(layout);

  //// Motion and Vacuum Widgets
  QHBoxLayout* g1 = new QHBoxLayout;

  /// Motion Widgets
  QGroupBox* box_move = new QGroupBox(tr("Motion Stage"));
  box_move->setStyleSheet("QGroupBox { font-weight: bold; } ");

  QVBoxLayout* l_move = new QVBoxLayout;

  // widget: Move Absolute
  QVBoxLayout* l_moveabs = new QVBoxLayout;

  w_moveabs_ = new AssemblyMoveWidget(manager_, "Move Absolute [mm, deg]");
  w_moveabs_->setToolTip("Moves to (x,y,z,a) position in the motion stage reference frame");
  w_moveabs_->use_move_relative(false);
  w_moveabs_->button()->setStyleSheet(QString::fromUtf8("text-align:center;"));

  l_moveabs->addWidget(w_moveabs_);

  l_move->addLayout(l_moveabs);
  // -----

  // widget: Move Relative
  QVBoxLayout* l_moverel = new QVBoxLayout;

  w_moverel_ = new AssemblyMoveWidget(manager_, "Move Relative [mm, deg]");
  w_moverel_->setToolTip("Relative movement (x,y,z,a) with respect to current position");
  w_moverel_->use_move_relative(true);
  w_moverel_->button()->setStyleSheet(QString::fromUtf8("text-align:center;"));

  l_moverel->addWidget(w_moverel_);

  l_move->addLayout(l_moverel);
  // -----

  this->stateChanged(manager_->model()->getDeviceState());

  connect(manager_->model(), SIGNAL(deviceStateChanged(State)), this, SLOT(stateChanged(State)));

  connect(manager_->model(), SIGNAL(motionStarted ()), this, SLOT(disableMotionTools()));
  connect(manager_->model(), SIGNAL(motionFinished()), this, SLOT( enableMotionTools()));

  box_move->setLayout(l_move);

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

  cb_lockMotionSettings_ = new QCheckBox("Lock Controller Settings");
  layout->addWidget(cb_lockMotionSettings_);

  //// LStepExpressWidget
  w_lStep_ = new LStepExpressWidget(manager_->model(), this);
  layout->addWidget(w_lStep_);

  connect(w_lStep_, SIGNAL(clearQueue_request()), manager_, SLOT(clear_motion_queue()));

  connect(cb_lockMotionSettings_, SIGNAL(toggled(bool)), w_lStep_, SLOT(lockMotionSettings(bool)));
  //// ------------------

  //// LStepExpressJoystickWidget
  w_lStepJoystick_ = new LStepExpressJoystickWidget(manager_->model(), this);
  layout->addWidget(w_lStepJoystick_);
  //// ------------------

//  // LStepExpressPositionWidget
//  w_lStepPosition_ = new LStepExpressPositionWidget(manager_, manager_->model(), this);
//  layout->addWidget(w_lStepPosition_);
//  //// ------------------

  // -------------------------------------------

  NQLog("AssemblyHardwareControlView", NQLog::Debug) << "constructed";
}

AssemblyHardwareControlView::~AssemblyHardwareControlView()
{
  if(w_lStep_){ delete w_lStep_; w_lStep_ = nullptr; }

  if(w_moveabs_){ delete w_moveabs_; w_moveabs_ = nullptr; }
  if(w_moverel_){ delete w_moverel_; w_moverel_ = nullptr; }

  if(w_lStepJoystick_){ delete w_lStepJoystick_; w_lStepJoystick_ = nullptr; }
  if(w_lStepPosition_){ delete w_lStepPosition_; w_lStepPosition_ = nullptr; }
}

void AssemblyHardwareControlView::stateChanged(const State& newState)
{
  if(w_moveabs_){ w_moveabs_->setEnabled(newState == READY); }
  if(w_moverel_){ w_moverel_->setEnabled(newState == READY); }
}

void AssemblyHardwareControlView::enableMotionTools(const bool enable)
{
  if(w_lStep_        ){ w_lStep_        ->enableMotionTools(enable); }
  if(w_lStepJoystick_){ w_lStepJoystick_->enableMotionTools(enable); }
  if(w_lStepPosition_){ w_lStepPosition_->enableMotionTools(enable); }

  if(w_moveabs_){ w_moveabs_->setEnabled(enable); }
  if(w_moverel_){ w_moverel_->setEnabled(enable); }
}

void AssemblyHardwareControlView::disableMotionTools()
{
  this->enableMotionTools(false);
}
