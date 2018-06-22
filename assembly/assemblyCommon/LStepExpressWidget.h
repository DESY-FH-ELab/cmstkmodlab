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

#ifndef LSTEPEXPRESSWIDGET_H
#define LSTEPEXPRESSWIDGET_H

#include <LStepExpressModel.h>

#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

class LStepExpressAxisWidget;

class LStepExpressWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit LStepExpressWidget(LStepExpressModel* model, QWidget* parent=nullptr);
  virtual ~LStepExpressWidget();

 protected:

  LStepExpressModel* model_;

  QCheckBox* lstepCheckBox_;
  QCheckBox* joystickCheckBox_;

  QPushButton* buttonOrigin_;
  QPushButton* buttonCalibrate_;
  QPushButton* buttonEmergencyStop_;
  QPushButton* buttonClearQueue_;

  LStepExpressAxisWidget* axisWidget_X_;
  LStepExpressAxisWidget* axisWidget_Y_;
  LStepExpressAxisWidget* axisWidget_Z_;
  LStepExpressAxisWidget* axisWidget_A_;

  QWidget* axisControlWidget_;

 public slots:

  void lstepStateChanged(State newState);
  void controlStateChanged(bool);
  void updateWidgets();
  void motionStarted();
  void motionFinished();

  void enableMotionControllers();

 signals:

  void clearQueue_request();

  void MotionControllers_enabled();
};

class LStepExpressAxisWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit LStepExpressAxisWidget(LStepExpressModel* model, unsigned int axis, QWidget* parent=nullptr);
  virtual ~LStepExpressAxisWidget();

 protected:

  LStepExpressModel* model_;
  unsigned int axis_;

  QFormLayout* layout_;
  QCheckBox* enabledCheckBox_;
  QCheckBox* joystickCheckBox_;
  QLabel* statusLabel_;
  QLabel* positionLabel_;
  QString axisDimensionName_;

 public slots:

  void lStepStateChanged(State state);
  void controlStateChanged(bool);
  void updateWidgets();
  void updateMotionWidgets();
  void enabledCheckBoxToggled(bool enabled);
  void joystickCheckBoxToggled(bool enabled);
  void motionStarted();
  void motionFinished();
};

#endif // LSTEPEXPRESSWIDGET_H
