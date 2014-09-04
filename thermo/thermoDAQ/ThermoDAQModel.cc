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
                               QObject *parent) :
    QObject(),
    daqState_(false),
    huberModel_(huberModel),
    keithleyModel_(keithleyModel),
    hamegModel_(hamegModel),
    pfeifferModel_(pfeifferModel)
{
    connect(huberModel_, SIGNAL(informationChanged()),
            this, SLOT(huberInfoChanged()));

    connect(keithleyModel_, SIGNAL(sensorStateChanged(uint,State)),
            this, SLOT(keithleySensorStateChanged(uint,State)));
    connect(keithleyModel_, SIGNAL(temperatureChanged(uint,double)),
            this, SLOT(keithleyTemperatureChanged(uint,double)));

    connect(pfeifferModel_, SIGNAL(informationChanged()),
            this, SLOT(pfeifferInfoChanged()));

    connect(hamegModel_, SIGNAL(informationChanged()),
            this, SLOT(hamegInfoChanged()));
}

void ThermoDAQModel::myMoveToThread(QThread *thread)
{
    huberModel_->moveToThread(thread);
    keithleyModel_->moveToThread(thread);
    pfeifferModel_->moveToThread(thread);
    hamegModel_->moveToThread(thread);
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

void ThermoDAQModel::createDAQStatusMessage(QString &buffer)
{
    QDateTime& utime = currentTime();

    QXmlStreamWriter xml(&buffer);
    xml.setAutoFormatting(true);

    xml.writeStartElement("HuberTemperature");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("circulator", huberModel_->isCirculatorEnabled()==true ? "1" : "0");
    xml.writeAttribute("work", QString::number(huberModel_->getWorkingTemperatureParameter().getValue(), 'f', 2));
    xml.writeAttribute("bath", QString::number(huberModel_->getBathTemperature(), 'f', 2));
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
        xml.writeAttribute("temperature", QString::number(keithleyModel_->getTemperature(sensor), 'f', 2));
        xml.writeEndElement();
    }

    xml.writeStartElement("PfeifferPressure");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("s1", QString::number((int)pfeifferModel_->getStatus1()));
    xml.writeAttribute("p1", QString::number(pfeifferModel_->getPressure1(), 'e', 2));
    xml.writeAttribute("s2", QString::number((int)pfeifferModel_->getStatus2()));
    xml.writeAttribute("p2", QString::number(pfeifferModel_->getPressure2(), 'e', 2));
    xml.writeEndElement();

    xml.writeStartElement("HamegSetup");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("remote", hamegRemoteMode_==true ? "1" : "0");
    xml.writeAttribute("outputs", hamegOutputsEnabled_==true ? "1" : "0");
    xml.writeAttribute("CV1", hamegCVMode_[0]==true ? "1" : "0");
    xml.writeAttribute("CV2", hamegCVMode_[1]==true ? "1" : "0");
    xml.writeEndElement();

    xml.writeStartElement("HamegSetvalues");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("V1", QString::number(hamegSetVoltage_[0]));
    xml.writeAttribute("C1", QString::number(hamegSetCurrent_[0]));
    xml.writeAttribute("V2", QString::number(hamegSetVoltage_[1]));
    xml.writeAttribute("C2", QString::number(hamegSetCurrent_[1]));
    xml.writeEndElement();

    xml.writeStartElement("HamegValues");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("V1", QString::number(hamegVoltage_[0]));
    xml.writeAttribute("C1", QString::number(hamegCurrent_[0]));
    xml.writeAttribute("V2", QString::number(hamegVoltage_[1]));
    xml.writeAttribute("C2", QString::number(hamegCurrent_[1]));
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
    std::cout << "ThermoDAQModel::huberInfoChanged()";
    if (thread()==QApplication::instance()->thread()) {
        std::cout << " running in main application thread" << std::endl;
    } else {
        std::cout << " running in dedicated DAQ thread" << std::endl;
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
        xml.writeAttribute("work", QString::number(huberWorkingTemperature_, 'f', 2));
        xml.writeAttribute("bath", QString::number(huberBathTemperature_, 'f', 2));
        xml.writeEndElement();

        emit daqMessage(buffer);
    }
    emit newDataAvailable();
}

void ThermoDAQModel::keithleySensorStateChanged(unsigned int sensor, State newState)
{
    std::cout << "ThermoDAQModel::keithleySensorStateChanged()";
    if (thread()==QApplication::instance()->thread()) {
        std::cout << " running in main application thread" << std::endl;
    } else {
        std::cout << " running in dedicated DAQ thread" << std::endl;
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
    std::cout << "ThermoDAQModel::keithleyInfoChanged()";
    if (thread()==QApplication::instance()->thread()) {
        std::cout << " running in main application thread" << std::endl;
    } else {
        std::cout << " running in dedicated DAQ thread" << std::endl;
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
        xml.writeAttribute("temperature", QString::number(temperature, 'f', 2));
        xml.writeEndElement();

        emit daqMessage(buffer);
        emit newDataAvailable();
    }
}

void ThermoDAQModel::pfeifferInfoChanged()
{
    std::cout << "ThermoDAQModel::pfeifferInfoChanged()";
    if (thread()==QApplication::instance()->thread()) {
        std::cout << " running in main application thread" << std::endl;
    } else {
        std::cout << " running in dedicated DAQ thread" << std::endl;
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
        xml.writeAttribute("p1", QString::number(pfeifferPressure1_, 'e', 3));
        xml.writeAttribute("s2", QString::number(pfeifferStatus2_));
        xml.writeAttribute("p2", QString::number(pfeifferPressure2_, 'e', 3));
        xml.writeEndElement();

        emit daqMessage(buffer);
    }

    emit newDataAvailable();
}

void ThermoDAQModel::hamegInfoChanged()
{
    std::cout << "ThermoDAQModel::hamegInfoChanged()";
    if (thread()==QApplication::instance()->thread()) {
        std::cout << " running in main application thread" << std::endl;
    } else {
        std::cout << " running in dedicated DAQ thread" << std::endl;
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
        xml.writeAttribute("V1", QString::number(hamegSetVoltage_[0]));
        xml.writeAttribute("C1", QString::number(hamegSetCurrent_[0]));
        xml.writeAttribute("V2", QString::number(hamegSetVoltage_[1]));
        xml.writeAttribute("C2", QString::number(hamegSetCurrent_[1]));
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
        xml.writeAttribute("V1", QString::number(hamegVoltage_[0]));
        xml.writeAttribute("C1", QString::number(hamegCurrent_[0]));
        xml.writeAttribute("V2", QString::number(hamegVoltage_[1]));
        xml.writeAttribute("C2", QString::number(hamegCurrent_[1]));
        xml.writeEndElement();
    }

    if (buffer.length()>0) emit daqMessage(buffer);
}
