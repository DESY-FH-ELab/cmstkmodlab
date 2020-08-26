#include <QDebug>

#include <nqlogger.h>

#include "MicroScriptThread.h"
#include "MicroScriptableGlobals.h"
#include "ScriptableIota.h"
#include "ScriptableArduinoPres.h"

MicroScriptThread::MicroScriptThread(MicroScriptModel* scriptModel,
                                     IotaModel* iotaModel,
                                     ArduinoPresModel* arduinoPresModel,
                                     QObject *parent) :
    QThread(parent),
    scriptModel_(scriptModel),
    iotaModel_(iotaModel),
    arduinoPresModel_(arduinoPresModel)
{

}

void MicroScriptThread::executeScript(const QString & script)
{
    script_ = script;

    engine_ = new QScriptEngine();
    engine_->setProcessEventsInterval(1000);

    MicroScriptableGlobals *globalsObj = new MicroScriptableGlobals(scriptModel_, this);
    QScriptValue globalsValue = engine_->newQObject(globalsObj);
    engine_->globalObject().setProperty("micro", globalsValue);

    ScriptableIota *iotaObj = new ScriptableIota(iotaModel_, this);
    QScriptValue iotaValue = engine_->newQObject(iotaObj);
    engine_->globalObject().setProperty("iota", iotaValue);

    ScriptableArduinoPres *arduinoPresObj = new ScriptableArduinoPres(arduinoPresModel_, this);
    QScriptValue arduinoPresValue = engine_->newQObject(arduinoPresObj);
    engine_->globalObject().setProperty("arduino", arduinoPresValue);

    start();
}

void MicroScriptThread::abortScript()
{
    NQLog("MicroScriptThread") << "abort";
    if (engine_) {
        NQLog("MicroScriptThread") << "abort " << (int)engine_->isEvaluating();
        engine_->abortEvaluation();
        //delete engine_;
        //engine_ = 0;
        //terminate();
    }
}

void MicroScriptThread::run()
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
