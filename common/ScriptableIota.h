#ifndef SCRIPTABLEIOTA_H
#define SCRIPTABLEIOTA_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <IotaModel.h>

class ScriptableIota : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableIota(IotaModel* petiteFleurModel,
                          QObject *parent = 0);

  public slots:

  void switchPumpOn();
  void switchPumpOff();
  void setPressure(float pressure);
  void setFlow(float flow);

  QScriptValue isPumpOn();
  QScriptValue getActPressure();
  QScriptValue getActFlow();
  QScriptValue getSetPressure();
  QScriptValue getSetFlow();

  void waitForPressureAbove(float pressure,
                            int timeout);
  void waitForPressureBelow(float pressure,
                            int timeout);
  void waitForStablePressure(float deltaP,
                             int delay,
                             int timeout);
  void waitForFlowAbove(float flow,
                        int timeout);
  void waitForFlowBelow(float flow,
                        int timeout);
  void waitForStableFlow(float deltaF,
                         int delay,
                         int timeout);

  protected:
  IotaModel* IotaModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLEIOTA_H
