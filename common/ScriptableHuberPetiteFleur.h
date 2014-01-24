#ifndef SCRIPTABLEHUBERPETITEFLEUR_H
#define SCRIPTABLEHUBERPETITEFLEUR_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <HuberPetiteFleurModel.h>

class ScriptableHuberPetiteFleur : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableHuberPetiteFleur(HuberPetiteFleurModel* petiteFleurModel,
                                      QObject *parent = 0);

public slots:

  void switchCirculatorOn();
  void switchCirculatorOff();
  void setWorkingTemperature(double temperature);

  QScriptValue isCirculatorOn();
  QScriptValue getWorkingTemperature();
  QScriptValue getBathTemperature();

  void waitForTemperatureAbove(float temperature,
                               int timeout);
  void waitForTemperatureBelow(float temperature,
                               int timeout);
  void waitForStableTemperature(float deltaT,
                                int delay,
                                int timeout);

protected:
  HuberPetiteFleurModel* huberPetiteFleurModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLEHUBERPETITEFLEUR_H
