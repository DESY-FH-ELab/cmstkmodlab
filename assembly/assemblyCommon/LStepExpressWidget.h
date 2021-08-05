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

#include <QWidget>
#include <QGroupBox>
#include <QCheckBox>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>

class LStepExpressAxisWidget;

class LStepExpressWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit LStepExpressWidget(LStepExpressModel* model, QWidget* parent=nullptr);
  virtual ~LStepExpressWidget();

 protected:

  LStepExpressModel* model_;

  QGroupBox* mot_settings_box_;
  QCheckBox* lstepCheckBox_;
  QCheckBox* joystickCheckBox_;
  QCheckBox* posCtrlCheckBox_;

  QPushButton* buttonOrigin_;
  QPushButton* buttonCalibrate_;
  QPushButton* buttonEmergencyStop_;
  QPushButton* buttonClearQueue_;
  QPushButton* buttonRestart_;
  QPushButton* buttonErrorQuit_;

  LStepExpressAxisWidget* axisWidget_X_;
  LStepExpressAxisWidget* axisWidget_Y_;
  LStepExpressAxisWidget* axisWidget_Z_;
  LStepExpressAxisWidget* axisWidget_A_;

  QWidget* axisControlWidget_;

  bool motionSettings_locked_;

  QTimer* restart_timer_;
  uint    restart_step_;
  uint    restart_attempts_;
  bool    restart_completed_;

  const uint RESTART_MAX_ATTEMPTS_ = 2;

 public slots:

  void lstepStateChanged(State newState);
  void controlStateChanged(bool);
  void updateWidgets();
  void motionStarted();
  void motionFinished();

  void enableMotionControllers();

  void restart();

  void   lockMotionSettings(const bool disable=true);
  void unlockMotionSettings();

  void  enableMotionTools(const bool  enable=true);
  void disableMotionTools(const bool disable=true);

  void confirmCalibrate();

 signals:

  void clearQueue_request();

  void MotionControllers_enabled();

  void restart_completed();

  void moveToOrigin_request();

  void startCalibrate();
};

class LStepExpressAxisWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit LStepExpressAxisWidget(LStepExpressModel* model, unsigned int axis, QWidget* parent=nullptr);
  virtual ~LStepExpressAxisWidget();

  QCheckBox* enabledCheckBox() const { return enabledCheckBox_; }

 protected:

  LStepExpressModel* model_;
  unsigned int axis_;

  QFormLayout* layout_;
  QCheckBox* enabledCheckBox_;
  QCheckBox* joystickCheckBox_;
  QLabel* statusLabel_;
  QLabel* positionLabel_;
  QDoubleSpinBox* velocitySpinBox_;
  QDoubleSpinBox* accelerationSpinBox_;
  QDoubleSpinBox* decelerationSpinBox_;
  QDoubleSpinBox* accelerationJerkSpinBox_;
  QDoubleSpinBox* decelerationJerkSpinBox_;
  QPushButton* buttonWriteParameter_;

  QString axisDimensionName_;

  bool motionTools_enabled_;
  bool motionSettings_locked_;

 public slots:

  void lStepStateChanged(State state);
  void controlStateChanged(bool);
  void updateWidgets();
  void updateMotionWidgets();
  void enabledCheckBoxToggled(bool enabled);
  void joystickCheckBoxToggled(bool enabled);
  void setVelocity(double value);
  void setAcceleration(double value);
  void setDeceleration(double value);
  void setAccelerationJerk(double value);
  void setDecelerationJerk(double value);
  void writeParameter();
  void motionStarted();
  void motionFinished();

  void  enableMotionTools(const bool enable=true);
  void disableMotionTools();

  void   lockMotionSettings(const bool disable=true);
  void unlockMotionSettings();
};

#endif // LSTEPEXPRESSWIDGET_H
