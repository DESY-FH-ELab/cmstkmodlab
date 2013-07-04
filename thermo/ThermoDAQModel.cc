#include <iostream>

#include <QDateTime>
#include <QXmlStreamWriter>

#include <qwt_date.h>

#include "ThermoDAQModel.h"

ThermoDAQModel::ThermoDAQModel(JulaboModel* julaboModel,
                               KeithleyModel* keithleyModel,
                               HamegModel* hamegModel,
                               PfeifferModel* pfeifferModel,
                               QObject *parent) :
    QObject(parent),
    julaboModel_(julaboModel),
    keithleyModel_(keithleyModel),
    hamegModel_(hamegModel),
    pfeifferModel_(pfeifferModel)
{
    connect(julaboModel_, SIGNAL(informationChanged()),
            this, SLOT(julaboInfoChanged()));

    connect(keithleyModel_, SIGNAL(sensorStateChanged(uint,State)),
            this, SLOT(keithleySensorStateChanged(uint,State)));
    connect(keithleyModel_, SIGNAL(temperatureChanged(uint,double)),
            this, SLOT(keithleyTemperatureChanged(uint,double)));

    connect(pfeifferModel_, SIGNAL(informationChanged()),
            this, SLOT(pfeifferInfoChanged()));

    connect(hamegModel_, SIGNAL(informationChanged()),
            this, SLOT(hamegInfoChanged()));
}

void ThermoDAQModel::startMeasurement()
{
    emit daqStateChanged(true);

    QDateTime& utime = currentTime();

    QString buffer;
    QXmlStreamWriter xml(&buffer);
    xml.setAutoFormatting(true);

    xml.writeStartElement("JulaboTemperature");
    xml.writeAttribute("time", utime.toString());
    xml.writeAttribute("work", QString::number(julaboModel_->getWorkingTemperatureParameter().getValue(), 'f', 2));
    xml.writeAttribute("bath", QString::number(julaboModel_->getBathTemperature(), 'f', 2));
    xml.writeEndElement();

    for (int sensor=0;sensor<10;++sensor) {
        xml.writeStartElement("KeithleyState");
        xml.writeAttribute("time", utime.toString());
        xml.writeAttribute("sensor", QString::number(sensor));
        xml.writeAttribute("state", QString::number(keithleyModel_->getSensorState(sensor)));
        xml.writeEndElement();
    }
    for (int sensor=0;sensor<10;++sensor) {
        xml.writeStartElement("KeithleyTemperature");
        xml.writeAttribute("time", utime.toString());
        xml.writeAttribute("sensor", QString::number(sensor));
        xml.writeAttribute("temperature", QString::number(keithleyModel_->getTemperature(sensor), 'f', 2));
        xml.writeEndElement();
    }

    xml.writeStartElement("PfeifferPressure");
    xml.writeAttribute("time", utime.toString());
    xml.writeAttribute("p1", QString::number(pfeifferModel_->getPressure1(), 'e', 2));
    xml.writeAttribute("p2", QString::number(pfeifferModel_->getPressure2(), 'e', 2));
    xml.writeEndElement();

    xml.writeStartElement("HamegSetup");
    xml.writeAttribute("time", utime.toString());
    xml.writeAttribute("remote", hamegRemoteMode_==true ? "1" : "0");
    xml.writeAttribute("outputs", hamegOutputsEnabled_==true ? "1" : "0");
    xml.writeAttribute("CV1", hamegCVMode_[0]==true ? "1" : "0");
    xml.writeAttribute("CV2", hamegCVMode_[1]==true ? "1" : "0");
    xml.writeEndElement();

    xml.writeStartElement("HamegSetvalues");
    xml.writeAttribute("time", utime.toString());
    xml.writeAttribute("V1", QString::number(hamegSetVoltage_[0]));
    xml.writeAttribute("C1", QString::number(hamegSetCurrent_[0]));
    xml.writeAttribute("V2", QString::number(hamegSetVoltage_[1]));
    xml.writeAttribute("C2", QString::number(hamegSetCurrent_[1]));
    xml.writeEndElement();

    xml.writeStartElement("HamegValues");
    xml.writeAttribute("time", utime.toString());
    xml.writeAttribute("V1", QString::number(hamegVoltage_[0]));
    xml.writeAttribute("C1", QString::number(hamegCurrent_[0]));
    xml.writeAttribute("V2", QString::number(hamegVoltage_[1]));
    xml.writeAttribute("C2", QString::number(hamegCurrent_[1]));
    xml.writeEndElement();

    emit daqMessage(buffer);
}

