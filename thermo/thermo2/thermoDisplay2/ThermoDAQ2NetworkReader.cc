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
  for (int channel=0;channel<3;++channel) {
    measurement_.nge103BState[channel] = false;
    measurement_.nge103BVoltage[channel] = 0.0;
    measurement_.nge103BCurrent[channel] = 0.0;
  }

  for (unsigned int card = 0;card<2;++card) {
    for (unsigned int channel = 0;channel<10;++channel) {
      measurement_.keithleyState[card][channel] = false;
      measurement_.keithleyTemperature[card][channel] = 0.0;
    }
  }
}

void ThermoDAQ2NetworkReader::run(QString& buffer)
{
  process(buffer);
  // NQLogDebug("ThermoDAQ2NetworkReader") << "run(QString& buffer)";
  emit finished();
}

void ThermoDAQ2NetworkReader::processHuberUnistat525w(QXmlStreamReader& xml)
{
  // NQLogDebug("ThermoDAQ2NetworkReader") << "processHuberUnistat525w(QXmlStreamReader& xml)";

  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.u525wState_ = xml.attributes().value("State").toString().toInt();
}

void ThermoDAQ2NetworkReader::processHuberUnistat525wControl(QXmlStreamReader& xml)
{
  // NQLogDebug("ThermoDAQ2NetworkReader") << "processHuberUnistat525wControl(QXmlStreamReader& xml)";

  float SetPoint = xml.attributes().value("SetPoint").toString().toFloat();
  bool ControlMode = xml.attributes().value("ControlMode").toString().toInt();
  bool ControlEnabled = xml.attributes().value("ControlEnabled").toString().toInt();
  bool CirculatorEnabled = xml.attributes().value("CirculatorEnabled").toString().toInt();

  measurement_.u525wTemperatureSetPoint_ = SetPoint;
  measurement_.u525wTemperatureControlMode_ = ControlMode;
  measurement_.u525wTemperatureControlEnabled_ = ControlEnabled;
  measurement_.u525wCirculatorEnabled_ = CirculatorEnabled;
}

void ThermoDAQ2NetworkReader::processHuberUnistat525wInfo(QXmlStreamReader& xml)
{
  float Bath = xml.attributes().value("Bath").toString().toFloat();
  float Return = xml.attributes().value("Return").toString().toFloat();
  float Pressure = xml.attributes().value("Pressure").toString().toFloat();
  int Power = xml.attributes().value("Power").toString().toInt();
  float CWI = xml.attributes().value("CWI").toString().toFloat();
  float CWO = xml.attributes().value("CWO").toString().toFloat();

  measurement_.u525wBathTemperature_ = Bath;
  measurement_.u525wReturnTemperature_ = Return;
  measurement_.u525wPumpPressure_ = Pressure;
  measurement_.u525wPower_ = Power;
  measurement_.u525wCWInletTemperature_ = CWI;
  measurement_.u525wCWOutletTemperature_ = CWO;
}

void ThermoDAQ2NetworkReader::processLeyboldGraphixOne(QXmlStreamReader& xml)
{
  // NQLogDebug("ThermoDAQ2NetworkReader") << "processLeyboldGraphixOne(QXmlStreamReader& xml)";

  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.leyboldState_ = xml.attributes().value("State").toString().toInt();
  measurement_.leyboldPressure_ = xml.attributes().value("Pressure").toString().toDouble();
}

void ThermoDAQ2NetworkReader::processRohdeSchwarzNGE103B(QXmlStreamReader& xml)
{
  // NQLogDebug("ThermoDAQ2NetworkReader") << "processRohdeSchwarzNGE103B(QXmlStreamReader& xml)";

  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);
}

void ThermoDAQ2NetworkReader::processRohdeSchwarzNGE103BChannel(QXmlStreamReader& xml)
{
  // NQLogDebug("ThermoDAQ2NetworkReader") << "processRohdeSchwarzNGE103BChannel(QXmlStreamReader& xml)";

  int id = xml.attributes().value("id").toString().toInt();
  bool state = xml.attributes().value("State").toString().toInt();
  float mU = xml.attributes().value("mU").toString().toFloat();
  float mI = xml.attributes().value("mI").toString().toFloat();

  measurement_.nge103BState[id-1] = state;
  measurement_.nge103BVoltage[id-1] = mU;
  measurement_.nge103BCurrent[id-1] = mI;
}

void ThermoDAQ2NetworkReader::processKeithleyDAQ6510(QXmlStreamReader& xml)
{
  // NQLogDebug("ThermoDAQ2NetworkReader") << "processKeithleyDAQ6510(QXmlStreamReader& xml)";

  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);
}

void ThermoDAQ2NetworkReader::processKeithleyDAQ6510Sensor(QXmlStreamReader& xml)
{
  // NQLogDebug("ThermoDAQ2NetworkReader") << "processKeithleyDAQ6510Sensor(QXmlStreamReader& xml)";

  int id = xml.attributes().value("id").toString().toInt();
  unsigned int card = id / 100 - 1;
  unsigned int channel = id % 100 - 1;
  bool state = xml.attributes().value("State").toString().toInt();
  float temp = xml.attributes().value("T").toString().toFloat();

  measurement_.keithleyState[card][channel] = state;
  measurement_.keithleyTemperature[card][channel] = temp;
}

void ThermoDAQ2NetworkReader::processLine(QString& line)
{
  QXmlStreamReader xml(line);
  while (!xml.atEnd()) {
    xml.readNextStartElement();
    if (xml.isStartElement()) {

      if (xml.name()=="HuberUnistat525w") {
        processHuberUnistat525w(xml);
      }
      if (xml.name()=="HuberUnistat525wControl") {
        processHuberUnistat525wControl(xml);
      }
      if (xml.name()=="HuberUnistat525wInfo") {
        processHuberUnistat525wInfo(xml);
      }

      if (xml.name()=="LeyboldGraphixOne") {
        processLeyboldGraphixOne(xml);
      }

      if (xml.name()=="RohdeSchwarzNGE103B") {
        processRohdeSchwarzNGE103B(xml);
      }
      if (xml.name()=="RohdeSchwarzNGE103BChannel") {
        processRohdeSchwarzNGE103BChannel(xml);
      }

      if (xml.name()=="KeithleyDAQ6510") {
        processKeithleyDAQ6510(xml);
      }
      if (xml.name()=="KeithleyDAQ6510Sensor") {
        processKeithleyDAQ6510Sensor(xml);
      }
    }
  }
}

void ThermoDAQ2NetworkReader::process(QString& buffer)
{
  QString line;
  QTextStream in(&buffer);
  while (!in.atEnd()) {
    line = in.readLine();

    // NQLogDebug("ThermoDAQ2NetworkReader") << "process(QString& buffer) " << line;

    processLine(line);
  }
}
