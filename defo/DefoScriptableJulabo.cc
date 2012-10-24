#include <iostream>

#include <QMutexLocker>

#include "DefoScriptableJulabo.h"

DefoScriptableJulabo::DefoScriptableJulabo(
    DefoJulaboModel* julaboModel
  , QObject *parent
) :
    QObject(parent)
  , julaboModel_(julaboModel)
{
  //connect(this, SIGNAL(enableSwitch(DefoJulaboModel::DeviceSwitch)),
  //       julaboModel_,SLOT(enableSwitch(DefoJulaboModel::DeviceSwitch)));
}

/*
void DefoScriptableJulabo::enablePanel(int panel) {
  
  QMutexLocker locker(&mutex_);

  if (panel<1 || panel>5) return;

  emit enableSwitch(static_cast<DefoJulaboModel::DeviceSwitch>(panel-1));
}

void DefoScriptableJulabo::disablePanel(int panel) {
  
  QMutexLocker locker(&mutex_);

  if (panel<1 || panel>5) return;

  emit disableSwitch(static_cast<DefoJulaboModel::DeviceSwitch>(panel-1));
}

QScriptValue DefoScriptableJulabo::panel(int panel) {

  QMutexLocker locker(&mutex_);

  if (panel<1 || panel>5) return QScriptValue(0);

  int state = static_cast<int>(julaboModel_->getSwitchState(static_cast<DefoJulaboModel::DeviceSwitch>(panel-1)));
  return QScriptValue(state);
}

void DefoScriptableJulabo::enableCalibrationLEDs() {

  QMutexLocker locker(&mutex_);

  emit enableSwitch(DefoJulaboModel::CALIBRATION_LEDS);
}

void DefoScriptableJulabo::disableCalibrationLEDs() {

  QMutexLocker locker(&mutex_);

  emit disableSwitch(DefoJulaboModel::CALIBRATION_LEDS);
}

QScriptValue DefoScriptableJulabo::calibrationLEDs() {

  QMutexLocker locker(&mutex_);

  int state = static_cast<int>(julaboModel_->getSwitchState(DefoJulaboModel::CALIBRATION_LEDS));
  return QScriptValue(state);
}

*/
