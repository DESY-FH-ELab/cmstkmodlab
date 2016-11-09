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
  for (int i=0;i<3;i++) pressure_[i] = -1;

  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );

  setDeviceEnabled(true);

  NQLog("LeyboldGraphixThree") << "constructed";
}

double LeyboldGraphixThreeModel::getPressure(int channel) const
{
  if (channel<0 || channel>2) return -1;
  return pressure_[channel];
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

    std::array<double,3> pressure;
    for (int i=0;i<3;++i) {
      pressure[i] = controller_->GetPressure(i);
    }

    if (pressure != pressure_) {

      pressure_ = pressure;

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
