/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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

#include <MattermostBot.h>

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

  statusTimer_ = new QTimer(this);
  statusTimer_->setInterval(60 * 1000);
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

  connect(statusTimer_, SIGNAL(timeout()),
          this, SLOT(statusTimeout()));
  statusTimer_->start();
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

  // NQLog("WatchDog") << "pressureChanged(" << sensor << ", " << p << ")";

  switch (sensor) {
  case 1:
    pressure1_.push_back(p);
    break;
  case 2:
    pressure2_.push_back(p);
    break;
  case 3:
    pressure3_.push_back(p);
    break;
  }
}

void WatchDog::statusTimeout()
{
  QMutexLocker locker(&mutex_);

  int pump1 = model_->getPumpChannel(1);
  int pump2 = model_->getPumpChannel(2);

  int valve1 = model_->getValveChannel(1);
  int valve2 = model_->getValveChannel(2);
  int valve3 = model_->getValveChannel(3);

  double psys = model_->getPressure(1);
  double p1 = model_->getPressure(2);
  double p2 = model_->getPressure(3);

  switchState_[pump1] = model_->getSwitchState(pump1);
  switchState_[pump2] = model_->getSwitchState(pump2);
  switchState_[valve1] = model_->getSwitchState(valve1);
  switchState_[valve2] = model_->getSwitchState(valve2);
  switchState_[valve3] = model_->getSwitchState(valve3);

  pressure1_.push_back(psys);
  pressure2_.push_back(p1);
  pressure3_.push_back(p2);

  NQLog("WatchDog") << pressure1_.deltaTime();
  
  if (pressure1_.deltaTime()>0) {
    NQLog("WatchDog") << "slopes: " << pressure1_.gradient();
  }
  
  QTime t = QTime::currentTime();
  if (t.minute()!=0 && t.minute()!=30) return;

  if (switchState_[valve3]==OFF) return;

  QString message;
  message += "pump 1: " + QString::number((int)switchState_[pump1]);
  message += "/" + QString::number((int)switchState_[valve1]) + " p=";
  if (p1 >= 10.0) {
    message += QString::number(p1, 'f', 0);
  } else if (p1 >= 0.1) {
    message += QString::number(p1, 'f', 1);
  } else {
    message += QString::number(p1, 'E', 1);
  }
  message += " mbar\n";

  message += "pump 2: " + QString::number((int)switchState_[valve2]);
  message += "/" + QString::number((int)switchState_[valve2]) + " p=";
  if (p2 >= 10.0) {
    message += QString::number(p2, 'f', 0);
  } else if (p2 >= 0.1) {
    message += QString::number(p2, 'f', 1);
  } else {
    message += QString::number(p2, 'E', 1);
  }
  message += " mbar\n";

  message += "system: " + QString::number((int)switchState_[valve3]) +" p=";
  if (psys >= 10.0) {
    message += QString::number(psys, 'f', 0);
  } else if (psys >= 0.1) {
    message += QString::number(psys, 'f', 1);
  } else {
    message += QString::number(psys, 'E', 1);
  }
  message += " mbar\n";

  MattermostBot bot;
  bot.postMessage(message);
}
