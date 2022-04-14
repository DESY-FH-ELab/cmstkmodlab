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

#include <QApplication>
#include <QDateTime>
#include <QXmlStreamWriter>

#include <nqlogger.h>

#include "Thermo2DAQModel.h"

Thermo2DAQModel::Thermo2DAQModel(HuberUnistat525wModel* huberModel,
    MartaModel* martaModel,
    AgilentTwisTorr304Model* agilentModel,
    LeyboldGraphixOneModel* leyboldModel,
    RohdeSchwarzNGE103BModel* nge103BModel,
    KeithleyDAQ6510Model* keithleyModel,
    QObject * /* parent */)
 : QObject(),
   daqState_(false),
   huberModel_(huberModel),
   martaModel_(martaModel),
   agilentModel_(agilentModel),
   leyboldModel_(leyboldModel),
   nge103BModel_(nge103BModel),
   keithleyModel_(keithleyModel)
{
  currentTime_ = QDateTime::currentDateTime();

  if (huberModel_) {
  	connect(huberModel_, SIGNAL(informationChanged()),
  			this, SLOT(huberInfoChanged()));
  }

  if (martaModel_) {
  	connect(martaModel_, SIGNAL(informationChanged()),
  			this, SLOT(martaInfoChanged()));
  }

  if (agilentModel_) {
  	connect(agilentModel_, SIGNAL(informationChanged()),
  			this, SLOT(agilentInfoChanged()));
  }

  if (leyboldModel_) {
  	connect(leyboldModel_, SIGNAL(informationChanged()),
  			this, SLOT(leyboldInfoChanged()));
  }

  connect(nge103BModel_, SIGNAL(informationChanged()),
  		this, SLOT(nge103BInfoChanged()));

  connect(keithleyModel_, SIGNAL(informationChanged()),
  		this, SLOT(keithleyInfoChanged()));
}

void Thermo2DAQModel::myMoveToThread(QThread *thread)
{
  if (huberModel_) huberModel_->moveToThread(thread);
  if (martaModel_) martaModel_->moveToThread(thread);
  if (agilentModel_) agilentModel_->moveToThread(thread);
  if (leyboldModel_) leyboldModel_->moveToThread(thread);
  nge103BModel_->moveToThread(thread);
  keithleyModel_->moveToThread(thread);

  this->moveToThread(thread);
}

void Thermo2DAQModel::startMeasurement()
{
  for (int i=0;i<3;++i) {
    nge103BOutputState_[i] = false;
    nge103BOutputMode_[i] = 0;
    nge103BVoltage_[i] = -1.0;
    nge103BCurrent_[i] = -1.0;
    nge103BMeasuredVoltage_[i] = 0.;
    nge103BMeasuredCurrent_[i] = 0.;
  }

  u525wState_ = false;
  u525wTemperatureSetPoint_ = 0.0;
  u525wTemperatureControlMode_ = false;
  u525wTemperatureControlEnabled_ = false;
  u525wCirculatorEnabled_ = false;
  u525wInternalTemperature_ = 0.0;
  u525wProcessTemperature_ = 0.0;
  u525wReturnTemperature_ = 0.0;
  u525wPumpPressure_ = 0.0;
  u525wPower_ = 0.0;
  u525wCWInletTemperature_ = 0.0;
  u525wCWOutletTemperature_ = 0.0;
  u525AutoPID_ = false;
  u525KpInternal_ = 0;
  u525TnInternal_ = 0.0;
  u525TvInternal_ = 0.0;
  u525KpJacket_ = 0;
  u525TnJacket_ = 0.0;
  u525TvJacket_ = 0.0;
  u525KpProcess_ = 0;
  u525TnProcess_ = 0.0;
  u525TvProcess_ = 0.0;

  agilentState_ = false;
  agilentPumpState_ = false;
  agilentPumpStatus_ = 0;
  agilentErrorCode_ = 0;

  leyboldState_ = false;
  leyboldPressure_ = 1.013;

  for (unsigned int card=0;card<2;++card) {
    for (unsigned int channel=0;channel<10;++channel) {
      keithleyState_[card][channel] = false;
      keithleyTemperature_[card][channel] = 0.;
    }
  }

  daqState_ = true;
  emit daqStateChanged(true);

  QString buffer;
  createDAQStatusMessage(buffer, true);
  emit daqMessage(buffer);

  NQLogMessage("thermo2DAQ") << "measurement started";
}

