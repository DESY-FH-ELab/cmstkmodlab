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

#ifndef HAMEGMODEL_H
#define HAMEGMODEL_H

#include <cmath>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Hameg/Hameg8143Fake.h"
typedef Hameg8143Fake Hameg8143_t;
#else
#include "devices/Hameg/Hameg8143.h"
typedef Hameg8143 Hameg8143_t;
#endif

/** @addtogroup common
 *  @{
 */

/** @addtogroup models
 *  @{
 */

/**
  Command and control model of the Hameg chiller.
  */
class HamegModel :
    public QObject
  , public AbstractDeviceModel<Hameg8143_t>
{

  Q_OBJECT
public:
  explicit HamegModel(const char* port,
		      float updateInterval = 5,
		      QObject *parent = 0);

  unsigned int getStatus() const { return status_; }
  bool isRemoteMode() const { return status_&VHameg8143::hmRM1; }
  bool isOutputEnabled() const { return status_&VHameg8143::hmOP1; }
  bool isConstantVoltageMode(int channel) const;

  const DeviceParameterFloat& getVoltageParameter(int channel) const;
  const DeviceParameterFloat& getCurrentParameter(int channel) const;

  float getVoltage(int channel) const;
  float getCurrent(int channel) const;

public slots:
  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);

  void setRemoteMode(bool remote);
  void switchOutputOn(bool on);
  void setVoltage(int channel, float voltage);
  void setCurrent(int channel, float current);

protected:

  const QString Hameg_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;
  bool forceRemoteMode_;

  void setDeviceState( State state );

  unsigned int status_;
  DeviceParameterFloat voltage1Parameter_;
  DeviceParameterFloat current1Parameter_;
  DeviceParameterFloat voltage2Parameter_;
  DeviceParameterFloat current2Parameter_;

  float voltage_[2];
  float current_[2];

protected slots:
  void updateInformation();

signals:
  void deviceStateChanged(State newState);
  void informationChanged();
  void message(const QString & text);
  void controlStateChanged(bool);
};

/** @} */

/** @} */

#endif // HAMEGMODEL_H
