#include <QDebug>

#include "DefoScriptThread.h"
#include "DefoScriptableGlobals.h"
#include "DefoScriptableConrad.h"
#include "DefoScriptableCamera.h"
#include "DefoScriptableJulabo.h"
#include "DefoScriptableKeithley.h"

DefoScriptThread::DefoScriptThread(
    DefoScriptModel* scriptModel
  , DefoConradModel* conradModel
  , DefoCameraModel* cameraModel
  , DefoJulaboModel* julaboModel
  , DefoKeithleyModel* keithleyModel
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

  DefoScriptableGlobals *globalsObj = new DefoScriptableGlobals(scriptModel_, this);
  QScriptValue globalsValue = engine_->newQObject(globalsObj);
  engine_->globalObject().setProperty("defo", globalsValue);

  DefoScriptableConrad *conradObj = new DefoScriptableConrad(conradModel_, this);
  QScriptValue conradValue = engine_->newQObject(conradObj);
  engine_->globalObject().setProperty("conrad", conradValue);

  DefoScriptableCamera *cameraObj = new DefoScriptableCamera(cameraModel_, this);
  QScriptValue cameraValue = engine_->newQObject(cameraObj);
  engine_->globalObject().setProperty("camera", cameraValue);

  DefoScriptableJulabo *julaboObj = new DefoScriptableJulabo(julaboModel_, this);
  QScriptValue julaboValue = engine_->newQObject(julaboObj);
  engine_->globalObject().setProperty("julabo", julaboValue);

  DefoScriptableKeithley *keithleyObj = new DefoScriptableKeithley(keithleyModel_, this);
  QScriptValue keithleyValue = engine_->newQObject(keithleyObj);
  engine_->globalObject().setProperty("keithley", keithleyValue);

  start();
  // engine_->evaluate(script_);
  // qDebug() << script_;
}

void DefoScriptThread::abortScript() {
  if (engine_) engine_->abortEvaluation();
}

void DefoScriptThread::run() {

  if (engine_->canEvaluate(script_)) {
    engine_->evaluate(script_);
  } else {
    qDebug() << script_;
  }

  delete engine_;
  engine_ = 0;
}
