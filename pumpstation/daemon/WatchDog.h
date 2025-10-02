/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <array>

#include <QObject>
#include <QMutex>
#include <QTimer>
#include <QTime>
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
  void statusTimeout();

protected:

  PumpStationModel* model_;

  QTimer* statusTimer_;

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

