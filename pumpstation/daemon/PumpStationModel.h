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

#ifndef PUMPSTATIONMODEL_H
#define PUMPSTATIONMODEL_H

#include <array>
#include <vector>

#include <QTimer>

#include <ConradModel.h>
#include <LeyboldGraphixThreeModel.h>

class PumpStationModel : public QObject
{

  Q_OBJECT
public:

  explicit PumpStationModel(ConradModel* conradModel,
                            LeyboldGraphixThreeModel* leyboldModel,
                            double updateInterval = 5,
                            QObject *parent = 0);

  bool getSwitchBlocked(int channel) const;
  const State& getSwitchState(int channel) const;

  std::string getSensorName(int sensor) const;
  int getSensorStatus(int sensor) const;
  double getPressure(int sensor) const;

  double getPumpOperatingHours(int pump) const;

  int getPumpChannel(int pump) const;
  int getValveChannel(int valve) const;

public slots:

  void setSwitchBlocked(int, bool);
  void setSwitchEnabled(int, bool);
  void setPumpOperatingHours(int, double);

protected slots:

  void updateInformation();
  void updateConrad();

  void pump1HeartBeat();
  void pump2HeartBeat();

protected:

  ConradModel* conradModel_;
  LeyboldGraphixThreeModel* leyboldModel_;
  bool conradDataValid_;
  bool leyboldDataValid_;
  bool dataValid_;
  
  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  double heartBeat_;
  QTimer* pump1timer_;
  QTimer* pump2timer_;

  std::array<bool,5> switchBlocked_;
  std::array<State,5> switchState_;

  std::array<LeyboldGraphixThree_t::SensorStatus,3> sensorStatus_;
  std::array<double,3> pressure_;

  std::vector<int> pumpChannels_;
  std::vector<int> valveChannels_;

  std::array<double,3> pumpOperatingHours_;

signals:

  void dataValid();
  void switchStateChanged(int,State);
  void pressureChanged(int,double);
  void sensorStatusChanged(int,int);
};

#endif // PUMPSTATIONMODEL_H