void Thermo2DAQModel::createDAQStatusMessage(QString &buffer, bool start)
{
  QDateTime& utime = currentTime();

  if (start) {
    buffer += QString("<ThermoDAQ2>");
  }

  QXmlStreamWriter xml(&buffer);
  xml.setAutoFormatting(true);

  //
  // Start of Huber Unistat 525w
  //
  if (huberModel_) {
  	xml.writeStartElement("HuberUnistat525w");
  	xml.writeAttribute("time", utime.toString(Qt::ISODate));
  	xml.writeAttribute("State", huberModel_->getDeviceState()==READY ? "1" : "0");

  	xml.writeStartElement("HuberUnistat525wControl");
  	xml.writeAttribute("SetPoint", QString::number(huberModel_->getTemperatureSetPoint(), 'f', 2));
  	xml.writeAttribute("ControlMode", QString::number(huberModel_->getTemperatureControlMode()));
  	xml.writeAttribute("ControlEnabled", QString::number(huberModel_->getTemperatureControlEnabled()));
  	xml.writeAttribute("CirculatorEnabled", QString::number(huberModel_->getCirculatorEnabled()));
  	xml.writeEndElement();

    xml.writeStartElement("HuberUnistat525wInfo");
    xml.writeAttribute("Internal", QString::number(huberModel_->getInternalTemperature(), 'f', 2));
    xml.writeAttribute("Process", QString::number(huberModel_->getProcessTemperature(), 'f', 2));
    xml.writeAttribute("Return", QString::number(huberModel_->getReturnTemperature(), 'f', 2));
    xml.writeAttribute("Pressure", QString::number(huberModel_->getPumpPressure(), 'f', 3));
    xml.writeAttribute("Power", QString::number(huberModel_->getPower()));
    xml.writeAttribute("CWI", QString::number(huberModel_->getCoolingWaterInletTemperature(), 'f', 2));
    xml.writeAttribute("CWO", QString::number(huberModel_->getCoolingWaterOutletTemperature(), 'f', 2));
    xml.writeEndElement();

    xml.writeStartElement("HuberUnistat525wPID");
    xml.writeAttribute("AutoPID", huberModel_->getAutoPID() ? "1" : "0");

    xml.writeStartElement("HuberUnistat525wPIDInternal");
    xml.writeAttribute("Kp", QString::number(huberModel_->getKpInternal()));
    xml.writeAttribute("Tn", QString::number(huberModel_->getTnInternal(), 'f', 1));
    xml.writeAttribute("Tv", QString::number(huberModel_->getTvInternal(), 'f', 1));
    xml.writeEndElement();

    xml.writeStartElement("HuberUnistat525wPIDJacket");
    xml.writeAttribute("Kp", QString::number(huberModel_->getKpJacket()));
    xml.writeAttribute("Tn", QString::number(huberModel_->getTnJacket(), 'f', 1));
    xml.writeAttribute("Tv", QString::number(huberModel_->getTvJacket(), 'f', 1));
    xml.writeEndElement();

    xml.writeStartElement("HuberUnistat525wPIDProcess");
    xml.writeAttribute("Kp", QString::number(huberModel_->getKpProcess()));
    xml.writeAttribute("Tn", QString::number(huberModel_->getTnProcess(), 'f', 1));
    xml.writeAttribute("Tv", QString::number(huberModel_->getTvProcess(), 'f', 1));
    xml.writeEndElement();

    xml.writeEndElement();

  	xml.writeEndElement();
  }
  //
  // End of Huber Unistat 525w
  //

  //
  // Start of Marta CO2 Chiller
  //
  if (martaModel_) {
  	xml.writeStartElement("Marta");
  	xml.writeAttribute("time", utime.toString(Qt::ISODate));
  	xml.writeAttribute("State", martaModel_->getDeviceState()==READY ? "1" : "0");

  	xml.writeStartElement("MartaR507");
  	xml.writeAttribute("PT03", QString::number(martaModel_->getPT03(), 'f', 2));
  	xml.writeAttribute("PT05", QString::number(martaModel_->getPT05(), 'f', 2));
  	xml.writeAttribute("TT02", QString::number(martaModel_->getTT02(), 'f', 2));
  	xml.writeAttribute("SH05", QString::number(martaModel_->getSH05(), 'f', 2));
  	xml.writeEndElement();

  	xml.writeStartElement("MartaPTCO2");
  	xml.writeAttribute("PT01CO2", QString::number(martaModel_->getPT01CO2(), 'f', 2));
  	xml.writeAttribute("PT02CO2", QString::number(martaModel_->getPT02CO2(), 'f', 2));
  	xml.writeAttribute("PT03CO2", QString::number(martaModel_->getPT03CO2(), 'f', 2));
  	xml.writeAttribute("PT04CO2", QString::number(martaModel_->getPT04CO2(), 'f', 2));
  	xml.writeAttribute("PT05CO2", QString::number(martaModel_->getPT05CO2(), 'f', 2));
  	xml.writeAttribute("PT06CO2", QString::number(martaModel_->getPT06CO2(), 'f', 2));
  	xml.writeEndElement();

  	xml.writeStartElement("MartaTTCO2");
  	xml.writeAttribute("TT01CO2", QString::number(martaModel_->getTT01CO2(), 'f', 2));
  	xml.writeAttribute("TT02CO2", QString::number(martaModel_->getTT02CO2(), 'f', 2));
  	xml.writeAttribute("TT03CO2", QString::number(martaModel_->getTT03CO2(), 'f', 2));
  	xml.writeAttribute("TT04CO2", QString::number(martaModel_->getTT04CO2(), 'f', 2));
  	xml.writeAttribute("TT05CO2", QString::number(martaModel_->getTT05CO2(), 'f', 2));
  	xml.writeAttribute("TT06CO2", QString::number(martaModel_->getTT06CO2(), 'f', 2));
  	xml.writeAttribute("TT07CO2", QString::number(martaModel_->getTT07CO2(), 'f', 2));
  	xml.writeEndElement();

  	xml.writeStartElement("MartaSCCO2");
  	xml.writeAttribute("SC01CO2", QString::number(martaModel_->getSC01CO2(), 'f', 2));
  	xml.writeAttribute("SC02CO2", QString::number(martaModel_->getSC02CO2(), 'f', 2));
  	xml.writeAttribute("SC03CO2", QString::number(martaModel_->getSC03CO2(), 'f', 2));
  	xml.writeAttribute("SC05CO2", QString::number(martaModel_->getSC05CO2(), 'f', 2));
  	xml.writeAttribute("SC06CO2", QString::number(martaModel_->getSC06CO2(), 'f', 2));
  	xml.writeEndElement();

  	xml.writeStartElement("MartaDPCO2");
  	xml.writeAttribute("DP01CO2", QString::number(martaModel_->getdP01CO2(), 'f', 2));
  	xml.writeAttribute("DP02CO2", QString::number(martaModel_->getdP02CO2(), 'f', 2));
  	xml.writeAttribute("DP03CO2", QString::number(martaModel_->getdP03CO2(), 'f', 2));
  	xml.writeAttribute("DP04CO2", QString::number(martaModel_->getdP04CO2(), 'f', 2));
  	xml.writeEndElement();

  	xml.writeStartElement("MartaDTCO2");
  	xml.writeAttribute("DT02CO2", QString::number(martaModel_->getdT02CO2(), 'f', 2));
  	xml.writeAttribute("DT03CO2", QString::number(martaModel_->getdT03CO2(), 'f', 2));
  	xml.writeEndElement();

  	xml.writeStartElement("MartaSTCO2");
  	xml.writeAttribute("ST01CO2", QString::number(martaModel_->getST01CO2(), 'f', 2));
  	xml.writeAttribute("ST02CO2", QString::number(martaModel_->getST02CO2(), 'f', 2));
  	xml.writeAttribute("ST03CO2", QString::number(martaModel_->getST03CO2(), 'f', 2));
  	xml.writeAttribute("ST04CO2", QString::number(martaModel_->getST04CO2(), 'f', 2));
  	xml.writeEndElement();

  	xml.writeStartElement("MartaFlow");
  	xml.writeAttribute("FT01CO2", QString::number(martaModel_->getFT01CO2(), 'f', 3));
  	xml.writeEndElement();

  	xml.writeStartElement("MartaSettings");
  	xml.writeAttribute("Speed", QString::number((int)martaModel_->getSpeedSetpoint()));
  	xml.writeAttribute("Flow", QString::number(martaModel_->getFlowSetpoint(), 'f', 1));
  	xml.writeAttribute("Temperature", QString::number(martaModel_->getTemperatureSetpoint(), 'f', 1));
  	xml.writeAttribute("Speed2", QString::number((int)martaModel_->getSpeedSetpoint2()));
  	xml.writeAttribute("Flow2", QString::number(martaModel_->getFlowSetpoint2(), 'f', 1));
  	xml.writeAttribute("Temperature2", QString::number(martaModel_->getTemperatureSetpoint2(), 'f', 1));
  	xml.writeAttribute("Status", QString("0x") + QStringLiteral("%1").arg(martaModel_->getStatus(), 4, 16, QLatin1Char('0')));
  	xml.writeEndElement();

  	xml.writeStartElement("MartaAlarms");
  	xml.writeAttribute("Status", QString::number(martaModel_->getAlarmStatus()));
  	for (int idx=0;idx<4;++idx) {
  		xml.writeAttribute(QString("Alarm") + QString::number(idx),
  				QString("0x") + QStringLiteral("%1").arg(martaModel_->getAlarms(idx), 4, 16, QLatin1Char('0')));
  	}
  	xml.writeEndElement();

  	xml.writeEndElement();
  }
  //
  // End of Marta CO2 Chiller
  //

  //
  // Start of Agilent TwisTorr 304
  //
  if (agilentModel_) {
  	xml.writeStartElement("AgilentTwisTorr304");
  	xml.writeAttribute("time", utime.toString(Qt::ISODate));
  	xml.writeAttribute("State", agilentModel_->getDeviceState()==READY ? "1" : "0");
  	xml.writeAttribute("PumpState", agilentModel_->getPumpState()==true ? "1" : "0");
  	xml.writeAttribute("PumpStatus", QString::number(agilentModel_->getPumpStatus()));
  	xml.writeAttribute("ErrorCode", QString::number(agilentModel_->getErrorCode()));
  	xml.writeEndElement();
  }
  //
  // End of Agilent TwisTorr 304
  //

  //
  // Start of Leybold Graphix One
  //
  if (leyboldModel_) {
  	xml.writeStartElement("LeyboldGraphixOne");
  	xml.writeAttribute("time", utime.toString(Qt::ISODate));
  	xml.writeAttribute("State", leyboldModel_->getDeviceState()==READY ? "1" : "0");
  	xml.writeAttribute("Pressure", QString::number(leyboldModel_->getPressure(), 'e', 3));
  	xml.writeEndElement();
  }
  //
  // End of Leybold Graphix One
  //

  //
  // Start of Rohde & Schwarz NGE103B
  //
  xml.writeStartElement("RohdeSchwarzNGE103B");
  xml.writeAttribute("time", utime.toString(Qt::ISODate));
  for (int i=0;i<3;++i) {
    xml.writeStartElement(QString("RohdeSchwarzNGE103BChannel"));
    xml.writeAttribute("id", QString::number(i+1));
    xml.writeAttribute("State", nge103BModel_->getOutputState(i+1)==true ? "1" : "0");
    xml.writeAttribute("Mode", QString::number(nge103BModel_->getOutputMode(i+1)));
    xml.writeAttribute("U", QString::number(nge103BModel_->getVoltage(i+1), 'f', 3));
    xml.writeAttribute("mU", QString::number(nge103BModel_->getMeasuredVoltage(i+1), 'f', 3));
    xml.writeAttribute("I", QString::number(nge103BModel_->getCurrent(i+1), 'f', 3));
    xml.writeAttribute("mI", QString::number(nge103BModel_->getMeasuredCurrent(i+1), 'f', 3));
    xml.writeEndElement();
  }
  xml.writeEndElement();
  //
  // End of Rohde & Schwarz NGE103B
  //

  //
  // Start of Keithley DAQ6510
  //
  xml.writeStartElement("KeithleyDAQ6510");
  xml.writeAttribute("time", utime.toString(Qt::ISODate));
  for (unsigned int card=1;card<=2;++card) {
    for (unsigned int channel=1;channel<=10;++channel) {
      unsigned int sensor = card*100 + channel;

      keithleyState_[card-1][channel-1] = keithleyModel_->getSensorState(sensor)==READY;
      if (keithleyState_[card-1][channel-1]) {
        keithleyTemperature_[card-1][channel-1] = keithleyModel_->getTemperature(sensor);
      } else {
        keithleyTemperature_[card-1][channel-1] = 0;
      }

      xml.writeStartElement(QString("KeithleyDAQ6510Sensor"));
      xml.writeAttribute("id", QString::number(sensor));
      xml.writeAttribute("State", keithleyState_[card-1][channel-1]==true ? "1" : "0");
      xml.writeAttribute("T", QString::number(keithleyTemperature_[card-1][channel-1], 'f', 4));
      xml.writeEndElement();
    }
  }
  xml.writeEndElement();
  //
  // End of Keithley DAQ6510
  //

  if (start) {
    xml.writeStartElement("DAQStarted");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeEndElement();
  }
}

