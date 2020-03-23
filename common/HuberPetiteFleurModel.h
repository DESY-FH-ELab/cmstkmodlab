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

#ifndef HUBERPETITEFLEURMODEL_H
#define HUBERPETITEFLEURMODEL_H

#include <cmath>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Huber/HuberPetiteFleurFake.h"
typedef HuberPetiteFleurFake HuberPetiteFleur_t;
#else
#include "devices/Huber/HuberPetiteFleur.h"
typedef HuberPetiteFleur HuberPetiteFleur_t;
#endif

/** @addtogroup common
 *  @{
 */

/** @addtogroup models
 *  @{
 */

/**
  Command and control model of the petiteFleur chiller.
  */
class HuberPetiteFleurModel : public QObject,
                              public AbstractDeviceModel<HuberPetiteFleur_t>
{
  Q_OBJECT
public:
  explicit HuberPetiteFleurModel(const char* port,
				 float updateInterval = 5,
				 QObject *parent = 0);

    bool isCirculatorEnabled() const;
    const DeviceParameterFloat& getWorkingTemperatureParameter() const;
    float getBathTemperature() const;

public slots:

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);

  void setCirculatorEnabled( bool enabled );

  void setWorkingTemperatureValue( double temperature );

protected:

  const QString HuberPetiteFleur_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  void setDeviceState( State state );

  template <class T> void updateParameterCache(DeviceParameter<T>& parameter,
					       const T& value);

  bool circulatorEnabled_;
  DeviceParameterFloat workingTemperature_; ///< Working temperature; units °C
  float bathTemperature_; ///< Current cooling bath temperature; units °C

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

#endif // HUBERPETITEFLEURMODEL_H
