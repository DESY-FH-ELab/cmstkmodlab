/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef LSTEPEXPRESSMOTIONVIEW_H
#define LSTEPEXPRESSMOTIONVIEW_H

#include <LStepExpressModel.h>
#include <LStepExpressMotionManager.h>
#include <LStepExpressWidget.h>
#include <LStepExpressJoystickWidget.h>
#include <LStepExpressPositionWidget.h>

#include <QWidget>

class LStepExpressMotionView : public QWidget
{
 Q_OBJECT

 public:

  explicit LStepExpressMotionView(LStepExpressModel*, LStepExpressMotionManager*, QWidget*);
  virtual ~LStepExpressMotionView();

 protected:

  LStepExpressModel*         model_;
  LStepExpressMotionManager* manager_;

  LStepExpressWidget*         w_lStep_;
  LStepExpressJoystickWidget* w_lStepJoystick_;
  LStepExpressPositionWidget* w_lStepPosition_;
};

#endif // LSTEPEXPRESSMOTIONVIEW_H
