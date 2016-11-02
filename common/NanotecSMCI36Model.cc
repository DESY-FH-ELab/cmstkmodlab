#include <QApplication>

#include <nqlogger.h>

#include "NanotecSMCI36Model.h"

/*
  NanotecSMCI36Model implementation
  */
NanotecSMCI36Model::NanotecSMCI36Model(const char* port,
                                       double updateInterval1,
                                       double updateInterval2,
                                       QObject * /*parent*/)
  : QObject(),
    AbstractDeviceModel<NanotecSMCI36_t>(),
    NanotecSMCI36_PORT(port),
    updateInterval1_(updateInterval1),
    updateInterval2_(updateInterval2),
    maxSpeedForOperation_(100),
    maxSpeedForRefRun_(5),
    ioPolarityMask_(0x107003F)
{
  inputPinFunction_[0] = 0;
  outputPinFunction_[0] = 0;

  timer1_ = new QTimer(this);
  timer1_->setInterval(updateInterval1_ * 1000);
  connect( timer1_, SIGNAL(timeout()), this, SLOT(updateInformation1()) );

  timer2_ = new QTimer(this);
  timer2_->setInterval(updateInterval2_ * 1000);
  connect( timer2_, SIGNAL(timeout()), this, SLOT(updateInformation2()) );

  setDeviceEnabled(true);

  NQLog("NanotecSMCI36Model") << "constructed";
}

void NanotecSMCI36Model::setMotorID(int id)
{
  if (state_!=READY) return;

  NQLogMessage("NanotecSMCI36Model") << "setMotorID(" << id << ")";

  controller_->SetMotorID(id);

  updateInformation2();
}

void NanotecSMCI36Model::setPhaseCurrent(int current)
{
  if (state_!=READY) return;

  NQLogMessage("NanotecSMCI36Model") << "setPhaseCurrent(" << current << ")";

  controller_->SetPhaseCurrent(current);

  updateInformation2();
}

void NanotecSMCI36Model::setStandStillPhaseCurrent(int current)
{
  if (state_!=READY) return;

  NQLogMessage("NanotecSMCI36Model") << "setStandStillPhaseCurrent(" << current << ")";

  controller_->SetStandStillPhaseCurrent(current);

  updateInformation2();
}

void NanotecSMCI36Model::setStepMode(int mode)
{
  if (state_!=READY) return;

  controller_->SetStepMode(mode);

  updateInformation2();
}

const std::map<int,std::string>& NanotecSMCI36Model::getStepModeNames() const
{
  return controller_->GetStepModeNames();
}

void NanotecSMCI36Model::setErrorCorrectionMode(int mode)
{
  if (state_!=READY) return;

  controller_->SetErrorCorrectionMode(mode);

  updateInformation2();
}

const std::map<int,std::string>& NanotecSMCI36Model::getErrorCorrectionModeNames() const
{
  return controller_->GetErrorCorrectionModeNames();
}

void NanotecSMCI36Model::setRampMode(int mode)
{
  if (state_!=READY) return;

  controller_->SetRampMode(mode);

  updateInformation2();
}

const std::map<int,std::string>& NanotecSMCI36Model::getRampModeNames() const
{
  return controller_->GetRampModeNames();
}

void NanotecSMCI36Model::setPositioningMode(int mode)
{
  if (state_!=READY) return;

  controller_->SetPositioningMode(mode);

  if (mode==VNanotecSMCI36::smciExternalRefRun &&
      (getMaxSpeed()>getMaxSpeedForRefRun() ||
       getMaxSpeed2()>getMaxSpeedForRefRun())) {
    setMaxSpeed(getMaxSpeedForRefRun());
    setMaxSpeed2(getMaxSpeedForRefRun());
  }

  checkPositionLimits();

  emit positionModeChanged(mode);

  updateInformation2();
}

const std::map<int,std::string>& NanotecSMCI36Model::getPositioningModeNames() const
{
  return controller_->GetPositioningModeNames();
}

void NanotecSMCI36Model::setMaxEncoderDeviation(int steps)
{
  if (state_!=READY) return;

  controller_->SetMaxEncoderDeviation(steps);

  updateInformation2();
}

void NanotecSMCI36Model::setDirection(bool direction)
{
  if (state_!=READY) return;

  direction_ = direction;
  controller_->SetDirection(direction);

  checkPositionLimits();

  updateInformation2();
}

