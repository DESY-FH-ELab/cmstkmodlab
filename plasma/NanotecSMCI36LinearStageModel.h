#ifndef NANOTECSMCI36LINEARSTAGEMODEL_H
#define NANOTECSMCI36LINEARSTAGEMODEL_H

#include <NanotecSMCI36Model.h>

class NanotecSMCI36LinearStageModel : public QObject
{

  Q_OBJECT
public:

  explicit NanotecSMCI36LinearStageModel(NanotecSMCI36Model * controller,
                                         QObject *parent = 0);

  State getDeviceState() const;

  unsigned int getStatus() const { return status_; }

  void setPitch(double pitch);
  double getPitch() const { return pitch_; }

  void setMinimumPosition(double position) { minPosition_ = position; }
  double getMinimumPosition() const { return minPosition_; }

  void setMaximumPosition(double position) { maxPosition_ = position; }
  double getMaximumPosition() const { return maxPosition_; }

  double getPosition() const { return position_; }

  const std::pair<double,double>& getSpeedLimits() const { return speedLimits_; }
  double getSpeed() const { return speed_; }

public slots:

  void updateDeviceState(State newState);
  void setControlsEnabled(bool enabled);
  void updateInformation();
  void setSpeed(double);
  void requestMove(double);
  void requestReferenceRun();
  void requestStop();
  void requestQuickStop();
  void requestResetError();

protected:

  NanotecSMCI36Model* controller_;

  double pitch_;
  double minPosition_;
  double maxPosition_;

  unsigned int status_;
  double position_;
  std::pair<double,double> speedLimits_;
  double speed_;

protected slots:

  void stepModeChanged(int);

signals:

  void deviceStateChanged(State newState);
  void informationChanged();
  void controlStateChanged(bool);
  void limitsChanged();
};

#endif // NANOTECSMCI36LINEARSTAGEMODEL_H
