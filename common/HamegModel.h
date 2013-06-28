#ifndef HAMEGMODEL_H
#define HAMEGMODEL_H

#include <cmath>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Hameg/Hameg8143Fake.h"
typedef Hameg8143Fake Hameg8143_t;
#else
#include "devices/Hameg/Hameg8143.h"
typedef Hameg8143 Hameg8143_t;
#endif

/**
  Command and control model of the Hameg chiller.
  */
class HamegModel :
    public QObject
  , public AbstractDeviceModel<Hameg8143_t>
{

  Q_OBJECT
public:
  explicit HamegModel(QObject *parent = 0);

public slots:
  void setDeviceEnabled( bool enabled );
  void setControlsEnabled(bool enabled);

protected:

  static const QString Hameg_PORT;

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

#endif // HAMEGMODEL_H
