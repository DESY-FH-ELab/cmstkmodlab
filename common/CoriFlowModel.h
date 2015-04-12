#ifndef CORIFLOWMODEL_H
#define CORIFLOWMODEL_H

#include <cmath>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Cori/CoriFlowFake.h"
typedef CoriFlowFake CoriFlow_t;
#else
#include "devices/Cori/CoriFlow.h"
typedef CoriFlow CoriFlow_t;
#endif

/**
  Command and control model of the Cori flow.
  */
class CoriFlowModel : public QObject,
public AbstractDeviceModel<CoriFlow_t>
{
  Q_OBJECT
public:
  explicit CoriFlowModel(const char* port,
                     float updateInterval = 4,
                     QObject *parent = 0);

  float getTemp() const;
  float getPres() const;
  float getMeasure() const;

  public slots:

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);
  
  protected:

  const QString CoriFlow_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  void setDeviceState( State state );

  template <class T> void updateParameterCache(DeviceParameter<T>& parameter,
                                               const T& value);

  float Temp_;
  float Pres_;
  float Measure_;

  protected slots:

  void updateInformation();

  signals:

  void deviceStateChanged( State newState );
  void informationChanged();
  void message(const QString & text);
  void controlStateChanged(bool);
};

#endif // CORIFLOWMODEL_H
