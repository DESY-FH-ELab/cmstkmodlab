/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef SCRIPTABLEPFEIFFER_H
#define SCRIPTABLEPFEIFFER_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <PfeifferModel.h>

/** @addtogroup common
 *  @{
 */

/** @addtogroup scripting
 *  @{
 */

class ScriptablePfeiffer : public QObject
{
  Q_OBJECT
public:
  explicit ScriptablePfeiffer(PfeifferModel* pfeifferModel, QObject *parent = 0);

public slots:

//  QScriptValue state(unsigned int channel);
//  QScriptValue temperature(unsigned int channel);

protected:
  PfeifferModel* pfeifferModel_;

  // For thread safety
  QMutex mutex_;
};

/** @} */

/** @} */

#endif // SCRIPTABLEPFEIFFER_H
