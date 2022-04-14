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
#include <cmath>

#include <QApplication>
#include <QDateTime>
#include <QXmlStreamWriter>

#include <nqlogger.h>
#include <MattermostBot.h>

#include <ApplicationConfig.h>

#include "Thermo2ThroughPlaneModel.h"

Thermo2ThroughPlaneModel::Thermo2ThroughPlaneModel(HuberUnistat525wModel* huberModel,
		RohdeSchwarzNGE103BModel* nge103BModel,
		KeithleyDAQ6510Model* keithleyModel,
		QObject * /* parent */)
 : QObject(),
   huberModel_(huberModel),
   nge103BModel_(nge103BModel),
   keithleyModel_(keithleyModel)
{
  ApplicationConfig* config = ApplicationConfig::instance();

  mattermostStatus_ = config->getValue<int>("main", "ThroughPlaneMattermostStatus");
  mattermostInterval_ = config->getValue<int>("main", "ThroughPlaneMattermostInterval");

  mattermostStatusTime_ = QDateTime::currentDateTime();
  QTime time = mattermostStatusTime_.time();
  time = time.addMSecs(-time.msec());
  time = time.addSecs(-time.second());
  time = time.addSecs(mattermostInterval_*60);
  mattermostStatusTime_.setTime(time);

  resistance_ = config->getValue<double>("main", "ThroughPlaneResistance");
  cableResistance_ = config->getValue<double>("main", "ThroughPlaneCableResistance");
  kBlock_ = config->getValue<double>("main", "ThroughPlaneKBlock");
  ABlock_ = config->getValue<double>("main", "ThroughPlaneABlock");
  nge103BChannel_ = config->getValue<unsigned int>("main", "ThroughPlaneNGE103BChannel");
  keithleyTopSensors_ = config->getValueArray<unsigned int,6>("main", "ThroughPlaneKeithleyTopSensors");
  keithleyTopPositions_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopPositions");
  keithleyTopCor0_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor0");
  keithleyTopCor1_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor1");
  keithleyTopCor2_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor2");
  keithleyTopCor3_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor3");
  keithleyBottomSensors_ = config->getValueArray<unsigned int,6>("main", "ThroughPlaneKeithleyBottomSensors");
  keithleyBottomPositions_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomPositions");
  keithleyBottomCor0_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor0");
  keithleyBottomCor1_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor1");
  keithleyBottomCor2_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor2");
  keithleyBottomCor3_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor3");
  keithleyAmbientSensor_ = config->getValue<double>("main", "KeithleyAmbientSensor");

  std::string sensorType;

  sensorType = config->getValue("main", "ThroughPlaneKeithleyTopSensorTypes");
  if (sensorType=="4WirePT100") {
    keithleyTopSensorTypes_ = VKeithleyDAQ6510::FourWireRTD_PT100;
  } else if (sensorType=="Therm10k") {
    keithleyTopSensorTypes_ = VKeithleyDAQ6510::Thermistor_10000;
  } else {
    keithleyTopSensorTypes_ = VKeithleyDAQ6510::FourWireRTD_PT100;
  }

  sensorType = config->getValue("main", "ThroughPlaneKeithleyBottomSensorTypes");
  if (sensorType=="4WirePT100") {
    keithleyBottomSensorTypes_ = VKeithleyDAQ6510::FourWireRTD_PT100;
  } else if (sensorType=="Therm10k") {
    keithleyBottomSensorTypes_ = VKeithleyDAQ6510::Thermistor_10000;
  } else {
    keithleyBottomSensorTypes_ = VKeithleyDAQ6510::FourWireRTD_PT100;
  }

  if (keithleyAmbientSensor_!=0) {
    sensorType = config->getValue("main", "KeithleyAmbientSensorType");
    if (sensorType=="4WirePT100") {
      keithleyAmbientSensorType_ = VKeithleyDAQ6510::FourWireRTD_PT100;
    }else if (sensorType=="Therm10k") {
      keithleyAmbientSensorType_ = VKeithleyDAQ6510::Thermistor_10000;
    } else {
      keithleyAmbientSensorType_ = VKeithleyDAQ6510::FourWireRTD_PT100;
    }
  }

  bool throughPlaneAutoConfig = config->getValue<int>("main", "ThroughPlaneAutoConfig");
  if (throughPlaneAutoConfig) {
    for (unsigned int i=0;i<6;++i) {
      keithleyModel_->setSensorMode(keithleyTopSensors_[i], keithleyTopSensorTypes_);
      keithleyModel_->setSensorEnabled(keithleyTopSensors_[i], true);

      keithleyModel_->setSensorMode(keithleyBottomSensors_[i], keithleyBottomSensorTypes_);
      keithleyModel_->setSensorEnabled(keithleyBottomSensors_[i], true);
    }

    if (keithleyAmbientSensor_!=0) {
      keithleyModel_->setSensorMode(keithleyAmbientSensor_, keithleyAmbientSensorType_);
      keithleyModel_->setSensorEnabled(keithleyAmbientSensor_, true);
    }
  }

  huberState_ = false;
  huberTemperatureSetPoint_ = 0;
  huberInternalTemperature_ = 0;
  huberProcessTemperature_ = 0;

  nge103BState_ = false;
  nge103BChannelState_ = false;
  nge103BCurrent_ = 0;
  sourcePower_ = 0;

  keithleyState_ = false;
  for (unsigned int c=0;c<6;++c) {
    keithleyTopSensorStates_[c] = false;
    keithleyBottomSensorStates_[c] = false;
    keithleyTopTemperatures_[c] = 0;
    keithleyBottomTemperatures_[c] = 0;

    // minimum Keithley readout frequency is 1/10s
    // storage for at least 1800 s
    keithleyTopTemperatureHistory_[c] = HistoryFifo<double>(1800/10);
    keithleyBottomTemperatureHistory_[c] = HistoryFifo<double>(1800/10);
  }

  calculationState_ = false;
  gradientTop_ = 0;
  powerTop_ = 0;
  sampleTTop_ = 0;
  sampleTMiddle_ = 0;
  sampleTBottom_ = 0;
  gradientBottom_ = 0;
  powerBottom_ = 0;

  connect(huberModel_, SIGNAL(informationChanged()),
          this, SLOT(huberInfoChanged()));

  connect(nge103BModel_, SIGNAL(informationChanged()),
          this, SLOT(nge103BInfoChanged()));

  connect(keithleyModel_, SIGNAL(informationChanged()),
          this, SLOT(keithleyInfoChanged()));

  mattermostTimer_ = new QTimer(this);
  connect(mattermostTimer_, SIGNAL(timeout()),
	  this, SLOT(sendMattermostStatus()));
  mattermostTimer_->start(60*1000);

  connect(config, SIGNAL(valueChanged()),
          this, SLOT(configurationChanged()));
}