void ThermoDAQModel::stopMeasurement()
{
    emit daqStateChanged(false);
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

void ThermoDAQModel::julaboInfoChanged()
{
    std::cout << "ThermoDAQModel::julaboInfoChanged()" << std::endl;

    QDateTime utime = currentTime();

    bool changed = false;

    changed |= julaboBathTemperature_.push(utime,
                                          julaboModel_->getWorkingTemperatureParameter().getValue());
    changed |= julaboBathTemperature_.push(utime,
                                           julaboModel_->getBathTemperature());


    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("JulaboTemperature");
        xml.writeAttribute("time", utime.toString());
        xml.writeAttribute("work", QString::number(julaboModel_->getWorkingTemperatureParameter().getValue(), 'f', 2));
        xml.writeAttribute("bath", QString::number(julaboModel_->getBathTemperature(), 'f', 2));
        xml.writeEndElement();

        emit daqMessage(buffer);
    }
    emit newDataAvailable();
}

void ThermoDAQModel::keithleySensorStateChanged(unsigned int sensor, State newState)
{
    std::cout << "ThermoDAQModel::keithleySensorStateChanged(unsigned int sensor, State newState)" << std::endl;

    QDateTime& utime = currentTime();

    bool changed = keithleySensorState_[sensor].pushIfChanged(utime, newState);

    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("KeithleyState");
        xml.writeAttribute("time", utime.toString());
        xml.writeAttribute("sensor", QString::number(sensor));
        xml.writeAttribute("state", QString::number(newState));
        xml.writeEndElement();

        emit daqMessage(buffer);
    }
}

void ThermoDAQModel::keithleyTemperatureChanged(unsigned int sensor, double temperature)
{
    std::cout << "ThermoDAQModel::keithleyInfoChanged(unsigned int sensor, double temperature)" << std::endl;

    QDateTime& utime = currentTime();

    bool changed = keithleyTemperature_[sensor].pushIfChanged(utime, temperature);

    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("KeithleyTemperature");
        xml.writeAttribute("time", utime.toString());
        xml.writeAttribute("sensor", QString::number(sensor));
        xml.writeAttribute("temperature", QString::number(temperature, 'f', 2));
        xml.writeEndElement();

        emit daqMessage(buffer);
        emit newDataAvailable();
    }
}

void ThermoDAQModel::pfeifferInfoChanged()
{
    QDateTime utime = currentTime();

    bool changed = false;

    changed |= pfeifferPressure1_.push(utime,
                                       pfeifferModel_->getPressure1());
    changed |= pfeifferPressure2_.push(utime,
                                       pfeifferModel_->getPressure2());

    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("PfeifferPressure");
        xml.writeAttribute("time", utime.toString());
        xml.writeAttribute("p1", QString::number(pfeifferModel_->getPressure1(), 'e', 2));
        xml.writeAttribute("p2", QString::number(pfeifferModel_->getPressure2(), 'e', 2));
        xml.writeEndElement();

        emit daqMessage(buffer);
    }

    emit newDataAvailable();
}

void ThermoDAQModel::hamegInfoChanged()
{
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
        xml.writeAttribute("time", utime.toString());
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
        xml.writeAttribute("time", utime.toString());
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
        xml.writeAttribute("time", utime.toString());
        xml.writeAttribute("V1", QString::number(hamegVoltage_[0]));
        xml.writeAttribute("C1", QString::number(hamegCurrent_[0]));
        xml.writeAttribute("V2", QString::number(hamegVoltage_[1]));
        xml.writeAttribute("C2", QString::number(hamegCurrent_[1]));
        xml.writeEndElement();
    }

    if (buffer.length()>0) emit daqMessage(buffer);
}
