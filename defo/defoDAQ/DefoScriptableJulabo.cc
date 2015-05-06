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
  connect(this, SIGNAL(changeTv(int)),
          julaboModel_,SLOT(setIntegralValue(int)));
  connect(this, SIGNAL(changeTd(int)),
          julaboModel_,SLOT(setDifferentialValue(int)));
  
  connect(this, SIGNAL(switchCirculator(bool)),
          julaboModel_, SLOT(setCirculatorEnabled(bool)));

  connect(this, SIGNAL(changeWorkingTemperature(double)),
          julaboModel_, SLOT(setWorkingTemperatureValue(double)));

  connect(this, SIGNAL(changePumpPressure(unsigned int)),
          julaboModel_, SLOT(setPumpPressureValue(unsigned int)));
}

void DefoScriptableJulabo::setP( double p ) {
  
  QMutexLocker locker(&mutex_);
  
  emit changeP(p);
}

void DefoScriptableJulabo::setI( int tv ) {
  
  QMutexLocker locker(&mutex_);
  
  emit changeTv(tv);
}

void DefoScriptableJulabo::setD( int td ) {

  QMutexLocker locker(&mutex_);
  
  emit changeTd(td);
}

void DefoScriptableJulabo::setPID( double p, int tv, int td ) {

  QMutexLocker locker(&mutex_);
  
  emit changeP(p);
  emit changeTv(tv);
  emit changeTd(td);
}

QScriptValue DefoScriptableJulabo::getP() {
  
  QMutexLocker locker(&mutex_);
  return QScriptValue(julaboModel_->getProportionalParameter().getValue());
}

QScriptValue DefoScriptableJulabo::getI() {
  
  QMutexLocker locker(&mutex_);
  return QScriptValue(julaboModel_->getIntegralParameter().getValue());
}

QScriptValue DefoScriptableJulabo::getD() {
  
  QMutexLocker locker(&mutex_);
  return QScriptValue(julaboModel_->getDifferentialParameter().getValue());
}

void DefoScriptableJulabo::enableCirculator() {

  QMutexLocker locker(&mutex_);
  emit switchCirculator( true );
}
void DefoScriptableJulabo::disableCirculator() {

  QMutexLocker locker(&mutex_);
  emit switchCirculator( false );
}

QScriptValue DefoScriptableJulabo::circulatorState() {
  
  QMutexLocker locker(&mutex_);
  return QScriptValue(julaboModel_->isCirculatorEnabled());
}

void DefoScriptableJulabo::setWorkingTemperature( double temperature ) {

  QMutexLocker locker(&mutex_);
  emit changeWorkingTemperature(temperature);
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

void DefoScriptableJulabo::setPumpPressure(unsigned int pressure) {

  QMutexLocker locker(&mutex_);
  emit changePumpPressure(pressure);
}

QScriptValue DefoScriptableJulabo::pumpPressure() {
  
  QMutexLocker locker(&mutex_);
  return QScriptValue(julaboModel_->getPumpPressureParameter().getValue());
}
