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

#include <QDebug>

#include <nqlogger.h>

#include "ThermoScriptThread.h"
#include "ThermoScriptableGlobals.h"
#include "ScriptableHuberPetiteFleur.h"
#include "ScriptableKeithley.h"
#include "ScriptableHameg.h"
#include "ScriptablePfeiffer.h"
#include "ScriptableIota.h"
#include "ScriptableArduinoPres.h"

ThermoScriptThread::ThermoScriptThread(ThermoScriptModel* scriptModel,
                                       HuberPetiteFleurModel* huberModel,
                                       KeithleyModel* keithleyModel,
                                       HamegModel* hamegModel,
                                       PfeifferModel* pfeifferModel,
                                       IotaModel* iotaModel,
                                       ArduinoPresModel* arduinoPresModel,
                                       QObject *parent) :
    QThread(parent),
    scriptModel_(scriptModel),
    huberModel_(huberModel),
    keithleyModel_(keithleyModel),
    hamegModel_(hamegModel),
    pfeifferModel_(pfeifferModel),
    iotaModel_(iotaModel),
    arduinoPresModel_(arduinoPresModel)
{

}

void ThermoScriptThread::executeScript(const QString & script)
{
  script_ = script;

  engine_ = new QScriptEngine();
  engine_->setProcessEventsInterval(1000);

  ThermoScriptableGlobals *globalsObj = new ThermoScriptableGlobals(scriptModel_, this);
  QScriptValue globalsValue = engine_->newQObject(globalsObj);
  engine_->globalObject().setProperty("thermo", globalsValue);

  ScriptableHuberPetiteFleur *huberObj = new ScriptableHuberPetiteFleur(huberModel_, this);
  QScriptValue huberValue = engine_->newQObject(huberObj);
  engine_->globalObject().setProperty("huber", huberValue);

  ScriptableKeithley *keithleyObj = new ScriptableKeithley(keithleyModel_, this);
  QScriptValue keithleyValue = engine_->newQObject(keithleyObj);
  engine_->globalObject().setProperty("keithley", keithleyValue);

  ScriptableHameg *hamegObj = new ScriptableHameg(hamegModel_, this);
  QScriptValue hamegValue = engine_->newQObject(hamegObj);
  engine_->globalObject().setProperty("hameg", hamegValue);

  ScriptablePfeiffer *pfeifferObj = new ScriptablePfeiffer(pfeifferModel_, this);
  QScriptValue pfeifferValue = engine_->newQObject(pfeifferObj);
  engine_->globalObject().setProperty("pfeiffer", pfeifferValue);

  ScriptableIota *iotaObj = new ScriptableIota(iotaModel_, this);
  QScriptValue iotaValue = engine_->newQObject(iotaObj);
  engine_->globalObject().setProperty("iota", iotaValue);

  ScriptableArduinoPres *arduinoPresObj = new ScriptableArduinoPres(arduinoPresModel_, this);
  QScriptValue arduinoPresValue = engine_->newQObject(arduinoPresObj);
  engine_->globalObject().setProperty("arduino", arduinoPresValue);

  start();
}

void ThermoScriptThread::abortScript()
{
  NQLog("ThermoScriptThread") << "abort";
  if (engine_) {
    NQLog("ThermoScriptThread") << "abort " << (int)engine_->isEvaluating();
    engine_->abortEvaluation();
    //delete engine_;
    //engine_ = 0;
    //terminate();
  }
}

void ThermoScriptThread::run()
{
  if (engine_->canEvaluate(script_)) {
    QScriptValue fun = engine_->evaluate(script_);
    //QScriptContext * context = engine_->pushContext();
    //QScriptValue v = context->activationObject();
    //v.setProperty("fun", fun);
    //engine_->evaluate("fun()");
    //engine_->popContext();
    delete engine_;
    engine_ = 0;
  } else {
    qDebug() << script_;
  }
}
