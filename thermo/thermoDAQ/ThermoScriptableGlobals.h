#ifndef THERMOSCRIPTABLEGLOBALS_H
#define THERMOSCRIPTABLEGLOBALS_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include "ThermoScriptModel.h"

class ThermoScriptableGlobals :
      public QObject
{
  Q_OBJECT

public:
  explicit ThermoScriptableGlobals(ThermoScriptModel* scriptModel, QObject *parent = 0);

public slots:
  //void newMeasurement();
  void wait(int seconds);
  void message(int value);
  void message(uint value);
  void message(double value);
  void message(const QString & text);
  QScriptValue uTime() const;
  QScriptValue eTime();
  QScriptValue mkUTime(int year, int month, int day,
                       int hour, int minute, int second) const;

protected:

  ThermoScriptModel* scriptModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // THERMOSCRIPTABLEGLOBALS_H
