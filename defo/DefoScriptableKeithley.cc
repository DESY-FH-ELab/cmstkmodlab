#include <iostream>

#include <QMutexLocker>

#include "DefoScriptableKeithley.h"

DefoScriptableKeithley::DefoScriptableKeithley(
    DefoKeithleyModel* keithleyModel
  , QObject *parent
) :
    QObject(parent)
  , keithleyModel_(keithleyModel)
{
  //connect(this, SIGNAL(enableSwitch(DefoKeithleyModel::DeviceSwitch)),
  //       keithleyModel_,SLOT(enableSwitch(DefoKeithleyModel::DeviceSwitch)));
}

/*
void DefoScriptableKeithley::enablePanel(int panel) {
  
  QMutexLocker locker(&mutex_);

  if (panel<1 || panel>5) return;

  emit enableSwitch(static_cast<DefoKeithleyModel::DeviceSwitch>(panel-1));
}

void DefoScriptableKeithley::disablePanel(int panel) {
  
  QMutexLocker locker(&mutex_);

  if (panel<1 || panel>5) return;

  emit disableSwitch(static_cast<DefoKeithleyModel::DeviceSwitch>(panel-1));
}

QScriptValue DefoScriptableKeithley::panel(int panel) {

  QMutexLocker locker(&mutex_);

  if (panel<1 || panel>5) return QScriptValue(0);

  int state = static_cast<int>(keithleyModel_->getSwitchState(static_cast<DefoKeithleyModel::DeviceSwitch>(panel-1)));
  return QScriptValue(state);
}

void DefoScriptableKeithley::enableCalibrationLEDs() {

  QMutexLocker locker(&mutex_);

  emit enableSwitch(DefoKeithleyModel::CALIBRATION_LEDS);
}

void DefoScriptableKeithley::disableCalibrationLEDs() {

  QMutexLocker locker(&mutex_);

  emit disableSwitch(DefoKeithleyModel::CALIBRATION_LEDS);
}

QScriptValue DefoScriptableKeithley::calibrationLEDs() {

  QMutexLocker locker(&mutex_);

  int state = static_cast<int>(keithleyModel_->getSwitchState(DefoKeithleyModel::CALIBRATION_LEDS));
  return QScriptValue(state);
}

*/
