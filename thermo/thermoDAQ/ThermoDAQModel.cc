#include <iostream>

#include <QApplication>
#include <QDateTime>
#include <QXmlStreamWriter>

#include <qwt_date.h>

#include <nqlogger.h>

#include "ThermoDAQModel.h"

ThermoDAQModel::ThermoDAQModel(HuberPetiteFleurModel* huberModel,
                               KeithleyModel* keithleyModel,
                               HamegModel* hamegModel,
                               PfeifferModel* pfeifferModel,
			       IotaModel* iotaModel,
			       ArduinoPresModel* arduinoPresModel,
                               QObject *parent) :
    QObject(),
    daqState_(false),
    huberModel_(huberModel),
    keithleyModel_(keithleyModel),
    hamegModel_(hamegModel),
    pfeifferModel_(pfeifferModel),
    iotaModel_(iotaModel),
    arduinoPresModel_(arduinoPresModel),
    huberCirculator_(false),
    huberWorkingTemperature_(0.0),
    huberBathTemperature_(0.0),
    hamegRemoteMode_(false),
    hamegOutputsEnabled_(false),
    pfeifferStatus1_(0),
    pfeifferPressure1_(0.0),
    pfeifferStatus2_(0),
    pfeifferPressure2_(0.0),
    iotaPumpEnabled_(false),
    iotaActPressure_(0.0),
    iotaSetPressure_(0.0),
    iotaActFlow_(0.0),
    iotaSetFlow_(0.0),
    arduinoPressureA_(0.0),
    arduinoPressureB_(0.0)
{
    for (int i=0;i<10;++i) {
      keithleySensorState_[i] = OFF;
      keithleyTemperature_[i] = 0;
    }
    for (int i=0;i<2;++i) {
      hamegCVMode_[i] = OFF;
      hamegSetVoltage_[i] = 0;
      hamegSetCurrent_[i] = 0;
      hamegVoltage_[i] = 0;
      hamegCurrent_[i] = 0;
    }

    connect(huberModel_, SIGNAL(informationChanged()),
            this, SLOT(huberInfoChanged()));

    connect(keithleyModel_, SIGNAL(sensorStateChanged(uint,State)),
            this, SLOT(keithleySensorStateChanged(uint,State)));
    connect(keithleyModel_, SIGNAL(temperatureChanged(uint,double)),
            this, SLOT(keithleyTemperatureChanged(uint,double)));

    connect(hamegModel_, SIGNAL(informationChanged()),
            this, SLOT(hamegInfoChanged()));

    connect(pfeifferModel_, SIGNAL(informationChanged()),
            this, SLOT(pfeifferInfoChanged()));

    connect(iotaModel_, SIGNAL(informationChanged()),
	    this, SLOT(iotaInfoChanged()));

    connect(arduinoPresModel_, SIGNAL(informationChanged()),
	    this, SLOT(arduinoPresInfoChanged()));
}

void ThermoDAQModel::myMoveToThread(QThread *thread)
{
    huberModel_->moveToThread(thread);
    keithleyModel_->moveToThread(thread);
    hamegModel_->moveToThread(thread);
    pfeifferModel_->moveToThread(thread);
    iotaModel_->moveToThread(thread);
    arduinoPresModel_->moveToThread(thread);
    this->moveToThread(thread);
}

void ThermoDAQModel::startMeasurement()
{
    daqState_ = true;
    emit daqStateChanged(true);

    QString buffer;
    createDAQStatusMessage(buffer);
    emit daqMessage(buffer);

    NQLog("thermoDAQ") << "measurement started";
}

const Measurement_t& ThermoDAQModel::getMeasurement()
{
  measurement_.dt = currentTime();
  measurement_.daqState = daqState();
  
  /*
  valueMap_["@DAQSTATE@"] = daqState();

  // valueMap_["@@"] = ;
  valueMap_["@HUBERCIRCULATOR@"] = huberCirculator_;
  valueMap_["@HUBERWORKINGTEMPERATURE@"] = huberWorkingTemperature_;
  valueMap_["@HUBERBATHTEMPERATURE@"] = huberBathTemperature_;
  
  for (int i=0;i<10;++i) {
    valueMap_[QString("@KEITHLEYSENSORSTATE%02d@").arg(i)] = (int)keithleySensorState_[i];
    valueMap_[QString("@KEITHLEYTEMPERATURE%02d@").arg(i)] = keithleyTemperature_[i];
  }

  return valueMap_;
  */

  return measurement_;
}

