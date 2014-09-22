#ifndef MICROSCRIPTABLEGLOBALS_H
#define MICROSCRIPTABLEGLOBALS_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include "MicroScriptModel.h"

class MicroScriptableGlobals :
      public QObject
{
  Q_OBJECT

public:
  explicit MicroScriptableGlobals(MicroScriptModel* scriptModel, QObject *parent = 0);

public slots:

  void startMeasurement();
  void startDAQ() { startMeasurement(); }
  void stopMeasurement();
  void stopDAQ() { stopMeasurement(); }

  void wait(int seconds);
  void message(int value);
  void message(uint value);
  void message(double value);
  void message(const QString & text);
  void log(const QString & text);
  QScriptValue uTime() const;
  QScriptValue eTime();
  QScriptValue mkUTime(int year, int month, int day,
                       int hour, int minute, int second) const;

  void tweet(const QString& user, const QString& pw,
             const QString& message);

protected:

  MicroScriptModel* scriptModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // MICROSCRIPTABLEGLOBALS_H