void Thermo2DAQModel::stopMeasurement()
{
  QString buffer("</ThermoDAQ2>");
  emit daqMessage(buffer);

  daqState_ = false;
  emit daqStateChanged(false);

  NQLogMessage("thermo2DAQ") << "measurement stopped";
}

void Thermo2DAQModel::clearHistory()
{

}

QDateTime& Thermo2DAQModel::currentTime()
{
  QDateTime ct = QDateTime::currentDateTime();
  if (currentTime_.secsTo(ct)>2) currentTime_ = ct;
  return currentTime_;
}

void Thermo2DAQModel::customDAQMessage(const QString & message)
{
  emit daqMessage(message);
}


void Thermo2DAQModel::huberInfoChanged()
{
  if (!daqState_) return;

  NQLogDebug("Thermo2DAQModel") << "huberInfoChanged()";

  if (thread()==QApplication::instance()->thread()) {
    NQLogDebug("Thermo2DAQModel") << "running in main application thread";
  } else {
    NQLogDebug("Thermo2DAQModel") << "running in dedicated DAQ thread";
  }

  QMutexLocker locker(&mutex_);

  QDateTime& utime = currentTime();

  QString buffer;

  bool changed = false;
  changed |= updateIfChanged<bool>(u525wState_, huberModel_->getDeviceState()==READY ? true : false);
  changed |= updateIfChanged<float>(u525wTemperatureSetPoint_, huberModel_->getTemperatureSetPoint());
  changed |= updateIfChanged<bool>(u525wTemperatureControlMode_, huberModel_->getTemperatureControlMode());
  changed |= updateIfChanged<bool>(u525wTemperatureControlEnabled_, huberModel_->getTemperatureControlEnabled());
  changed |= updateIfChanged<bool>(u525wCirculatorEnabled_, huberModel_->getCirculatorEnabled());
  changed |= updateIfChanged<float>(u525wInternalTemperature_, huberModel_->getInternalTemperature());
  changed |= updateIfChanged<float>(u525wProcessTemperature_, huberModel_->getProcessTemperature());
  changed |= updateIfChanged<float>(u525wReturnTemperature_, huberModel_->getReturnTemperature());
  changed |= updateIfChanged<float>(u525wPumpPressure_, huberModel_->getPumpPressure());
  changed |= updateIfChanged<int>(u525wPower_, huberModel_->getPower());
  changed |= updateIfChanged<float>(u525wCWInletTemperature_, huberModel_->getCoolingWaterInletTemperature());
  changed |= updateIfChanged<float>(u525wCWOutletTemperature_, huberModel_->getCoolingWaterOutletTemperature());
  changed |= updateIfChanged<bool>(u525AutoPID_, huberModel_->getAutoPID());
  changed |= updateIfChanged<int>(u525KpInternal_, huberModel_->getKpInternal());
  changed |= updateIfChanged<float>(u525TnInternal_, huberModel_->getTnInternal());
  changed |= updateIfChanged<float>(u525TvInternal_, huberModel_->getTvInternal());
  changed |= updateIfChanged<int>(u525KpJacket_, huberModel_->getKpJacket());
  changed |= updateIfChanged<float>(u525TnJacket_, huberModel_->getTnJacket());
  changed |= updateIfChanged<float>(u525TvJacket_, huberModel_->getTvJacket());
  changed |= updateIfChanged<int>(u525KpProcess_, huberModel_->getKpProcess());
  changed |= updateIfChanged<float>(u525TnProcess_, huberModel_->getTnProcess());
  changed |= updateIfChanged<float>(u525TvProcess_, huberModel_->getTvProcess());

  if (changed) {
    QXmlStreamWriter xml(&buffer);
    xml.setAutoFormatting(true);

    xml.writeStartElement("HuberUnistat525w");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("State", QString::number(u525wState_));

    xml.writeStartElement("HuberUnistat525wControl");
    xml.writeAttribute("SetPoint", QString::number(u525wTemperatureSetPoint_, 'f', 2));
    xml.writeAttribute("ControlMode", QString::number(u525wTemperatureControlMode_));
    xml.writeAttribute("ControlEnabled", QString::number(u525wTemperatureControlEnabled_));
    xml.writeAttribute("CirculatorEnabled", QString::number(u525wCirculatorEnabled_));
    xml.writeEndElement();

    xml.writeStartElement("HuberUnistat525wInfo");
    xml.writeAttribute("Internal", QString::number(u525wInternalTemperature_, 'f', 2));
    xml.writeAttribute("Process", QString::number(u525wProcessTemperature_, 'f', 2));
    xml.writeAttribute("Return", QString::number(u525wReturnTemperature_, 'f', 2));
    xml.writeAttribute("Pressure", QString::number(u525wPumpPressure_, 'f', 3));
    xml.writeAttribute("Power", QString::number(u525wPower_));
    xml.writeAttribute("CWI", QString::number(u525wCWInletTemperature_, 'f', 2));
    xml.writeAttribute("CWO", QString::number(u525wCWOutletTemperature_, 'f', 2));
    xml.writeEndElement();

    xml.writeStartElement("HuberUnistat525wPID");
    xml.writeAttribute("AutoPID", u525AutoPID_ ? "1" : "0");
    if (!u525AutoPID_) {
      if (!u525wTemperatureControlMode_) {
        xml.writeStartElement("HuberUnistat525wPIDInternal");
        xml.writeAttribute("Kp", QString::number(u525KpInternal_));
        xml.writeAttribute("Tn", QString::number(u525TnInternal_, 'f', 1));
        xml.writeAttribute("Tv", QString::number(u525TvInternal_, 'f', 1));
        xml.writeEndElement();
      } else {
        xml.writeStartElement("HuberUnistat525wPIDProcess");
        xml.writeAttribute("Kp", QString::number(u525KpProcess_));
        xml.writeAttribute("Tn", QString::number(u525TnProcess_, 'f', 1));
        xml.writeAttribute("Tv", QString::number(u525TvProcess_, 'f', 1));
        xml.writeEndElement();
     }
    }
    xml.writeEndElement();

    xml.writeEndElement();
  }

  if (buffer.length()>0) {
    emit daqMessage(buffer);
    emit newDataAvailable();
  }
}

