#ifndef THERMOSCRIPTTHREAD_H
#define THERMOSCRIPTTHREAD_H

#include <QObject>
#include <QThread>
#include <QScriptEngine>

class ThermoScriptModel;

#include "JulaboModel.h"
#include "KeithleyModel.h"
#include "HamegModel.h"
#include "PfeifferModel.h"

class ThermoScriptThread : public QThread
{
public:
  explicit ThermoScriptThread(ThermoScriptModel* scriptModel,
                              JulaboModel* julaboModel,
                              KeithleyModel* keithleyModel,
                              HamegModel* hamegModel,
                              PfeifferModel* pfeifferModel,
                              QObject *parent = 0);

  void executeScript(const QString & script);
  void abortScript();

protected:
  void run();

  QString script_;
  QScriptEngine* engine_;

  ThermoScriptModel* scriptModel_;
  JulaboModel* julaboModel_;
  KeithleyModel* keithleyModel_;
  HamegModel* hamegModel_;
  PfeifferModel* pfeifferModel_;
};

#endif // THERMOSCRIPTTHREAD_H
