#ifndef LEYBOLDGRAPHIXTHREEMODEL_H
#define LEYBOLDGRAPHIXTHREEMODEL_H

#include <cmath>
#include <map>
#include <array>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Leybold/LeyboldGraphixThreeFake.h"
typedef LeyboldGraphixThreeFake LeyboldGraphixThree_t;
#else
#include "devices/Leybold/LeyboldGraphixThree.h"
typedef LeyboldGraphixThree LeyboldGraphixThree_t;
#endif

/**
  Command and control model of the Leybold vacuum controller.
  */
class LeyboldGraphixThreeModel : public QObject, public AbstractDeviceModel<LeyboldGraphixThree_t>
{

  Q_OBJECT
public:

  explicit LeyboldGraphixThreeModel(const char* port,
                                    double updateInterval = 5,
                                    QObject *parent = 0);

  LeyboldGraphixThree_t::SensorStatus getSensorStatus(int sensor) const;
  double getPressure(int sensor) const;

  LeyboldGraphixThree_t::DisplayUnit getDisplayUnit() const;
  void setDisplayUnit(LeyboldGraphixThree_t::DisplayUnit unit);

public slots:

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);

  void updateInformation();

protected:

  const QString LeyboldGraphixThree_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  void setDeviceState(State state);

  std::array<LeyboldGraphixThree_t::SensorStatus,3> status_;
  std::array<double,3> pressure_;
  LeyboldGraphixThree_t::DisplayUnit displayUnit_;

signals:

  void deviceStateChanged(State newState);
  void informationChanged();
  void controlStateChanged(bool);
};

#endif // LEYBOLDGRAPHIXTHREEMODEL_H
