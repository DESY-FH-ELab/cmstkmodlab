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
  
  NQLogDebug("watchdog") << "void WatchDog::switchStateChanged(" << device << ", " << (int)newState << ")";
  
  switchState_[device] = newState;
  
  checkValues();
}

void WatchDog::pressureChanged(int sensor, double p)
{
  QMutexLocker locker(&mutex_);
  
  NQLogDebug("watchdog") << "void DataLogger::pressureChanged(" << sensor << ", " << p << ")";
  
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
  
  checkValues();
}

void WatchDog::checkValues()
{
	NQLogDebug("WatchDog") << "checkValues";

  for (int i=0;i<5;++i) {
    if (model_->getSwitchBlocked(i)) {
      emit setSwitchBlocked(i, false);
    }
  }
}
