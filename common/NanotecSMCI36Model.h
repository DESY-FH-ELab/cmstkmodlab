#ifndef NANOTECSMCI36MODEL_H
#define NANOTECSMCI36MODEL_H

#include <cmath>
#include <map>
#include <array>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Nanotec/NanotecSMCI36Fake.h"
typedef NanotecSMCI36Fake NanotecSMCI36_t;
#else
#include "devices/Nanotec/NanotecSMCI36.h"
typedef NanotecSMCI36 NanotecSMCI36_t;
#endif

/**
  Command and control model of the Nanotec SMCI36 controller.
  */
class NanotecSMCI36Model : public QObject, public AbstractDeviceModel<NanotecSMCI36_t>
{

  Q_OBJECT
public:

  explicit NanotecSMCI36Model(const char* port,
                              double updateInterval1 = 1,
                              double updateInterval2 = 5,
                              QObject *parent = 0);

  int getDriveAddress() const { return driveAddress_; }

  bool isReady() const;
  unsigned int getStatus() const { return status_; }
  const QString getStatusText() const;

  int getMotorID() const { return motorID_; }

  int getPhaseCurrent() const { return phaseCurrent_; }
  int getStandStillPhaseCurrent() const { return standStillPhaseCurrent_; }

  int getStepMode() const { return stepMode_; }
  const std::string getStepModeName() const;
  const std::map<int,std::string>& getStepModeNames() const;

  int getRampMode() const { return rampMode_; }
  const std::string getRampModeName() const;
  const std::map<int,std::string>& getRampModeNames() const;

  int getErrorCorrectionMode() const { return errorCorrectionMode_; }
  const std::map<int,std::string>& getErrorCorrectionModeNames() const;

  int getMaxEncoderDeviation() const { return maxEncoderDeviation_; }

  int getControllerPosition() const { return controllerSteps_; }
  int getEncoderPosition() const { return encoderSteps_; }

  int getPositioningMode() const { return positioningMode_; }
  const std::string getPositioningModeName() const;
  const std::map<int,std::string>& getPositioningModeNames() const;

  bool getDirection() const { return direction_; }
  bool getEncoderDirection() const { return encoderDirection_; }

  int getTravelDistance() const { return travelDistance_; }
  const std::pair<int,int>& getMinFrequencyLimits() const;
  int getMinFrequency() const { return minFrequency_; }
  const std::pair<int,int>& getMaxFrequencyLimits() const;
  int getMaxFrequency() const { return maxFrequency_; }
  const std::pair<int,int>& getMaxFrequency2Limits() const;
  int getMaxFrequency2() const { return maxFrequency2_; }

  int getQuickstopRampHzPerSecond() const { return quickstopRamp_; }
  int getAccelerationRampHzPerSecond() const { return accelRamp_; }
  int getDecelerationRampHzPerSecond() const { return decelRamp_; }

  int getInputPinFunction(int pin) const;
  const std::string getInputPinFunctionName(int pin) const;
  const std::map<int,std::string>& getInputPinFunctionNames() const;

  bool getInputPolarity(int pin) const;
  bool getInputPinState(int pin) const;

  int getOutputPinFunction(int pin) const;
  const std::string getOutputPinFunctionName(int pin) const;
  const std::map<int,std::string>& getOutputPinFunctionNames() const;

  bool getOutputPolarity(int pin) const;
  bool getOutputPinState(int pin) const;

  void setMotorID(int id);

  void setPhaseCurrent(int current);
  void setStandStillPhaseCurrent(int current);

  void setStepMode(int mode);
  void setRampMode(int mode);
  void setErrorCorrectionMode(int mode);
  void setMaxEncoderDeviation(int steps);
  void setEncoderDirection(bool direction);

  void setQuickstopRampHzPerSecond(int ramp);
  void setAccelerationRampHzPerSecond(int ramp);
  void setDecelerationRampHzPerSecond(int ramp);

  void setIOMask(unsigned int mask);
  void setReversePolarityMask(unsigned int mask);

  unsigned int getIO() const { return io_; }
  void setIO(unsigned int mask);

  void setInputPinFunction(int pin, int function);
  void setInputPolarity(int pin, bool reverse);

  void setOutputPinFunction(int pin, int function);
  void setOutputPolarity(int pin, bool reverse);

public slots:

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);

  void setPositioningMode(int mode);
  void setDirection(bool direction);
  void setTravelDistance(int distance);
  void setMinFrequency(int frequency);
  void setMaxFrequency(int frequency);
  void setMaxFrequency2(int frequency);

  void start();
  void stop();
  void quickStop();
  void resetPositionError();

  void setOutputPinState(int pin, bool state);
  void toggleOutputPin(int pin);

  void updateInformation1();
  void updateInformation2();

protected:

  const QString NanotecSMCI36_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval1_;
  QTimer* timer1_;
  const double updateInterval2_;
  QTimer* timer2_;

  void setDeviceState( State state );

  int driveAddress_;
  unsigned int status_;
  int motorID_;
  int phaseCurrent_;
  int standStillPhaseCurrent_;
  int stepMode_;
  int rampMode_;
  int errorCorrectionMode_;
  int maxEncoderDeviation_;
  int controllerSteps_;
  int encoderSteps_;
  int positioningMode_;
  bool direction_;
  bool encoderDirection_;
  int travelDistance_;
  int minFrequency_;
  int maxFrequency_;
  int maxFrequency2_;
  int quickstopRamp_;
  int accelRamp_;
  int decelRamp_;

  std::array<int,7> inputPinFunction_;
  std::array<int,4> outputPinFunction_;
  unsigned int ioMask_;
  unsigned int ioPolarityMask_;
  unsigned io_;

signals:

  void deviceStateChanged(State newState);
  void informationChanged();
  void controlStateChanged(bool);
  void stepModeChanged(int);

  void motionStarted();
  void motionFinished();
};

#endif // NANOTECSMCI36MODEL_H
