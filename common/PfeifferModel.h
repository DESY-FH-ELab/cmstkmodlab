#ifndef PFEIFFERMODEL_H
#define PFEIFFERMODEL_H

#include <cmath>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Pfeiffer/PfeifferTPG262Fake.h"
typedef PfeifferTPG262Fake PfeifferTPG262_t;
#else
#include "devices/Pfeiffer/PfeifferTPG262.h"
typedef PfeifferTPG262 PfeifferTPG262_t;
#endif

/**
  Command and control model of the Pfeiffer TPG262 gauge.
  */
class PfeifferModel :
    public QObject
  , public AbstractDeviceModel<PfeifferTPG262_t>
{

  Q_OBJECT
public:
  explicit PfeifferModel(const char* port,
			 float updateInterval = 5,
			 QObject *parent = 0);

    VPfeifferTPG262::GaugeStatus getStatus1() const;
    double getPressure1() const;
    VPfeifferTPG262::GaugeStatus getStatus2() const;
    double getPressure2() const;

public slots:
  void setDeviceEnabled( bool enabled );
  void setControlsEnabled(bool enabled);

protected:

  const QString Pfeiffer_PORT;

  void initialize();

  VPfeifferTPG262::GaugeStatus status1_;
  double pressure1_;
  VPfeifferTPG262::GaugeStatus status2_;
  double pressure2_;

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  void setDeviceState( State state );

protected slots:
  void updateInformation();

signals:
  void deviceStateChanged( State newState );
  void informationChanged();
  void message(const QString & text);
  void controlStateChanged(bool);
};

#endif // PFEIFFERMODEL_H
