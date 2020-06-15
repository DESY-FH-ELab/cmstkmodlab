#ifndef IOTAMODEL_H
#define IOTAMODEL_H

#include <cmath>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Iota/Iota300Fake.h"
typedef Iota300Fake Iota_t;
#else
#include "devices/Iota/Iota300.h"
typedef Iota300 Iota_t;
#endif

/** @addtogroup common
 *  @{
 */

/** @addtogroup models
 *  @{
 */

/**
  Command and control model of the petiteFleur chiller.
  */
class IotaModel : public QObject,
public AbstractDeviceModel<Iota_t>
{
  Q_OBJECT
public:
  explicit IotaModel(const char* port,
                     float updateInterval = 2,
                     QObject *parent = 0);

  bool isPumpEnabled() const;
  const DeviceParameterFloat& getStatusParameter() const;
  float getActPressure() const;
  float getActFlow() const;
  const DeviceParameterFloat& getSetPressureParameter() const;
  const DeviceParameterFloat& getSetFlowParameter() const;

  public slots:

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);
  
  void setPumpEnabled( bool enabled );
  
  void setPressureValue( double pressure );
  void setFlowValue( double flow );

  protected:

  const QString Iota_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  void setDeviceState( State state );

  template <class T> void updateParameterCache(DeviceParameter<T>& parameter,
                                               const T& value);

  bool pumpEnabled_;
  float actPressure_;
  DeviceParameterFloat setPressure_;
  float actFlow_;
  DeviceParameterFloat setFlow_;

  protected slots:

  void updateInformation();

  signals:

  void deviceStateChanged( State newState );
  void informationChanged();
  void message(const QString & text);
  void controlStateChanged(bool);
};

/** @} */

/** @} */

#endif // IOTAMODEL_H
