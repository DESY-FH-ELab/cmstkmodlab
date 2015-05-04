#include <iostream>

#include <QApplication>
#include <QDateTime>
#include <QXmlStreamWriter>

#include <qwt_date.h>

#include <nqlogger.h>

#include "DefoDAQModel.h"

DefoDAQModel::DefoDAQModel(DefoConradModel* conradModel,
			   DefoJulaboModel* julaboModel,
			   KeithleyModel* keithleyModel,
			   QObject *parent)
  : QObject(parent),
    daqState_(false),
    conradModel_(conradModel),
    julaboModel_(julaboModel),
    keithleyModel_(keithleyModel),
    julaboState_(OFF),
    julaboCirculator_(false),
    julaboWorkingTemperature_(0.0),
    julaboBathTemperature_(0.0)
{
    for (int i=0;i<10;++i) {
      keithleySensorState_[i] = OFF;
      keithleyTemperature_[i] = 0;
    }
    for (int i=0;i<8;++i) {
      conradSwitchState_[i] = OFF;
    }

    connect(keithleyModel_, SIGNAL(sensorStateChanged(uint,State)),
            this, SLOT(keithleySensorStateChanged(uint,State)));
    connect(keithleyModel_, SIGNAL(temperatureChanged(uint,double)),
            this, SLOT(keithleyTemperatureChanged(uint,double)));

    connect(julaboModel_, SIGNAL(informationChanged()),
            this, SLOT(julaboInfoChanged()));

    connect(conradModel_, SIGNAL(switchStateChanged(DefoConradModel::DeviceSwitch, State)),
	    this, SLOT(conradSwitchStateChanged(DefoConradModel::DeviceSwitch, State)));
}

void DefoDAQModel::myMoveToThread(QThread *thread)
{
    julaboModel_->moveToThread(thread);
    keithleyModel_->moveToThread(thread);
    this->moveToThread(thread);
}

void DefoDAQModel::startMeasurement()
{
    daqState_ = true;
    emit daqStateChanged(true);

    QString buffer;
    createDAQStatusMessage(buffer);
    emit daqMessage(buffer);

    NQLog("defoDAQ") << "measurement started";
}

const Measurement_t& DefoDAQModel::getMeasurement()
{
  measurement_.dt = currentTime();
  measurement_.daqState = daqState();
  
  measurement_.chillerState = julaboState_;
  measurement_.circulator = julaboCirculator_;
  measurement_.workingTemperature = julaboWorkingTemperature_;
  measurement_.bathTemperature = julaboBathTemperature_;

  for (int i=0;i<10;++i) {
    measurement_.channelActive[i] = (int)keithleySensorState_[i];
    measurement_.temperature[i] = keithleyTemperature_[i];
  }

  for (int i=0;i<8;++i) {
    measurement_.conradState[i] = (int)conradSwitchState_[i];
  }
  
  return measurement_;
}

void DefoDAQModel::createDAQStatusMessage(QString &buffer)
{
    QDateTime& utime = currentTime();

    QXmlStreamWriter xml(&buffer);
    xml.setAutoFormatting(true);

    xml.writeStartElement("JulaboTemperature");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("state", QString::number(julaboModel_->getDeviceState()));
    xml.writeAttribute("circulator", julaboModel_->isCirculatorEnabled()==true ? "1" : "0");
    xml.writeAttribute("work", QString::number(julaboModel_->getWorkingTemperatureParameter().getValue(), 'f', 6));
    xml.writeAttribute("bath", QString::number(julaboModel_->getBathTemperature(), 'f', 6));
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

    for (int i=0;i<8;++i) {
      xml.writeStartElement("ConradSwitchState");
      xml.writeAttribute("time", utime.toString(Qt::ISODate));
      xml.writeAttribute("switch", QString::number(i));
      xml.writeAttribute("state", QString::number(conradModel_->getSwitchState(i)));
      xml.writeEndElement();
    }

    xml.writeStartElement("DAQStarted");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeEndElement();
}

void DefoDAQModel::stopMeasurement()
{
    daqState_ = false;
    emit daqStateChanged(false);

    NQLog("defoDAQ") << "measurement stopped";
}

void DefoDAQModel::clearHistory()
{

}

QDateTime& DefoDAQModel::currentTime()
{
    QDateTime ct = QDateTime::currentDateTime();
    if (currentTime_.secsTo(ct)>2) currentTime_ = ct;
    return currentTime_;
}

void DefoDAQModel::customDAQMessage(const QString & message)
{
    emit daqMessage(message);
}

void DefoDAQModel::julaboInfoChanged()
{
    NQLog("DefoDAQModel", NQLog::Debug) << "julaboInfoChanged()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("DefoDAQModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("DefoDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
    }

    QMutexLocker locker(&mutex_);

    QDateTime utime = currentTime();

    bool changed = false;

    changed |= updateIfChanged<State>(julaboState_,
				      julaboModel_->getDeviceState());
    changed |= updateIfChanged<float>(julaboWorkingTemperature_,
                                      julaboModel_->getWorkingTemperatureParameter().getValue());
    changed |= updateIfChanged<float>(julaboBathTemperature_, julaboModel_->getBathTemperature());
    changed |= updateIfChanged<bool>(julaboCirculator_, julaboModel_->isCirculatorEnabled());

    if (changed) {
        QString buffer;
        QXmlStreamWriter xml(&buffer);

        xml.writeStartElement("JulaboTemperature");
        xml.writeAttribute("time", utime.toString(Qt::ISODate));
        xml.writeAttribute("state", QString::number(julaboState_));
        xml.writeAttribute("circulator", julaboCirculator_==true ? "1" : "0");
        xml.writeAttribute("work", QString::number(julaboWorkingTemperature_, 'f', 6));
        xml.writeAttribute("bath", QString::number(julaboBathTemperature_, 'f', 6));
        xml.writeEndElement();

        emit daqMessage(buffer);
	emit newDataAvailable();
    }
}

void DefoDAQModel::keithleySensorStateChanged(unsigned int sensor, State newState)
{
    NQLog("DefoDAQModel", NQLog::Message) << "keithleySensorStateChanged()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("DefoDAQModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("DefoDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
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

void DefoDAQModel::keithleyTemperatureChanged(unsigned int sensor, double temperature)
{
    NQLog("DefoDAQModel", NQLog::Debug) << "keithleyInfoChanged()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("DefoDAQModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("DefoDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
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

void DefoDAQModel::conradSwitchStateChanged(DefoConradModel::DeviceSwitch device, State newState)
{
  NQLog("DefoDAQModel", NQLog::Debug) << "conradSwitchStateChanged()";

  if (thread()==QApplication::instance()->thread()) {
    NQLog("DefoDAQModel", NQLog::Debug) << " running in main application thread";
  } else {
    NQLog("DefoDAQModel", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  QMutexLocker locker(&mutex_);
  
  QDateTime& utime = currentTime();
  
  bool changed = updateIfChanged<State>(conradSwitchState_[device], newState);
  
  if (changed) {
    QString buffer;
    QXmlStreamWriter xml(&buffer);
    
    xml.writeStartElement("ConradSwitchState");
    xml.writeAttribute("time", utime.toString(Qt::ISODate));
    xml.writeAttribute("switch", QString::number(device));
    xml.writeAttribute("state", QString::number(newState));
    xml.writeEndElement();
    
    emit daqMessage(buffer);
    emit newDataAvailable();
  }
}