void Thermo2DAQModel::martaInfoChanged()
{
  if (!daqState_) return;

  NQLogDebug("Thermo2DAQModel") << "martaInfoChanged()";

  if (thread()==QApplication::instance()->thread()) {
    NQLogDebug("Thermo2DAQModel") << "running in main application thread";
  } else {
    NQLogDebug("Thermo2DAQModel") << "running in dedicated DAQ thread";
  }

  QMutexLocker locker(&mutex_);

  QDateTime& utime = currentTime();

  QString buffer;

  bool changed = false;
  changed |= updateIfChanged<bool>(martaState_, martaModel_->getDeviceState()==READY ? true : false);
  changed |= updateIfChanged<float>(martaPT03_, martaModel_->getPT03());
  changed |= updateIfChanged<float>(martaPT05_, martaModel_->getPT05());
  changed |= updateIfChanged<float>(martaTT02_, martaModel_->getTT02());
  changed |= updateIfChanged<float>(martaSH05_, martaModel_->getSH05());
  changed |= updateIfChanged<float>(martaPT01CO2_, martaModel_->getPT01CO2());
  changed |= updateIfChanged<float>(martaPT02CO2_, martaModel_->getPT02CO2());
  changed |= updateIfChanged<float>(martaPT03CO2_, martaModel_->getPT03CO2());
  changed |= updateIfChanged<float>(martaPT04CO2_, martaModel_->getPT04CO2());
  changed |= updateIfChanged<float>(martaPT05CO2_, martaModel_->getPT05CO2());
  changed |= updateIfChanged<float>(martaPT06CO2_, martaModel_->getPT06CO2());
  changed |= updateIfChanged<float>(martaTT01CO2_, martaModel_->getTT01CO2());
  changed |= updateIfChanged<float>(martaTT02CO2_, martaModel_->getTT02CO2());
  changed |= updateIfChanged<float>(martaTT03CO2_, martaModel_->getTT03CO2());
  changed |= updateIfChanged<float>(martaTT04CO2_, martaModel_->getTT04CO2());
  changed |= updateIfChanged<float>(martaTT05CO2_, martaModel_->getTT05CO2());
  changed |= updateIfChanged<float>(martaTT06CO2_, martaModel_->getTT06CO2());
  changed |= updateIfChanged<float>(martaTT07CO2_, martaModel_->getTT07CO2());
  changed |= updateIfChanged<float>(martaSC01CO2_, martaModel_->getSC01CO2());
  changed |= updateIfChanged<float>(martaSC02CO2_, martaModel_->getSC02CO2());
  changed |= updateIfChanged<float>(martaSC03CO2_, martaModel_->getSC03CO2());
  changed |= updateIfChanged<float>(martaSC05CO2_, martaModel_->getSC05CO2());
  changed |= updateIfChanged<float>(martaSC06CO2_, martaModel_->getSC06CO2());
  changed |= updateIfChanged<float>(martaDP01CO2_, martaModel_->getdP01CO2());
  changed |= updateIfChanged<float>(martaDP02CO2_, martaModel_->getdP02CO2());
  changed |= updateIfChanged<float>(martaDP03CO2_, martaModel_->getdP03CO2());
  changed |= updateIfChanged<float>(martaDP04CO2_, martaModel_->getdP04CO2());
  changed |= updateIfChanged<float>(martaDT02CO2_, martaModel_->getdT02CO2());
  changed |= updateIfChanged<float>(martaDT03CO2_, martaModel_->getdT03CO2());
  changed |= updateIfChanged<float>(martaST01CO2_, martaModel_->getST01CO2());
  changed |= updateIfChanged<float>(martaST02CO2_, martaModel_->getST02CO2());
  changed |= updateIfChanged<float>(martaST03CO2_, martaModel_->getST03CO2());
  changed |= updateIfChanged<float>(martaST04CO2_, martaModel_->getST04CO2());
  changed |= updateIfChanged<float>(martaFT01CO2_, martaModel_->getFT01CO2());
  changed |= updateIfChanged<float>(martaSpeedSetpoint_, martaModel_->getSpeedSetpoint());
  changed |= updateIfChanged<float>(martaFlowSetpoint_, martaModel_->getSpeedSetpoint());
  changed |= updateIfChanged<float>(martaTemperatureSetpoint_, martaModel_->getSpeedSetpoint());
  changed |= updateIfChanged<float>(martaSpeedSetpoint2_, martaModel_->getSpeedSetpoint2());
  changed |= updateIfChanged<float>(martaFlowSetpoint2_, martaModel_->getSpeedSetpoint2());
  changed |= updateIfChanged<float>(martaTemperatureSetpoint2_, martaModel_->getSpeedSetpoint2());
  changed |= updateIfChanged<uint16_t>(martaStatus_, martaModel_->getStatus());
  changed |= updateIfChanged<uint16_t>(martaAlarms_[0], martaModel_->getAlarms(0));
  changed |= updateIfChanged<uint16_t>(martaAlarms_[1], martaModel_->getAlarms(1));
  changed |= updateIfChanged<uint16_t>(martaAlarms_[2], martaModel_->getAlarms(2));
  changed |= updateIfChanged<uint16_t>(martaAlarms_[3], martaModel_->getAlarms(3));
  changed |= updateIfChanged<uint16_t>(martaAlarmStatus_, martaModel_->getAlarmStatus());

  if (changed) {
    QXmlStreamWriter xml(&buffer);
    xml.setAutoFormatting(true);

    xml.writeStartElement("Marta");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("State", QString::number(martaState_));

    xml.writeStartElement("MartaR507");
    xml.writeAttribute("PT03", QString::number(martaPT03_, 'f', 2));
    xml.writeAttribute("PT05", QString::number(martaPT05_, 'f', 2));
    xml.writeAttribute("TT02", QString::number(martaTT02_, 'f', 2));
    xml.writeAttribute("SH05", QString::number(martaSH05_, 'f', 2));
    xml.writeEndElement();

    xml.writeStartElement("MartaPTCO2");
    xml.writeAttribute("PT01CO2", QString::number(martaPT01CO2_, 'f', 2));
    xml.writeAttribute("PT02CO2", QString::number(martaPT02CO2_, 'f', 2));
    xml.writeAttribute("PT03CO2", QString::number(martaPT03CO2_, 'f', 2));
    xml.writeAttribute("PT04CO2", QString::number(martaPT04CO2_, 'f', 2));
    xml.writeAttribute("PT05CO2", QString::number(martaPT05CO2_, 'f', 2));
    xml.writeAttribute("PT06CO2", QString::number(martaPT06CO2_, 'f', 2));
    xml.writeEndElement();

    xml.writeStartElement("MartaTTCO2");
    xml.writeAttribute("TT01CO2", QString::number(martaTT01CO2_, 'f', 2));
    xml.writeAttribute("TT02CO2", QString::number(martaTT02CO2_, 'f', 2));
    xml.writeAttribute("TT03CO2", QString::number(martaTT03CO2_, 'f', 2));
    xml.writeAttribute("TT04CO2", QString::number(martaTT04CO2_, 'f', 2));
    xml.writeAttribute("TT05CO2", QString::number(martaTT05CO2_, 'f', 2));
    xml.writeAttribute("TT06CO2", QString::number(martaTT06CO2_, 'f', 2));
    xml.writeAttribute("TT07CO2", QString::number(martaTT07CO2_, 'f', 2));
    xml.writeEndElement();

    xml.writeStartElement("MartaSCCO2");
    xml.writeAttribute("SC01CO2", QString::number(martaSC01CO2_, 'f', 2));
    xml.writeAttribute("SC02CO2", QString::number(martaSC02CO2_, 'f', 2));
    xml.writeAttribute("SC03CO2", QString::number(martaSC03CO2_, 'f', 2));
    xml.writeAttribute("SC05CO2", QString::number(martaSC05CO2_, 'f', 2));
    xml.writeAttribute("SC06CO2", QString::number(martaSC06CO2_, 'f', 2));
    xml.writeEndElement();

    xml.writeStartElement("MartaDPCO2");
    xml.writeAttribute("DP01CO2", QString::number(martaDP01CO2_, 'f', 2));
    xml.writeAttribute("DP02CO2", QString::number(martaDP02CO2_, 'f', 2));
    xml.writeAttribute("DP03CO2", QString::number(martaDP03CO2_, 'f', 2));
    xml.writeAttribute("DP04CO2", QString::number(martaDP04CO2_, 'f', 2));
    xml.writeEndElement();

    xml.writeStartElement("MartaDTCO2");
    xml.writeAttribute("DT02CO2", QString::number(martaDT02CO2_, 'f', 2));
    xml.writeAttribute("DT03CO2", QString::number(martaDT03CO2_, 'f', 2));
    xml.writeEndElement();

    xml.writeStartElement("MartaSTCO2");
    xml.writeAttribute("ST01CO2", QString::number(martaST01CO2_, 'f', 2));
    xml.writeAttribute("ST02CO2", QString::number(martaST02CO2_, 'f', 2));
    xml.writeAttribute("ST03CO2", QString::number(martaST03CO2_, 'f', 2));
    xml.writeAttribute("ST04CO2", QString::number(martaST04CO2_, 'f', 2));
    xml.writeEndElement();

    xml.writeStartElement("MartaFlow");
    xml.writeAttribute("FT01CO2", QString::number(martaFT01CO2_, 'f', 3));
    xml.writeEndElement();

    xml.writeStartElement("MartaSettings");
    xml.writeAttribute("Speed", QString::number((int)martaSpeedSetpoint_));
    xml.writeAttribute("Flow", QString::number(martaFlowSetpoint_, 'f', 1));
    xml.writeAttribute("Temperature", QString::number(martaTemperatureSetpoint_, 'f', 1));
    xml.writeAttribute("Speed2", QString::number((int)martaSpeedSetpoint2_));
    xml.writeAttribute("Flow2", QString::number(martaFlowSetpoint2_, 'f', 1));
    xml.writeAttribute("Temperature2", QString::number(martaTemperatureSetpoint2_, 'f', 1));
    xml.writeAttribute("Status", QString("0x") + QStringLiteral("%1").arg(martaStatus_, 4, 16, QLatin1Char('0')));
    xml.writeEndElement();

    xml.writeStartElement("MartaAlarms");
    xml.writeAttribute("Status", QString::number(martaAlarmStatus_));
    for (int idx=0;idx<4;++idx) {
    	xml.writeAttribute(QString("Alarm") + QString::number(idx),
  											 QString("0x") + QStringLiteral("%1").arg(martaAlarms_[idx], 4, 16, QLatin1Char('0')));
    }
    xml.writeEndElement();

    xml.writeEndElement();
  }

  if (buffer.length()>0) {
    emit daqMessage(buffer);
    emit newDataAvailable();
  }
}

