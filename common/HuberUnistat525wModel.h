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

#ifndef HUBERUNISTAT525WMODEL_H
#define HUBERUNISTAT525WMODEL_H

#include <cmath>

#include <QString>
#include <QTimer>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Huber/HuberPilotOneFake.h"
typedef HuberPilotOneFake HuberPilotOne_t;
#else
#include "devices/Huber/HuberPilotOne.h"
typedef HuberPilotOne HuberPilotOne_t;
#endif

/**
  Command and control model of the Huber Unistat 525w chiller.
  */
class HuberUnistat525wModel : public QObject,
                              public AbstractDeviceModel<HuberPilotOne_t>
{
  Q_OBJECT
public:

  explicit HuberUnistat525wModel(const char* port,
                                 float updateInterval = 5,
                                 QObject *parent = 0);

  double getTemperatureSetPoint() const;
  bool getTemperatureControlMode() const;
  bool getTemperatureControlEnabled() const;
  bool getCirculatorEnabled() const;

  double getInternalTemperature() const;
  double getProcessTemperature() const;
  double getReturnTemperature() const;

  double getPumpPressure() const;
  int getPower() const;

  double getCoolingWaterInletTemperature() const;
  double getCoolingWaterOutletTemperature() const;

  bool getAutoPID() const;
  int getKpInternal() const;
  double getTnInternal() const;
  double getTvInternal() const;
  int getKpJacket() const;
  double getTnJacket() const;
  double getTvJacket() const;
  int getKpProcess() const;
  double getTnProcess() const;
  double getTvProcess() const;
  int getKp() const;
  double getTn() const;
  double getTv() const;

  void statusMessage(const QString & text);

public slots:

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);

  void setTemperatureSetPoint(double temperature);
  void setTemperatureControlMode(bool process);
  void setTemperatureControlEnabled(bool enabled);
  void setCirculatorEnabled(bool enabled);

  void setAutoPID(bool autoPID);

  void setKpInternal(int Kp);
  void setTnInternal(double Tn);
  void setTvInternal(double Tv);

  void setKpJacket(int Kp);
  void setTnJacket(double Tn);
  void setTvJacket(double Tv);

  void setKpProcess(int Kp);
  void setTnProcess(double Tn);
  void setTvProcess(double Tv);

  void setKp(int Kp);
  void setTn(double Tn);
  void setTv(double Tv);
  void setPID(int Kp, double Tn, double Tv);

protected:

  const QString HuberUnistat525w_PORT;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  void setDeviceState( State state );

  double temperatureSetPoint_;
  bool temperatureControlMode_;
  bool temperatureControlEnabled_;
  bool circulatorEnabled_;

  double internalTemperature_;
  double processTemperature_;
  double returnTemperature_;

  double pumpPressure_;
  int power_;

  double cwInletTemperature_;
  double cwOutletTemperature_;

  bool autoPID_;
  int KpInternal_;
  double TnInternal_;
  double TvInternal_;
  int KpJacket_;
  double TnJacket_;
  double TvJacket_;
  int KpProcess_;
  double TnProcess_;
  double TvProcess_;

protected slots:

  void updateInformation();

signals:

  void deviceStateChanged( State newState );
  void informationChanged();
  void message(const QString & text);
  void log(const QString & text);
  void controlStateChanged(bool);
};

#endif // HUBERUNISTAT525WMODEL_H
