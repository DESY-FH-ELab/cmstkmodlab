#ifndef DEFOSCRIPTTHREAD_H
#define DEFOSCRIPTTHREAD_H

#include <QObject>
#include <QThread>
#include <QScriptEngine>

class DefoScriptModel;

#include "DefoConradModel.h"
#include "DefoCameraModel.h"
#include "DefoJulaboModel.h"
#include "DefoKeithleyModel.h"

class DefoScriptThread : 
      public QThread
{
public:
  explicit DefoScriptThread(
      DefoScriptModel* scriptModel
    , DefoConradModel* conradModel
    , DefoCameraModel* cameraModel
    , DefoJulaboModel* julaboModel
    , DefoKeithleyModel* keithleyModel
    , QObject *parent = 0
  );

  void executeScript(const QString & script);
  void abortScript();
protected:
  void run();

  QString script_;
  QScriptEngine* engine_;

  DefoScriptModel* scriptModel_;
  DefoConradModel* conradModel_;
  DefoCameraModel* cameraModel_;
  DefoJulaboModel* julaboModel_;
  DefoKeithleyModel* keithleyModel_;
};

#endif // DEFOSCRIPTTHREAD_H
