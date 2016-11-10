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
  connect(leyboldModel_, SIGNAL(informationChanged()),
          this, SLOT(updateInformation()));

  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateConrad()) );

  timer_->start();

  updateInformation();
  updateConrad();

  NQLog("PumpStationModel") << "constructed";
}

const State& PumpStationModel::getSwitchState( int channel ) const
{
  return switchState_[channel];
}

double PumpStationModel::getPressure(int channel) const
{
  return pressure_[channel];
}

void PumpStationModel::setSwitchEnabled(int channel, bool enabled)
{
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
  // NQLog("PumpStationModel", NQLog::Message) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    // NQLog("PumpStationModel", NQLog::Debug) << " running in main application thread";
  } else {
    // NQLog("PumpStationModel", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  std::array<double,3> pressure;
  for (int i=0;i<3;++i) {
    pressure[i] = leyboldModel_->getPressure(i);
    if (pressure_[i] != pressure[i]) {
      emit pressureChanged(i, pressure_[i]);
    }
  }

  pressure_ = pressure;
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