void NanotecSMCI36Model::setTravelDistance(double distance)
void NanotecSMCI36Model::setEncoderDirection(bool direction)
{
  if (state_!=READY) return;

  travelDistance_ = distance;

  checkPositionLimits();
  NQLogMessage("NanotecSMCI36Model") << "setEncoderDirection(" << direction << ")";

  controller_->SetEncoderDirection(direction);

  updateInformation2();
}

void NanotecSMCI36Model::setMinFrequency(double frequency)
{
  if (state_!=READY) return;

  controller_->SetMinimumFrequency(frequency);

  updateInformation2();
}

void NanotecSMCI36Model::setMaxFrequency(double frequency)
{
  if (state_!=READY) return;

  controller_->SetMaximumFrequency(frequency);

  updateInformation2();
}

void NanotecSMCI36Model::setMaxFrequency2(double frequency)
{
  if (state_!=READY) return;

  controller_->SetMaximumFrequency2(frequency);

  updateInformation2();
}

void NanotecSMCI36Model::setTravelDistanceInMM(double distance)
{
  setTravelDistance(distance*getStepMode()/getPitch());

  updateInformation2();
}

void NanotecSMCI36Model::setMinSpeed(double speed)
{
  setMinFrequency(speed*getStepMode()/getPitch());

  updateInformation2();
}

void NanotecSMCI36Model::setMaxSpeed(double speed)
{
  if (getPositioningMode()==VNanotecSMCI36::smciExternalRefRun &&
      speed>getMaxSpeedForRefRun()) {
    speed = getMaxSpeedForRefRun();
  }

  setMaxFrequency(speed*getStepMode()/getPitch());

  updateInformation2();
}

void NanotecSMCI36Model::setMaxSpeed2(double speed)
{
  if (getPositioningMode()==VNanotecSMCI36::smciExternalRefRun &&
      speed>getMaxSpeedForRefRun()) {
    speed = getMaxSpeedForRefRun();
  }

  setMaxFrequency2(speed*getStepMode()/getPitch());

  updateInformation2();
}

void NanotecSMCI36Model::setMinPositionInMM(double position)
{
  bool infoChanged = !(minPositionInMM_==position);

  minPositionInMM_ = position;

  if (infoChanged) emit informationChanged();
}

void NanotecSMCI36Model::setMaxPositionInMM(double position)
{
  bool infoChanged = !(maxPositionInMM_==position);

  maxPositionInMM_ = position;

  if (infoChanged) emit informationChanged();
}

void NanotecSMCI36Model::setTravelDistanceNoCheck(double distance)
{
  if (state_!=READY) return;

  travelDistance_ = distance;

  controller_->SetTravelDistance(distance);

  updateInformation2();
}

void NanotecSMCI36Model::setTravelDistanceInMMNoCheck(double distance)
{
  setTravelDistanceNoCheck(distance*getStepMode()/getPitch());
}

void NanotecSMCI36Model::checkPositionLimits()
{
  if (getPositioningMode()==VNanotecSMCI36::smciExternalRefRun) return;

  double expectedPosition = getEncoderPosition();

  if (getPositioningMode()==VNanotecSMCI36::smciRelativePositioning) {
    if (getDirection()) {
      expectedPosition -= getTravelDistanceInMM();

      if (expectedPosition < getMinPositionInMM()) {
        setTravelDistanceInMMNoCheck(getEncoderPosition()-getMinPositionInMM());
      }

    } else {
      expectedPosition += getTravelDistanceInMM();

      if (expectedPosition > getMaxPositionInMM()) {
        setTravelDistanceInMMNoCheck(getMaxPositionInMM()-getEncoderPosition());
      }

    }
  }

  if (getPositioningMode()==VNanotecSMCI36::smciAbsolutePositioning) {
    expectedPosition = getTravelDistanceInMM();

    if (expectedPosition < getMinPositionInMM()) {
      setTravelDistanceInMMNoCheck(getMinPositionInMM());
    } else if (expectedPosition > getMaxPositionInMM()) {
      setTravelDistanceInMMNoCheck(getMaxPositionInMM());
    }
  }
}

