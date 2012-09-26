#include <QDebug>

#include "DefoScriptThread.h"
#include "DefoScriptableGlobals.h"
#include "DefoScriptableCamera.h"

DefoScriptThread::DefoScriptThread(
    DefoScriptModel* scriptModel
  , DefoCameraModel* cameraModel
  , QObject *parent
) :
    QThread(parent)
  , scriptModel_(scriptModel)
  , cameraModel_(cameraModel)
{

}

void DefoScriptThread::executeScript(const QString & script)
{
  script_ = script;

  engine_ = new QScriptEngine();

  DefoScriptableGlobals *globalsObj = new DefoScriptableGlobals(scriptModel_, this);
  QScriptValue globalsValue = engine_->newQObject(globalsObj);
  engine_->globalObject().setProperty("defo", globalsValue);

  DefoScriptableCamera *cameraObj = new DefoScriptableCamera(cameraModel_, this);
  QScriptValue cameraValue = engine_->newQObject(cameraObj);
  engine_->globalObject().setProperty("camera", cameraValue);
  
  start();
}

void DefoScriptThread::abortScript() {

  if (engine_) engine_->abortEvaluation();
}

void DefoScriptThread::run() {

  qDebug() << "void DefoScriptThread::run()";
  qDebug() << script_;

  engine_->evaluate(script_);

  delete engine_;
  engine_ = 0;
}
