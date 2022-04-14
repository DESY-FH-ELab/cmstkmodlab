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

#include <TDatime.h>

#include "ThermoDAQ2StreamReader.h"

ThermoDAQ2StreamReader::ThermoDAQ2StreamReader(const QStringList &parameters,
    const QString &filename,
    QObject* parent)
  : QObject(parent),
    parameters_(parameters),
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
  measurement_.datime = TDatime(measurement_.uTime);

  measurement_.u525wState_ = xml.attributes().value("State").toString().toInt();
}

void ThermoDAQ2StreamReader::processHuberUnistat525wControl(QXmlStreamReader& xml)
{
  float SetPoint = xml.attributes().value("SetPoint").toString().toFloat();
  bool ControlMode = xml.attributes().value("ControlMode").toString().toInt();
  bool ControlEnabled = xml.attributes().value("ControlEnabled").toString().toInt();
  bool CirculatorEnabled = xml.attributes().value("CirculatorEnabled").toString().toInt();

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
  measurement_.datime = TDatime(measurement_.uTime);

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
  measurement_.datime = TDatime(measurement_.uTime);

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
  measurement_.datime = TDatime(measurement_.uTime);

  measurement_.leyboldState_ = xml.attributes().value("State").toString().toInt();
  measurement_.leyboldPressure_ = xml.attributes().value("Pressure").toString().toFloat();
}

void ThermoDAQ2StreamReader::processRohdeSchwarzNGE103B(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = TDatime(measurement_.uTime);
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
  measurement_.datime = TDatime(measurement_.uTime);
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
  log_.datime = TDatime(measurement_.uTime);

  log_.message = xml.readElementText(QXmlStreamReader::IncludeChildElements).toStdString();

  ologtree_->Fill();
}

void ThermoDAQ2StreamReader::processDAQStarted(QXmlStreamReader& xml)
{
  QString time = xml.attributes().value("time").toString();
  QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
  measurement_.uTime = dt.toTime_t();
  measurement_.datime = TDatime(measurement_.uTime);

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

      // std::cout << "end element name: '" << xml.name().toString().toStdString() << "'" << ", text: '" << xml.text().toString().toStdString() << "'" << std::endl;

      if (triggerKeithley_ && xml.name()=="KeithleyDAQ6510") {
        if (measurementValid_) otree_->Fill();
      }

      if (triggerHuber_ && xml.name()=="HuberUnistat525w") {
        if (measurementValid_) otree_->Fill();
      }

    } else if (xml.hasError()) {

      std::cout << "XML error: " << xml.errorString().toStdString() << std::endl;

    } else if (xml.atEnd()) {

      std::cout << "End of file reached" << std::endl;

    }
  }
}

