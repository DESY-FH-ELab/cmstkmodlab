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
                              int updateInterval = 10,
                              QObject *parent = 0);

  unsigned int getStatus() const { return status_; }

public slots:

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);

protected:

  const QString NanotecSMCI36_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  void setDeviceState( State state );

  unsigned int status_;

protected slots:

  void updateInformation();

signals:

  void deviceStateChanged(State newState);
  void informationChanged();
  void message(const QString & text);
  void controlStateChanged(bool);
};

#endif // NANOTECSMCI36MODEL_H
