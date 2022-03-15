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

#include <iostream>

#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include "ThermoDAQStreamReader.h"

ThermoDAQStreamReader::ThermoDAQStreamReader(const QString &filename,
    QObject* parent)
: QObject(parent),
  filename_(filename),
  measurementValid_(false)
{
  log_.message = "";
}

void ThermoDAQStreamReader::run()
{
  process();
  emit finished();
}

void ThermoDAQStreamReader::processHuberTemperature(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime= dt;

  float work = xml.attributes().value("work").toString().toFloat();
  bool circulator = xml.attributes().value("circulator").toString().toInt();

  if (!measurementValid_ || measurement_.workingTemperature!= work) {
    std::cout << measurement_.datime.toString("[yyyy-MM-dd-hh-mm-ss] - ").toStdString();
    std::cout << "Huber work = " << work << std::endl;
  }
  if (!measurementValid_ || measurement_.circulator!= circulator) {
    std::cout << measurement_.datime.toString("[yyyy-MM-dd-hh-mm-ss] - ").toStdString();
    std::cout << "Huber circulator = " << circulator << std::endl;
  }

  measurement_.bathTemperature = xml.attributes().value("bath").toString().toFloat();
  measurement_.workingTemperature = work;
  measurement_.circulator = circulator;
}

void ThermoDAQStreamReader::processKeithleyState(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime= dt;

  int idx = xml.attributes().value("sensor").toString().toInt();
  measurement_.channelActive[idx] = xml.attributes().value("state").toString().toInt();
}

void ThermoDAQStreamReader::processKeithleyTemperature(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime= dt;

  int idx = xml.attributes().value("sensor").toString().toInt();
  measurement_.temperature[idx] = xml.attributes().value("temperature").toString().toFloat();
}

void ThermoDAQStreamReader::processPfeifferPressure(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime= dt;

  measurement_.gaugeStatus1 = xml.attributes().value("s1").toString().toInt();
  measurement_.gaugePressure1 = xml.attributes().value("p1").toString().toFloat();
  measurement_.gaugeStatus2 = xml.attributes().value("s2").toString().toInt();
  measurement_.gaugePressure2 = xml.attributes().value("p2").toString().toFloat();
}

void ThermoDAQStreamReader::processHamegSetup(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime= dt;

  measurement_.powerRemote = xml.attributes().value("remote").toString().toInt();
  measurement_.powerOn = xml.attributes().value("outputs").toString().toInt();
  measurement_.cv1 = xml.attributes().value("CV1").toString().toInt();
  measurement_.cv2 = xml.attributes().value("CV2").toString().toInt();
}

void ThermoDAQStreamReader::processHamegSetvalues(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime= dt;

  measurement_.setVoltage1 = xml.attributes().value("V1").toString().toFloat();
  measurement_.setCurrent1 = xml.attributes().value("C1").toString().toFloat();
  measurement_.setVoltage2 = xml.attributes().value("V2").toString().toFloat();
  measurement_.setCurrent2 = xml.attributes().value("C2").toString().toFloat();
}

void ThermoDAQStreamReader::processHamegValues(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime= dt;

  measurement_.voltage1 = xml.attributes().value("V1").toString().toFloat();
  measurement_.current1 = xml.attributes().value("C1").toString().toFloat();
  measurement_.voltage2 = xml.attributes().value("V2").toString().toFloat();
  measurement_.current2 = xml.attributes().value("C2").toString().toFloat();
}

void ThermoDAQStreamReader::processIotaSetup(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime= dt;

  measurement_.iotaPumpEnabled = xml.attributes().value("enabled").toString().toInt();
  measurement_.iotaSetPressure = xml.attributes().value("pressure").toString().toFloat();
  measurement_.iotaSetFlow = xml.attributes().value("flow").toString().toFloat();
}

void ThermoDAQStreamReader::processIotaValues(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime= dt;

  measurement_.iotaActPressure = xml.attributes().value("pressure").toString().toFloat();
  measurement_.iotaActFlow = xml.attributes().value("flow").toString().toFloat();
}

void ThermoDAQStreamReader::processArduinoPressure(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime= dt;

  measurement_.arduinoPressureA = xml.attributes().value("pA").toString().toFloat();
  measurement_.arduinoPressureB = xml.attributes().value("pB").toString().toFloat();
}

void ThermoDAQStreamReader::processLog(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::TextDate);
  log_.uTime = dt.toTime_t();
  log_.datime= dt;

  log_.message = xml.readElementText(QXmlStreamReader::IncludeChildElements).toStdString();


  std::cout << measurement_.datime.toString("[yyyy-MM-dd-hh-mm-ss] - ").toStdString();
  std::cout << "LOG: " << log_.message << std::endl;
}

void ThermoDAQStreamReader::processDAQStarted(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime= dt;

  measurementValid_ = true;
}

void ThermoDAQStreamReader::processLine(QString& line)
{
  QXmlStreamReader xml(line);
  while (!xml.atEnd()) {
    xml.readNextStartElement();
    if (xml.isStartElement()) {
      if (xml.name()=="HuberTemperature") {
        processHuberTemperature(xml);
      }
      if (xml.name()=="KeithleyState") {
        processKeithleyState(xml);
      }
      if (xml.name()=="KeithleyTemperature") {
        processKeithleyTemperature(xml);
      }
      if (xml.name()=="PfeifferPressure") {
        processPfeifferPressure(xml);
      }
      if (xml.name()=="HamegSetup") {
        processHamegSetup(xml);
      }
      if (xml.name()=="HamegSetvalues") {
        processHamegSetvalues(xml);
      }
      if (xml.name()=="HamegValues") {
        processHamegValues(xml);
      }
      if (xml.name()=="IotaSetup") {
        processIotaSetup(xml);
      }
      if (xml.name()=="IotaValues") {
        processIotaValues(xml);
      }
      if (xml.name()=="ArduinoPressure") {
        processArduinoPressure(xml);
      }
      if (xml.name()=="Log") {
        processLog(xml);
      }
      if (xml.name()=="DAQStarted") {
        processDAQStarted(xml);
      }
    }
  }
}

void ThermoDAQStreamReader::process()
{
  QFile file(filename_);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

  QString line;
  QTextStream in(&file);
  while (!in.atEnd()) {
    line = in.readLine();
    processLine(line);
  }
}
