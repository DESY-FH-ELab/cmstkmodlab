#include <QApplication>

#include <nqlogger.h>

#include "NanotecSMCI36Model.h"

NanotecSMCI36Model::NanotecSMCI36Model(const char* port,
                                       double updateInterval1,
                                       double updateInterval2,
                                       QObject * /*parent*/)
  : QObject(),
    AbstractDeviceModel<NanotecSMCI36_t>(),
    NanotecSMCI36_PORT(port),
    updateInterval1_(updateInterval1),
    updateInterval2_(updateInterval2)
{
  driveAddress_ = 0;
  status_ = 0xffff;
  motorID_ = -1;
  phaseCurrent_ = -1;
  standStillPhaseCurrent_ = -1;
  stepMode_ = -1;
  rampMode_ = -1;
  errorCorrectionMode_ = -1;
  maxEncoderDeviation_ = -1;
  positioningMode_ = -1;
  direction_ = false;
  encoderDirection_ = false;
  travelDistance_ = 0;
  minFrequency_ = -1;
  maxFrequency_ = -1;
  maxFrequency2_ = -1;
  quickstopRamp_ = -1;
  accelRamp_ = -1;
  decelRamp_ = -1;

  inputPinFunction_[0] = 0;
  for (int i=1;i<7;i++) inputPinFunction_[i] = -1;

  outputPinFunction_[0] = 0;
  for (int i=1;i<4;i++) outputPinFunction_[i] = -1;

  ioMask_ = 0xffffffff;
  ioPolarityMask_ = 0xffffffff;
  io_ = 0xffffffff;

  timer1_ = new QTimer(this);
  timer1_->setInterval(updateInterval1_ * 1000);
  connect( timer1_, SIGNAL(timeout()), this, SLOT(updateInformation1()) );

  timer2_ = new QTimer(this);
  timer2_->setInterval(updateInterval2_ * 1000);
  connect( timer2_, SIGNAL(timeout()), this, SLOT(updateInformation2()) );

  setDeviceEnabled(true);

  NQLog("NanotecSMCI36Model") << "constructed";
}

bool NanotecSMCI36Model::isReady() const
{
  return (status_ & VNanotecSMCI36::smciReady);
}

const QString NanotecSMCI36Model::getStatusText() const
{
  QString text;

  if ((status_ & VNanotecSMCI36::smciReady) &&
      (status_ & VNanotecSMCI36::smciZero) &&
      (status_ & VNanotecSMCI36::smciPosError)) {

    text += "Ready | Zero | Position Error";

  } else if ((status_ & VNanotecSMCI36::smciReady) &&
             (status_ & VNanotecSMCI36::smciZero)) {

    text += "Ready | Zero";

  } else if ((status_ & VNanotecSMCI36::smciZero) &&
             (status_ & VNanotecSMCI36::smciPosError)) {

    text += "Zero | Position Error";

  } else if ((status_ & VNanotecSMCI36::smciReady) &&
             (status_ & VNanotecSMCI36::smciPosError)) {

    text += "Zero | Position Error";

  } else if (status_ & VNanotecSMCI36::smciPosError) {

    text += "Position Error";

  } else if (status_ & VNanotecSMCI36::smciReady) {

    text += "Ready";

  } else {

    text += "Moving";

  }

  text += " (";
  text += QString::number(status_);
  text += ")";

  return text;
}

void NanotecSMCI36Model::setMotorID(int id)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setMotorID(" << id << ")";

  if (motorID_ != id) {
    controller_->SetMotorID(id);
    motorID_ = id;

    emit informationChanged();
  }
}

void NanotecSMCI36Model::setPhaseCurrent(int current)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setPhaseCurrent(" << current << ")";

  if (phaseCurrent_ != current) {
    controller_->SetPhaseCurrent(current);
    phaseCurrent_ = current;

    emit informationChanged();
  }
}

void NanotecSMCI36Model::setStandStillPhaseCurrent(int current)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setStandStillPhaseCurrent(" << current << ")";

  if (standStillPhaseCurrent_ != current) {
    controller_->SetStandStillPhaseCurrent(current);
    standStillPhaseCurrent_ = current;

    emit informationChanged();
  }
}

void NanotecSMCI36Model::setStepMode(int mode)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setStepMode(" << mode << ")";

  if (stepMode_ != mode) {
    controller_->SetStepMode(mode);
    stepMode_ = mode;

    emit informationChanged();
    emit stepModeChanged(stepMode_);
  }
}

