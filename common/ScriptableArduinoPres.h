#ifndef SCRIPTABLEARDUINOPRES_H
#define SCRIPTABLEARDUINOPRES_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <ArduinoPresModel.h>

/** @addtogroup common
 *  @{
 */

/** @addtogroup scripting
 *  @{
 */

class ScriptableArduinoPres : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableArduinoPres(ArduinoPresModel* arduinoPresModel,
                                 QObject *parent = 0);

  public slots:

  QScriptValue getPressureA();
  QScriptValue getPressureB();
  

  void waitForPressureAAbove(float pressure,int timeout);
  void waitForPressureBAbove(float pressure,int timeout);
  void waitForPressureABelow(float pressure,int timeout);
  void waitForPressureBBelow(float pressure,int timeout);
  void waitForStablePressureA(float deltaP,int delay,int timeout);
  void waitForStablePressureB(float deltaP,int delay,int timeout);


  protected:
  ArduinoPresModel* ArduinoPresModel_;

  // For thread safety
  QMutex mutex_;
};

/** @} */

/** @} */

#endif // SCRIPTABLEARDUINOPRES_H
