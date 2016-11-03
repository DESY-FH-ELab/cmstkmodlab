#include <QApplication>

#include <nqlogger.h>

#include "NanotecSMCI36LinearStageModel.h"

NanotecSMCI36LinearStageModel::NanotecSMCI36LinearStageModel(NanotecSMCI36Model * controller,
                                                             QObject * /*parent*/)
  : QObject(),
    controller_(controller),
    status_(0),
    position_(0)
{
  // Connect all the signals
  connect(controller_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(controller_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(setControlsEnabled(bool)));
  connect(controller_, SIGNAL(informationChanged()),
          this, SLOT(updateInformation()));

  connect(controller_, SIGNAL(motionStarted()),
          this, SLOT(motionHasStarted()));
  connect(controller_, SIGNAL(motionFinished()),
          this, SLOT(motionHasFinished()));

  updateDeviceState(controller_->getDeviceState());
  updateInformation();

  NQLog("NanotecSMCI36LinearStageModel") << "constructed";
}

bool NanotecSMCI36LinearStageModel::isReady() const
{
  return (status_ & VNanotecSMCI36::smciReady);
}

const QString NanotecSMCI36LinearStageModel::getStatusText() const
{
  return controller_->getStatusText();
}

void NanotecSMCI36LinearStageModel::setPitch(double pitch)
{
  pitch_ = pitch;
  double stepMode = controller_->getStepMode();

  double min, max;

  min = pitch_*controller_->getMinFrequency()/stepMode;
  max = pitch_*controller_->getMaxFrequencyLimits().second/stepMode;
  speedLimits_ = std::pair<double,double>(min, max);

  emit limitsChanged();
}

void NanotecSMCI36LinearStageModel::stepModeChanged(int stepMode)
{
  double min, max;

  min = getPitch()*controller_->getMinFrequency()/stepMode;
  max = getPitch()*controller_->getMinFrequencyLimits().second/stepMode;
  speedLimits_ = std::pair<double,double>(min, max);

  emit limitsChanged();
}

double NanotecSMCI36LinearStageModel::getStepSize() const
{
  return getPitch()/controller_->getStepMode();
}

void NanotecSMCI36LinearStageModel::motionHasStarted()
{
  status_ |= ~VNanotecSMCI36::smciReady;

  emit deviceStateChanged(getDeviceState());

  emit motionStarted();
}

void NanotecSMCI36LinearStageModel::motionHasFinished()
{
  emit motionFinished();
}

void NanotecSMCI36LinearStageModel::setSpeed(double speed)
{
  double frequency = speed*controller_->getStepMode()/getPitch();
  controller_->setMaxFrequency(frequency);
}

void NanotecSMCI36LinearStageModel::requestMove(double position)
{
  double thePosition = position;

  if (thePosition < getMinimumPosition()) thePosition = getMinimumPosition();
  if (thePosition > getMaximumPosition()) thePosition = getMaximumPosition();

  controller_->setPositioningMode(VNanotecSMCI36::smciAbsolutePositioning);
  double travelDistance = thePosition*controller_->getStepMode()/getPitch();

  // NQLogMessage("NanotecSMCI36LinearStageModel") << "requestMove(" << position << ") " << travelDistance;

  controller_->setTravelDistance(travelDistance);
  controller_->start();
}

void NanotecSMCI36LinearStageModel::requestReferenceRun()
{
  // NQLogMessage("NanotecSMCI36LinearStageModel") << "requestReferenceRun()";

  controller_->setPositioningMode(VNanotecSMCI36::smciExternalRefRun);
  controller_->start();
}

void NanotecSMCI36LinearStageModel::requestStop()
{
  // NQLogMessage("NanotecSMCI36LinearStageModel") << "requestStop()";

  controller_->stop();
}

void NanotecSMCI36LinearStageModel::requestQuickStop()
{
  // NQLogMessage("NanotecSMCI36LinearStageModel") << "requestQuickStop()";

  controller_->quickStop();
}

void NanotecSMCI36LinearStageModel::requestResetError()
{
  // NQLogMessage("NanotecSMCI36LinearStageModel") << "requestResetError()";

  controller_->resetPositionError();
}

bool NanotecSMCI36LinearStageModel::getInputPinState(int pin) const
{
  return controller_->getInputPinState(pin);
}

bool NanotecSMCI36LinearStageModel::getOutputPinState(int pin) const
{
  return controller_->getOutputPinState(pin);
}

void NanotecSMCI36LinearStageModel::toggleOutputPin(int pin)
{
  controller_->toggleOutputPin(pin);
}

State NanotecSMCI36LinearStageModel::getDeviceState() const
{
  return controller_->getDeviceState();
}

void NanotecSMCI36LinearStageModel::updateDeviceState(State newState)
{
  emit deviceStateChanged(newState);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36LinearStageModel::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}

void NanotecSMCI36LinearStageModel::updateInformation()
{
  // NQLog("NanotecSMCI36LinearStageModel", NQLog::Message) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    // NQLog("NanotecSMCI36LinearStageModel", NQLog::Debug) << " running in main application thread";
  } else {
    // NQLog("NanotecSMCI36LinearStageModel", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  double pitch = getPitch();
  double stepMode = controller_->getStepMode();
  double controllerSteps = controller_->getControllerPosition();
  double encoderSteps = controller_->getEncoderPosition();
  double minFrequency = controller_->getMinFrequency();
  double maxFrequency = controller_->getMaxFrequency();

  unsigned int status = controller_->getStatus();
  double position = pitch*controllerSteps/stepMode;

  double min = pitch*minFrequency/stepMode;
  double max = pitch*controller_->getMinFrequencyLimits().second/stepMode;

  double speed = pitch*maxFrequency/stepMode;

  unsigned int io = controller_->getIO();

  if (min != speedLimits_.first ||
      max != speedLimits_.second) {
    speedLimits_ = std::pair<double,double>(min, max);
    emit limitsChanged();
  }

  if (status_ != status ||
      position_ != position ||
      speed_ != speed ||
      io_ != io) {

    if (status != status_) {
      status_ = status;
      emit deviceStateChanged(getDeviceState());
    }

    position_ = position;
    speed_ = speed;
    io_ = io;

    emit informationChanged();
  }
}
