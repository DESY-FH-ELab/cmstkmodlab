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

#include <QDebug>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "PumpStationHTTPModel.h"

PumpStationHTTPModel::PumpStationHTTPModel(double updateInterval,
                                           QObject * /* parent */)
  : QObject(),
    updateInterval_(updateInterval)
{
  SwitchBlocked0_ = SwitchBlocked1_ = SwitchBlocked2_ = SwitchBlocked3_ = SwitchBlocked4_ = false;
  SwitchState0_ = SwitchState1_ = SwitchState2_ = SwitchState3_ = SwitchState4_ = false;
  SensorState0_ = SensorState1_ = SensorState2_ = 0;
  Pressure0_ = Pressure1_ = Pressure2_ = -1.0;
  Pump0Hours_ = Pump1Hours_ = -1.0;

  getNetManager_ = new QNetworkAccessManager(this);
  connect(getNetManager_, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(getRequestReceived(QNetworkReply*)));

  setNetManager_ = new QNetworkAccessManager(this);
  connect(setNetManager_, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(setRequestReceived(QNetworkReply*)));

  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));
  timer_->start();
}

PumpStationHTTPModel::~PumpStationHTTPModel()
{
  delete getNetManager_;
  delete setNetManager_;
}

void PumpStationHTTPModel::updateInformation()
{
  qDebug() << "void PumpStationHTTPModel::updateInformation()";

  getNetManager_->get(QNetworkRequest(QUrl("http://cmsvakusaug.desy.de/PumpStationStatus.php")));
}

