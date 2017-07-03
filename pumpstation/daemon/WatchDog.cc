#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <iostream>

#include <QApplication>
#include <QMutexLocker>
#include <QProcess>
#include <QXmlStreamWriter>

#include <nqlogger.h>
#include <ApplicationConfig.h>
#include <SlackBot.h>

#include "WatchDog.h"

WatchDog::WatchDog(PumpStationModel* model,
		               int history,
		               QObject *parent)
 : QObject(parent),
   model_(model),
   pressure1_(HistoryFifo<double>(history)),
   pressure2_(HistoryFifo<double>(history)),
   pressure3_(HistoryFifo<double>(history))
{
  connect(model_, SIGNAL(dataValid()),
          this, SLOT(initialize()));
}

void WatchDog::initialize()
{
  connect(model_, SIGNAL(switchStateChanged(int, State)),
          this, SLOT(switchStateChanged(int, State)));

  connect(model_, SIGNAL(pressureChanged(int,double)),
          this, SLOT(pressureChanged(int,double)));

  connect(this, SIGNAL(setSwitchBlocked(int, bool)),
          model_, SLOT(setSwitchBlocked(int, bool)));

  connect(this, SIGNAL(setSwitchEnabled(int, bool)),
          model_, SLOT(setSwitchEnabled(int, bool)));

}

void WatchDog::switchStateChanged(int device, State newState)
{
  if (newState!=OFF && newState!=READY) return;
  
  QMutexLocker locker(&mutex_);
  
  // NQLogSpam("WatchDog") << "switchStateChanged(" << device << ", " << (int)newState << ")";
  
  switchState_[device] = newState;

	/*
	int pumpChannel;
	int valveChannel;

	for (int pump=1;pump<=2;pump++) {
		pumpChannel = model_->getPumpChannel(pump);
		valveChannel = model_->getValveChannel(pump);

		// pump is on and corresponding valve is blocked => unblock valve
		if (switchState_[pumpChannel]==READY && model_->getSwitchBlocked(valveChannel)==true) {
			NQLogSpam("WatchDog") << "unblock valve " << pump;
			emit setSwitchBlocked(valveChannel, false);
		}

		// pump is off and corresponding valve is open => close and block valve
		if (switchState_[pumpChannel]==OFF) {
			if (switchState_[valveChannel]==READY) {
				NQLogSpam("WatchDog") << "close valve " << pump;
				emit setSwitchEnabled(valveChannel, false);
			}
			if (model_->getSwitchBlocked(valveChannel)==false) {
				NQLogSpam("WatchDog") << "block valve " << pump;
				emit setSwitchBlocked(valveChannel, true);
			}
		}
	}
  */

}

void WatchDog::pressureChanged(int sensor, double p)
{
  QMutexLocker locker(&mutex_);

  // NQLogSpam("WatchDog") << "pressureChanged(" << sensor << ", " << p << ")";

  switch (sensor) {
  case 1:
    pressure1_.push(p);
    break;
  case 2:
    pressure2_.push(p);
    break;
  case 3:
    pressure3_.push(p);
    break;
  }
}