void Thermo2DAQModel::agilentInfoChanged()
{
  if (!daqState_) return;

  NQLogDebug("Thermo2DAQModel") << "agilentInfoChanged()";

  if (thread()==QApplication::instance()->thread()) {
    NQLogDebug("Thermo2DAQModel") << "running in main application thread";
  } else {
    NQLogDebug("Thermo2DAQModel") << "running in dedicated DAQ thread";
  }

  QMutexLocker locker(&mutex_);

  QDateTime& utime = currentTime();

  QString buffer;

  bool changed = false;
  changed |= updateIfChanged<bool>(agilentState_, agilentModel_->getDeviceState()==READY ? true : false);
  changed |= updateIfChanged<bool>(agilentPumpState_, agilentModel_->getPumpState());
  changed |= updateIfChanged<unsigned int>(agilentPumpStatus_, agilentModel_->getPumpStatus());
  changed |= updateIfChanged<unsigned int>(agilentErrorCode_, agilentModel_->getErrorCode());

  if (changed) {
    QXmlStreamWriter xml(&buffer);
    xml.setAutoFormatting(true);

    xml.writeStartElement("AgilentTwisTorr304");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("State", agilentState_==READY ? "1" : "0");
    xml.writeAttribute("PumpState", agilentPumpState_==true ? "1" : "0");
    xml.writeAttribute("PumpStatus", QString::number(agilentPumpStatus_));
    xml.writeAttribute("ErrorCode", QString::number(agilentErrorCode_));

    xml.writeEndElement();
  }

  if (buffer.length()>0) {
    emit daqMessage(buffer);
    emit newDataAvailable();
  }
}

