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

#ifndef ASSEMBLYHARDWARECONTROLVIEW_H
#define ASSEMBLYHARDWARECONTROLVIEW_H

#include <LStepExpressModel.h>
#include <LStepExpressMotionManager.h>
#include <LStepExpressWidget.h>
#include <LStepExpressJoystickWidget.h>
#include <LStepExpressPositionWidget.h>
#include <AssemblyMotionWidgets.h>
#include <AssemblyVacuumWidget.h>

#include <QWidget>

class AssemblyHardwareControlView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyHardwareControlView(const LStepExpressMotionManager* const, QWidget*);
  virtual ~AssemblyHardwareControlView();

  const LStepExpressMotionManager* manager() const { return manager_; }

  LStepExpressWidget* LStepExpress_Widget() const { return w_lStep_; }

  AssemblyVacuumWidget* Vacuum_Widget() const { return w_vacuum_; }

 protected:

  const LStepExpressMotionManager* const manager_;

  LStepExpressWidget*         w_lStep_;
  LStepExpressJoystickWidget* w_lStepJoystick_;
  LStepExpressPositionWidget* w_lStepPosition_;

  AssemblyMoveWidget*   w_moveabs_;
  AssemblyMoveWidget*   w_moverel_;
  AssemblyVacuumWidget* w_vacuum_;

 public slots:

  void stateChanged(const State&);
};

#endif // ASSEMBLYHARDWARECONTROLVIEW_H
