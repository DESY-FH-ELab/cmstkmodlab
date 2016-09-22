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

  unsigned int getStatus() const { return status_; }
  int getMotorID() const { return motorID_; }

  int getStepMode() const { return stepMode_; }
  const std::vector<std::pair<int,std::string>>& getStepModeNames() const;

  int getControllerSteps() const { return controllerSteps_; }
  int getEncoderSteps() const { return encoderSteps_; }

  int getPositioningMode() const { return positioningMode_; }
  const std::vector<std::pair<int,std::string>>& getPositioningModeNames() const;

  bool getDirection() const { return direction_; }
  double getTravelDistance() const { return travelDistance_; }

public slots:

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);

  void setMotorID(int motorID);
  void setStepMode(int mode);
  void setPositioningMode(int mode);
  void setDirection(bool direction);
  void setTravelDistance(double distance);

  void start();
  void stop();
  void quickStop();

protected:

  const QString NanotecSMCI36_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval1_;
  QTimer* timer1_;
  const double updateInterval2_;
  QTimer* timer2_;

  void setDeviceState( State state );

  unsigned int status_;
  int motorID_;
  int stepMode_;
  int controllerSteps_;
  int encoderSteps_;
  int positioningMode_;
  bool direction_;
  double travelDistance_;

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
