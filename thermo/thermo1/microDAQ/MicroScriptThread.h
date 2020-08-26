#ifndef MICROSCRIPTTHREAD_H
#define MICROSCRIPTTHREAD_H

#include <QObject>
#include <QThread>
#include <QScriptEngine>

class MicroScriptModel;

#include "IotaModel.h"
#include "ArduinoPresModel.h"

class MicroScriptThread : public QThread
{
public:
    explicit MicroScriptThread(MicroScriptModel* scriptModel,
                               IotaModel* iotaModel,
                               ArduinoPresModel* arduinoPresModel,
                               QObject *parent = 0);

    void executeScript(const QString & script);
    void abortScript();

protected:
    void run();

    QString script_;
    QScriptEngine* engine_;

    MicroScriptModel* scriptModel_;
    IotaModel* iotaModel_;
    ArduinoPresModel* arduinoPresModel_;
};

#endif // MICROSCRIPTTHREAD_H
