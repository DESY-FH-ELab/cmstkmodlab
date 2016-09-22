#include <QApplication>

#include <nqlogger.h>

#include "NanotecSMCI36Model.h"

/*
  NanotecSMCI36Model implementation
  */
NanotecSMCI36Model::NanotecSMCI36Model(const char* port,
                                       int updateInterval,
                                       QObject * /*parent*/)
  : QObject(),
    AbstractDeviceModel<NanotecSMCI36_t>(),
    NanotecSMCI36_PORT(port),
    updateInterval_(updateInterval)
{
  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );

  setDeviceEnabled(true);
void NanotecSMCI36Model::setMotorID(unsigned int motorID)
{
  if (state_!=READY) return;

  controller_->SetMotorID(motorID);
}

void NanotecSMCI36Model::setStepMode(unsigned int mode)
{
  if (state_!=READY) return;

  controller_->SetStepMode(mode);
}

const std::vector<std::pair<int,std::string>>& NanotecSMCI36Model::getStepModeNames() const
{
  return controller_->GetStepModeNames();
}

void NanotecSMCI36Model::setPositioningMode(unsigned int mode)
{
  if (state_!=READY) return;

  controller_->SetPositioningMode(mode);
}

const std::vector<std::pair<int,std::string>>& NanotecSMCI36Model::getPositioningModeNames() const
{
  return controller_->GetPositioningModeNames();
}

void NanotecSMCI36Model::setDirection(bool direction)
{
  if (state_!=READY) return;

  controller_->SetDirection(direction);
}

void NanotecSMCI36Model::setTravelDistance(double distance)
{
  if (state_!=READY) return;

  controller_->SetTravelDistance(distance);
}

void NanotecSMCI36Model::start()
{
  if (state_!=READY) return;

  if (status_ & VNanotecSMCI36::smciReady) {
    controller_->Start();
  }
}

void NanotecSMCI36Model::stop()
{
  if (state_!=READY) return;

  controller_->Stop(false);
}

void NanotecSMCI36Model::quickStop()
{
  if (state_!=READY) return;

  controller_->Stop(true);
}

void NanotecSMCI36Model::initialize()
{
  setDeviceState(INITIALIZING);

  renewController(NanotecSMCI36_PORT);

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
void NanotecSMCI36Model::setDeviceState( State state )
{
  if ( state_ != state ) {
    state_ = state;

    // No need to run the timer if the chiller is not ready
    if ( state_ == READY )
      timer_->start();
    else
      timer_->stop();

    emit deviceStateChanged(state);
  }
}

/**
  Updates the cached information about the NanotecSMCI36 chiller and signals any
  changes.
  */
void NanotecSMCI36Model::updateInformation()
{
  NQLog("NanotecSMCI36Model", NQLog::Debug) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    NQLog("NanotecSMCI36Model", NQLog::Debug) << " running in main application thread";
  } else {
    NQLog("NanotecSMCI36Model", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  if ( state_ == READY ) {

    unsigned int newStatus = controller_->GetStatus();

    if (newStatus != status_) {

      status_ = newStatus;

      NQLog("NanotecSMCI36Model", NQLog::Spam) << "information changed";

      emit informationChanged();
    }
  }
}

/// Attempts to enable/disable the (communication with) the NanotecSMCI36 controller.
void NanotecSMCI36Model::setDeviceEnabled(bool enabled)
{
  AbstractDeviceModel<NanotecSMCI36_t>::setDeviceEnabled(enabled);
}

void NanotecSMCI36Model::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}
