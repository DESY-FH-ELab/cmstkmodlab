#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <array>

#include <QObject>
#include <QMutex>
#include <QTimer>
#include <QDateTime>
#include <QXmlStreamWriter>
#include <QSocketNotifier>

#include <HistoryFifo.h>

#include <PumpStationModel.h>

class WatchDog : public QObject
{
  Q_OBJECT

public:

	WatchDog(PumpStationModel* model,
		     	 int history,
			     QObject *parent = 0);

public slots:
  
  void initialize();
  
protected slots:

  void switchStateChanged(int device, State newState);
  void pressureChanged(int sensor, double p);

protected:

  void checkValues();

  PumpStationModel* model_;

  QMutex mutex_;

  std::array<State,5> switchState_;
  HistoryFifo<double> pressure1_;
  HistoryFifo<double> pressure2_;
  HistoryFifo<double> pressure3_;

signals:

  void setSwitchEnabled(int channel, bool enabled);
  void setSwitchBlocked(int channel, bool blocked);
};

#endif // WATCHDOG_H

