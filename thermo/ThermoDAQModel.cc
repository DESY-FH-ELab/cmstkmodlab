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
    std::cout << "----> ThermoDAQModel::pfeifferInfoChanged()" << std::endl;

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
