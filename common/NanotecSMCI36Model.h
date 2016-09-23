#ifndef NANOTECSMCI36MODEL_H
#define NANOTECSMCI36MODEL_H

#include <cmath>

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
  Command and control model of the Hameg chiller.
  */
class NanotecSMCI36Model : public QObject, public AbstractDeviceModel<NanotecSMCI36_t>
{

  Q_OBJECT
public:

  explicit NanotecSMCI36Model(const char* port,
                              double updateInterval1 = 1,
                              double updateInterval2 = 5,
                              QObject *parent = 0);

  double getPitch() const { return pitch_; }

  unsigned int getStatus() const { return status_; }
  int getMotorID() const { return motorID_; }

  int getStepMode() const { return stepMode_; }
  const std::vector<std::pair<int,std::string>>& getStepModeNames() const;

  int getRampMode() const { return rampMode_; }
  const std::vector<std::pair<int,std::string>>& getRampModeNames() const;

  int getErrorCorrectionMode() const { return errorCorrectionMode_; }
  const std::vector<std::pair<int,std::string>>& getErrorCorrectionModeNames() const;

  int getMaxEncoderDeviation() const { return maxEncoderDeviation_; }

  int getControllerSteps() const { return controllerSteps_; }
  double getControllerPosition() const { return getControllerSteps()/getPitch()/getStepMode(); }

  int getEncoderSteps() const { return encoderSteps_; }
  double getEncoderPosition() const { return getEncoderSteps()/getPitch()/getStepMode(); }

  int getPositioningMode() const { return positioningMode_; }
  const std::vector<std::pair<int,std::string>>& getPositioningModeNames() const;

  bool getDirection() const { return direction_; }

  double getTravelDistance() const { return travelDistance_; }
  double getMinFrequency() const { return minFrequency_; }
  double getMaxFrequency() const { return maxFrequency_; }
  double getMaxFrequency2() const { return maxFrequency2_; }

  double getTravelDistanceInMM() const { return getTravelDistance()/getPitch()/getStepMode(); }
  double getMinSpeed() const { return getMinFrequency()/getPitch()/getStepMode(); }
  double getMaxSpeed() const { return getMaxFrequency()/getPitch()/getStepMode(); }
  double getMaxSpeed2() const { return getMaxFrequency2()/getPitch()/getStepMode(); }

public slots:

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);

  void setPitch(double pitch);
  void setMotorID(int motorID);
  void setStepMode(int mode);
  void setRampMode(int mode);
  void setErrorCorrectionMode(int mode);
  void setMaxEncoderDeviation(int steps);
  void setPositioningMode(int mode);
  void setDirection(bool direction);
  void setTravelDistance(double distance);
  void setMinFrequency(double frequency);
  void setMaxFrequency(double frequency);
  void setMaxFrequency2(double frequency);
  void setTravelDistanceInMM(double distance);
  void setMinSpeed(double speed);
  void setMaxSpeed(double speed);
  void setMaxSpeed2(double speed);

  void start();
  void stop();
  void quickStop();
  void resetPositionError();

protected:

  const QString NanotecSMCI36_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval1_;
  QTimer* timer1_;
  const double updateInterval2_;
  QTimer* timer2_;

  void setDeviceState( State state );

  double pitch_;
  unsigned int status_;
  int motorID_;
  int stepMode_;
  int rampMode_;
  int errorCorrectionMode_;
  int maxEncoderDeviation_;
  int controllerSteps_;
  int encoderSteps_;
  int positioningMode_;
  bool direction_;
  double travelDistance_;
  double minFrequency_;
  double maxFrequency_;
  double maxFrequency2_;

protected slots:

  void updateInformation1();
  void updateInformation2();

signals:

  void deviceStateChanged(State newState);
  void informationChanged();
  void message(const QString & text);
  void controlStateChanged(bool);
};

#endif // NANOTECSMCI36MODEL_H
