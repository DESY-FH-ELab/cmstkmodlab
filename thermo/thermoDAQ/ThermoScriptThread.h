#ifndef THERMOSCRIPTTHREAD_H
#define THERMOSCRIPTTHREAD_H

#include <QObject>
#include <QThread>
#include <QScriptEngine>

class ThermoScriptModel;

#include "HuberPetiteFleurModel.h"
#include "KeithleyModel.h"
#include "HamegModel.h"
#include "PfeifferModel.h"
#include "IotaModel.h"
#include "ArduinoPresModel.h"

class ThermoScriptThread : public QThread
{
public:
  explicit ThermoScriptThread(ThermoScriptModel* scriptModel,
                              HuberPetiteFleurModel* huberModel,
                              KeithleyModel* keithleyModel,
                              HamegModel* hamegModel,
                              PfeifferModel* pfeifferModel,
                              IotaModel* iotaModel,
                              ArduinoPresModel* arduinoPresModel,
                              QObject *parent = 0);

  void executeScript(const QString & script);
  void abortScript();

protected:
  void run();

  QString script_;
  QScriptEngine* engine_;

  ThermoScriptModel* scriptModel_;
  HuberPetiteFleurModel* huberModel_;
  KeithleyModel* keithleyModel_;
  HamegModel* hamegModel_;
  PfeifferModel* pfeifferModel_;
  IotaModel* iotaModel_;
  ArduinoPresModel* arduinoPresModel_;
};

#endif // THERMOSCRIPTTHREAD_H
