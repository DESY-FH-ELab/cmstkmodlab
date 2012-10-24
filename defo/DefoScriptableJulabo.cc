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
  connect(this, SIGNAL(changeP(double)),
          julaboModel_,SLOT(setProportionalValue(double)));
  connect(this, SIGNAL(changeTv(double)),
          julaboModel_,SLOT(setIntegralValue(double)));
  connect(this, SIGNAL(changeTd(double)),
          julaboModel_,SLOT(setDifferentialValue(double)));
  
  connect(this, SIGNAL(switchCirculator(bool)),
          julaboModel_, SLOT(setCirculatorEnabled(bool)));

  connect(this, SIGNAL(changeWorkingTemperature(double)),
          julaboModel_, SLOT(setWorkingTemperatureValue(double)));
}

void DefoScriptableJulabo::setPID( double p, double tv, double td ) {

  QMutexLocker locker(&mutex_);
  
  emit changeP(p);
  emit changeTv(tv);
  emit changeTd(td);
}

void DefoScriptableJulabo::enableCirculator() {

  QMutexLocker locker(&mutex_);
  emit switchCirculator( true );
}
void DefoScriptableJulabo::disableCirculator() {

  QMutexLocker locker(&mutex_);
  emit switchCirculator( false );
}

void DefoScriptableJulabo::setWorkingTemperature( double temperature ) {

  QMutexLocker locker(&mutex_);
  emit changeWorkingTemperature(temperature);
}

QScriptValue DefoScriptableJulabo::circulator() {
  
  QMutexLocker locker(&mutex_);
  return QScriptValue(julaboModel_->isCirculatorEnabled());
}

QScriptValue DefoScriptableJulabo::workingTemperature() {
  
  QMutexLocker locker(&mutex_);
  return QScriptValue(julaboModel_->getWorkingTemperatureParameter().getValue());
}

QScriptValue DefoScriptableJulabo::bath() {
  
  QMutexLocker locker(&mutex_);
  return QScriptValue(julaboModel_->getBathTemperature());
}

QScriptValue DefoScriptableJulabo::safety() {
  
  QMutexLocker locker(&mutex_);
  return QScriptValue(julaboModel_->getSafetySensorTemperature());
}
