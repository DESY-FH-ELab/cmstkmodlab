#include <QApplication>

#include <nqlogger.h>

#include "LeyboldGraphixThreeModel.h"

LeyboldGraphixThreeModel::LeyboldGraphixThreeModel(const char* port,
                                                   double updateInterval,
                                                   QObject * /*parent*/)
  : QObject(),
    AbstractDeviceModel<LeyboldGraphixThree_t>(),
    LeyboldGraphixThree_PORT(port),
    updateInterval_(updateInterval)
{
  for (int i=0;i<3;i++) status_[i] = LeyboldGraphixThree_t::SensorStatus_unknown;
  for (int i=0;i<3;i++) pressure_[i] = -999;
  displayUnit_ = LeyboldGraphixThree_t::DisplayUnit_unknown;

  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );

  setDeviceEnabled(true);

  NQLog("LeyboldGraphixThree") << "constructed";
}

LeyboldGraphixThree_t::SensorStatus LeyboldGraphixThreeModel::getSensorStatus(int sensor) const
{
  if (sensor<1 || sensor>3) return LeyboldGraphixThree_t::SensorStatus_unknown;
  return status_[sensor-1];
}

double LeyboldGraphixThreeModel::getPressure(int sensor) const
{
  if (sensor<1 || sensor>3) return -1.;
  return pressure_[sensor-1];
}

LeyboldGraphixThree_t::DisplayUnit LeyboldGraphixThreeModel::getDisplayUnit() const
{
  return displayUnit_;
}

void LeyboldGraphixThreeModel::setDisplayUnit(LeyboldGraphixThree_t::DisplayUnit unit)
{
  if (displayUnit_ != unit) {
    controller_->SetDisplayUnit(unit);
    displayUnit_ = unit;
    emit informationChanged();
  }
}

void LeyboldGraphixThreeModel::initialize()
{
  NQLog("LeyboldGraphixThreeModel") << "initialize() " << LeyboldGraphixThree_PORT;

  setDeviceState(INITIALIZING);

  renewController(LeyboldGraphixThree_PORT);

  bool enabled = ( controller_ != NULL ) && ( controller_->DeviceAvailable() );

  if ( enabled ) {
    setDeviceState(READY);
    updateInformation();
  }
  else {
    setDeviceState( OFF );
    delete controller_;
    controller_ = NULL;
  }
}

/// \see AbstractDeviceModel::setDeviceState
void LeyboldGraphixThreeModel::setDeviceState( State state )
{
  if ( state_ != state ) {
    state_ = state;

    // No need to run the timer if the chiller is not ready
    if ( state_ == READY ) {
      timer_->start();
    } else {
      timer_->stop();
    }

    emit deviceStateChanged(state);
  }
}

void LeyboldGraphixThreeModel::updateInformation()
{
  // NQLog("LeyboldGraphixThreeModel", NQLog::Debug) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    // NQLog("LeyboldGraphixThreeModel", NQLog::Debug) << " running in main application thread";
  } else {
    // NQLog("LeyboldGraphixThreeModel", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  if ( state_ == READY ) {

    // unsigned int status = controller_->GetStatus();

    std::array<LeyboldGraphixThree_t::SensorStatus,3> status;
    std::array<double,3> pressure;

    for (int i=1;i<4;++i) {
      status[i] = controller_->GetSensorStatus(i);
      pressure[i] = controller_->GetPressure(i);
    }

    LeyboldGraphixThree_t::DisplayUnit displayUnit = controller_->GetDisplayUnit();

    if (status != status_ ||
        pressure != pressure_ ||
        displayUnit != displayUnit_) {

      status_ = status;
      pressure_ = pressure;

      displayUnit_ = displayUnit;

      // NQLog("LeyboldGraphixThreeModel", NQLog::Spam) << "information changed";

      emit informationChanged();
    }
  }
}

/// Attempts to enable/disable the (communication with) the LeyboldGraphixThree controller.
void LeyboldGraphixThreeModel::setDeviceEnabled(bool enabled)
{
  AbstractDeviceModel<LeyboldGraphixThree_t>::setDeviceEnabled(enabled);
}

void LeyboldGraphixThreeModel::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}
