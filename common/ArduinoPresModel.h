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

#ifndef ARDUINOPRESMODEL_H
#define ARDUINOPRESMODEL_H

#include <cmath>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Arduino/ArduinoPresFake.h"
typedef ArduinoPresFake ArduinoPres_t;
#else
#include "devices/Arduino/ArduinoPres.h"
typedef ArduinoPres ArduinoPres_t;
#endif

/** @addtogroup common
 *  @{
 */

/** @addtogroup models
 *  @{
 */

class ArduinoPresModel : public QObject,
                         public AbstractDeviceModel<ArduinoPres_t>
{
  Q_OBJECT
public:
  explicit ArduinoPresModel(const char* port,
                            float updateInterval = 2,
                            QObject *parent = 0);

  float getPressureA() const;
  float getPressureB() const;

  public slots:

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);
  
  protected:

  const QString ArduinoPres_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  void setDeviceState( State state );

  template <class T> void updateParameterCache(DeviceParameter<T>& parameter,
                                               const T& value);

  float PressureA_;
  float PressureB_;

  protected slots:

  void updateInformation();

  signals:

  void deviceStateChanged( State newState );
  void informationChanged();
  void message(const QString & text);
  void controlStateChanged(bool);
};

/** @} */

/** @} */

#endif // ARDUINOPRESMODEL_H