const std::string NanotecSMCI36Model::getStepModeName() const
{
  if (state_!=READY) return std::string("unknown");
  return controller_->GetStepModeName(getStepMode());
}

const std::map<int,std::string>& NanotecSMCI36Model::getStepModeNames() const
{
  return controller_->GetStepModeNames();
}

void NanotecSMCI36Model::setErrorCorrectionMode(int mode)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setErrorCorrectionMode(" << mode << ")";

  if (errorCorrectionMode_ != mode) {
    controller_->SetErrorCorrectionMode(mode);
    errorCorrectionMode_ = mode;

    emit informationChanged();
  }
}

const std::map<int,std::string>& NanotecSMCI36Model::getErrorCorrectionModeNames() const
{
  return controller_->GetErrorCorrectionModeNames();
}

void NanotecSMCI36Model::setRampMode(int mode)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setRampMode(" << mode << ")";

  if (rampMode_ != mode) {
    controller_->SetRampMode(mode);
    rampMode_ = mode;

    emit informationChanged();
  }
}

const std::string NanotecSMCI36Model::getRampModeName() const
{
  if (state_!=READY) return std::string("unknown");
  return controller_->GetRampModeName(getRampMode());
}

const std::map<int,std::string>& NanotecSMCI36Model::getRampModeNames() const
{
  return controller_->GetRampModeNames();
}

void NanotecSMCI36Model::setPositioningMode(int mode)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setPositioningMode(" << mode << ")";

  if (positioningMode_ != mode) {
    controller_->SetPositioningMode(mode);
    positioningMode_ = mode;

    emit informationChanged();
  }
}

const std::string NanotecSMCI36Model::getPositioningModeName() const
{
  if (state_!=READY) return std::string("unknown");
  return controller_->GetPositioningModeName(getPositioningMode());
}

const std::map<int,std::string>& NanotecSMCI36Model::getPositioningModeNames() const
{
  return controller_->GetPositioningModeNames();
}

const std::pair<int,int>& NanotecSMCI36Model::getMinFrequencyLimits() const
{
  return controller_->GetMinFrequencyLimits();
}

const std::pair<int,int>& NanotecSMCI36Model::getMaxFrequencyLimits() const
{
  return controller_->GetMaxFrequencyLimits();
}

const std::pair<int,int>& NanotecSMCI36Model::getMaxFrequency2Limits() const
{
  return controller_->GetMaxFrequency2Limits();
}

void NanotecSMCI36Model::setMaxEncoderDeviation(int steps)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setMaxEncoderDeviation(" << steps << ")";

  if (maxEncoderDeviation_ != steps) {
    controller_->SetMaxEncoderDeviation(steps);
    maxEncoderDeviation_ = steps;

    emit informationChanged();
  }
}

void NanotecSMCI36Model::setDirection(bool direction)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setDirection(" << direction << ")";

  if (direction_ != direction) {
    controller_->SetDirection(direction);
    direction_ = direction;

    emit informationChanged();
  }
}

void NanotecSMCI36Model::setEncoderDirection(bool direction)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setEncoderDirection(" << direction << ")";

  if (encoderDirection_ != direction) {
    controller_->SetEncoderDirection(direction);
    encoderDirection_ = direction;
    emit informationChanged();
  }
}

void NanotecSMCI36Model::setTravelDistance(int distance)
{
  if (state_!=READY) return;

  if (travelDistance_ != distance) {
    controller_->SetTravelDistance(distance);
    travelDistance_ = distance;

    emit informationChanged();
  }
}

void NanotecSMCI36Model::setMinFrequency(int frequency)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setMinFrequency(" << frequency << ")";

  if (minFrequency_ != frequency) {
    controller_->SetMinimumFrequency(frequency);
    minFrequency_ = frequency;

    emit informationChanged();
  }
}

void NanotecSMCI36Model::setMaxFrequency(int frequency)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setMaxFrequency(" << frequency << ")";

  if (maxFrequency_ != frequency) {
    controller_->SetMaximumFrequency(frequency);
    maxFrequency_ = frequency;
    emit informationChanged();
  }
}

void NanotecSMCI36Model::setMaxFrequency2(int frequency)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setMaxFrequency2(" << frequency << ")";

  if (maxFrequency2_ != frequency) {
    controller_->SetMaximumFrequency2(frequency);
    maxFrequency2_ = frequency;

    emit informationChanged();
  }
}

