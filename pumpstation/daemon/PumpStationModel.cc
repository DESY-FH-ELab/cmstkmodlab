#include <QApplication>

#include <nqlogger.h>

#include "PumpStationModel.h"

PumpStationModel::PumpStationModel(ConradModel* conradModel,
                                   LeyboldGraphixThreeModel* leyboldModel,
                                   double updateInterval,
                                   QObject * /* parent */)
  : QObject(),
    conradModel_(conradModel),
    leyboldModel_(leyboldModel),
    updateInterval_(updateInterval)
{
	for (int i=0;i<5;++i) switchBlocked_[i] = true;

  for (int i=0;i<3;i++) sensorStatus_[i] = LeyboldGraphixThree_t::SensorStatus_unknown;
  for (int i=0;i<3;i++) pressure_[i] = -999;

  connect(leyboldModel_, SIGNAL(informationChanged()),
          this, SLOT(updateInformation()));

  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateConrad()) );

  timer_->start();

  //updateInformation();
  //updateConrad();

  NQLog("PumpStationModel") << "constructed";
}

bool PumpStationModel::getSwitchBlocked(int channel) const
{
	return switchBlocked_[channel];
}

const State& PumpStationModel::getSwitchState( int channel ) const
{
  return switchState_[channel];
}

std::string PumpStationModel::getSensorName(int sensor) const
{
  return leyboldModel_->getSensorName(sensor);
}

double PumpStationModel::getPressure(int sensor) const
{
  if (sensor<1 || sensor>3) return -1.0;
  return pressure_[sensor-1];
}

int PumpStationModel::getSensorStatus(int sensor) const
{
  if (sensor<1 || sensor>3) return (int)LeyboldGraphixThree_t::SensorStatus_unknown;
  return (int)sensorStatus_[sensor-1];
}

void PumpStationModel::setSwitchBlocked(int channel, bool blocked)
{
	switchBlocked_[channel] = blocked;
}

void PumpStationModel::setSwitchEnabled(int channel, bool enabled)
{
	if (switchBlocked_[channel]) return;

  conradModel_->setSwitchEnabled(channel, enabled);

  if (enabled) {
    if (switchState_[channel] != READY) {
      switchState_[channel] = READY;
      emit switchStateChanged(channel, switchState_[channel]);
    }
  } else {
    if (switchState_[channel] != OFF) {
      switchState_[channel] = OFF;
      emit switchStateChanged(channel, switchState_[channel]);
    }
  }
}

void PumpStationModel::updateInformation()
{
  NQLog("PumpStationModel", NQLog::Message) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    // NQLog("PumpStationModel", NQLog::Debug) << " running in main application thread";
  } else {
    // NQLog("PumpStationModel", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  std::array<LeyboldGraphixThree_t::SensorStatus,3> sensorStatus;
  std::array<double,3> pressure;
  for (int i=0;i<3;++i) {

    sensorStatus[i] = leyboldModel_->getSensorStatus(i+1);
    if (sensorStatus_[i] != sensorStatus[i]) {
      sensorStatus_[i] = sensorStatus[i];
      emit sensorStatusChanged(i+1, (int)sensorStatus_[i]);
    }

    pressure[i] = leyboldModel_->getPressure(i+1);

    if (pressure_[i] != pressure[i]) {
      pressure_[i] = pressure[i];
      emit pressureChanged(i+1, pressure_[i]);
    }
  }
}

void PumpStationModel::updateConrad()
{
  // NQLog("PumpStationModel", NQLog::Message) << "updateConrad()";

  if (thread()==QApplication::instance()->thread()) {
    // NQLog("PumpStationModel", NQLog::Debug) << " running in main application thread";
  } else {
    // NQLog("PumpStationModel", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  std::array<State,5> switchState;
  for (int i=0;i<5;++i) {
    switchState[i] = conradModel_->getSwitchState(i);
    if (switchState_[i] != switchState[i]) {
      emit switchStateChanged(i, switchState[i]);
    }
  }

  switchState_ = switchState;
}
