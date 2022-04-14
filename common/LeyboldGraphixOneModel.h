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

#ifndef LEYBOLDGRAPHIXONEMODEL_H
#define LEYBOLDGRAPHIXONEMODEL_H

#include <cmath>
#include <map>
#include <array>

#include <QString>
#include <QTimer>
#include <QDateTime>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Leybold/LeyboldGraphixOneFake.h"
typedef LeyboldGraphixOneFake LeyboldGraphixOne_t;
#else
#include "devices/Leybold/LeyboldGraphixOne.h"
typedef LeyboldGraphixOne LeyboldGraphixOne_t;
#endif

/**
  Command and control model of the Leybold vacuum controller.
  */
class LeyboldGraphixOneModel : public QObject, public AbstractDeviceModel<LeyboldGraphixOne_t>
{

  Q_OBJECT
public:

  explicit LeyboldGraphixOneModel(const char* port,
                                  double updateInterval = 5,
                                  QObject *parent = 0);

  LeyboldGraphixOne_t::SensorDetectionMode getSensorDetectionMode() const;
  void setSensorDetectionMode(LeyboldGraphixOne_t::SensorDetectionMode mode);

  LeyboldGraphixOne_t::SensorType getSensorType() const;
  void setSensorType(LeyboldGraphixOne_t::SensorType type);

  const std::string getSensorName() const;
  void setSensorName(const std::string& name);

  LeyboldGraphixOne_t::SensorStatus getSensorStatus() const;
  const std::string getSensorStatusText(LeyboldGraphixOne_t::SensorStatus status) const;
  double getPressure() const;

  LeyboldGraphixOne_t::DisplayUnit getDisplayUnit() const;
  void setDisplayUnit(LeyboldGraphixOne_t::DisplayUnit unit);

  const QDateTime getDateTime() const;
  void setDateTime(const QDateTime& dt);

  void statusMessage(const QString & text);

public slots:

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);

  void updateInformation();

protected:

  const QString LeyboldGraphixOne_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  void setDeviceState(State state);

  LeyboldGraphixOne_t::SensorStatus status_;
  double pressure_;
  LeyboldGraphixOne_t::DisplayUnit displayUnit_;

signals:

  void deviceStateChanged(State newState);
  void informationChanged();
  void message(const QString & text);
  void log(const QString & text);
  void controlStateChanged(bool);
};

#endif // LEYBOLDGRAPHIXONEMODEL_H
