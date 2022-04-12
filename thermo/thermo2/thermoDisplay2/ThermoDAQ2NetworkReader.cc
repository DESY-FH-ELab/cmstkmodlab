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

  measurement_.martaState_ = false;
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
  float Internal = xml.attributes().value("Internal").toString().toFloat();
  float Process = xml.attributes().value("Process").toString().toFloat();
  float Return = xml.attributes().value("Return").toString().toFloat();
  float Pressure = xml.attributes().value("Pressure").toString().toFloat();
  int Power = xml.attributes().value("Power").toString().toInt();
  float CWI = xml.attributes().value("CWI").toString().toFloat();
  float CWO = xml.attributes().value("CWO").toString().toFloat();

  measurement_.u525wInternalTemperature_ = Internal;
  measurement_.u525wProcessTemperature_ = Process;
  measurement_.u525wReturnTemperature_ = Return;
  measurement_.u525wPumpPressure_ = Pressure;
  measurement_.u525wPower_ = Power;
  measurement_.u525wCWInletTemperature_ = CWI;
  measurement_.u525wCWOutletTemperature_ = CWO;
}

void ThermoDAQ2NetworkReader::processMarta(QXmlStreamReader& xml)
{
  // NQLogDebug("ThermoDAQ2NetworkReader") << "processMarta(QXmlStreamReader& xml)";

  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.martaState_ = xml.attributes().value("State").toString().toInt();

}

void ThermoDAQ2NetworkReader::processMartaR507(QXmlStreamReader& xml)
{
	float martaPT03 = xml.attributes().value("PT03").toString().toFloat();
	float martaPT05 = xml.attributes().value("PT05").toString().toFloat();
	float martaTT02 = xml.attributes().value("TT02").toString().toFloat();
	float martaSH05 = xml.attributes().value("SH05").toString().toFloat();

	measurement_.martaPT03_ = martaPT03;
	measurement_.martaPT05_ = martaPT05;
	measurement_.martaTT02_ = martaTT02;
	measurement_.martaSH05_ = martaSH05;
}

void ThermoDAQ2NetworkReader::processMartaPTCO2(QXmlStreamReader& xml)
{
	float martaPT01CO2 = xml.attributes().value("PT01CO2").toString().toFloat();
	float martaPT02CO2 = xml.attributes().value("PT02CO2").toString().toFloat();
	float martaPT03CO2 = xml.attributes().value("PT03CO2").toString().toFloat();
	float martaPT04CO2 = xml.attributes().value("PT04CO2").toString().toFloat();
	float martaPT05CO2 = xml.attributes().value("PT05CO2").toString().toFloat();
	float martaPT06CO2 = xml.attributes().value("PT06CO2").toString().toFloat();

	measurement_.martaPT01CO2_ = martaPT01CO2;
	measurement_.martaPT02CO2_ = martaPT02CO2;
	measurement_.martaPT03CO2_ = martaPT03CO2;
	measurement_.martaPT04CO2_ = martaPT04CO2;
	measurement_.martaPT05CO2_ = martaPT05CO2;
	measurement_.martaPT06CO2_ = martaPT06CO2;
}

void ThermoDAQ2NetworkReader::processMartaTTCO2(QXmlStreamReader& xml)
{
	float martaTT01CO2 = xml.attributes().value("TT01CO2").toString().toFloat();
	float martaTT02CO2 = xml.attributes().value("TT02CO2").toString().toFloat();
	float martaTT03CO2 = xml.attributes().value("TT03CO2").toString().toFloat();
	float martaTT04CO2 = xml.attributes().value("TT04CO2").toString().toFloat();
	float martaTT05CO2 = xml.attributes().value("TT05CO2").toString().toFloat();
	float martaTT06CO2 = xml.attributes().value("TT06CO2").toString().toFloat();
	float martaTT07CO2 = xml.attributes().value("TT07CO2").toString().toFloat();

	measurement_.martaTT01CO2_ = martaTT01CO2;
	measurement_.martaTT02CO2_ = martaTT02CO2;
	measurement_.martaTT03CO2_ = martaTT03CO2;
	measurement_.martaTT04CO2_ = martaTT04CO2;
	measurement_.martaTT05CO2_ = martaTT05CO2;
	measurement_.martaTT06CO2_ = martaTT06CO2;
	measurement_.martaTT07CO2_ = martaTT07CO2;
}

void ThermoDAQ2NetworkReader::processMartaSCCO2(QXmlStreamReader& xml)
{
	float martaSC01CO2 = xml.attributes().value("SC01CO2").toString().toFloat();
	float martaSC02CO2 = xml.attributes().value("SC02CO2").toString().toFloat();
	float martaSC03CO2 = xml.attributes().value("SC03CO2").toString().toFloat();
	float martaSC05CO2 = xml.attributes().value("SC05CO2").toString().toFloat();
	float martaSC06CO2 = xml.attributes().value("SC06CO2").toString().toFloat();

	measurement_.martaSC01CO2_ = martaSC01CO2;
	measurement_.martaSC02CO2_ = martaSC02CO2;
	measurement_.martaSC03CO2_ = martaSC03CO2;
	measurement_.martaSC05CO2_ = martaSC05CO2;
	measurement_.martaSC06CO2_ = martaSC06CO2;
}

void ThermoDAQ2NetworkReader::processMartaDPCO2(QXmlStreamReader& xml)
{
	float martaDP01CO2 = xml.attributes().value("DP01CO2").toString().toFloat();
	float martaDP02CO2 = xml.attributes().value("DP02CO2").toString().toFloat();
	float martaDP03CO2 = xml.attributes().value("DP03CO2").toString().toFloat();
	float martaDP04CO2 = xml.attributes().value("DP04CO2").toString().toFloat();

	measurement_.martaDP01CO2_ = martaDP01CO2;
	measurement_.martaDP02CO2_ = martaDP02CO2;
	measurement_.martaDP03CO2_ = martaDP03CO2;
	measurement_.martaDP04CO2_ = martaDP04CO2;
}

