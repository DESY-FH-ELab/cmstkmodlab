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

#ifndef LSTEPEXPRESSJOYSTICKWIDGET_H
#define LSTEPEXPRESSJOYSTICKWIDGET_H

#include <vector>

#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>

#include "LStepExpressModel.h"

class LStepExpressJoystickAxisWidget;

class LStepExpressJoystickWidget : public QWidget
{
  Q_OBJECT

  public:
   explicit LStepExpressJoystickWidget(LStepExpressModel* model, QWidget *parent = 0);
   ~LStepExpressJoystickWidget();

  protected:
   LStepExpressModel* model_;
   QWidget* axisControlWidget_;

   LStepExpressJoystickAxisWidget* w_joyax_X_;
   LStepExpressJoystickAxisWidget* w_joyax_Y_;
   LStepExpressJoystickAxisWidget* w_joyax_Z_;
   LStepExpressJoystickAxisWidget* w_joyax_A_;

  public slots:
   void lstepStateChanged(State newState);
   void controlStateChanged(bool);
   void motionStarted();
   void motionFinished();

   void  enableMotionTools(const bool  enable=true);
   void disableMotionTools(const bool disable=true);
};

class LStepExpressJoystickAxisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LStepExpressJoystickAxisWidget(LStepExpressModel* model_,
                                            unsigned int axis,
                                            QWidget *parent = 0);
    ~LStepExpressJoystickAxisWidget();

protected:
    LStepExpressModel* model_;
    unsigned int axis_;

    QVBoxLayout* layout_;
    QPushButton *upButton_;
    QPushButton *downButton_;
    QDoubleSpinBox *stepBox_;

    bool motionTools_enabled_;

public slots:
    void lStepStateChanged( State state );
    void controlStateChanged(bool);
    void updateWidgets();
    void updateMotionWidgets();
    void upButtonClicked();
    void downButtonClicked();
    void motionStarted();
    void motionFinished();

    void  enableMotionTools(const bool enable=true);
    void disableMotionTools();
};

#endif // LSTEPEXPRESSJOYSTICKWIDGET_H
