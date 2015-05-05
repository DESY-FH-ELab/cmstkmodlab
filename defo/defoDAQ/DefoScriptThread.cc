#include <QDebug>

#include "DefoScriptThread.h"
#include "DefoScriptableGlobals.h"
#include "DefoScriptableConrad.h"
#include "DefoScriptableCamera.h"
#include "DefoScriptableJulabo.h"
#include "ScriptableKeithley.h"

DefoScriptThread::DefoScriptThread(
    DefoScriptModel* scriptModel
  , DefoConradModel* conradModel
  , DefoCameraModel* cameraModel
  , DefoJulaboModel* julaboModel
  , KeithleyModel* keithleyModel
  , QObject *parent
) :
    QThread(parent)
  , scriptModel_(scriptModel)
  , conradModel_(conradModel)
  , cameraModel_(cameraModel)
  , julaboModel_(julaboModel)
  , keithleyModel_(keithleyModel)
{

}

void DefoScriptThread::executeScript(const QString & script)
{
  script_ = script;

  engine_ = new QScriptEngine();
  engine_->setProcessEventsInterval(1000);

  DefoScriptableGlobals *globalsObj = new DefoScriptableGlobals(scriptModel_, this);
  QScriptValue globalsValue = engine_->newQObject(globalsObj);
  engine_->globalObject().setProperty("defo", globalsValue);

  /*
  DefoScriptableConrad *conradObj = new DefoScriptableConrad(scriptModel_, conradModel_, this);
  QScriptValue conradValue = engine_->newQObject(conradObj);
  engine_->globalObject().setProperty("conrad", conradValue);

  DefoScriptableCamera *cameraObj = new DefoScriptableCamera(scriptModel_, cameraModel_, this);
  QScriptValue cameraValue = engine_->newQObject(cameraObj);
  engine_->globalObject().setProperty("camera", cameraValue);

  DefoScriptableJulabo *julaboObj = new DefoScriptableJulabo(julaboModel_, this);
  QScriptValue julaboValue = engine_->newQObject(julaboObj);
  engine_->globalObject().setProperty("julabo", julaboValue);

  ScriptableKeithley *keithleyObj = new ScriptableKeithley(keithleyModel_, this);
  QScriptValue keithleyValue = engine_->newQObject(keithleyObj);
  engine_->globalObject().setProperty("keithley", keithleyValue);
  */

  start();
}

void DefoScriptThread::abortScript() {
  std::cout << "abort" << std::endl;
  if (engine_) {
    std::cout << "abort " << (int)engine_->isEvaluating() << std::endl;
    engine_->abortEvaluation();
    //delete engine_;
    //engine_ = 0;
    //terminate();
  }
}

void DefoScriptThread::run() {

  std::cout << script_.toStdString() << " " << engine_ << std::endl;

  if (engine_->canEvaluate(script_)) {

    std::cout << script_.toStdString() << " " << engine_ << std::endl;
    
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
