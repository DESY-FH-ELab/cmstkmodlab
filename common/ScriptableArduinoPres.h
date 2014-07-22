#ifndef SCRIPTABLEARDUINOPRES_H
#define SCRIPTABLEARDUINOPRES_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <ArduinoPresModel.h>

class ScriptableArduinoPres : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableArduinoPres(ArduinoPresModel* arduinoPresModel,
                                      QObject *parent = 0);

public slots:

  QScriptValue getPressureA();
  QScriptValue getPressureB();

  void waitForPressureAbove(float pressure,
                               int timeout);
  void waitForPressureBelow(float pressure,
                               int timeout);
  void waitForStablePressure(float deltaP,
                                int delay,
                                int timeout);

protected:
  ArduinoPresModel* ArduinoPresModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLEARDUINOPRES_H
