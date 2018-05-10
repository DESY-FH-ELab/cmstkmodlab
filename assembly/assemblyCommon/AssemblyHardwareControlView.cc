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

AssemblyHardwareControlView::AssemblyHardwareControlView(const LStepExpressMotionManager* const manager, QWidget* parent) :
  QWidget(parent),
  manager_(manager),

  w_lStep_        (nullptr),
  w_lStepJoystick_(nullptr),
  w_lStepPosition_(nullptr),

  w_moveabs_(nullptr),
  w_moverel_(nullptr),
  w_vacuum_ (nullptr)
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
  QHBoxLayout* g1 = new QHBoxLayout(this);

  /// Motion Widgets
  QGroupBox* box_move = new QGroupBox(tr("Motion Stage"));

  QVBoxLayout* l_move = new QVBoxLayout;

  // widget: Move Absolute
  QVBoxLayout* l_moveabs = new QVBoxLayout;

  w_moveabs_ = new AssemblyMoveWidget("Move Absolute [mm] (format: X, Y, Z, A)", "0, 0, 0, 0", this);
  w_moveabs_->setToolTip("(1) Moves x,y,z,a stage using moveAbsolute routine (with respect to origin)");
  w_moveabs_->useMoveRelative(false);
  w_moveabs_->button()->setStyleSheet(QString::fromUtf8("text-align:center;"));

  l_moveabs->addWidget(w_moveabs_->button());
  l_moveabs->addWidget(w_moveabs_->lineed());

  connect(w_moveabs_, SIGNAL(moveAbsolute(double, double, double, double)), manager_, SLOT(moveAbsolute(double, double, double, double)));
  connect(manager_, SIGNAL(motion_finished()), w_moveabs_, SLOT(enable()));

  l_move->addLayout(l_moveabs);
  // -----

  // widget: Move Relative
  QVBoxLayout* l_moverel = new QVBoxLayout;

  w_moverel_ = new AssemblyMoveWidget("Move Relative [mm] (format: dX, dY, dZ, dA)", "0, 0, 0, 0", this);
  w_moverel_->setToolTip("(2) Moves x,y,z,a stage using moveRelative routine (with respect to current position)");
  w_moverel_->useMoveRelative(true);
  w_moverel_->button()->setStyleSheet(QString::fromUtf8("text-align:center;"));

  l_moverel->addWidget(w_moverel_->button());
  l_moverel->addWidget(w_moverel_->lineed());

  connect(w_moverel_, SIGNAL(moveRelative(double, double, double, double)), manager_, SLOT(moveRelative(double, double, double, double)));
  connect(manager_, SIGNAL(motion_finished()), w_moverel_, SLOT(enable()));

  l_move->addLayout(l_moverel);
  // -----

  this->stateChanged(manager_->model()->getDeviceState());

  connect(manager_->model(), SIGNAL(deviceStateChanged(State)), this, SLOT(stateChanged(State)));

  box_move->setLayout(l_move);

  g1->addWidget(box_move, 60);
  /// -------------

  /// Vacuum Widget
  QGroupBox* box_vacuum = new QGroupBox(tr("Vacuum"));

  w_vacuum_ = new AssemblyVacuumWidget("Toggle Vacuum", this);
  w_vacuum_->setToolTip("(3) Controls vacuum valves");

  box_vacuum->setLayout(w_vacuum_->layout());

  g1->addWidget(box_vacuum, 40);
  /// -------------

  layout->addLayout(g1);

  //// ------------------

  //// LStepExpressWidget
  w_lStep_ = new LStepExpressWidget(manager_->model(), this);
  layout->addWidget(w_lStep_);

  connect(w_lStep_, SIGNAL(clearQueue_request()), manager_, SLOT(clear_motion_queue()));
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
  w_moverel_->enable(newState == READY);
  w_moveabs_->enable(newState == READY);

  return;
}
