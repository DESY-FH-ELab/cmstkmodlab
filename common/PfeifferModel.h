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
  explicit PfeifferModel(QObject *parent = 0);

public slots:
  void setDeviceEnabled( bool enabled );
  void setControlsEnabled(bool enabled);

protected:

  static const QString Pfeiffer_PORT;

  void initialize();

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