void NanotecSMCI36Model::setQuickstopRampHzPerSecond(int ramp)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setQuickstopRampHzPerSecond(" << ramp << ")";

  if (quickstopRamp_ != ramp) {
    controller_->SetQuickstopRampHzPerSecond(ramp);
    quickstopRamp_ = ramp;

    emit informationChanged();
  }
}

void NanotecSMCI36Model::setAccelerationRampHzPerSecond(int ramp)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setAccelerationRampHzPerSecond(" << ramp << ")";

  if (accelRamp_ != ramp) {
    controller_->SetAccelerationRampHzPerSecond(ramp);
    accelRamp_ = ramp;

    emit informationChanged();
  }
}

void NanotecSMCI36Model::setDecelerationRampHzPerSecond(int ramp)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setDecelerationRampHzPerSecond(" << ramp << ")";

  if (accelRamp_ != ramp) {
    controller_->SetDecelerationRampHzPerSecond(ramp);
    accelRamp_ = ramp;

    emit informationChanged();
  }
}

void NanotecSMCI36Model::start()
{
  if (state_!=READY) return;

  bool newPositionRequested = true;
  if (getPositioningMode()==VNanotecSMCI36::smciAbsolutePositioning &&
      getControllerPosition()==getTravelDistance()) {
    newPositionRequested = false;
  }

  if (status_ & VNanotecSMCI36::smciReady && newPositionRequested) {
    status_ |= ~VNanotecSMCI36::smciReady;

    // NQLogMessage("NanotecSMCI36Model") << "start() - 1";

    emit deviceStateChanged(state_);

    // NQLogMessage("NanotecSMCI36Model") << "start() - 2";

    emit motionStarted();

    controller_->Start();
  }
}

void NanotecSMCI36Model::stop()
{
  if (state_!=READY) return;

  controller_->Stop(false);

  emit motionFinished();
}

void NanotecSMCI36Model::quickStop()
{
  if (state_!=READY) return;

  controller_->Stop(true);

  emit motionFinished();
}

void NanotecSMCI36Model::resetPositionError()
{
  if (state_!=READY) return;

  controller_->ResetPositionError(controllerSteps_);
}

void NanotecSMCI36Model::setIOMask(unsigned int mask)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setIOMask(" << mask << ")";

  if (ioMask_ != mask) {
    controller_->SetIOMask(mask);
    ioMask_ = mask;

    emit informationChanged();
  }
}

void NanotecSMCI36Model::setReversePolarityMask(unsigned int mask)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setReversePolarityMask(" << mask << ")";

  if (ioPolarityMask_ != mask) {
    controller_->SetReversePolarityMask(mask);
    ioPolarityMask_ = mask;

    emit informationChanged();
  }
}

int NanotecSMCI36Model::getInputPinFunction(int pin) const
{
  return inputPinFunction_[pin];
}

const std::string NanotecSMCI36Model::getInputPinFunctionName(int pin) const
{
  if (state_!=READY) return std::string("unknown");
  return controller_->GetInputPinFunctionName(getInputPinFunction(pin));
}

const std::map<int,std::string>& NanotecSMCI36Model::getInputPinFunctionNames() const
{
  return controller_->GetInputPinFunctionNames();
}

void NanotecSMCI36Model::setInputPinFunction(int pin, int function)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setInputPinFunction(" << pin << ", " << function << ")";

  if (inputPinFunction_[pin] != function) {
    controller_->SetInputPinFunction(pin, function);
    inputPinFunction_[pin] = function;

    emit informationChanged();
  }
}

bool NanotecSMCI36Model::getInputPolarity(int pin) const
{
  unsigned int bit = controller_->GetInputBitForPin(pin);
  if (bit==0) return false;

  return (ioPolarityMask_&bit);
}

void NanotecSMCI36Model::setInputPolarity(int pin, bool reverse)
{
  unsigned int bit = controller_->GetInputBitForPin(pin);
  if (bit==0) return;

  // NQLogMessage("NanotecSMCI36Model") << "setInputPolarity(" << pin << ", " << reverse << ")";

  unsigned int ioPolarityMask = ioPolarityMask_;

  if (reverse) {
    if (!(ioPolarityMask&bit)) {
      ioPolarityMask |= bit;
    }
  } else {
    if (ioPolarityMask&bit) {
      ioPolarityMask &= ~bit;
    }
  }

  setReversePolarityMask(ioPolarityMask);
}

