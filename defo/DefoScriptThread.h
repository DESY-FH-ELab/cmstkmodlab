#ifndef DEFOSCRIPTTHREAD_H
#define DEFOSCRIPTTHREAD_H

#include <QObject>
#include <QThread>
#include <QScriptEngine>

class DefoScriptModel;

#include "DefoCameraModel.h"

class DefoScriptThread : 
      public QThread
{
public:
  explicit DefoScriptThread(
      DefoScriptModel* scriptModel
    , DefoCameraModel* cameraModel
    , QObject *parent = 0
  );

  void executeScript(const QString & script);
  void abortScript();
protected:
  void run();

  QString script_;
  QScriptEngine* engine_;

  DefoScriptModel* scriptModel_;
  DefoCameraModel* cameraModel_;
};

#endif // DEFOSCRIPTTHREAD_H
