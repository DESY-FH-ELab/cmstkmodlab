#ifndef SCRIPTABLECORIFLOW_H
#define SCRIPTABLECORIFLOW_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <CoriFlowModel.h>

/** @addtogroup common
 *  @{
 */

/** @addtogroup scripting
 *  @{
 */

class ScriptableCoriFlow : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableCoriFlow(CoriFlowModel* coriFlowModel,
                                 QObject *parent = 0);

  public slots:

  QScriptValue getTemp();

  void waitForTempBelow(float temperature,
                             int timeout);


  protected:
  CoriFlowModel* CoriFlowModel_;

  // For thread safety
  QMutex mutex_;
};

/** @} */

/** @} */

#endif // SCRIPTABLEARDUINOPRES_H
