#include <iostream>

#include <QMutexLocker>

#include "ScriptableJulabo.h"

ScriptableJulabo::ScriptableJulabo(JulaboModel* julaboModel, QObject *parent) :
    QObject(parent),
    julaboModel_(julaboModel)
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
}

void ScriptableJulabo::setPID( double p, int tv, int td ) {

  QMutexLocker locker(&mutex_);
  
  emit changeP(p);
  emit changeTv(tv);
  emit changeTd(td);
}

void ScriptableJulabo::enableCirculator() {

  QMutexLocker locker(&mutex_);
  emit switchCirculator( true );
}
void ScriptableJulabo::disableCirculator() {

  QMutexLocker locker(&mutex_);
  emit switchCirculator( false );
}

void ScriptableJulabo::setWorkingTemperature( double temperature ) {

  QMutexLocker locker(&mutex_);
  emit changeWorkingTemperature(temperature);
}

QScriptValue ScriptableJulabo::circulator() {
  
  QMutexLocker locker(&mutex_);
  return QScriptValue(julaboModel_->isCirculatorEnabled());
}

QScriptValue ScriptableJulabo::workingTemperature() {
  
  QMutexLocker locker(&mutex_);
  return QScriptValue(julaboModel_->getWorkingTemperatureParameter().getValue());
}

QScriptValue ScriptableJulabo::bath() {
  
  QMutexLocker locker(&mutex_);
  return QScriptValue(julaboModel_->getBathTemperature());
}

QScriptValue ScriptableJulabo::safety() {
  
  QMutexLocker locker(&mutex_);
  return QScriptValue(julaboModel_->getSafetySensorTemperature());
}