void ThermoDAQ2StreamReader::process()
{
  for (auto p : parameters_) {
    if (p.contains("Keithley")) {
      triggerKeithley_ = true;
    }
    if (p.contains("Huber")) {
      triggerHuber_ = true;
    }
  }
  if (!triggerKeithley_ && !triggerHuber_) {
    triggerKeithley_ = true;
  }

  QFile file(filename_);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

  QFileInfo fi(file);
  QString base = fi.baseName();
  std::string rootname = base.toStdString();
  rootname += ".root";

  ofile_ = new TFile(rootname.c_str(), "RECREATE");

  char branchName[40];
  char branchLeafList[40];

  otree_ = new TTree("thermoDAQ2", "thermoDAQ2");

  otree_->Branch("uTime", &measurement_.uTime, "uTime/i");
  otree_->Branch("datime", "TDatime", &measurement_.datime);

  otree_->Branch("u525wState", &measurement_.u525wState_, "u525wState/O");
  otree_->Branch("u525wTemperatureSetPoint", &measurement_.u525wTemperatureSetPoint_, "u525wTemperatureSetPoint/F");
  otree_->Branch("u525wTemperatureControlMode", &measurement_.u525wTemperatureControlMode_, "u525wTemperatureControlMode/O");
  otree_->Branch("u525wTemperatureControlEnabled", &measurement_.u525wTemperatureControlEnabled_, "u525wTemperatureControlEnabled/O");
  otree_->Branch("u525wCirculatorEnabled", &measurement_.u525wCirculatorEnabled_, "u525wCirculatorEnabled/O");
  otree_->Branch("u525wBathTemperature", &measurement_.u525wInternalTemperature_, "u525wBathTemperature/F");
  otree_->Branch("u525wInternalTemperature", &measurement_.u525wInternalTemperature_, "u525wInternalTemperature/F");
  otree_->Branch("u525wSigmaInternalTemperature", &measurement_.u525wSigmaInternalTemperature_, "u525wSigmaInternalTemperature/F");
  otree_->Branch("u525wProcessTemperature", &measurement_.u525wProcessTemperature_, "u525wProcessTemperature/F");
  otree_->Branch("u525wSigmaProcessTemperature", &measurement_.u525wSigmaProcessTemperature_, "u525wSigmaProcessTemperature/F");
  otree_->Branch("u525wReturnTemperature", &measurement_.u525wReturnTemperature_, "u525wReturnTemperature/F");
  otree_->Branch("u525wPumpPressure", &measurement_.u525wPumpPressure_, "u525wPumpPressure/F");
  otree_->Branch("u525wPower", &measurement_.u525wPower_, "u525wPower/I");
  otree_->Branch("u525wCWInletTemperature", &measurement_.u525wCWInletTemperature_, "u525wCWInletTemperature/F");
  otree_->Branch("u525wCWOutletTemperature", &measurement_.u525wCWOutletTemperature_, "u525wCWOutletTemperature/F");
  otree_->Branch("u525wAutoPID", &measurement_.u525wAutoPID_, "u525wAutoPID/O");
  otree_->Branch("u525wKpInternal", &measurement_.u525wKpInternal_, "u525wKpInternal/I");
  otree_->Branch("u525wTnInternal", &measurement_.u525wTnInternal_, "u525wTnInternal/F");
  otree_->Branch("u525wTvInternal", &measurement_.u525wTvInternal_, "u525wTvInternal/F");
  otree_->Branch("u525wKpJacket", &measurement_.u525wKpJacket_, "u525wKpJacket/I");
  otree_->Branch("u525wTnJacket", &measurement_.u525wTnJacket_, "u525wTnJacket/F");
  otree_->Branch("u525wTvJacket", &measurement_.u525wTvJacket_, "u525wTvJacket/F");
  otree_->Branch("u525wKpProcess", &measurement_.u525wKpProcess_, "u525wKpProcess/I");
  otree_->Branch("u525wTnProcess", &measurement_.u525wTnProcess_, "u525wTnProcess/F");
  otree_->Branch("u525wTvProcess", &measurement_.u525wTvProcess_, "u525wTvProcess/F");

  otree_->Branch("martaState", &measurement_.martaState_, "martaState/O");
  otree_->Branch("martaPT03", &measurement_.martaPT03_, "martaPT03/F");
  otree_->Branch("martaPT05", &measurement_.martaPT05_, "martaPT05/F");
  otree_->Branch("martaPT01CO2", &measurement_.martaPT01CO2_, "martaPT01CO2/F");
  otree_->Branch("martaPT02CO2", &measurement_.martaPT02CO2_, "martaPT02CO2/F");
  otree_->Branch("martaPT03CO2", &measurement_.martaPT03CO2_, "martaPT03CO2/F");
  otree_->Branch("martaPT04CO2", &measurement_.martaPT04CO2_, "martaPT04CO2/F");
  otree_->Branch("martaPT05CO2", &measurement_.martaPT05CO2_, "martaPT05CO2/F");
  otree_->Branch("martaPT06CO2", &measurement_.martaPT06CO2_, "martaPT06CO2/F");
  otree_->Branch("martaTT02", &measurement_.martaTT02_, "martaTT02/F");
  otree_->Branch("martaTT01CO2", &measurement_.martaTT01CO2_, "martaTT01CO2/F");
  otree_->Branch("martaTT02CO2", &measurement_.martaTT02CO2_, "martaTT02CO2/F");
  otree_->Branch("martaTT03CO2", &measurement_.martaTT03CO2_, "martaTT03CO2/F");
  otree_->Branch("martaTT04CO2", &measurement_.martaTT04CO2_, "martaTT04CO2/F");
  otree_->Branch("martaTT05CO2", &measurement_.martaTT05CO2_, "martaTT05CO2/F");
  otree_->Branch("martaTT06CO2", &measurement_.martaTT06CO2_, "martaTT06CO2/F");
  otree_->Branch("martaTT07CO2", &measurement_.martaTT07CO2_, "martaTT07CO2/F");
  otree_->Branch("martaSH05", &measurement_.martaSH05_, "martaSH05/F");
  otree_->Branch("martaSC01CO2", &measurement_.martaSC01CO2_, "martaSC01CO2/F");
  otree_->Branch("martaSC02CO2", &measurement_.martaSC02CO2_, "martaSC02CO2/F");
  otree_->Branch("martaSC03CO2", &measurement_.martaSC03CO2_, "martaSC03CO2/F");
  otree_->Branch("martaSC05CO2", &measurement_.martaSC05CO2_, "martaSC05CO2/F");
  otree_->Branch("martaSC06CO2", &measurement_.martaSC06CO2_, "martaSC06CO2/F");
  otree_->Branch("martaDP01CO2", &measurement_.martaDP01CO2_, "martaDP01CO2/F");
  otree_->Branch("martaDP02CO2", &measurement_.martaDP02CO2_, "martaDP02CO2/F");
  otree_->Branch("martaDP03CO2", &measurement_.martaDP03CO2_, "martaDP03CO2/F");
  otree_->Branch("martaDP04CO2", &measurement_.martaDP04CO2_, "martaDP04CO2/F");
  otree_->Branch("martaDT02CO2", &measurement_.martaDT02CO2_, "martaDT02CO2/F");
  otree_->Branch("martaDT03CO2", &measurement_.martaDT03CO2_, "martaDT03CO2/F");
  otree_->Branch("martaST01CO2", &measurement_.martaST01CO2_, "martaST01CO2/F");
  otree_->Branch("martaST02CO2", &measurement_.martaST02CO2_, "martaST02CO2/F");
  otree_->Branch("martaST03CO2", &measurement_.martaST03CO2_, "martaST03CO2/F");
  otree_->Branch("martaST04CO2", &measurement_.martaST04CO2_, "martaST04CO2/F");
  otree_->Branch("martaFT01CO2", &measurement_.martaFT01CO2_, "martaFT01CO2/F");
  otree_->Branch("martaSpeedSetpoint", &measurement_.martaSpeedSetpoint_, "martaSpeedSetpoint/F");
  otree_->Branch("martaFlowSetpoint", &measurement_.martaFlowSetpoint_, "martaFlowSetpoint/F");
  otree_->Branch("martaTemperatureSetpoint", &measurement_.martaTemperatureSetpoint_, "martaTemperatureSetpoint/F");
  otree_->Branch("martaStatus", &measurement_.martaStatus_, "martaStatus/s");
  otree_->Branch("martaSpeedSetpoint2", &measurement_.martaSpeedSetpoint2_, "martaSpeedSetpoint2/F");
  otree_->Branch("martaFlowSetpoint2", &measurement_.martaFlowSetpoint2_, "martaFlowSetpoint2/F");
  otree_->Branch("martaTemperatureSetpoint2", &measurement_.martaTemperatureSetpoint2_, "martaTemperatureSetpoint2/F");
  otree_->Branch("martaAlarms0", &measurement_.martaAlarms_[0], "martaAlarms0/s");
  otree_->Branch("martaAlarms1", &measurement_.martaAlarms_[1], "martaAlarms1/s");
  otree_->Branch("martaAlarms2", &measurement_.martaAlarms_[2], "martaAlarms2/s");
  otree_->Branch("martaAlarms3", &measurement_.martaAlarms_[3], "martaAlarms3/s");

  otree_->Branch("agilentState", &measurement_.agilentState_, "agilentState/O");
  otree_->Branch("agilentPumpState", &measurement_.agilentPumpState_, "agilentPumpState/O");
  otree_->Branch("agilentPumpStatus", &measurement_.agilentPumpStatus_, "agilentPumpStatus/i");
  otree_->Branch("agilentErrorCode", &measurement_.agilentErrorCode_, "agilentErrorCode/i");

  otree_->Branch("leyboldState", &measurement_.leyboldState_, "leyboldState/O");
  otree_->Branch("leyboldPressure", &measurement_.leyboldPressure_, "leyboldPressure/F");

  for (int i=0;i<3;++i) {
    sprintf(branchName, "nge103BState_%d", i+1);
    sprintf(branchLeafList, "nge103BState_%d/O", i+1);
    otree_->Branch(branchName, &measurement_.nge103BState[i], branchLeafList);

    sprintf(branchName, "nge103BMode_%d", i+1);
    sprintf(branchLeafList, "nge103BMode_%d/O", i+1);
    otree_->Branch(branchName, &measurement_.nge103BMode[i], branchLeafList);

    sprintf(branchName, "nge103BVoltage_%d", i+1);
    sprintf(branchLeafList, "nge103BVoltage_%d/F", i+1);
    otree_->Branch(branchName, &measurement_.nge103BVoltage[i], branchLeafList);

    sprintf(branchName, "nge103BMVoltage_%d", i+1);
    sprintf(branchLeafList, "nge103BMVoltage_%d/F", i+1);
    otree_->Branch(branchName, &measurement_.nge103BMVoltage[i], branchLeafList);

    sprintf(branchName, "nge103BCurrent_%d", i+1);
    sprintf(branchLeafList, "nge103BCurrent_%d/F", i+1);
    otree_->Branch(branchName, &measurement_.nge103BCurrent[i], branchLeafList);

    sprintf(branchName, "nge103BMCurrent_%d", i+1);
    sprintf(branchLeafList, "nge103BMCurrent_%d/F", i+1);
    otree_->Branch(branchName, &measurement_.nge103BMCurrent[i], branchLeafList);
  }

  for (int i=0;i<2;++i) {
    for (int j=0;j<10;++j) {
      sprintf(branchName, "KeithleyDAQ6510State_%d%02d", i+1, j+1);
      sprintf(branchLeafList, "KeithleyDAQ6510State_%d%02d/O", i+1, j+1);
      otree_->Branch(branchName, &measurement_.keithleyState[i][j], branchLeafList);

      sprintf(branchName, "KeithleyDAQ6510Temperature_%d%02d", i+1, j+1);
      sprintf(branchLeafList, "KeithleyDAQ6510Temperature_%d%02d/F", i+1, j+1);
      otree_->Branch(branchName, &measurement_.keithleyTemperature[i][j], branchLeafList);

      sprintf(branchName, "KeithleyDAQ6510SigmaTemperature_%d%02d", i+1, j+1);
      sprintf(branchLeafList, "KeithleyDAQ6510SigmaTemperature_%d%02d/F", i+1, j+1);
      otree_->Branch(branchName, &measurement_.keithleySigmaTemperature[i][j], branchLeafList);
    }
  }

  ologtree_ = new TTree("thermoLog", "thermoLog");
  ologtree_->Branch("uTime", &log_.uTime, "uTime/i");
  ologtree_->Branch("datime", "TDatime", &log_.datime);
  ologtree_->Branch("message", &log_.message);

  processFile(&file);

  ofile_->Write();
  delete ofile_;
}
