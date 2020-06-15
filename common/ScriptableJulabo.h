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

#ifndef SCRIPTABLEJULABO_H
#define SCRIPTABLEJULABO_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <JulaboModel.h>

/** @addtogroup common
 *  @{
 */

/** @addtogroup scripting
 *  @{
 */

class ScriptableJulabo : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableJulabo(JulaboModel* julaboModel, QObject *parent = 0);

public slots:

  void setPID( double p, int tv, int td );
  void enableCirculator();
  void disableCirculator();
  void setWorkingTemperature( double temperature );

  QScriptValue circulator();
  QScriptValue workingTemperature();
  QScriptValue bath();
  QScriptValue safety();

signals:
  void changeP( double p );
  void changeTv( int tv );
  void changeTd( int td );
  void switchCirculator( bool );
  void changeWorkingTemperature( double temperature );

protected:
  JulaboModel* julaboModel_;

  // For thread safety
  QMutex mutex_;
};

/** @} */

/** @} */

#endif // SCRIPTABLEJULABO_H
