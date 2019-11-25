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

#include <iostream>

#include <QDateTime>
#include <QTextStream>

#include <nqlogger.h>

#include "ThermoDAQ2NetworkReader.h"

ThermoDAQ2NetworkReader::ThermoDAQ2NetworkReader(QObject* parent)
  : QObject(parent)
{

}

void ThermoDAQ2NetworkReader::run(QString& buffer)
{
  process(buffer);
  NQLogDebug("ThermoDAQ2NetworkReader") << "run(QString& buffer)";
  emit finished();
}

void ThermoDAQ2NetworkReader::processRohdeSchwarzNGE103B(QXmlStreamReader& xml)
{
  NQLogDebug("ThermoDAQ2NetworkReader") << "processRohdeSchwarzNGE103B(QXmlStreamReader& xml)";

  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);
}

void ThermoDAQ2NetworkReader::processRohdeSchwarzNGE103BChannel(QXmlStreamReader& xml)
{
  NQLogDebug("ThermoDAQ2NetworkReader") << "processRohdeSchwarzNGE103BChannel(QXmlStreamReader& xml)";

  int id = xml.attributes().value("id").toString().toInt();
  float mU = xml.attributes().value("mU").toString().toFloat();
  float mI = xml.attributes().value("mI").toString().toFloat();

  measurement_.nge103BVoltage[id-1] = mU;
  measurement_.nge103BCurrent[id-1] = mI;
}

/*
void ThermoDAQ2NetworkReader::processHuberTemperature(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

  measurement_.bathTemperature = xml.attributes().value("bath").toString().toFloat();
  measurement_.workingTemperature = xml.attributes().value("work").toString().toFloat();
  measurement_.circulator = xml.attributes().value("circulator").toString().toInt();
}

void ThermoDAQ2NetworkReader::processKeithleyState(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

  int idx = xml.attributes().value("sensor").toString().toInt();
  measurement_.channelActive[idx] = xml.attributes().value("state").toString().toInt();
}

void ThermoDAQ2NetworkReader::processKeithleyTemperature(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

  int idx = xml.attributes().value("sensor").toString().toInt();
  measurement_.temperature[idx] = xml.attributes().value("temperature").toString().toFloat();
}

void ThermoDAQ2NetworkReader::processPfeifferPressure(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

  measurement_.gaugeStatus1 = xml.attributes().value("s1").toString().toInt();
  measurement_.gaugePressure1 = xml.attributes().value("p1").toString().toFloat();
  measurement_.gaugeStatus2 = xml.attributes().value("s2").toString().toInt();
  measurement_.gaugePressure2 = xml.attributes().value("p2").toString().toFloat();
}

void ThermoDAQ2NetworkReader::processHamegSetup(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

  measurement_.powerRemote = xml.attributes().value("remote").toString().toInt();
  measurement_.powerOn = xml.attributes().value("outputs").toString().toInt();
  measurement_.cv1 = xml.attributes().value("CV1").toString().toInt();
  measurement_.cv2 = xml.attributes().value("CV2").toString().toInt();
}

void ThermoDAQ2NetworkReader::processHamegSetvalues(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

  measurement_.setVoltage1 = xml.attributes().value("V1").toString().toFloat();
  measurement_.setCurrent1 = xml.attributes().value("C1").toString().toFloat();
  measurement_.setVoltage2 = xml.attributes().value("V2").toString().toFloat();
  measurement_.setCurrent2 = xml.attributes().value("C2").toString().toFloat();
}

void ThermoDAQ2NetworkReader::processHamegValues(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

  measurement_.voltage1 = xml.attributes().value("V1").toString().toFloat();
  measurement_.current1 = xml.attributes().value("C1").toString().toFloat();
  measurement_.voltage2 = xml.attributes().value("V2").toString().toFloat();
  measurement_.current2 = xml.attributes().value("C2").toString().toFloat();
}
void ThermoDAQ2NetworkReader::processIotaSetup(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

  measurement_.iotaPumpEnabled = xml.attributes().value("enabled").toString().toInt();
  measurement_.iotaSetPressure = xml.attributes().value("pressure").toString().toFloat();
  measurement_.iotaSetFlow = xml.attributes().value("flow").toString().toFloat();
}

void ThermoDAQ2NetworkReader::processIotaValues(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

  measurement_.iotaActPressure = xml.attributes().value("pressure").toString().toFloat();
  measurement_.iotaActFlow = xml.attributes().value("flow").toString().toFloat();
}

void ThermoDAQ2NetworkReader::processArduinoPressure(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

  measurement_.arduinoPressureA = xml.attributes().value("pA").toString().toFloat();
  measurement_.arduinoPressureB = xml.attributes().value("pB").toString().toFloat();
}
*/

void ThermoDAQ2NetworkReader::processLine(QString& line)
{
  QXmlStreamReader xml(line);
  while (!xml.atEnd()) {
    xml.readNextStartElement();
    if (xml.isStartElement()) {

      if (xml.name()=="RohdeSchwarzNGE103B") {
        processRohdeSchwarzNGE103B(xml);
      }
      if (xml.name()=="RohdeSchwarzNGE103BChannel") {
        processRohdeSchwarzNGE103BChannel(xml);
      }

      /*
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
      */
    }
  }
}

void ThermoDAQ2NetworkReader::process(QString& buffer)
{
  QString line;
  QTextStream in(&buffer);
  while (!in.atEnd()) {
    line = in.readLine();

    NQLogDebug("ThermoDAQ2NetworkReader") << "process(QString& buffer) " << line;

    processLine(line);
  }
}