void ThermoDAQModel::createDAQStatusMessage(QString &buffer)
{
    QDateTime& utime = currentTime();

    QXmlStreamWriter xml(&buffer);
    xml.setAutoFormatting(true);

    xml.writeStartElement("HuberTemperature");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("circulator", huberModel_->isCirculatorEnabled()==true ? "1" : "0");
    xml.writeAttribute("work", QString::number(huberModel_->getWorkingTemperatureParameter().getValue(), 'f', 6));
    xml.writeAttribute("bath", QString::number(huberModel_->getBathTemperature(), 'f', 6));
    xml.writeEndElement();

    for (int sensor=0;sensor<10;++sensor) {
        xml.writeStartElement("KeithleyState");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("sensor", QString::number(sensor));
        xml.writeAttribute("state", QString::number(keithleyModel_->getSensorState(sensor)));
        xml.writeEndElement();
    }
    for (int sensor=0;sensor<10;++sensor) {
        xml.writeStartElement("KeithleyTemperature");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("sensor", QString::number(sensor));
        xml.writeAttribute("temperature", QString::number(keithleyModel_->getTemperature(sensor), 'f', 6));
        xml.writeEndElement();
    }

    xml.writeStartElement("HamegSetup");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("remote", hamegRemoteMode_==true ? "1" : "0");
    xml.writeAttribute("outputs", hamegOutputsEnabled_==true ? "1" : "0");
    xml.writeAttribute("CV1", hamegCVMode_[0]==true ? "1" : "0");
    xml.writeAttribute("CV2", hamegCVMode_[1]==true ? "1" : "0");
    xml.writeEndElement();

    xml.writeStartElement("HamegSetvalues");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("V1", QString::number(hamegSetVoltage_[0], 'f', 6));
    xml.writeAttribute("C1", QString::number(hamegSetCurrent_[0], 'f', 6));
    xml.writeAttribute("V2", QString::number(hamegSetVoltage_[1], 'f', 6));
    xml.writeAttribute("C2", QString::number(hamegSetCurrent_[1], 'f', 6));
    xml.writeEndElement();

    xml.writeStartElement("HamegValues");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("V1", QString::number(hamegVoltage_[0], 'f', 6));
    xml.writeAttribute("C1", QString::number(hamegCurrent_[0], 'f', 6));
    xml.writeAttribute("V2", QString::number(hamegVoltage_[1], 'f', 6));
    xml.writeAttribute("C2", QString::number(hamegCurrent_[1], 'f', 6));
    xml.writeEndElement();

    xml.writeStartElement("PfeifferPressure");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("s1", QString::number((int)pfeifferModel_->getStatus1()));
    xml.writeAttribute("p1", QString::number(pfeifferModel_->getPressure1(), 'e', 6));
    xml.writeAttribute("s2", QString::number((int)pfeifferModel_->getStatus2()));
    xml.writeAttribute("p2", QString::number(pfeifferModel_->getPressure2(), 'e', 6));
    xml.writeEndElement();

    xml.writeStartElement("IotaSetup");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("enabled", iotaPumpEnabled_==true ? "1" : "0");
    xml.writeAttribute("pressure", QString::number(iotaSetPressure_, 'e', 6));
    xml.writeAttribute("flow", QString::number(iotaSetFlow_, 'e', 6));
    xml.writeEndElement();

    xml.writeStartElement("IotaValues");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("pressure", QString::number(iotaActPressure_, 'e', 6));
    xml.writeAttribute("flow", QString::number(iotaActFlow_, 'e', 6));
    xml.writeEndElement();

    xml.writeStartElement("ArduinoPressure");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("pA", QString::number(arduinoPressureA_, 'e', 6));
    xml.writeAttribute("pB", QString::number(arduinoPressureB_, 'e', 6));
    xml.writeEndElement();

    xml.writeStartElement("DAQStarted");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeEndElement();
}

void ThermoDAQModel::stopMeasurement()
{
    daqState_ = false;
    emit daqStateChanged(false);

    NQLog("thermoDAQ") << "measurement stopped";
}

void ThermoDAQModel::clearHistory()
{

}

QDateTime& ThermoDAQModel::currentTime()
{
    QDateTime ct = QDateTime::currentDateTime();
    if (currentTime_.secsTo(ct)>2) currentTime_ = ct;
    return currentTime_;
}

void ThermoDAQModel::customDAQMessage(const QString & message)
{
    emit daqMessage(message);
}