void ThermoDAQ2NetworkReader::processMartaDTCO2(QXmlStreamReader& xml)
{
	float martaDT02CO2 = xml.attributes().value("DT02CO2").toString().toFloat();
	float martaDT03CO2 = xml.attributes().value("DT03CO2").toString().toFloat();

	measurement_.martaDT02CO2_ = martaDT02CO2;
	measurement_.martaDT03CO2_ = martaDT03CO2;
}

void ThermoDAQ2NetworkReader::processMartaSTCO2(QXmlStreamReader& xml)
{
	float martaST01CO2 = xml.attributes().value("ST01CO2").toString().toFloat();
	float martaST02CO2 = xml.attributes().value("ST02CO2").toString().toFloat();
	float martaST03CO2 = xml.attributes().value("ST03CO2").toString().toFloat();
	float martaST04CO2 = xml.attributes().value("ST04CO2").toString().toFloat();

	measurement_.martaST01CO2_ = martaST01CO2;
	measurement_.martaST02CO2_ = martaST02CO2;
	measurement_.martaST03CO2_ = martaST03CO2;
	measurement_.martaST04CO2_ = martaST04CO2;
}

void ThermoDAQ2NetworkReader::processMartaFlow(QXmlStreamReader& xml)
{
	float martaFT01CO2 = xml.attributes().value("FT01CO2").toString().toFloat();

	measurement_.martaFT01CO2_ = martaFT01CO2;
}

void ThermoDAQ2NetworkReader::processMartaSettings(QXmlStreamReader& xml)
{
	int martaSpeedSetpoint = xml.attributes().value("Speed").toString().toInt();
	float martaFlowSetpoint = xml.attributes().value("Flow").toString().toFloat();
	float martaTemperatureSetpoint = xml.attributes().value("Temperature").toString().toFloat();
	int martaSpeedSetpoint2 = xml.attributes().value("Speed2").toString().toInt();
	float martaFlowSetpoint2 = xml.attributes().value("Flow2").toString().toFloat();
	float martaTemperatureSetpoint2 = xml.attributes().value("Temperature2").toString().toFloat();
	uint16_t martaStatus = xml.attributes().value("Status").toString().toInt();

	measurement_.martaSpeedSetpoint_ = martaSpeedSetpoint;
	measurement_.martaFlowSetpoint_ = martaFlowSetpoint;
	measurement_.martaTemperatureSetpoint_ = martaTemperatureSetpoint;
	measurement_.martaSpeedSetpoint2_ = martaSpeedSetpoint2;
	measurement_.martaFlowSetpoint2_ = martaFlowSetpoint2;
	measurement_.martaTemperatureSetpoint2_ = martaTemperatureSetpoint2;
	measurement_.martaStatus_ = martaStatus;
}

void ThermoDAQ2NetworkReader::processMartaAlarms(QXmlStreamReader& xml)
{
  for (int idx=0;idx<4;++idx) {
  		int martaAlarm = xml.attributes().value(QString("Alarm") + QString::number(idx)).toString().toInt();
  		measurement_.martaAlarms_[idx] = martaAlarm;
  }
}

void ThermoDAQ2NetworkReader::processAgilentTwisTorr304(QXmlStreamReader& xml)
{
  // NQLogDebug("ThermoDAQ2NetworkReader") << "processAgilentTwisTorr304(QXmlStreamReader& xml)";

  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.agilentState_ = xml.attributes().value("State").toString().toInt();
  measurement_.agilentPumpState_ = xml.attributes().value("PumpState").toString().toInt();
  measurement_.agilentPumpStatus_ = xml.attributes().value("PumpStatus").toString().toInt();
  measurement_.agilentErrorCode_ = xml.attributes().value("ErrorCode").toString().toInt();
}

void ThermoDAQ2NetworkReader::processLeyboldGraphixOne(QXmlStreamReader& xml)
{
  // NQLogDebug("ThermoDAQ2NetworkReader") << "processLeyboldGraphixOne(QXmlStreamReader& xml)";

  QString time = xml.attributes().value("time").toString();
  measurement_.dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.leyboldState_ = xml.attributes().value("State").toString().toInt();
  measurement_.leyboldPressure_ = xml.attributes().value("Pressure").toString().toFloat();
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

      if (xml.name()=="Marta") {
        processMarta(xml);
      }
      if (xml.name()=="MartaR507") {
        processMartaR507(xml);
      }
      if (xml.name()=="MartaPTCO2") {
        processMartaPTCO2(xml);
      }
      if (xml.name()=="MartaTTCO2") {
        processMartaTTCO2(xml);
      }
      if (xml.name()=="MartaSCCO2") {
        processMartaSCCO2(xml);
      }
      if (xml.name()=="MartaDPCO2") {
        processMartaDPCO2(xml);
      }
      if (xml.name()=="MartaDTCO2") {
        processMartaDTCO2(xml);
      }
      if (xml.name()=="MartaSTCO2") {
        processMartaSTCO2(xml);
      }
      if (xml.name()=="MartaFlow") {
        processMartaFlow(xml);
      }
      if (xml.name()=="MartaSettings") {
        processMartaSettings(xml);
      }
      if (xml.name()=="MartaAlarms") {
        processMartaAlarms(xml);
      }

      if (xml.name()=="AgilentTwisTorr304") {
      	processAgilentTwisTorr304(xml);
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
