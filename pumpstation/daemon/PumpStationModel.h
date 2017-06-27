#ifndef PUMPSTATIONMODEL_H
#define PUMPSTATIONMODEL_H

#include <array>

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

public slots:

  void setSwitchBlocked(int, bool);
  void setSwitchEnabled(int, bool);

protected slots:

  void updateInformation();
  void updateConrad();

protected:

  ConradModel* conradModel_;
  LeyboldGraphixThreeModel* leyboldModel_;
  bool conradDataValid_;
  bool leyboldDataValid_;
  bool dataValid_;
  
  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  std::array<bool,5> switchBlocked_;
  std::array<State,5> switchState_;

  std::array<LeyboldGraphixThree_t::SensorStatus,3> sensorStatus_;
  std::array<double,3> pressure_;

signals:

  void dataValid();
  void switchStateChanged(int,State);
  void pressureChanged(int,double);
  void sensorStatusChanged(int,int);
};

#endif // PUMPSTATIONMODEL_H
