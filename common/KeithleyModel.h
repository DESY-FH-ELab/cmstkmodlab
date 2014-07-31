#ifndef KEITHLEYMODEL_H
#define KEITHLEYMODEL_H

#include <vector>
#include <limits>

#include <QObject>
#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "Ringbuffer.h"

#ifdef USE_FAKEIO
#include "devices/Keithley/Keithley2700Fake.h"
typedef Keithley2700Fake Keithley2700_t;
#else
#include "devices/Keithley/Keithley2700.h"
typedef Keithley2700 Keithley2700_t;
#endif

class KeithleyModel : public QObject,
                      public AbstractDeviceModel<Keithley2700_t>
{
    Q_OBJECT

public:
  static const unsigned int SENSOR_COUNT = 10;

  explicit KeithleyModel(const char* port,
                         int updateInterval = 60,
                         QObject *parent = 0);

  const State& getSensorState( unsigned int sensor ) const;
  double getTemperature( unsigned int sensor ) const;
  int getUpdateInterval() const { return updateInterval_; }

  void statusMessage(const QString & text);

public slots:

  void setDeviceEnabled( bool enabled );
  void setSensorEnabled( unsigned int sensor, bool enabled );
  void setControlsEnabled(bool enabled);
  void setUpdateInterval(int updateInterval);

protected:

  // FIXME Find out which port this needs to be or autoconfigure

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  QString port_;
  int updateInterval_;
  QTimer* timer_;

  // cached config information
  std::vector<State> sensorStates_;
  std::vector<double> temperatures_;
  std::vector<double> gradients_;

  Ringbuffer<double,4> timeBuffer_;
  Ringbuffer<std::vector<double>,4> temperatureBuffer_;
  double absoluteTime_;

  void setDeviceState( State state );
  void setSensorState( unsigned int sensor, State state );

  static std::string constructString( unsigned int sensor );

protected slots:
  void scanTemperatures();

signals:
  void deviceStateChanged(State newState);
  void sensorStateChanged(unsigned int sensor, State newState);
  void temperatureChanged(unsigned int sensor, double temperature);
  void temperatureGradientChanged(unsigned int sensor, double gradient);
  void message(const QString & text);
  void controlStateChanged(bool);
};

#endif // KEITHLEYMODEL_H