void ThermoDAQModel::huberInfoChanged()
{
    NQLog("ThermoDAQModel", NQLog::Debug) << "huberInfoChanged()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
    }

    QMutexLocker locker(&mutex_);

    QDateTime utime = currentTime();

    bool changed = false;

    changed |= updateIfChanged<float>(huberWorkingTemperature_,
                                      huberModel_->getWorkingTemperatureParameter().getValue());
    changed |= updateIfChanged<float>(huberBathTemperature_, huberModel_->getBathTemperature());
    changed |= updateIfChanged<bool>(huberCirculator_, huberModel_->isCirculatorEnabled());

    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("HuberTemperature");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("circulator", huberCirculator_==true ? "1" : "0");
        xml.writeAttribute("work", QString::number(huberWorkingTemperature_, 'f', 6));
        xml.writeAttribute("bath", QString::number(huberBathTemperature_, 'f', 6));
        xml.writeEndElement();

        emit daqMessage(buffer);
	emit newDataAvailable();
    }
}

void ThermoDAQModel::keithleySensorStateChanged(unsigned int sensor, State newState)
{
    NQLog("ThermoDAQModel", NQLog::Message) << "keithleySensorStateChanged()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
    }

    QMutexLocker locker(&mutex_);

    QDateTime& utime = currentTime();

    bool changed = updateIfChanged<State>(keithleySensorState_[sensor], newState);

    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("KeithleyState");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("sensor", QString::number(sensor));
        xml.writeAttribute("state", QString::number(newState));
        xml.writeEndElement();

        emit daqMessage(buffer);
    }
}

void ThermoDAQModel::keithleyTemperatureChanged(unsigned int sensor, double temperature)
{
    NQLog("ThermoDAQModel", NQLog::Debug) << "keithleyInfoChanged()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
    }

    QMutexLocker locker(&mutex_);

    QDateTime& utime = currentTime();

    bool changed = updateIfChanged<double>(keithleyTemperature_[sensor], temperature);

    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("KeithleyTemperature");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("sensor", QString::number(sensor));
        xml.writeAttribute("temperature", QString::number(temperature, 'f', 6));
        xml.writeEndElement();

        emit daqMessage(buffer);
        emit newDataAvailable();
    }
}

void ThermoDAQModel::hamegInfoChanged()
{
    NQLog("ThermoDAQModel", NQLog::Debug) << "hamegInfoChanged()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
    }

    QMutexLocker locker(&mutex_);

    QDateTime utime = currentTime();

    QString buffer;
    QXmlStreamWriter xml(&buffer);
    xml.setAutoFormatting(true);

    // setup
    bool changed = false;
    changed |= updateIfChanged<bool>(hamegRemoteMode_, hamegModel_->isRemoteMode());
    changed |= updateIfChanged<bool>(hamegOutputsEnabled_, hamegModel_->isOutputEnabled());
    changed |= updateIfChanged<bool>(hamegCVMode_[0], hamegModel_->isConstantVoltageMode(1));
    changed |= updateIfChanged<bool>(hamegCVMode_[1], hamegModel_->isConstantVoltageMode(2));
    if (changed) {
        xml.writeStartElement("HamegSetup");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("remote", hamegRemoteMode_==true ? "1" : "0");
        xml.writeAttribute("outputs", hamegOutputsEnabled_==true ? "1" : "0");
        xml.writeAttribute("CV1", hamegCVMode_[0]==true ? "1" : "0");
        xml.writeAttribute("CV2", hamegCVMode_[1]==true ? "1" : "0");
        xml.writeEndElement();
    }

    // set values
    changed = false;
    changed |= updateIfChanged<float>(hamegSetVoltage_[0], hamegModel_->getVoltageParameter(1).getValue());
    changed |= updateIfChanged<float>(hamegSetVoltage_[1], hamegModel_->getVoltageParameter(2).getValue());
    changed |= updateIfChanged<float>(hamegSetCurrent_[0], hamegModel_->getCurrentParameter(1).getValue());
    changed |= updateIfChanged<float>(hamegSetCurrent_[1], hamegModel_->getCurrentParameter(2).getValue());
    if (changed) {
        xml.writeStartElement("HamegSetvalues");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("V1", QString::number(hamegSetVoltage_[0], 'f', 6));
        xml.writeAttribute("C1", QString::number(hamegSetCurrent_[0], 'f', 6));
        xml.writeAttribute("V2", QString::number(hamegSetVoltage_[1], 'f', 6));
        xml.writeAttribute("C2", QString::number(hamegSetCurrent_[1], 'f', 6));
        xml.writeEndElement();
    }

    // measured values
    changed = false;
    changed |= updateIfChanged<float>(hamegVoltage_[0], hamegModel_->getVoltage(1));
    changed |= updateIfChanged<float>(hamegVoltage_[1], hamegModel_->getVoltage(2));
    changed |= updateIfChanged<float>(hamegCurrent_[0], hamegModel_->getCurrent(1));
    changed |= updateIfChanged<float>(hamegCurrent_[1], hamegModel_->getCurrent(2));
    if (changed) {
        xml.writeStartElement("HamegValues");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("V1", QString::number(hamegVoltage_[0], 'f', 6));
        xml.writeAttribute("C1", QString::number(hamegCurrent_[0], 'f', 6));
        xml.writeAttribute("V2", QString::number(hamegVoltage_[1], 'f', 6));
        xml.writeAttribute("C2", QString::number(hamegCurrent_[1], 'f', 6));
        xml.writeEndElement();
    }

    if (buffer.length()>0) {
      emit daqMessage(buffer);
      emit newDataAvailable();
    }
}

