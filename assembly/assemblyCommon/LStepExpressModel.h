#ifndef LSTEPEXPRESSMODEL_H
#define LSTEPEXPRESSMODEL_H

#include <cmath>
#include <vector>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Lang/LStepExpressFake.h"
typedef LStepExpressFake LStepExpress_t;
#else
#include "devices/Lang/LStepExpress.h"
typedef LStepExpress LStepExpress_t;
#endif

class LStepExpressModel : public QObject, public AbstractDeviceModel<LStepExpress_t>
{
  Q_OBJECT

public:

  explicit LStepExpressModel(const char* port,
                             float updateInterval = 5,
                             QObject *parent = 0);

public slots:

  void setDeviceEnabled(bool enabled = true);
  void setControlsEnabled(bool enabled);

protected:

  const QString LStepExpress_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  void setDeviceState( State state );

  std::vector<int> axisStatus_;
  std::vector<int> axis_;
  std::vector<int> axisDirection_;
  std::vector<int> dim_;
  std::vector<int> pa_;

protected slots:

  void updateInformation();

signals:

  void deviceStateChanged(State newState);
  void informationChanged();
  void message(const QString & text);
  void controlStateChanged(bool);
};

#endif // LSTEPEXPRESSMODEL_H
