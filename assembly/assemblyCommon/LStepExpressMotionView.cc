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
#include <AssemblyMotionWidgets.h>
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

  //// Motion Widgets
  QGridLayout* g_move = new QGridLayout;

  // widget: Move Absolute
  AssemblyMoveWidget* w_moveabs = new AssemblyMoveWidget("Move Absolute [X, Y, Z, A]", "0,0,0,0", this);
  w_moveabs->setToolTip("(1) Moves x,y,z,a stage using moveAbsolute routine (with respect to origin)");
  w_moveabs->useMoveRelative(false);
  w_moveabs->button()->setStyleSheet(QString::fromUtf8("text-align:center;"));

  g_move->addWidget(w_moveabs->button(), 0, 0);
  g_move->addWidget(w_moveabs->lineed(), 1, 0);

  connect(w_moveabs, SIGNAL(moveAbsolute(double, double, double, double)), manager_, SLOT(moveAbsolute(double, double, double, double)));
  connect(manager_, SIGNAL(motion_finished()), w_moveabs, SLOT(enable()));
  // -----

  // widget: Move Relative
  AssemblyMoveWidget* w_moverel = new AssemblyMoveWidget("Move Relative [dX, dY, dZ, dA]", "0,0,0,0", this);
  w_moverel->setToolTip("(2) Moves x,y,z,a stage using moveRelative routine (with respect to current position)");
  w_moverel->useMoveRelative(true);
  w_moverel->button()->setStyleSheet(QString::fromUtf8("text-align:center;"));

  g_move->addWidget(w_moverel->button(), 0, 1);
  g_move->addWidget(w_moverel->lineed(), 1, 1);

  connect(w_moverel, SIGNAL(moveRelative(double, double, double, double)), manager_, SLOT(moveRelative(double, double, double, double)));
  connect(manager_, SIGNAL(motion_finished()), w_moverel, SLOT(enable()));
  // -----

  layout_1->addLayout(g_move);

  //// --------------

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
