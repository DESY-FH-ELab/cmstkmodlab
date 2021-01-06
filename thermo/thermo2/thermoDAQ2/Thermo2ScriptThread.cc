/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <chrono>
#include <thread>

#include <QDebug>

#include <nqlogger.h>

#include "Thermo2ScriptThread.h"
#include "Thermo2ScriptableGlobals.h"

#include "ScriptableHuberUnistat525w.h"
#include "ScriptableMarta.h"
#include "ScriptableRohdeSchwarzNGE103B.h"
#include "ScriptableKeithleyDAQ6510.h"

Thermo2ScriptThread::Thermo2ScriptThread(Thermo2ScriptModel* scriptModel,
                                         HuberUnistat525wModel* huberModel,
																				 MartaModel* martaModel,
                                         RohdeSchwarzNGE103BModel* nge103BModel,
                                         KeithleyDAQ6510Model* keithleyModel,
                                         QObject *parent)
  : QThread(parent),
    scriptModel_(scriptModel),
    huberModel_(huberModel),
		martaModel_(martaModel),
    nge103BModel_(nge103BModel),
    keithleyModel_(keithleyModel)
{

}

void Thermo2ScriptThread::executeScript(const QString & script)
{
  script_ = script;

  engine_ = new QScriptEngine();
  engine_->setProcessEventsInterval(10);

  Thermo2ScriptableGlobals *globalsObj = new Thermo2ScriptableGlobals(scriptModel_, this);
  QScriptValue globalsValue = engine_->newQObject(globalsObj);
  engine_->globalObject().setProperty("thermo2", globalsValue);

  ScriptableHuberUnistat525w* huberobj = new ScriptableHuberUnistat525w(huberModel_, this);
  QScriptValue huberValue = engine_->newQObject(huberobj);
  engine_->globalObject().setProperty("huber", huberValue);

  ScriptableMarta* martaobj = new ScriptableMarta(martaModel_, this);
  QScriptValue martaValue = engine_->newQObject(martaobj);
  engine_->globalObject().setProperty("marta", martaValue);

  ScriptableRohdeSchwarzNGE103B *nge103BObj = new ScriptableRohdeSchwarzNGE103B(nge103BModel_, this);
  QScriptValue nge103BValue = engine_->newQObject(nge103BObj);
  engine_->globalObject().setProperty("nge103b", nge103BValue);

  ScriptableKeithleyDAQ6510 *keithleyObj = new ScriptableKeithleyDAQ6510(keithleyModel_, this);
  QScriptValue keithleyValue = engine_->newQObject(keithleyObj);
  engine_->globalObject().setProperty("daq6510", keithleyValue);

  start();
}

void Thermo2ScriptThread::abortScript()
{
  NQLogMessage("Thermo2ScriptThread") << "abortScript()";
  if (engine_) {
    NQLogMessage("Thermo2ScriptThread") << "abort " << (int)engine_->isEvaluating();
    engine_->abortEvaluation();
    //delete engine_;
    //engine_ = 0;
    //terminate();
  }
}

void Thermo2ScriptThread::run()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

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