void NanotecSMCI36Model::start()
{
  if (state_!=READY) return;

  checkPositionLimits();

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

void NanotecSMCI36Model::resetPositionError()
{
  if (state_!=READY) return;

  controller_->ResetPositionError(controllerSteps_);
}

int NanotecSMCI36Model::getInputPinFunction(int pin) const
{
  return inputPinFunction_[pin];
}

const std::map<int,std::string>& NanotecSMCI36Model::getInputPinFunctionNames() const
{
  return controller_->GetInputPinFunctionNames();
}

void NanotecSMCI36Model::setInputPinFunction(int pin, int function)
{
  if (state_!=READY) return;

  controller_->SetInputPinFunction(pin, function);

  updateInformation2();
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

  if (reverse) {
    if (!(ioPolarityMask_&bit)) {
      controller_->SetReversePolarityMask(ioPolarityMask_|bit);
      updateInformation2();
    }
  } else {
    if (ioPolarityMask_&bit) {
      controller_->SetReversePolarityMask(ioPolarityMask_ & ~bit);
      updateInformation2();
    }
  }
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

const std::map<int,std::string>& NanotecSMCI36Model::getOutputPinFunctionNames() const
{
  return controller_->GetOutputPinFunctionNames();
}

void NanotecSMCI36Model::setOutputPinFunction(int pin, int function)
{
  if (state_!=READY) return;

  controller_->SetOutputPinFunction(pin, function);

  updateInformation2();
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

  if (reverse) {
    if (!(ioPolarityMask_&bit)) {
      controller_->SetReversePolarityMask(ioPolarityMask_|bit);
      updateInformation2();
    }
  } else {
    if (ioPolarityMask_&bit) {
      controller_->SetReversePolarityMask(ioPolarityMask_ & ~bit);
      updateInformation2();
    }
  }
}

bool NanotecSMCI36Model::getOutputPinState(int pin) const
{
  unsigned int bit = controller_->GetOutputBitForPin(pin);
  if (bit==0) return false;

  return (io_&bit);
}

void NanotecSMCI36Model::setOutputPinState(int pin, bool state)
{
  unsigned int bit = controller_->GetOutputBitForPin(pin);
  if (bit==0) return;

  if (state) {
    if (!(io_&bit)) {
      controller_->SetIO(io_ | bit);
      updateInformation1();
    }
  } else {
    if (io_&bit) {
      controller_->SetIO(io_ & ~bit);
      updateInformation1();
    }
  }
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

      status_ = status;
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

    int motorID = controller_->GetMotorID();
    int stepMode = controller_->GetStepMode();
    int rampMode = controller_->GetRampMode();
    int positioningMode = controller_->GetPositioningMode();
    int errorCorrectionMode = controller_->GetErrorCorrectionMode();
    int maxEncoderDeviation = controller_->GetMaxEncoderDeviation();
    bool direction = controller_->GetDirection();
    double travelDistance = controller_->GetTravelDistance();
    double minFrequency = controller_->GetMinimumFrequency();
    double maxFrequency = controller_->GetMaximumFrequency();
    double maxFrequency2 = controller_->GetMaximumFrequency2();

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

    unsigned int ioPolarityMask = controller_->GetReversePolarityMask();

    if (motorID != motorID_ ||
        stepMode != stepMode_ ||
        rampMode != rampMode_ ||
        positioningMode != positioningMode_ ||
        errorCorrectionMode != errorCorrectionMode_ ||
        maxEncoderDeviation != maxEncoderDeviation_ ||
        direction != direction_ ||
        travelDistance != travelDistance_ ||
        minFrequency != minFrequency_ ||
        maxFrequency != maxFrequency_ ||
        maxFrequency2 != maxFrequency2_ ||
        inputPinFunction != inputPinFunction_ ||
        outputPinFunction != outputPinFunction_ ||
        ioPolarityMask != ioPolarityMask_) {

      motorID_ = motorID;
      stepMode_ = stepMode;
      rampMode_ = rampMode;
      positioningMode_ = positioningMode;
      errorCorrectionMode_ = errorCorrectionMode;
      maxEncoderDeviation_ = maxEncoderDeviation;
      direction_ = direction;
      travelDistance_ = travelDistance;
      minFrequency_ = minFrequency;
      maxFrequency_ = maxFrequency;
      maxFrequency2_ = maxFrequency2;

      inputPinFunction_ = inputPinFunction;
      outputPinFunction_ = outputPinFunction;
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
