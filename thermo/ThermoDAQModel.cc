#include <iostream>

#include <QDateTime>
#include <QXmlStreamWriter>

#include "ThermoDAQModel.h"

ThermoDAQModel::ThermoDAQModel(JulaboModel* julaboModel,
                               KeithleyModel* keithleyModel,
                               HamegModel* hamegModel,
                               PfeifferModel* pfeifferModel,
                               QObject *parent) :
    julaboModel_(julaboModel),
    keithleyModel_(keithleyModel),
    hamegModel_(hamegModel),
    pfeifferModel_(pfeifferModel),
    currentTime_(0)
{
    connect(julaboModel_, SIGNAL(informationChanged()),
            this, SLOT(julaboInfoChanged()));

    connect(keithleyModel_, SIGNAL(sensorStateChanged(uint,State)),
            this, SLOT(keithleySensorStateChanged(uint,State)));
    connect(keithleyModel_, SIGNAL(temperatureChanged(uint,double)),
            this, SLOT(keithleyTemperatureChanged(uint,double)));
}

unsigned int ThermoDAQModel::currentTime()
{
    unsigned int ct = QDateTime::currentDateTimeUtc().toTime_t();
    if (ct-currentTime_>2) currentTime_ = ct;
    return currentTime_;
}

void ThermoDAQModel::julaboInfoChanged()
{
    std::cout << "ThermoDAQModel::julaboInfoChanged()" << std::endl;

    unsigned int utime = currentTime();

    bool changed = false;

    changed |= julaboBathTemperature_.pushIfChanged(utime,
                                                    julaboModel_->getWorkingTemperatureParameter().getValue());
    changed |= julaboBathTemperature_.pushIfChanged(utime,
                                                    julaboModel_->getBathTemperature());


    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("JulaboTemperature");
        xml.writeAttribute("time", QString::number(utime));
        xml.writeAttribute("work", QString::number(julaboModel_->getWorkingTemperatureParameter().getValue()));
        xml.writeAttribute("bath", QString::number(julaboModel_->getBathTemperature()));
        xml.writeEndElement();

        emit daqMessage(buffer);
    }
}

void ThermoDAQModel::keithleySensorStateChanged(unsigned int sensor, State newState)
{
    std::cout << "ThermoDAQModel::keithleySensorStateChanged(unsigned int sensor, State newState)" << std::endl;

    unsigned int utime = currentTime();

    bool changed = keithleySensorState_[sensor].pushIfChanged(utime, newState);

    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("KeithleyState");
        xml.writeAttribute("time", QString::number(utime));
        xml.writeAttribute("sensor", QString::number(sensor));
        xml.writeAttribute("state", QString::number(newState));
        xml.writeEndElement();

        emit daqMessage(buffer);
    }
}

void ThermoDAQModel::keithleyTemperatureChanged(unsigned int sensor, double temperature)
{
    std::cout << "ThermoDAQModel::keithleyInfoChanged(unsigned int sensor, double temperature)" << std::endl;

    unsigned int utime = currentTime();

    bool changed = keithleyTemperature_[sensor].pushIfChanged(utime, temperature);

    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("KeithleyTemperature");
        xml.writeAttribute("time", QString::number(utime));
        xml.writeAttribute("sensor", QString::number(sensor));
        xml.writeAttribute("temperature", QString::number(temperature));
        xml.writeEndElement();

        emit daqMessage(buffer);
    }
}
