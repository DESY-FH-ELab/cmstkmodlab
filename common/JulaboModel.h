/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef JULABOMODEL_H
#define JULABOMODEL_H

#include <cmath>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Julabo/JulaboFP50Fake.h"
typedef JulaboFP50Fake JulaboFP50_t;
#else
#include "devices/Julabo/JulaboFP50.h"
typedef JulaboFP50 JulaboFP50_t;
#endif

/** @addtogroup common
 *  @{
 */

/** @addtogroup models
 *  @{
 */

/**
  Command and control model of the Julabo chiller.
  */
class JulaboModel :
    public QObject
  , public AbstractDeviceModel<JulaboFP50_t>
{

  Q_OBJECT
public:
  explicit JulaboModel(float updateInterval = 5, QObject *parent = 0);

  const DeviceParameterFloat& getProportionalParameter() const;
  const DeviceParameterUInt& getIntegralParameter() const;
  const DeviceParameterUInt& getDifferentialParameter() const;

  bool isCirculatorEnabled() const;

  const DeviceParameterUInt& getPumpPressureParameter() const;
  const DeviceParameterFloat& getWorkingTemperatureParameter() const;
  float getBathTemperature() const;
  float getSafetySensorTemperature() const;
  unsigned int getPower() const;

public slots:
  void setDeviceEnabled( bool enabled );
  void setControlsEnabled(bool enabled);

  void setProportionalValue( double value );
  void setIntegralValue( int value );
  void setDifferentialValue( int value );

  void setCirculatorEnabled( bool enabled );

  void setPumpPressureValue( int pressure );

  void setWorkingTemperatureValue( double temperature );

protected:

  static const QString JULABO_PORT;

//  VJulaboFP50* controller_;
//  void renewController( const QString& port );
  void initialize();
//  void close();

  /// Time interval between cache refreshes; in seconds.
  const float updateInterval_;
  QTimer* timer_;

  // cached config information
//  State state_;
  void setDeviceState( State state );

  template <class T> void updateParameterCache(
      DeviceParameter<T>& parameter
    , const T& value
  );

  DeviceParameterFloat proportional_; ///< Proportional range >Xp<; units K
  DeviceParameterUInt integral_; ///< Resetting time >Tn<; units s
  DeviceParameterUInt differential_; ///< Lead time >Tv<; units s

  bool circulatorEnabled_;
  DeviceParameterUInt pumpPressure_; ///< Pump pressure stage; no units
  DeviceParameterFloat workingTemperature_; ///< Working temperature; units °C

  float bathTemperature_; ///< Current cooling bath temperature; units °C
  float safetySensorTemperature_;
  unsigned int power_; ///< Current fraction of maximum power; units %

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

#endif // JULABOMODEL_H
