#ifndef ARDUINOPRESMODEL_H
#define ARDUINOPRESMODEL_H

#include <cmath>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Arduino/ArduinoPresFake.h"
typedef ArduinoPresFake ArduinoPres_t;
#else
#include "devices/Arduino/ArduinoPres.h"
typedef ArduinoPres ArduinoPres_t;
#endif

/**
  Command and control model of the petiteFleur chiller.
  */
class ArduinoPresModel : public QObject,
                         public AbstractDeviceModel<ArduinoPres_t>
{
  Q_OBJECT
public:
  explicit ArduinoPresModel(const char* port,
                            float updateInterval = 5,
                            QObject *parent = 0);

  float getPressureA() const;
  float getPressureB() const;

  public slots:

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);
  
  protected:

  const QString ArduinoPres_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  void setDeviceState( State state );

  template <class T> void updateParameterCache(DeviceParameter<T>& parameter,
                                               const T& value);

  float PressureA_;
  float PressureB_;

  protected slots:

  void updateInformation();

  signals:

  void deviceStateChanged( State newState );
  void informationChanged();
  void message(const QString & text);
  void controlStateChanged(bool);
};

#endif // ARDUINOPRESMODEL_H