void PumpStationHTTPModel::getRequestReceived(QNetworkReply* reply)
{
  qDebug() << "void PumpStationHTTPModel::getRequestReceived(QNetworkReply* reply)";

  reply->deleteLater();

  QString json;

  if (reply->error() == QNetworkReply::NoError) {
    int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (v >= 200 && v < 300) {
      // Here we got the final reply
      json = reply->readAll();
    }
  } else {
    qDebug() << reply->errorString();
    QFile file;
    file.setFileName("/Users/mussgill/Downloads/PumpStationStatus.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    json = file.readAll();
    file.close();
  }

  QJsonDocument jdoc = QJsonDocument::fromJson(json.toUtf8());
  QJsonObject jobj = jdoc.object();

  bool changed = false;

  QJsonValue jvalue;
  QString svalue;
  bool bvalue;
  int ivalue;
  double dvalue;

  jvalue = jobj.value(QString("SwitchBlocked0"));
  bvalue = jvalue.toString().toInt();
  if (SwitchBlocked0_!=bvalue) {
    changed = true;
    SwitchBlocked0_ = bvalue;
    qDebug() << "SwitchBlocked0 changed:" <<  SwitchBlocked0_;
  }

  jvalue = jobj.value(QString("SwitchBlocked1"));
  bvalue = jvalue.toString().toInt();
  if (SwitchBlocked1_!=bvalue) {
    changed = true;
    SwitchBlocked1_ = bvalue;
    qDebug() << "SwitchBlocked1 changed:" <<  SwitchBlocked1_;
  }

  jvalue = jobj.value(QString("SwitchBlocked2"));
  bvalue = jvalue.toString().toInt();
  if (SwitchBlocked2_!=bvalue) {
    changed = true;
    SwitchBlocked2_ = bvalue;
    qDebug() << "SwitchBlocked2 changed:" <<  SwitchBlocked2_;
  }

  jvalue = jobj.value(QString("SwitchBlocked3"));
  bvalue = jvalue.toString().toInt();
  if (SwitchBlocked3_!=bvalue) {
    changed = true;
    SwitchBlocked3_ = bvalue;
    qDebug() << "SwitchBlocked3 changed:" <<  SwitchBlocked3_;
  }

  jvalue = jobj.value(QString("SwitchBlocked4"));
  bvalue = jvalue.toString().toInt();
  if (SwitchBlocked4_!=bvalue) {
    changed = true;
    SwitchBlocked4_ = bvalue;
    qDebug() << "SwitchBlocked4 changed:" <<  SwitchBlocked4_;
  }

  jvalue = jobj.value(QString("SwitchState0"));
  bvalue = jvalue.toString().toInt();
  if (SwitchState0_!=bvalue) {
    changed = true;
    SwitchState0_ = bvalue;
    qDebug() << "SwitchState0 changed:" <<  SwitchState0_;
  }

  jvalue = jobj.value(QString("SwitchState1"));
  bvalue = jvalue.toString().toInt();
  if (SwitchState1_!=bvalue) {
    changed = true;
    SwitchState1_ = bvalue;
    qDebug() << "SwitchState1 changed:" <<  SwitchState1_;
  }

  jvalue = jobj.value(QString("SwitchState2"));
  bvalue = jvalue.toString().toInt();
  if (SwitchState2_!=bvalue) {
    changed = true;
    SwitchState2_ = bvalue;
    qDebug() << "SwitchState2 changed:" <<  SwitchState2_;
  }

  jvalue = jobj.value(QString("SwitchState3"));
  bvalue = jvalue.toString().toInt();
  if (SwitchState3_!=bvalue) {
    changed = true;
    SwitchState3_ = bvalue;
    qDebug() << "SwitchState3 changed:" <<  SwitchState3_;
  }

  jvalue = jobj.value(QString("SwitchState4"));
  bvalue = jvalue.toString().toInt();
  if (SwitchState4_!=bvalue) {
    changed = true;
    SwitchState4_ = bvalue;
    qDebug() << "SwitchState4 changed:" <<  SwitchState4_;
  }

  jvalue = jobj.value(QString("SensorState0"));
  ivalue = jvalue.toString().toInt();
  if (SensorState0_!=ivalue) {
    changed = true;
    SensorState0_ = ivalue;
    qDebug() << "SensorState0 changed:" <<  SensorState0_;
  }

  jvalue = jobj.value(QString("SensorState1"));
  ivalue = jvalue.toString().toInt();
  if (SensorState1_!=ivalue) {
    changed = true;
    SensorState1_ = ivalue;
    qDebug() << "SensorState1 changed:" <<  SensorState1_;
  }

  jvalue = jobj.value(QString("SensorState2"));
  ivalue = jvalue.toString().toInt();
  if (SensorState2_!=ivalue) {
    changed = true;
    SensorState2_ = ivalue;
    qDebug() << "SensorState2 changed:" <<  SensorState2_;
  }

  jvalue = jobj.value(QString("Pressure0"));
  dvalue = jvalue.toString().toDouble();
  if (Pressure0_!=dvalue) {
    changed = true;
    Pressure0_ = dvalue;
    qDebug() << "Pressure0 changed:" <<  Pressure0_;
  }

  jvalue = jobj.value(QString("Pressure1"));
  dvalue = jvalue.toString().toDouble();
  if (Pressure1_!=dvalue) {
    changed = true;
    Pressure1_ = dvalue;
    qDebug() << "Pressure1 changed:" <<  Pressure1_;
  }

  jvalue = jobj.value(QString("Pressure2"));
  dvalue = jvalue.toString().toDouble();
  if (Pressure2_!=dvalue) {
    changed = true;
    Pressure2_ = dvalue;
    qDebug() << "Pressure2 changed:" <<  Pressure2_;
  }

  jvalue = jobj.value(QString("Pump0Hours"));
  dvalue = jvalue.toString().toDouble();
  if (Pump0Hours_!=dvalue) {
    changed = true;
    Pump0Hours_ = dvalue;
    qDebug() << "Pump0Hours changed:" <<  Pump0Hours_;
  }

  jvalue = jobj.value(QString("Pump1Hours"));
  dvalue = jvalue.toString().toDouble();
  if (Pump1Hours_!=dvalue) {
    changed = true;
    Pump1Hours_ = dvalue;
    qDebug() << "Pump1Hours changed:" <<  Pump1Hours_;
  }

  jvalue = jobj.value(QString("Timestamp"));
  svalue = jvalue.toString();
  timestamp_ = QDateTime::fromString(svalue, Qt::ISODate);
  qDebug() << "timestamp: " << svalue;

  emit timestampChanged();

  if (changed) emit valuesChanged();
}

void PumpStationHTTPModel::toggleSwitch(int n)
{
  qDebug() << "void PumpStationHTTPModel::toggleSwitch(int switch) " << n;

  timer_->stop();

  QString url = "http://cmsvakusaug.desy.de/ConradSwitch.php?channel=" + QString::number(n);

  setNetManager_->get(QNetworkRequest(QUrl(url)));
}

void PumpStationHTTPModel::setRequestReceived(QNetworkReply* reply)
{
  qDebug() << "void PumpStationHTTPModel::setRequestReceived(QNetworkReply* reply)";

  reply->deleteLater();

  if (reply->error() == QNetworkReply::NoError) {
    int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (v >= 200 && v < 300) {
      // Here we got the final reply
      qDebug() << reply->readAll();
    }
  }

  QTimer::singleShot(2000, this, SLOT(toggleFinished()));
}

void PumpStationHTTPModel::toggleFinished()
{
  updateInformation();

  timer_->start();

  emit enableWidgets();
}
