#include <QApplication>

#include <nqlogger.h>

#include "LStepExpressModel.h"

LStepExpressModel::LStepExpressModel(const char* port,
                                     float updateInterval,
                                     QObject * /*parent*/)
: QObject(),
  AbstractDeviceModel<LStepExpress_t>(),
  LStepExpress_PORT(port),
  updateInterval_(updateInterval)
{
  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));
}

void LStepExpressModel::initialize() {

  setDeviceState(INITIALIZING);

  renewController(LStepExpress_PORT);

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

void LStepExpressModel::setDeviceState( State state )
{
  if ( state_ != state ) {
    state_ = state;

    if ( state_ == READY )
      timer_->start();
    else
      timer_->stop();

    emit deviceStateChanged(state);
  }
}

void LStepExpressModel::updateInformation()
{
  if ( state_ == READY ) {

    NQLog("LStepExpressModel", NQLog::Debug) << "updateInformation()";

    if (thread()==QApplication::instance()->thread()) {
      NQLog("LStepExpressModel", NQLog::Debug) << " running in main application thread";
    } else {
      NQLog("LStepExpressModel", NQLog::Debug) << " running in dedicated thread";
    }


    bool changed = false;

    std::vector<int> ivalues;

    controller_->GetAxisEnabled(ivalues);
    if (ivalues!=axis_) {
      axis_ = ivalues;
      changed = true;
    }

    controller_->GetAxisStatus(ivalues);
    if (ivalues!=axisStatus_) {
      axisStatus_ = ivalues;
      changed = true;
    }

    if (changed) {
      NQLog("LStepExpressModel", NQLog::Spam) << "information changed";
      emit informationChanged();
    }
  }
}

void LStepExpressModel::setDeviceEnabled(bool enabled)
{
  if (state_ == READY && !enabled) {
    std::vector<int> allZeros{ 0, 0, 0, 0 };
    controller_->SetPowerAmplifierStatus(allZeros);
    controller_->SetAxisEnabled(allZeros);
  }

  AbstractDeviceModel<LStepExpress_t>::setDeviceEnabled(enabled);
}

void LStepExpressModel::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}
