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

#include "ThermoDAQ2StreamReader.h"

ThermoDAQ2StreamReader::ThermoDAQ2StreamReader(const QString &filename,
    QObject* parent)
: QObject(parent),
  filename_(filename),
  measurementValid_(false)
{
  log_.message = "";

  triggerKeithley_ = false;
  triggerHuber_ = false;
}

void ThermoDAQ2StreamReader::run()
{
  process();
  emit finished();
}

void ThermoDAQ2StreamReader::processHuberUnistat525w(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = dt;

  measurement_.u525wState_ = xml.attributes().value("State").toString().toInt();
}

void ThermoDAQ2StreamReader::processHuberUnistat525wControl(QXmlStreamReader& xml)
{
  float SetPoint = xml.attributes().value("SetPoint").toString().toFloat();
  bool ControlMode = xml.attributes().value("ControlMode").toString().toInt();
  bool ControlEnabled = xml.attributes().value("ControlEnabled").toString().toInt();
  bool CirculatorEnabled = xml.attributes().value("CirculatorEnabled").toString().toInt();

  if (!measurementValid_ || measurement_.u525wTemperatureSetPoint_!= SetPoint) {
    std::cout << measurement_.datime.toString("[yyyy-MM-dd-hh-mm-ss] - ").toStdString();
    std::cout << "Huber Unistat525w SetPoint = " << SetPoint << std::endl;
  }
  if (!measurementValid_ || measurement_.u525wTemperatureControlMode_!= ControlMode) {
    std::cout << measurement_.datime.toString("[yyyy-MM-dd-hh-mm-ss] - ").toStdString();
    std::cout << "Huber Unistat525w ControlMode = " << ControlMode << std::endl;
  }
  if (!measurementValid_ || measurement_.u525wTemperatureControlEnabled_!= ControlEnabled) {
    std::cout << measurement_.datime.toString("[yyyy-MM-dd-hh-mm-ss] - ").toStdString();
    std::cout << "Huber Unistat525w ControlEnabled = " << ControlEnabled << std::endl;
  }

  measurement_.u525wTemperatureSetPoint_ = SetPoint;
  measurement_.u525wTemperatureControlMode_ = ControlMode;
  measurement_.u525wTemperatureControlEnabled_ = ControlEnabled;
  measurement_.u525wCirculatorEnabled_ = CirculatorEnabled;
}

void ThermoDAQ2StreamReader::processHuberUnistat525wInfo(QXmlStreamReader& xml)
{
  float Internal, Process;
  if (xml.attributes().hasAttribute("Bath")) {
    Internal = xml.attributes().value("Bath").toString().toFloat();
    Process = Internal;
  } else {
    Internal = xml.attributes().value("Internal").toString().toFloat();
    Process = xml.attributes().value("Process").toString().toFloat();
  }
  float Return = xml.attributes().value("Return").toString().toFloat();
  float Pressure = xml.attributes().value("Pressure").toString().toFloat();
  int Power = xml.attributes().value("Power").toString().toInt();
  float CWI = xml.attributes().value("CWI").toString().toFloat();
  float CWO = xml.attributes().value("CWO").toString().toFloat();

  measurement_.u525wInternalTemperature_ = Internal;
  measurement_.u525wSigmaInternalTemperature_ = 0.0;
  measurement_.u525wProcessTemperature_ = Process;
  measurement_.u525wSigmaProcessTemperature_ = 0.0;
  measurement_.u525wReturnTemperature_ = Return;
  measurement_.u525wPumpPressure_ = Pressure;
  measurement_.u525wPower_ = Power;
  measurement_.u525wCWInletTemperature_ = CWI;
  measurement_.u525wCWOutletTemperature_ = CWO;
}

void ThermoDAQ2StreamReader::processHuberUnistat525wPID(QXmlStreamReader& xml)
{
  measurement_.u525wAutoPID_ = xml.attributes().value("AutoPID").toString().toInt();
}

void ThermoDAQ2StreamReader::processHuberUnistat525wPIDInternal(QXmlStreamReader& xml)
{
  measurement_.u525wKpInternal_ = xml.attributes().value("Kp").toString().toInt();
  measurement_.u525wTnInternal_ = xml.attributes().value("Tn").toString().toFloat();
  measurement_.u525wTvInternal_ = xml.attributes().value("Tv").toString().toFloat();
}