bool NanotecSMCI36Model::getInputPinState(int pin) const
{
  unsigned int bit = controller_->GetInputBitForPin(pin);
  if (bit==0) return false;

  return (io_&bit);
}

int NanotecSMCI36Model::getOutputPinFunction(int pin) const
{
  return outputPinFunction_[pin];
}

const std::string NanotecSMCI36Model::getOutputPinFunctionName(int pin) const
{
  if (state_!=READY) return std::string("unknown");
  return controller_->GetOutputPinFunctionName(getOutputPinFunction(pin));
}

const std::map<int,std::string>& NanotecSMCI36Model::getOutputPinFunctionNames() const
{
  return controller_->GetOutputPinFunctionNames();
}

void NanotecSMCI36Model::setOutputPinFunction(int pin, int function)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setOutputPinFunction(" << pin << ", " << function << ")";

  if (outputPinFunction_[pin] != function) {
    controller_->SetOutputPinFunction(pin, function);
    outputPinFunction_[pin] = function;

    emit informationChanged();
  }
}

bool NanotecSMCI36Model::getOutputPolarity(int pin) const
{
  unsigned int bit = controller_->GetOutputBitForPin(pin);
  if (bit==0) return false;

  return (ioPolarityMask_&bit);
}

void NanotecSMCI36Model::setOutputPolarity(int pin, bool reverse)
{
  unsigned int bit = controller_->GetOutputBitForPin(pin);
  if (bit==0) return;

  // NQLogMessage("NanotecSMCI36Model") << "setOutputPolarity(" << pin << ", " << reverse << ")";

  unsigned int ioPolarityMask = ioPolarityMask_;

  if (reverse) {
    if (!(ioPolarityMask&bit)) {
      ioPolarityMask |= bit;
    }
  } else {
    if (ioPolarityMask&bit) {
      ioPolarityMask &= ~bit;
    }
  }

  setReversePolarityMask(ioPolarityMask);
}

bool NanotecSMCI36Model::getOutputPinState(int pin) const
{
  unsigned int bit = controller_->GetOutputBitForPin(pin);
  if (bit==0) return false;

  return (io_&bit);
}

void NanotecSMCI36Model::setIO(unsigned int mask)
{
  if (state_!=READY) return;

  // NQLogMessage("NanotecSMCI36Model") << "setIO(" << mask << ")";

  if (io_ != mask) {
    controller_->SetIO(mask);
    io_ = mask;

    emit informationChanged();
  }
}

void NanotecSMCI36Model::setOutputPinState(int pin, bool state)
{
  unsigned int bit = controller_->GetOutputBitForPin(pin);
  if (bit==0) return;

  // NQLogMessage("NanotecSMCI36Model") << "setOutputPinState(" << pin << ", " << state << ")";

  unsigned int io = io_;

  if (state) {
    if (!(io&bit)) {
      io |= bit;
    }
  } else {
    if (io&bit) {
      io &= ~bit;
    }
  }

  setIO(io);
}

void NanotecSMCI36Model::toggleOutputPin(int pin)
{
  if (state_!=READY) return;

  bool state = getOutputPinState(pin);
  setOutputPinState(pin, !state);
}

void NanotecSMCI36Model::initialize()
{
  NQLog("NanotecSMCI36Model") << "initialize() " << NanotecSMCI36_PORT;

  setDeviceState(INITIALIZING);

  renewController(NanotecSMCI36_PORT);

  bool enabled = ( controller_ != NULL ) && ( controller_->DeviceAvailable() );

  if ( enabled ) {
    setDeviceState(READY);
    updateInformation1();
    updateInformation2();
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
    if ( state_ == READY ) {
      timer1_->start();
      timer2_->start();
    } else {
      timer1_->stop();
      timer2_->stop();
    }

    emit deviceStateChanged(state);
  }
}

void NanotecSMCI36Model::updateInformation1()
{
  // NQLog("NanotecSMCI36Model", NQLog::Debug) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    // NQLog("NanotecSMCI36Model", NQLog::Debug) << " running in main application thread";
  } else {
    // NQLog("NanotecSMCI36Model", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  if ( state_ == READY ) {

    unsigned int status = controller_->GetStatus();
    int controllerSteps = controller_->GetPosition();
    int encoderSteps = controller_->GetEncoderPosition();

    unsigned int io = controller_->GetIO();

    if (status != status_ ||
        controllerSteps != controllerSteps_ ||
        encoderSteps != encoderSteps_ ||
        io != io_) {

      if (status != status_) {
        status_ = status;
        emit deviceStateChanged(state_);
      }

      controllerSteps_ = controllerSteps;
      encoderSteps_ = encoderSteps;

      io_ = io;

      // NQLog("NanotecSMCI36Model", NQLog::Spam) << "information changed";

      emit informationChanged();
    }
  }
}