void Thermo2DAQModel::leyboldInfoChanged()
{
  if (!daqState_) return;

  NQLogDebug("Thermo2DAQModel") << "leyboldInfoChanged()";

  if (thread()==QApplication::instance()->thread()) {
    NQLogDebug("Thermo2DAQModel") << "running in main application thread";
  } else {
    NQLogDebug("Thermo2DAQModel") << "running in dedicated DAQ thread";
  }

  QMutexLocker locker(&mutex_);

  QDateTime& utime = currentTime();

  QString buffer;

  bool changed = false;
  changed |= updateIfChanged<bool>(leyboldState_, leyboldModel_->getDeviceState()==READY ? true : false);
  changed |= updateIfChanged<double>(leyboldPressure_, leyboldModel_->getPressure());

  if (changed) {
    QXmlStreamWriter xml(&buffer);
    xml.setAutoFormatting(true);

    xml.writeStartElement("LeyboldGraphixOne");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("State", leyboldState_==READY ? "1" : "0");
    xml.writeAttribute("Pressure", QString::number(leyboldPressure_, 'e', 3));
    xml.writeEndElement();

    xml.writeEndElement();
  }

  if (buffer.length()>0) {
    emit daqMessage(buffer);
    emit newDataAvailable();
  }
}

void Thermo2DAQModel::nge103BInfoChanged()
{
  if (!daqState_) return;

  NQLogDebug("Thermo2DAQModel") << "nge103BInfoChanged()";

  if (thread()==QApplication::instance()->thread()) {
    NQLogDebug("Thermo2DAQModel") << "running in main application thread";
  } else {
    NQLogDebug("Thermo2DAQModel") << "running in dedicated DAQ thread";
  }

  QMutexLocker locker(&mutex_);

  QDateTime& utime = currentTime();

  QString buffer;
  QXmlStreamWriter xml(&buffer);
  xml.setAutoFormatting(true);

  xml.writeStartElement("RohdeSchwarzNGE103B");
  xml.writeAttribute("time", utime.toString(Qt::ISODate));

  for (int i=0;i<3;++i) {

    bool changed = false;
    changed |= updateIfChanged<bool>(nge103BOutputState_[i], nge103BModel_->getOutputState(i+1));
    changed |= updateIfChanged<unsigned int>(nge103BOutputMode_[i], nge103BModel_->getOutputMode(i+1));
    changed |= updateIfChanged<float>(nge103BVoltage_[i], nge103BModel_->getVoltage(i+1));
    changed |= updateIfChanged<float>(nge103BCurrent_[i], nge103BModel_->getCurrent(i+1));
    changed |= updateIfChanged<float>(nge103BMeasuredVoltage_[i], nge103BModel_->getMeasuredVoltage(i+1));
    changed |= updateIfChanged<float>(nge103BMeasuredCurrent_[i], nge103BModel_->getMeasuredCurrent(i+1));

    if (changed) {
      xml.writeStartElement(QString("RohdeSchwarzNGE103BChannel"));
      xml.writeAttribute("id", QString::number(i+1));
      xml.writeAttribute("State", nge103BOutputState_[i]==true ? "1" : "0");
      xml.writeAttribute("Mode", QString::number(nge103BOutputMode_[i]));
      xml.writeAttribute("U", QString::number(nge103BVoltage_[i], 'f', 3));
      xml.writeAttribute("mU", QString::number(nge103BMeasuredVoltage_[i], 'f', 3));
      xml.writeAttribute("I", QString::number(nge103BCurrent_[i], 'f', 3));
      xml.writeAttribute("mI", QString::number(nge103BMeasuredCurrent_[i], 'f', 3));
      xml.writeEndElement();
    }
  }
  xml.writeEndElement();

  if (buffer.length()>0) {
    emit daqMessage(buffer);
    emit newDataAvailable();
  }
}