void ThermoDAQ2StreamReader::processHuberUnistat525wPIDJacket(QXmlStreamReader& xml)
{
  measurement_.u525wKpJacket_ = xml.attributes().value("Kp").toString().toInt();
  measurement_.u525wTnJacket_ = xml.attributes().value("Tn").toString().toFloat();
  measurement_.u525wTvJacket_ = xml.attributes().value("Tv").toString().toFloat();
}

void ThermoDAQ2StreamReader::processHuberUnistat525wPIDProcess(QXmlStreamReader& xml)
{
  measurement_.u525wKpProcess_ = xml.attributes().value("Kp").toString().toInt();
  measurement_.u525wTnProcess_ = xml.attributes().value("Tn").toString().toFloat();
  measurement_.u525wTvProcess_ = xml.attributes().value("Tv").toString().toFloat();
}

void ThermoDAQ2StreamReader::processMarta(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = dt;

  measurement_.martaState_ = xml.attributes().value("State").toString().toInt();
}

void ThermoDAQ2StreamReader::processMartaR507(QXmlStreamReader& xml)
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

void ThermoDAQ2StreamReader::processMartaPTCO2(QXmlStreamReader& xml)
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

void ThermoDAQ2StreamReader::processMartaTTCO2(QXmlStreamReader& xml)
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

void ThermoDAQ2StreamReader::processMartaSCCO2(QXmlStreamReader& xml)
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

void ThermoDAQ2StreamReader::processMartaDPCO2(QXmlStreamReader& xml)
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

void ThermoDAQ2StreamReader::processMartaDTCO2(QXmlStreamReader& xml)
{
  float martaDT02CO2 = xml.attributes().value("DT02CO2").toString().toFloat();
  float martaDT03CO2 = xml.attributes().value("DT03CO2").toString().toFloat();

  measurement_.martaDT02CO2_ = martaDT02CO2;
  measurement_.martaDT03CO2_ = martaDT03CO2;
}

void ThermoDAQ2StreamReader::processMartaSTCO2(QXmlStreamReader& xml)
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

void ThermoDAQ2StreamReader::processMartaFlow(QXmlStreamReader& xml)
{
  float martaFT01CO2 = xml.attributes().value("FT01CO2").toString().toFloat();

  measurement_.martaFT01CO2_ = martaFT01CO2;
}

void ThermoDAQ2StreamReader::processMartaSettings(QXmlStreamReader& xml)
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

void ThermoDAQ2StreamReader::processMartaAlarms(QXmlStreamReader& xml)
{
  for (int idx=0;idx<4;++idx) {
    int martaAlarm = xml.attributes().value(QString("Alarm") + QString::number(idx)).toString().toInt();
    measurement_.martaAlarms_[idx] = martaAlarm;
  }
}

void ThermoDAQ2StreamReader::processAgilentTwisTorr304(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = dt;

  measurement_.agilentState_ = xml.attributes().value("State").toString().toInt();
  measurement_.agilentPumpState_ = xml.attributes().value("PumpState").toString().toInt();
  measurement_.agilentPumpStatus_ = xml.attributes().value("PumpStatus").toString().toInt();
  measurement_.agilentErrorCode_ = xml.attributes().value("ErrorCode").toString().toInt();
}

void ThermoDAQ2StreamReader::processLeyboldGraphixOne(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = dt;

  measurement_.leyboldState_ = xml.attributes().value("State").toString().toInt();
  measurement_.leyboldPressure_ = xml.attributes().value("Pressure").toString().toFloat();
}

void ThermoDAQ2StreamReader::processRohdeSchwarzNGE103B(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = dt;
}

