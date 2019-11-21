/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef ROHDESCHWARZNGE103BMODEL_H
#define ROHDESCHWARZNGE103BMODEL_H

#include <cmath>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/RohdeSchwarz/RohdeSchwarzNGE103BFake.h"
typedef RohdeSchwarzNGE103BFake RohdeSchwarzNGE103B_t;
#else
#include "devices/RohdeSchwarz/RohdeSchwarzNGE103B.h"
typedef RohdeSchwarzNGE103B RohdeSchwarzNGE103B_t;
#endif

/**
  Command and control model of the RohdeSchwarzNGE103B power supply.
  */
class RohdeSchwarzNGE103BModel :
    public QObject
  , public AbstractDeviceModel<RohdeSchwarzNGE103B_t>
{

  Q_OBJECT
public:
  explicit RohdeSchwarzNGE103BModel(const char* port,
		      float updateInterval = 5,
		      QObject *parent = 0);

public slots:
  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);

protected:

  const QString RohdeSchwarzNGE103B_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  void setDeviceState( State state );

protected slots:
  void updateInformation();

signals:
  void deviceStateChanged(State newState);
  void informationChanged();
  void message(const QString & text);
  void controlStateChanged(bool);
};

#endif // ROHDESCHWARZNGE103BMODEL_H