void NanotecSMCI36Model::updateInformation2()
{
  // NQLog("NanotecSMCI36Model", NQLog::Debug) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    // NQLog("NanotecSMCI36Model", NQLog::Debug) << " running in main application thread";
  } else {
    // NQLog("NanotecSMCI36Model", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  if ( state_ == READY ) {

    int driveAddress = controller_->GetDriveAddress();
    int motorID = controller_->GetMotorID();
    int phaseCurrent = controller_->GetPhaseCurrent();
    int standStillPhaseCurrent = controller_->GetStandStillPhaseCurrent();
    int stepMode = controller_->GetStepMode();
    int rampMode = controller_->GetRampMode();
    int positioningMode = controller_->GetPositioningMode();
    int errorCorrectionMode = controller_->GetErrorCorrectionMode();
    int maxEncoderDeviation = controller_->GetMaxEncoderDeviation();
    bool direction = controller_->GetDirection();
    bool encoderDirection = controller_->GetEncoderDirection();
    int travelDistance = controller_->GetTravelDistance();
    int minFrequency = controller_->GetMinimumFrequency();
    int maxFrequency = controller_->GetMaximumFrequency();
    int maxFrequency2 = controller_->GetMaximumFrequency2();
    int quickstopRamp = controller_->GetQuickstopRampHzPerSecond();
    int accelRamp = controller_->GetAccelerationRampHzPerSecond();
    int decelRamp = controller_->GetDecelerationRampHzPerSecond();

    std::array<int,7> inputPinFunction;
    inputPinFunction[0] = 0;
    for (int i=1;i<7;++i) {
      inputPinFunction[i] = controller_->GetInputPinFunction(i);
    }

    std::array<int,4> outputPinFunction;
    outputPinFunction[0] = 0;
    for (int i=1;i<4;++i) {
      outputPinFunction[i] = controller_->GetOutputPinFunction(i);
    }

    unsigned int ioMask = controller_->GetIOMask();
    unsigned int ioPolarityMask = controller_->GetReversePolarityMask();

    if (driveAddress != driveAddress_ ||
        motorID != motorID_ ||
        phaseCurrent != phaseCurrent_ ||
        standStillPhaseCurrent != standStillPhaseCurrent_ ||
        stepMode != stepMode_ ||
        rampMode != rampMode_ ||
        positioningMode != positioningMode_ ||
        errorCorrectionMode != errorCorrectionMode_ ||
        maxEncoderDeviation != maxEncoderDeviation_ ||
        direction != direction_ ||
        encoderDirection != encoderDirection_ ||
        travelDistance != travelDistance_ ||
        minFrequency != minFrequency_ ||
        maxFrequency != maxFrequency_ ||
        maxFrequency2 != maxFrequency2_ ||
        quickstopRamp != quickstopRamp_ ||
        accelRamp != accelRamp_ ||
        decelRamp != decelRamp_ ||
        inputPinFunction != inputPinFunction_ ||
        outputPinFunction != outputPinFunction_ ||
        ioMask != ioMask_ ||
        ioPolarityMask != ioPolarityMask_) {

      driveAddress_ = driveAddress;
      motorID_ = motorID;
      phaseCurrent_ = phaseCurrent;
      standStillPhaseCurrent_ = standStillPhaseCurrent;
      stepMode_ = stepMode;
      rampMode_ = rampMode;
      positioningMode_ = positioningMode;
      errorCorrectionMode_ = errorCorrectionMode;
      maxEncoderDeviation_ = maxEncoderDeviation;
      direction_ = direction;
      encoderDirection_ = encoderDirection;
      travelDistance_ = travelDistance;
      minFrequency_ = minFrequency;
      maxFrequency_ = maxFrequency;
      maxFrequency2_ = maxFrequency2;
      quickstopRamp_ = quickstopRamp;
      accelRamp_ = accelRamp;
      decelRamp_= decelRamp;

      inputPinFunction_ = inputPinFunction;
      outputPinFunction_ = outputPinFunction;
      ioMask_ = ioMask;
      ioPolarityMask_ = ioPolarityMask;

      // NQLog("NanotecSMCI36Model", NQLog::Spam) << "information changed";

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