void Thermo2ThroughPlaneModel::configurationChanged()
{
  QMutexLocker locker(&mutex_);

  ApplicationConfig* config = ApplicationConfig::instance();

  resistance_ = config->getValue<double>("main", "ThroughPlaneResistance");
  cableResistance_ = config->getValue<double>("main", "ThroughPlaneCableResistance");
  kBlock_ = config->getValue<double>("main", "ThroughPlaneKBlock");
  ABlock_ = config->getValue<double>("main", "ThroughPlaneABlock");
  keithleyTopPositions_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopPositions");
  keithleyTopCor0_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor0");
  keithleyTopCor1_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor1");
  keithleyTopCor2_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor2");
  keithleyTopCor3_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyTopCor3");
  keithleyBottomPositions_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomPositions");
  keithleyBottomCor0_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor0");
  keithleyBottomCor1_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor1");
  keithleyBottomCor2_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor2");
  keithleyBottomCor3_ = config->getValueArray<double,6>("main", "ThroughPlaneKeithleyBottomCor3");
}

void Thermo2ThroughPlaneModel::setTemperatureSetPoint(double temperature)
{
  QMutexLocker locker(&mutex_);

  huberModel_->setTemperatureSetPoint(temperature);
}

void Thermo2ThroughPlaneModel::setSourcePower(double power)
{
  QMutexLocker locker(&mutex_);

  double current = std::round(std::sqrt(power/resistance_) * 100) / 100;

  nge103BModel_->setCurrent(nge103BChannel_, current);
}