void ThermoDAQModel::pfeifferInfoChanged()
{
    NQLog("ThermoDAQModel", NQLog::Debug) << "pfeifferInfoChanged()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
    }

    QMutexLocker locker(&mutex_);

    QDateTime utime = currentTime();

    bool changed = false;
    changed |= updateIfChanged<int>(pfeifferStatus1_, (int)pfeifferModel_->getStatus1());
    changed |= updateIfChanged<double>(pfeifferPressure1_, pfeifferModel_->getPressure1());
    changed |= updateIfChanged<int>(pfeifferStatus2_, (int)pfeifferModel_->getStatus2());
    changed |= updateIfChanged<double>(pfeifferPressure2_, pfeifferModel_->getPressure2());

    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("PfeifferPressure");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("s1", QString::number(pfeifferStatus1_));
        xml.writeAttribute("p1", QString::number(pfeifferPressure1_, 'e', 6));
        xml.writeAttribute("s2", QString::number(pfeifferStatus2_));
        xml.writeAttribute("p2", QString::number(pfeifferPressure2_, 'e', 6));
        xml.writeEndElement();

        emit daqMessage(buffer);
	emit newDataAvailable();
    }
}

void ThermoDAQModel::iotaInfoChanged()
{
    NQLog("ThermoDAQModel", NQLog::Debug) << "iotaInfoChanged()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
    }

    QMutexLocker locker(&mutex_);

    QDateTime utime = currentTime();

    QString buffer;
    QXmlStreamWriter xml(&buffer);
    xml.setAutoFormatting(true);

    // set values
    bool changed = false;
    changed |= updateIfChanged<bool>(iotaPumpEnabled_, iotaModel_->isPumpEnabled());
    changed |= updateIfChanged<float>(iotaSetPressure_, iotaModel_->getSetPressureParameter().getValue());
    changed |= updateIfChanged<float>(iotaSetFlow_, iotaModel_->getSetFlowParameter().getValue());
    if (changed) {
        xml.writeStartElement("IotaSetup");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("enabled", iotaPumpEnabled_==true ? "1" : "0");
        xml.writeAttribute("pressure", QString::number(iotaSetPressure_, 'e', 6));
        xml.writeAttribute("flow", QString::number(iotaSetFlow_, 'e', 6));
        xml.writeEndElement();
    }

    changed = false;
    changed |= updateIfChanged<float>(iotaActPressure_, iotaModel_->getActPressure());
    changed |= updateIfChanged<float>(iotaActFlow_, iotaModel_->getActFlow());
    if (changed) {
        xml.writeStartElement("IotaValues");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("pressure", QString::number(iotaActPressure_, 'e', 6));
        xml.writeAttribute("flow", QString::number(iotaActFlow_, 'e', 6));
        xml.writeEndElement();
    }
    
    if (buffer.length()>0) {
      emit daqMessage(buffer);
      emit newDataAvailable();
    }
}

void ThermoDAQModel::arduinoPresInfoChanged()
{
    NQLog("ThermoDAQModel", NQLog::Debug) << "arduinoPresInfoChanged()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("ThermoDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
    }

    QMutexLocker locker(&mutex_);

    QDateTime utime = currentTime();

    bool changed = false;
    changed |= updateIfChanged<float>(arduinoPressureA_, arduinoPresModel_->getPressureA());
    changed |= updateIfChanged<float>(arduinoPressureB_, arduinoPresModel_->getPressureB());

    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("ArduinoPressure");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("pA", QString::number(arduinoPressureA_, 'e', 6));
        xml.writeAttribute("pB", QString::number(arduinoPressureB_, 'e', 6));
        xml.writeEndElement();

        emit daqMessage(buffer);
	emit newDataAvailable();
    }
 }