void Thermo2DAQModel::keithleyInfoChanged()
{
  if (!daqState_) return;

  NQLogDebug("Thermo2DAQModel") << "keithleyInfoChanged()";

  if (thread()==QApplication::instance()->thread()) {
    NQLogDebug("Thermo2DAQModel") << "running in main application thread";
  } else {
    NQLogDebug("Thermo2DAQModel") << "running in dedicated DAQ thread";
  }

  QMutexLocker locker(&mutex_);

  QDateTime& utime = currentTime();

  QString buffer;
  QXmlStreamWriter xml(&buffer);
  xml.setAutoFormatting(true);

  xml.writeStartElement("KeithleyDAQ6510");
  xml.writeAttribute("time", utime.toString(Qt::ISODate));

  for (unsigned int card=0;card<2;++card) {
    for (unsigned int channel=0;channel<10;++channel) {
      unsigned int sensor = (card+1)*100 + channel + 1;

      bool changed = false;
      changed |= updateIfChanged<bool>(keithleyState_[card][channel], keithleyModel_->getSensorState(sensor)==READY);
      if (keithleyState_[card][channel]) {
        changed |= updateIfChanged<float>(keithleyTemperature_[card][channel], keithleyModel_->getTemperature(sensor));
      } else {
        keithleyTemperature_[card][channel] = 0;
      }

      if (changed) {
        xml.writeStartElement(QString("KeithleyDAQ6510Sensor"));
        xml.writeAttribute("id", QString::number(sensor));
        xml.writeAttribute("State", keithleyState_[card][channel]==true ? "1" : "0");
        xml.writeAttribute("T", QString::number(keithleyTemperature_[card][channel], 'f', 4));
        xml.writeEndElement();
      }
    }
  } 
  xml.writeEndElement();

  if (buffer.length()>0) {
    emit daqMessage(buffer);
    emit newDataAvailable();
  }
}
