#include <iostream>

#include <QApplication>
#include <QDateTime>
#include <QXmlStreamWriter>

#include <qwt_date.h>

#include <nqlogger.h>

#include "MicroDAQModel.h"

MicroDAQModel::MicroDAQModel(IotaModel* iotaModel,
                             ArduinoPresModel* arduinoPresModel,
                             CoriFlowModel* coriModel,
                             QObject *parent) :
    QObject(),
    daqState_(false),
    iotaModel_(iotaModel),
    arduinoPresModel_(arduinoPresModel),
    coriModel_(coriModel),
    iotaPumpEnabled_(false),
    iotaActPressure_(0.0),
    iotaSetPressure_(0.0),
    iotaActFlow_(0.0),
    iotaSetFlow_(0.0),
    arduinoPressureA_(0.0),
    arduinoPressureB_(0.0),
    coriTemp_(0.0),
    coriMeasure_(0.0)
{
    connect(iotaModel_, SIGNAL(informationChanged()),
            this, SLOT(iotaInfoChanged()));

    connect(arduinoPresModel_, SIGNAL(informationChanged()),
            this, SLOT(arduinoPresInfoChanged()));

    connect(coriModel_, SIGNAL(informationChanged()),
            this, SLOT(coriInfoChanged()));
}

void MicroDAQModel::myMoveToThread(QThread *thread)
{
    iotaModel_->moveToThread(thread);
    arduinoPresModel_->moveToThread(thread);
    coriModel_->moveToThread(thread);
    this->moveToThread(thread);
}

void MicroDAQModel::startMeasurement()
{
    daqState_ = true;
    emit daqStateChanged(true);

    QString buffer;
    createDAQStatusMessage(buffer);
    emit daqMessage(buffer);

    NQLog("MicroDAQ") << "measurement started";
}

void MicroDAQModel::createDAQStatusMessage(QString &buffer)
{
    QDateTime& utime = currentTime();

    QXmlStreamWriter xml(&buffer);
    xml.setAutoFormatting(true);

    xml.writeStartElement("IotaSetup");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("enabled", iotaPumpEnabled_==true ? "1" : "0");
    xml.writeAttribute("pressure", QString::number(iotaSetPressure_, 'e', 3));
    xml.writeAttribute("flow", QString::number(iotaSetFlow_, 'e', 3));
    xml.writeEndElement();

    xml.writeStartElement("IotaValues");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("pressure", QString::number(iotaActPressure_, 'e', 3));
    xml.writeAttribute("flow", QString::number(iotaActFlow_, 'e', 3));
    xml.writeEndElement();

    xml.writeStartElement("ArduinoPressure");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("pA", QString::number(arduinoPressureA_, 'e', 3));
    xml.writeAttribute("pB", QString::number(arduinoPressureB_, 'e', 3));
    xml.writeEndElement();


    xml.writeStartElement("CoriFlow");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("Temp", QString::number(coriTemp_, 'e', 3));
    xml.writeAttribute("Flow", QString::number(coriMeasure_, 'e', 3));
    xml.writeEndElement();

    xml.writeStartElement("DAQStarted");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeEndElement();
}

void MicroDAQModel::stopMeasurement()
{
    daqState_ = false;
    emit daqStateChanged(false);

    NQLog("MicroDAQ") << "measurement stopped";
}

void MicroDAQModel::clearHistory()
{

}

QDateTime& MicroDAQModel::currentTime()
{
    QDateTime ct = QDateTime::currentDateTime();
    if (currentTime_.secsTo(ct)>2) currentTime_ = ct;
    return currentTime_;
}

void MicroDAQModel::customDAQMessage(const QString & message)
{
    emit daqMessage(message);
}

void MicroDAQModel::iotaInfoChanged()
{
    NQLog("MicroDAQModel", NQLog::Debug) << "iotaInfoChanged()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("MicroDAQModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("MicroDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
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
        xml.writeAttribute("pressure", QString::number(iotaSetPressure_, 'e', 3));
        xml.writeAttribute("flow", QString::number(iotaSetFlow_, 'e', 3));
        xml.writeEndElement();
    }

    changed = false;
    changed |= updateIfChanged<float>(iotaActPressure_, iotaModel_->getActPressure());
    changed |= updateIfChanged<float>(iotaActFlow_, iotaModel_->getActFlow());
    if (changed) {
        xml.writeStartElement("IotaValues");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("pressure", QString::number(iotaActPressure_, 'e', 3));
        xml.writeAttribute("flow", QString::number(iotaActFlow_, 'e', 3));
        xml.writeEndElement();
    }
    
    if (buffer.length()>0) {
        emit daqMessage(buffer);
        emit newDataAvailable();
    }
}

void MicroDAQModel::arduinoPresInfoChanged()
{
    NQLog("MicroDAQModel", NQLog::Debug) << "arduinoPresInfoChanged()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("MicroDAQModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("MicroDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
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
        xml.writeAttribute("pA", QString::number(arduinoPressureA_, 'e', 3));
        xml.writeAttribute("pB", QString::number(arduinoPressureB_, 'e', 3));
        xml.writeEndElement();

        emit daqMessage(buffer);
        emit newDataAvailable();
    }
}

void MicroDAQModel::coriInfoChanged()
{
    NQLog("MicroDAQModel", NQLog::Debug) << "coriInfoChanged()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("MicroDAQModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("MicroDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
    }

    QMutexLocker locker(&mutex_);

    QDateTime utime = currentTime();

    bool changed = false;
    changed |= updateIfChanged<float>(coriTemp_, coriModel_->getTemp());
    changed |= updateIfChanged<float>(coriMeasure_, coriModel_->getMeasure());

    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("CoriFlow");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("Temp", QString::number(coriTemp_, 'e', 3));
        xml.writeAttribute("Flow", QString::number(coriMeasure_, 'e', 3));
        xml.writeEndElement();

        emit daqMessage(buffer);
        emit newDataAvailable();
    }
}
