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
#include <nqlogger.h>

#include <QHBoxLayout>
#include <QVBoxLayout>

LStepExpressMotionView::LStepExpressMotionView(LStepExpressModel* model, LStepExpressMotionManager* manager, QWidget* parent) :
  QWidget(parent),
  model_(model),
  manager_(manager),
  w_lStep_(nullptr),
  w_moveabs_(nullptr),
  w_moverel_(nullptr),
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

  //// Motion Widgets
  if(manager_->model() != nullptr)
  {
    QGridLayout* g_move = new QGridLayout;

    // widget: Move Absolute
    w_moveabs_ = new AssemblyMoveWidget("Move Absolute [X, Y, Z, A]", "0,0,0,0", this);
    w_moveabs_->setToolTip("(1) Moves x,y,z,a stage using moveAbsolute routine (with respect to origin)");
    w_moveabs_->useMoveRelative(false);
    w_moveabs_->button()->setStyleSheet(QString::fromUtf8("text-align:center;"));

    g_move->addWidget(w_moveabs_->button(), 0, 0);
    g_move->addWidget(w_moveabs_->lineed(), 1, 0);

    connect(w_moveabs_, SIGNAL(moveAbsolute(double, double, double, double)), manager_, SLOT(moveAbsolute(double, double, double, double)));
    connect(manager_, SIGNAL(motion_finished()), w_moveabs_, SLOT(enable()));
    // -----

    // widget: Move Relative
    w_moverel_ = new AssemblyMoveWidget("Move Relative [dX, dY, dZ, dA]", "0,0,0,0", this);
    w_moverel_->setToolTip("(2) Moves x,y,z,a stage using moveRelative routine (with respect to current position)");
    w_moverel_->useMoveRelative(true);
    w_moverel_->button()->setStyleSheet(QString::fromUtf8("text-align:center;"));

    g_move->addWidget(w_moverel_->button(), 0, 1);
    g_move->addWidget(w_moverel_->lineed(), 1, 1);

    connect(w_moverel_, SIGNAL(moveRelative(double, double, double, double)), manager_, SLOT(moveRelative(double, double, double, double)));
    connect(manager_, SIGNAL(motion_finished()), w_moverel_, SLOT(enable()));
    // -----

    this->stateChanged(manager_->model()->getDeviceState());

    connect(manager_->model(), SIGNAL(deviceStateChanged(State)), this, SLOT(stateChanged(State)));

    layout_1->addLayout(g_move);
  }
  else
  {
    NQLog("LStepExpressMotionView", NQLog::Critical)
      << "instance of LStepExpressMotionManager does not point to a valid LStepExpressModel (null pointer)"
      << ", motion-widgets not added to view";
  }
  //// --------------

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

  if(w_moveabs_){ delete w_moveabs_; w_moveabs_ = nullptr; }
  if(w_moverel_){ delete w_moverel_; w_moverel_ = nullptr; }

  if(w_lStepJoystick_){ delete w_lStepJoystick_; w_lStepJoystick_ = nullptr; }
  if(w_lStepPosition_){ delete w_lStepPosition_; w_lStepPosition_ = nullptr; }
}

void LStepExpressMotionView::stateChanged(const State& newState)
{
  w_moverel_->enable(newState == READY);
  w_moveabs_->enable(newState == READY);

  return;
}