void Thermo2ThroughPlaneModel::huberInfoChanged()
{
  NQLogDebug("Thermo2ThroughPlaneModel") << "huberInfoChanged()";

  QMutexLocker locker(&mutex_);

  bool changed = false;
  changed |= updateIfChanged<bool>(huberState_, huberModel_->getDeviceState()==READY ? true : false);
  changed |= updateIfChanged<double>(huberTemperatureSetPoint_, huberModel_->getTemperatureSetPoint());
  changed |= updateIfChanged<double>(huberInternalTemperature_, huberModel_->getInternalTemperature());
  changed |= updateIfChanged<double>(huberProcessTemperature_, huberModel_->getProcessTemperature());

  if (changed) {
    emit informationChanged();
  }
}

void Thermo2ThroughPlaneModel::nge103BInfoChanged()
{
  NQLogDebug("Thermo2ThroughPlaneModel") << "nge103BInfoChanged()";

  QMutexLocker locker(&mutex_);

  bool changed = false;
  changed |= updateIfChanged<bool>(nge103BState_, nge103BModel_->getDeviceState()==READY ? true : false);
  changed |= updateIfChanged<bool>(nge103BChannelState_, nge103BModel_->getOutputState(nge103BChannel_));
  changed |= updateIfChanged<double>(nge103BCurrent_, nge103BModel_->getCurrent(nge103BChannel_));

  if (changed) {

    sourcePower_ = resistance_ * std::pow(nge103BCurrent_, 2);

    emit informationChanged();
  }
}

