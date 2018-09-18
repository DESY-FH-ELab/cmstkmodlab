/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef PUMPSTATIONHTTPMODEL_H
#define PUMPSTATIONHTTPMODEL_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QDateTime>

class PumpStationHTTPModel : public QObject
{

  Q_OBJECT
public:

  explicit PumpStationHTTPModel(double updateInterval = 10,
                                QObject *parent = 0);
  ~PumpStationHTTPModel();

  bool getValveState1() { return SwitchState2_; }
  bool getValveState2() { return SwitchState3_; }
  bool getValveState3() { return SwitchState4_; }

  bool getValveBlocked1() { return SwitchBlocked2_; }
  bool getValveBlocked2() { return SwitchBlocked3_; }
  bool getValveBlocked3() { return SwitchBlocked4_; }

  bool getPumpState1() { return SwitchState0_; }
  bool getPumpState2() { return SwitchState1_; }

  bool getPumpBlocked1() { return SwitchBlocked0_; }
  bool getPumpBlocked2() { return SwitchBlocked1_; }

  double getPressure1() { return Pressure0_; }
  double getPressure2() { return Pressure1_; }
  double getPressure3() { return Pressure2_; }

  double getPump1Hours() { return Pump0Hours_; }
  double getPump2Hours() { return Pump1Hours_; }

  const QDateTime& getTimestamp() { return timestamp_; }
public slots:

  void updateInformation();
  void toggleSwitch(int);
  void toggleFinished();

protected slots:

  void getRequestReceived(QNetworkReply* reply);
  void setRequestReceived(QNetworkReply* reply);

protected:

  QTimer * timer_;
  int updateInterval_;

  QNetworkAccessManager * getNetManager_;
  QNetworkAccessManager * setNetManager_;

  bool SwitchBlocked0_, SwitchBlocked1_, SwitchBlocked2_, SwitchBlocked3_, SwitchBlocked4_;
  bool SwitchState0_, SwitchState1_, SwitchState2_, SwitchState3_, SwitchState4_;
  int SensorState0_, SensorState1_, SensorState2_;
  double Pressure0_, Pressure1_, Pressure2_;
  double Pump0Hours_, Pump1Hours_;
  QDateTime timestamp_;

signals:

  void valuesChanged();
  void timestampChanged();
  void enableWidgets();
};

#endif // PUMPSTATIONHTTPMODEL_H
