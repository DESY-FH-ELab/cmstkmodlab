#include <iostream>

#include <QMutexLocker>

#include "DefoScriptableConrad.h"

DefoScriptableConrad::DefoScriptableConrad(
    DefoScriptModel* scriptModel
  , DefoConradModel* conradModel
  , QObject *parent
) :
    QObject(parent)
  , scriptModel_(scriptModel)
  , conradModel_(conradModel)
{
  connect(this, SIGNAL(enableSwitch(DefoConradModel::DeviceSwitch)),
          conradModel_,SLOT(enableSwitch(DefoConradModel::DeviceSwitch)));
  connect(this, SIGNAL(disableSwitch(DefoConradModel::DeviceSwitch)),
          conradModel_,SLOT(disableSwitch(DefoConradModel::DeviceSwitch)));
}

void DefoScriptableConrad::enablePanel(unsigned int panel) {
  
  QMutexLocker locker(&mutex_);

  if (panel<1 || panel>5) return;

  QString message = QString("enable panel %1").arg(panel);
  scriptModel_->message(message);

  emit enableSwitch(static_cast<DefoConradModel::DeviceSwitch>(panel-1));
}

void DefoScriptableConrad::disablePanel(unsigned int panel) {
  
  QMutexLocker locker(&mutex_);

  if (panel<1 || panel>5) return;

  QString message = QString("disable panel %1").arg(panel);
  scriptModel_->message(message);

  emit disableSwitch(static_cast<DefoConradModel::DeviceSwitch>(panel-1));
}

QScriptValue DefoScriptableConrad::panel(unsigned int panel) {

  QMutexLocker locker(&mutex_);

  if (panel<1 || panel>5) return QScriptValue(0);

  int state = static_cast<int>(conradModel_->getSwitchState(static_cast<DefoConradModel::DeviceSwitch>(panel-1)));

  QString message = QString("state of panel %1 is %2").arg(panel).arg(state);
  scriptModel_->message(message);

  return QScriptValue(state);
}

void DefoScriptableConrad::enableCalibrationLEDs() {

  QMutexLocker locker(&mutex_);

  scriptModel_->message("enable calibration LEDs");

  emit enableSwitch(DefoConradModel::CALIBRATION_LEDS);
}

void DefoScriptableConrad::disableCalibrationLEDs() {

  QMutexLocker locker(&mutex_);

  scriptModel_->message("disable calibration LEDs");

  emit disableSwitch(DefoConradModel::CALIBRATION_LEDS);
}

QScriptValue DefoScriptableConrad::calibrationLEDs() {

  QMutexLocker locker(&mutex_);

  int state = static_cast<int>(conradModel_->getSwitchState(DefoConradModel::CALIBRATION_LEDS));

  QString message = QString("state of calibration LEDs is %1").arg(state);
  scriptModel_->message(message);

  return QScriptValue(state);
}