void ThermoDAQ2StreamReader::processRohdeSchwarzNGE103BChannel(QXmlStreamReader& xml)
{
  int id = xml.attributes().value("id").toString().toInt();
  bool state = xml.attributes().value("State").toString().toInt();
  int mode = xml.attributes().value("Mode").toString().toInt();
  float U = xml.attributes().value("U").toString().toFloat();
  float mU = xml.attributes().value("mU").toString().toFloat();
  float I = xml.attributes().value("I").toString().toFloat();
  float mI = xml.attributes().value("mI").toString().toFloat();

  if (!measurementValid_ || measurement_.nge103BState[id-1]!= state) {
    std::cout << measurement_.datime.toString("[yyyy-MM-dd-hh-mm-ss] - ").toStdString();
    std::cout << "Rohde & Schwarz NGE103B State[" << id << "] = " << state << std::endl;
  }
  if (!measurementValid_ || measurement_.nge103BMode[id-1]!= mode) {
    std::cout << measurement_.datime.toString("[yyyy-MM-dd-hh-mm-ss] - ").toStdString();
    std::cout << "Rohde & Schwarz NGE103B Mode[" << id << "] = " << mode << std::endl;
  }
  if (!measurementValid_ || measurement_.nge103BState[id-1]!= state) {
    std::cout << measurement_.datime.toString("[yyyy-MM-dd-hh-mm-ss] - ").toStdString();
    std::cout << "Rohde & Schwarz NGE103B State[" << id << "] = " << state << std::endl;
  }
  if (!measurementValid_ || measurement_.nge103BVoltage[id-1]!= U) {
    std::cout << measurement_.datime.toString("[yyyy-MM-dd-hh-mm-ss] - ").toStdString();
    std::cout << "Rohde & Schwarz NGE103B Voltage[" << id << "] = " << U << std::endl;
  }
  if (!measurementValid_ || measurement_.nge103BCurrent[id-1]!= I) {
    std::cout << measurement_.datime.toString("[yyyy-MM-dd-hh-mm-ss] - ").toStdString();
    std::cout << "Rohde & Schwarz NGE103B Current[" << id << "] = " << I << std::endl;
  }

  measurement_.nge103BState[id-1] = state;
  measurement_.nge103BMode[id-1] = mode;
  measurement_.nge103BVoltage[id-1] = U;
  measurement_.nge103BMVoltage[id-1] = mU;
  measurement_.nge103BCurrent[id-1] = I;
  measurement_.nge103BMCurrent[id-1] = mI;
}

void ThermoDAQ2StreamReader::processKeithleyDAQ6510(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = dt;
}

void ThermoDAQ2StreamReader::processKeithleyDAQ6510Sensor(QXmlStreamReader& xml)
{
  int id = xml.attributes().value("id").toString().toInt();
  unsigned int card = id / 100 - 1;
  unsigned int channel = id % 100 - 1;
  bool state = xml.attributes().value("State").toString().toInt();
  float temp = xml.attributes().value("T").toString().toFloat();

  measurement_.keithleyState[card][channel] = state;
  measurement_.keithleyTemperature[card][channel] = temp;
  measurement_.keithleySigmaTemperature[card][channel] = 0.0;
}

void ThermoDAQ2StreamReader::processLog(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::TextDate);
  log_.uTime = dt.toTime_t();
  log_.datime= dt;

  log_.message = xml.readElementText(QXmlStreamReader::IncludeChildElements).toStdString();

  std::cout << measurement_.datime.toString("[yyyy-MM-dd-hh-mm-ss] - ").toStdString();
  std::cout << "LOG: " << log_.message << std::endl;
}

void ThermoDAQ2StreamReader::processDAQStarted(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = dt;

  std::cout << measurement_.datime.toString("[yyyy-MM-dd-hh-mm-ss] - ").toStdString();
  std::cout << "DAQ started" << std::endl;

  measurementValid_ = true;
}

void ThermoDAQ2StreamReader::processFile(QFile* file)
{
  QXmlStreamReader xml(file);

  while (!xml.atEnd() && !xml.hasError()) {
    xml.readNext();
    if (xml.isStartElement()) {

      // std::cout << "start element name: '" << xml.name().toString().toStdString() << "'" << ", text: '" << xml.text().toString().toStdString() << "'" << std::endl;

      if (xml.name()=="HuberUnistat525w") {
        processHuberUnistat525w(xml);
      }
      if (xml.name()=="HuberUnistat525wControl") {
        processHuberUnistat525wControl(xml);
      }
      if (xml.name()=="HuberUnistat525wInfo") {
        processHuberUnistat525wInfo(xml);
      }
      if (xml.name()=="HuberUnistat525wPID") {
        processHuberUnistat525wPID(xml);
      }
      if (xml.name()=="HuberUnistat525wPIDInternal") {
        processHuberUnistat525wPIDInternal(xml);
      }
      if (xml.name()=="HuberUnistat525wPIDJacket") {
        processHuberUnistat525wPIDJacket(xml);
      }
      if (xml.name()=="HuberUnistat525wPIDProcess") {
        processHuberUnistat525wPIDProcess(xml);
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

      if (xml.name()=="Log") {
        processLog(xml);
      }
      if (xml.name()=="DAQStarted") {
        processDAQStarted(xml);
      }

    } else if (xml.isEndElement()) {


    } else if (xml.hasError()) {

      std::cout << "XML error: " << xml.errorString().toStdString() << std::endl;

    } else if (xml.atEnd()) {

      std::cout << "End of file reached" << std::endl;

    }
  }
}

void ThermoDAQ2StreamReader::process()
{
  QFile file(filename_);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

  processFile(&file);
}