void Thermo2ThroughPlaneModel::keithleyInfoChanged()
{
  NQLogDebug("Thermo2ThroughPlaneModel") << "keithleyInfoChanged()";

  QMutexLocker locker(&mutex_);

  bool changed = false;
  unsigned int countTop = 0;
  unsigned int countBottom = 0;
  double temp, tcor;
  changed |= updateIfChanged<bool>(keithleyState_, keithleyModel_->getDeviceState()==READY ? true : false);
  for (unsigned int i=0;i<6;++i) {
    changed |= updateIfChanged<bool>(keithleyTopSensorStates_[i],
        keithleyModel_->getSensorState(keithleyTopSensors_[i])==READY ? true : false);

    temp = keithleyModel_->getTemperature(keithleyTopSensors_[i]);
    tcor =  keithleyTopCor0_[i];
    tcor += keithleyTopCor1_[i] * temp;
    tcor += keithleyTopCor2_[i] * temp * temp;
    tcor += keithleyTopCor3_[i] * temp * temp * temp;
    changed |= updateIfChanged<double>(keithleyTopTemperatures_[i], tcor);
    if (keithleyTopSensorStates_[i]) countTop++;

    changed |= updateIfChanged<bool>(keithleyBottomSensorStates_[i],
        keithleyModel_->getSensorState(keithleyBottomSensors_[i])==READY ? true : false);

    temp = keithleyModel_->getTemperature(keithleyBottomSensors_[i]);
    tcor =  keithleyBottomCor0_[i];
    tcor += keithleyBottomCor1_[i] * temp;
    tcor += keithleyBottomCor2_[i] * temp * temp;
    tcor += keithleyBottomCor3_[i] * temp * temp * temp;
    changed |= updateIfChanged<double>(keithleyBottomTemperatures_[i], tcor);
    if (keithleyBottomSensorStates_[i]) countBottom++;
  }

  if (keithleyAmbientSensor_!=0) {
    changed |= updateIfChanged<bool>(keithleyAmbientSensorState_,
        keithleyModel_->getSensorState(keithleyAmbientSensor_)==READY ? true : false);
    changed |= updateIfChanged<double>(keithleyAmbientTemperature_,
        keithleyModel_->getTemperature(keithleyAmbientSensor_));
  }

  const QDateTime dt(QDateTime::currentDateTime());
  for (unsigned int i=0;i<6;++i) {
    keithleyTopTemperatureHistory_[i].push_back(dt, keithleyTopTemperatures_[i]);
    keithleyBottomTemperatureHistory_[i].push_back(dt, keithleyBottomTemperatures_[i]);
  }

  size_t pos = keithleyTopTemperatureHistory_[0].indexInPast(1800);
  double deltaTime = keithleyTopTemperatureHistory_[0].deltaTime(pos); // [s]

  if (deltaTime!=0) {

    /*
    NQLogMessage("Thermo2ThroughPlaneModel") << "stats " << dt.toSecsSinceEpoch();
    NQLogMessage("Thermo2ThroughPlaneModel") << "fillLevel " << keithleyTopTemperatureHistory_[0].fillLevel();
    NQLogMessage("Thermo2ThroughPlaneModel") << "deltaTime " << keithleyTopTemperatureHistory_[0].deltaTime();
    */
    
    double delta, gradient, mean, variance;
    std::pair<double,double> stats;
    for (unsigned int i=0;i<6;++i) {
      delta = keithleyTopTemperatureHistory_[i].delta(pos); // [K]
      gradient = keithleyTopTemperatureHistory_[i].gradient(pos); // [K/s]
      stats = keithleyTopTemperatureHistory_[i].stats(pos);
      mean = stats.first; // [K]
      variance = stats.second; // [K^2]

      /*
      NQLogMessage("Thermo2ThroughPlaneModel") << "delta_top " << i << " " << delta;
      NQLogMessage("Thermo2ThroughPlaneModel") << "gradient_top " << i << " " << gradient;
      NQLogMessage("Thermo2ThroughPlaneModel") << "mean_top " << i << " " << mean;
      NQLogMessage("Thermo2ThroughPlaneModel") << "variance_top " << i << " " << variance;
      */
    }
    for (unsigned int i=0;i<6;++i) {
      delta = keithleyBottomTemperatureHistory_[i].delta(pos); // [K]
      gradient = keithleyBottomTemperatureHistory_[i].gradient(pos); // [K/s]
      stats = keithleyBottomTemperatureHistory_[i].stats(pos);
      mean = stats.first; // [K]
      variance = stats.second; // [K^2]

      /*
      NQLogMessage("Thermo2ThroughPlaneModel") << "delta_bot " << i << " " << delta;
      NQLogMessage("Thermo2ThroughPlaneModel") << "gradient_bot " << i << " " << gradient;
      NQLogMessage("Thermo2ThroughPlaneModel") << "mean_bot " << i << " " << mean;
      NQLogMessage("Thermo2ThroughPlaneModel") << "variance_bot " << i << " " << variance;
      */
    }
  }

  if (changed) {

    if (countTop>=2 && countBottom>=2) {
      calculationState_ = true;

      std::vector<std::pair<double,double> > values;
      double p0, p1;

      values.clear();
      for (unsigned int i=0;i<6;i++) {
        if (keithleyTopSensorStates_[i]) {
          values.push_back(std::pair<double,double>(keithleyTopPositions_[i], keithleyTopTemperatures_[i]));
        }
      }
      fitter_.fit(values, 2, p0, p1);

      sampleTTop_ = p0; // [degC]
      gradientTop_ = -1.0 * p1 * 1000.; // [degC/m]
      powerTop_ = -1.0 * gradientTop_ * kBlock_ * ABlock_ * 1e-6; // [W]

      NQLogDebug("Thermo2ThroughPlaneModel") << "gradientTop_ = " << gradientTop_;
      NQLogDebug("Thermo2ThroughPlaneModel") << "powerTop_ = " << powerTop_;

      values.clear();
      for (unsigned int i=0;i<6;i++) {
        if (keithleyTopSensorStates_[i]) {
          values.push_back(std::pair<double,double>(keithleyBottomPositions_[i], keithleyBottomTemperatures_[i]));
        }
      }
      fitter_.fit(values, 2, p0, p1);

      sampleTBottom_ = p0;
      gradientBottom_ = 1.0 * p1 * 1000.;
      powerBottom_ = -1.0 * gradientBottom_ * kBlock_ * ABlock_ * 1e-6;

      NQLogDebug("Thermo2ThroughPlaneModel") << "gradientBottom_ = " << gradientBottom_;
      NQLogDebug("Thermo2ThroughPlaneModel") << "powerBottom_ = " << powerBottom_;

      sampleTMiddle_ = 0.5*(sampleTTop_ + sampleTBottom_);

    } else {
      calculationState_ = false;
    }

    emit informationChanged();
  }
}

