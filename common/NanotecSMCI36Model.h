#ifndef NANOTECSMCI36MODEL_H
#define NANOTECSMCI36MODEL_H

#include <cmath>
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

  unsigned int getStatus() const { return status_; }
  int getMotorID() const { return motorID_; }

  int getPhaseCurrent() const { return phaseCurrent_; }
  int getStandStillPhaseCurrent() const { return standStillPhaseCurrent_; }

  int getStepMode() const { return stepMode_; }
  const std::map<int,std::string>& getStepModeNames() const;

  int getRampMode() const { return rampMode_; }
  const std::map<int,std::string>& getRampModeNames() const;

  int getErrorCorrectionMode() const { return errorCorrectionMode_; }
  const std::map<int,std::string>& getErrorCorrectionModeNames() const;

  int getMaxEncoderDeviation() const { return maxEncoderDeviation_; }

  int getControllerPosition() const { return controllerSteps_; }

  int getEncoderPosition() const { return encoderSteps_; }

  int getPositioningMode() const { return positioningMode_; }
  const std::map<int,std::string>& getPositioningModeNames() const;

  bool getDirection() const { return direction_; }
  bool getEncoderDirection() const { return encoderDirection_; }


  double getTravelDistanceInMM() const { return getPitch()*getTravelDistance()/getStepMode(); }
  double getMinSpeed() const { return getPitch()*getMinFrequency()/getStepMode(); }
  double getMaxSpeed() const { return getPitch()*getMaxFrequency()/getStepMode(); }
  double getMaxSpeed2() const { return getPitch()*getMaxFrequency2()/getStepMode(); }

  double getMinPositionInMM() const { return minPositionInMM_; }
  double getMaxPositionInMM() const { return maxPositionInMM_; }
  int getTravelDistance() const { return travelDistance_; }
  int getMinFrequency() const { return minFrequency_; }
  int getMaxFrequency() const { return maxFrequency_; }
  int getMaxFrequency2() const { return maxFrequency2_; }

  void setMaxSpeedForOperation(double speed)  { maxSpeedForOperation_ = speed; }
  double getMaxSpeedForOperation() const { return maxSpeedForOperation_; }

  void setMaxSpeedForRefRun(double speed)  { maxSpeedForRefRun_ = speed; }
  double getMaxSpeedForRefRun() const { return maxSpeedForRefRun_; }

  int getInputPinFunction(int pin) const;
  const std::map<int,std::string>& getInputPinFunctionNames() const;

  bool getInputPolarity(int pin) const;
  bool getInputPinState(int pin) const;

  int getOutputPinFunction(int pin) const;
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
  void setPositioningMode(int mode);
  void setDirection(bool direction);
  void setTravelDistanceInMM(double distance);
  void setMinSpeed(double speed);
  void setMaxSpeed(double speed);
  void setMaxSpeed2(double speed);

  void setMinPositionInMM(double position);
  void setMaxPositionInMM(double position);
  void setEncoderDirection(bool direction);

  void start();
  void stop();
  void quickStop();
  void resetPositionError();

  void setInputPinFunction(int pin, int function);
  void setInputPolarity(int pin, bool reverse);

  void setOutputPinFunction(int pin, int function);
  void setOutputPolarity(int pin, bool reverse);

public slots:

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);

  void setPositioningMode(int mode);
  void setTravelDistance(int distance);
  void setMinFrequency(int frequency);
  void setMaxFrequency(int frequency);
  void setMaxFrequency2(int frequency);
  void setOutputPinState(int pin, bool state);

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

  double maxSpeedForOperation_;
  double maxSpeedForRefRun_;
  double minPositionInMM_;
  double maxPositionInMM_;
  bool encoderDirection_;
  int travelDistance_;
  int minFrequency_;
  int maxFrequency_;
  int maxFrequency2_;

  std::array<int,7> inputPinFunction_;
  std::array<int,4> outputPinFunction_;
  unsigned int ioPolarityMask_;
  unsigned io_;

  void setTravelDistanceNoCheck(double distance);
  void setTravelDistanceInMMNoCheck(double distance);
  void checkPositionLimits();

signals:

  void deviceStateChanged(State newState);
  void informationChanged();
  void positionModeChanged(int mode);
  void message(const QString & text);
  void controlStateChanged(bool);
};

#endif // NANOTECSMCI36MODEL_H
