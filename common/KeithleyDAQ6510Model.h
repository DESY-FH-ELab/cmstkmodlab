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

#ifndef KEITHLEYDAQ6510MODEL_H
#define KEITHLEYDAQ6510MODEL_H

#include <vector>
#include <limits>
#include <chrono>

#include <QObject>
#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "Ringbuffer.h"

#ifdef USE_FAKEIO
#include "devices/Keithley/KeithleyDAQ6510Fake.h"
typedef KeithleyDAQ6510Fake KeithleyDAQ6510_t;
#else
#include "devices/Keithley/KeithleyDAQ6510.h"
typedef KeithleyDAQ6510 KeithleyDAQ6510_t;
#endif

class KeithleyDAQ6510Model : public QObject,
                             public AbstractDeviceModel<KeithleyDAQ6510_t>
{
    Q_OBJECT

public:
  static const unsigned int SENSOR_COUNT = 10;

  explicit KeithleyDAQ6510Model(const char* port,
                                int updateInterval = 60,
                                QObject *parent = 0);

  const State& getSensorState(unsigned int sensor) const;
  bool getScanState() const { return scanState_; }
  VKeithleyDAQ6510::ChannelMode_t getSensorMode(unsigned int sensor) const;
  const std::map<VKeithleyDAQ6510::ChannelMode_t,std::string>& getSensorModeNames() const;

  double getTemperature(unsigned int sensor) const;
  int getUpdateInterval() const { return updateInterval_; }

  void statusMessage(const QString & text);

public slots:

  void setDeviceEnabled(bool enabled);
  void setScanEnabled(bool enabled);
  void setSensorEnabled(unsigned int sensor, bool enabled);
  void setSensorMode(unsigned int sensor, VKeithleyDAQ6510::ChannelMode_t mode);
  void setControlsEnabled(bool enabled);
  void setUpdateInterval(int updateInterval);

protected:

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  QString port_;
  int updateInterval_;
  QTimer* timer_;
  bool scanState_;
  unsigned int scanDuration_;

  // cached config information
  std::array<std::array<State,10>,2> sensorStates_;
  std::array<std::array<double,10>,2> temperatures_;

  void setDeviceState( State state );
  void setSensorState( unsigned int sensor, State state );

  static std::string constructString(unsigned int sensor);

protected slots:

  virtual void scanTemperatures();
  virtual void scanComplete();

signals:

  void deviceStateChanged(State newState);
  void scanStateChanged(bool enabled);
  void sensorStateChanged(unsigned int sensor, State newState);
  void sensorModeChanged(unsigned int sensor, VKeithleyDAQ6510::ChannelMode_t newMode);
  void activeSensorCountChanged(unsigned int sensors);
  void temperatureChanged(unsigned int sensor, double temperature);
  void informationChanged();
  void temperatureGradientChanged(unsigned int sensor, double gradient);
  void message(const QString & text);
  void log(const QString & text);
  void controlStateChanged(bool);
};

#endif // KEITHLEYDAQ6510MODEL_H