void Thermo2ThroughPlaneModel::sendMattermostStatus()
{
  if (!mattermostStatus_) return;

  auto currentDateTime = QDateTime::currentDateTime();
  if (currentDateTime<mattermostStatusTime_) return;

  NQLogDebug("Thermo2ThroughPlaneModel") << "sendMattermostStatus()";

  QString msg;

  msg += "#### Through-Plane Status (";
  msg += mattermostStatusTime_.toString("dd.MM.yyyy hh:mm");
  msg += ")\n";

  msg += "| #        |    1     |    2     |    3     |    4     |    5     |    6     |\n";
  msg += "|:---------|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|\n";

  msg += "| T_top    |";
  for (unsigned int i=0;i<6;++i) {
    if (keithleyTopSensorStates_[i]) {
      msg += QString::number(keithleyTopTemperatures_[i], 'f', 2);
      msg += "°C |";
    } else {
      msg += " |";
    }
  }
  msg += "\n";
  msg += "| T_bottom    |";
  for (unsigned int i=0;i<6;++i) {
    if (keithleyBottomSensorStates_[i]) {
      msg += QString::number(keithleyBottomTemperatures_[i], 'f', 2);
      msg += "°C |";
    } else {
      msg += " |";
    }
  }
  msg += "\n\n";

  msg += "|  Variable      | Value    |\n";
  msg += "|:--------------:|:--------:|\n";

  msg += "| Sample top |";
  if (calculationState_) {
    msg += QString::number(sampleTTop_, 'f', 2);
    msg += "°C |\n";
  } else {
    msg += " |\n";
  }

  msg += "| Sample middle |";
  if (calculationState_) {
    msg += QString::number(sampleTMiddle_, 'f', 2);
    msg += "°C |\n";
  } else {
    msg += " |\n";
  }

  msg += "| Sample bottom |";
  if (calculationState_) {
    msg += QString::number(sampleTBottom_, 'f', 2);
    msg += "°C |\n";
  } else {
    msg += " |\n";
  }

  msg += "| Power source |";
  if (calculationState_) {
    msg += QString::number(sourcePower_, 'f', 2);
    msg += "W |\n";
  } else {
    msg += " |\n";
  }

  msg += "| Power top |";
  if (calculationState_) {
    msg += QString::number(powerTop_, 'f', 2);
    msg += "W |\n";
  } else {
    msg += " |\n";
  }

  msg += "| Power bottom |";
  if (calculationState_) {
    msg += QString::number(powerBottom_, 'f', 2);
    msg += "W |\n";
  } else {
    msg += " |\n";
  }

  msg += "| Sink |";
  if (calculationState_) {
    msg += QString::number(huberTemperatureSetPoint_, 'f', 2);
    msg += "°C |\n";
  } else {
    msg += " |\n";
  }

  MattermostBot bot(
      ApplicationConfig::instance()->getValue<QString>("main", "mattermostchannel"),
      ApplicationConfig::instance()->getValue<QString>("main", "mattermostusername"),
      ApplicationConfig::instance()->getValue<QString>("main", "mattermostwebhook")
  );

  bot.postMessage(msg);

  mattermostStatusTime_ = mattermostStatusTime_.addSecs(mattermostInterval_*60);
}

void Thermo2ThroughPlaneModel::statusMessage(const QString & text)
{
  emit message(text);
}
