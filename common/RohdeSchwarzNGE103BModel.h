/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef ROHDESCHWARZNGE103BMODEL_H
#define ROHDESCHWARZNGE103BMODEL_H

#include <cmath>
#include <array>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/RohdeSchwarz/RohdeSchwarzNGE103BFake.h"
typedef RohdeSchwarzNGE103BFake RohdeSchwarzNGE103B_t;
#else
#include "devices/RohdeSchwarz/RohdeSchwarzNGE103B.h"
typedef RohdeSchwarzNGE103B RohdeSchwarzNGE103B_t;
#endif

/**
  Command and control model of the RohdeSchwarzNGE103B power supply.
  */
class RohdeSchwarzNGE103BModel :
    public QObject
  , public AbstractDeviceModel<RohdeSchwarzNGE103B_t>
{
  Q_OBJECT
public:
  explicit RohdeSchwarzNGE103BModel(const char* port,
		      float updateInterval = 5,
		      QObject *parent = 0);

  bool getOutputState(int channel) const;
  unsigned int getOutputMode(int channel) const;
  float getVoltage(int channel) const;
  float getMeasuredVoltage(int channel) const;
  float getCurrent(int channel) const;
  float getMeasuredCurrent(int channel) const;
  float getEasyRampDuration(int channel) const;
  bool getEasyRampState(int channel) const;

public slots:
  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);

  void setOutputState(int channel, bool state);
  void setVoltage(int channel, float voltage);
  void setCurrent(int channel, float current);
  void setEasyRampDuration(int channel, float duration);
  void setEasyRampState(int channel, bool state);

protected:

  const QString RohdeSchwarzNGE103B_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  void setDeviceState( State state );

  DeviceParameterFloat voltageParameter_;
  DeviceParameterFloat currentParameter_;
  DeviceParameterFloat easyRampDurationParameter_;

  std::array<bool,3> outputState_;
  std::array<unsigned int,3> outputMode_;
  std::array<float,3> voltage_;
  std::array<float,3> measuredVoltage_;
  std::array<float,3> current_;
  std::array<float,3> measuredCurrent_;
  std::array<float,3> easyRampDuration_;
  std::array<bool,3> easyRampState_;

protected slots:
  void updateInformation();

signals:
  void deviceStateChanged(State newState);
  void informationChanged();
  void message(const QString & text);
  void log(const QString & text);
  void controlStateChanged(bool);
};

#endif // ROHDESCHWARZNGE103BMODEL_H
