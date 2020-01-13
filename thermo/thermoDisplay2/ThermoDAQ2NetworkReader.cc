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

void ThermoDAQ2NetworkReader::processKeithleyDAQ6510(QXmlStreamReader& xml)
{
  NQLogDebug("ThermoDAQ2NetworkReader") << "processKeithleyDAQ6510(QXmlStreamReader& xml)";

  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);
}

void ThermoDAQ2NetworkReader::processKeithleyDAQ6510Sensor(QXmlStreamReader& xml)
{
  NQLogDebug("ThermoDAQ2NetworkReader") << "processKeithleyDAQ6510Sensor(QXmlStreamReader& xml)";

  int id = xml.attributes().value("id").toString().toInt();
  unsigned int card = id / 100 - 1;
  unsigned int channel = id % 100 - 1;
  float temp = xml.attributes().value("T").toString().toFloat();

  NQLogDebug("ThermoDAQ2NetworkReader") << "card " << card+1 << " channel " << channel << " -> " << temp;

  measurement_.keithleyTemperature[card][channel] = temp;
}

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

    NQLogDebug("ThermoDAQ2NetworkReader") << "process(QString& buffer) " << line;

    processLine(line);
  }
}
