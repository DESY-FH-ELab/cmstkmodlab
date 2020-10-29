/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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
