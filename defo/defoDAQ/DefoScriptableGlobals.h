#ifndef DEFOSCRIPTABLEGLOBALS_H
#define DEFOSCRIPTABLEGLOBALS_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include "DefoScriptModel.h"

class DefoScriptableGlobals :
      public QObject
{
  Q_OBJECT

public:
  explicit DefoScriptableGlobals(DefoScriptModel* scriptModel,
				 QObject *parent = 0);

public slots:

  void wait(int seconds);
  void message(int value);
  void message(uint value);
  void message(double value);
  void message(const QString & text);
  void newMeasurement();

  QScriptValue uTime() const;
  QScriptValue eTime();
  QScriptValue mkUTime(int year, int month, int day,
                       int hour, int minute, int second) const;

protected:

  DefoScriptModel* scriptModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // DEFOSCRIPTABLEGLOBALS_H
