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

#include "ThermoDAQStreamReader.h"

ThermoDAQStreamReader::ThermoDAQStreamReader(const QStringList &parameters,
    const QString &filename,
    QObject* parent)
  : QObject(parent),
    parameters_(parameters),
    filename_(filename),
    measurementValid_(false)
{
    log_.message = "";
}

void ThermoDAQStreamReader::run()
{
    process();
    emit finished();
}

void ThermoDAQStreamReader::processHuberTemperature(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.bathTemperature = xml.attributes().value("bath").toString().toFloat();
    measurement_.workingTemperature = xml.attributes().value("work").toString().toFloat();
    measurement_.circulator = xml.attributes().value("circulator").toString().toInt();
}

void ThermoDAQStreamReader::processKeithleyState(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    int idx = xml.attributes().value("sensor").toString().toInt();
    measurement_.channelActive[idx] = xml.attributes().value("state").toString().toInt();
}

void ThermoDAQStreamReader::processKeithleyTemperature(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    int idx = xml.attributes().value("sensor").toString().toInt();
    measurement_.temperature[idx] = xml.attributes().value("temperature").toString().toFloat();
}

void ThermoDAQStreamReader::processPfeifferPressure(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.gaugeStatus1 = xml.attributes().value("s1").toString().toInt();
    measurement_.gaugePressure1 = xml.attributes().value("p1").toString().toFloat();
    measurement_.gaugeStatus2 = xml.attributes().value("s2").toString().toInt();
    measurement_.gaugePressure2 = xml.attributes().value("p2").toString().toFloat();
}

void ThermoDAQStreamReader::processHamegSetup(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.powerRemote = xml.attributes().value("remote").toString().toInt();
    measurement_.powerOn = xml.attributes().value("outputs").toString().toInt();
    measurement_.cv1 = xml.attributes().value("CV1").toString().toInt();
    measurement_.cv2 = xml.attributes().value("CV2").toString().toInt();
}

void ThermoDAQStreamReader::processHamegSetvalues(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.setVoltage1 = xml.attributes().value("V1").toString().toFloat();
    measurement_.setCurrent1 = xml.attributes().value("C1").toString().toFloat();
    measurement_.setVoltage2 = xml.attributes().value("V2").toString().toFloat();
    measurement_.setCurrent2 = xml.attributes().value("C2").toString().toFloat();
}

void ThermoDAQStreamReader::processHamegValues(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.voltage1 = xml.attributes().value("V1").toString().toFloat();
    measurement_.current1 = xml.attributes().value("C1").toString().toFloat();
    measurement_.voltage2 = xml.attributes().value("V2").toString().toFloat();
    measurement_.current2 = xml.attributes().value("C2").toString().toFloat();
}

void ThermoDAQStreamReader::processIotaSetup(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.iotaPumpEnabled = xml.attributes().value("enabled").toString().toInt();
    measurement_.iotaSetPressure = xml.attributes().value("pressure").toString().toFloat();
    measurement_.iotaSetFlow = xml.attributes().value("flow").toString().toFloat();
}

void ThermoDAQStreamReader::processIotaValues(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.iotaActPressure = xml.attributes().value("pressure").toString().toFloat();
    measurement_.iotaActFlow = xml.attributes().value("flow").toString().toFloat();
}

void ThermoDAQStreamReader::processArduinoPressure(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.arduinoPressureA = xml.attributes().value("pA").toString().toFloat();
    measurement_.arduinoPressureB = xml.attributes().value("pB").toString().toFloat();
}

void ThermoDAQStreamReader::processLog(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::TextDate);
    log_.uTime = dt.toTime_t();
    log_.datime = TDatime(measurement_.uTime);

    log_.message = xml.readElementText(QXmlStreamReader::IncludeChildElements).toStdString();

    ologtree_->Fill();
}

void ThermoDAQStreamReader::processDAQStarted(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurementValid_ = true;
}

void ThermoDAQStreamReader::processLine(QString& line)
{
    QXmlStreamReader xml(line);
    while (!xml.atEnd()) {
        xml.readNextStartElement();
        if (xml.isStartElement()) {
            if (xml.name()=="HuberTemperature") {
                processHuberTemperature(xml);
            }
            if (xml.name()=="KeithleyState") {
                processKeithleyState(xml);
            }
            if (xml.name()=="KeithleyTemperature") {
                processKeithleyTemperature(xml);
            }
            if (xml.name()=="PfeifferPressure") {
                processPfeifferPressure(xml);
            }
            if (xml.name()=="HamegSetup") {
                processHamegSetup(xml);
            }
            if (xml.name()=="HamegSetvalues") {
                processHamegSetvalues(xml);
            }
            if (xml.name()=="HamegValues") {
                processHamegValues(xml);
            }
            if (xml.name()=="IotaSetup") {
                processIotaSetup(xml);
            }
            if (xml.name()=="IotaValues") {
                processIotaValues(xml);
            }
            if (xml.name()=="ArduinoPressure") {
                processArduinoPressure(xml);
            }
            if (xml.name()=="Log") {
                processLog(xml);
            }
            if (xml.name()=="DAQStarted") {
                processDAQStarted(xml);
            }
        }
    }

    if (measurementValid_) otree_->Fill();
}

void ThermoDAQStreamReader::process()
{
  /*
  for (auto p : parameters_) {

  }
  */

  QFile file(filename_);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QFileInfo fi(file);
    QString base = fi.baseName();
    std::string rootname = base.toStdString();
    rootname += ".root";

    ofile_ = new TFile(rootname.c_str(), "RECREATE");

    otree_ = new TTree("thermoDAQ", "thermoDAQ");

    otree_->Branch("uTime", &measurement_.uTime, "uTime/i");
    otree_->Branch("datime", &measurement_.datime, 1024, 2);

    otree_->Branch("bathTemperature", &measurement_.bathTemperature, "bathTemperature/F");
    otree_->Branch("workingTemperature", &measurement_.workingTemperature, "workingTemperature/F");
    otree_->Branch("circulator", &measurement_.circulator, "circulator/I");

    for (int i=0;i<10;++i) {
        char dummy1[20];
        char dummy2[20];

        sprintf(dummy1, "channelActive%d", i);
        sprintf(dummy2, "channelActive%d/I", i);
        otree_->Branch(dummy1, &measurement_.channelActive[i], dummy2);

        sprintf(dummy1, "temperature%d", i);
        sprintf(dummy2, "temperature%d/F", i);
        otree_->Branch(dummy1, &measurement_.temperature[i], dummy2);
    }

    otree_->Branch("gaugeStatus1", &measurement_.gaugeStatus1, "gaugeStatus1/I");
    otree_->Branch("gaugePressure1", &measurement_.gaugePressure1, "gaugePressure1/F");
    otree_->Branch("gaugeStatus2", &measurement_.gaugeStatus2, "gaugeStatus2/I");
    otree_->Branch("gaugePressure2", &measurement_.gaugePressure2, "gaugePressure2/F");

    otree_->Branch("powerRemote", &measurement_.powerRemote, "powerRemote/I");
    otree_->Branch("powerOn", &measurement_.powerOn, "powerOn/I");
    otree_->Branch("cv1", &measurement_.cv1, "cv1/I");
    otree_->Branch("cv2", &measurement_.cv2, "cv2/I");

    otree_->Branch("setVoltage1", &measurement_.setVoltage1, "setVoltage1/F");
    otree_->Branch("setCurrent1", &measurement_.setCurrent1, "setCurrent1/F");
    otree_->Branch("setVoltage2", &measurement_.setVoltage2, "setVoltage2/F");
    otree_->Branch("setCurrent2", &measurement_.setCurrent2, "setCurrent2/F");

    otree_->Branch("voltage1", &measurement_.voltage1, "voltage1/F");
    otree_->Branch("current1", &measurement_.current1, "current1/F");
    otree_->Branch("voltage2", &measurement_.voltage2, "voltage2/F");
    otree_->Branch("current2", &measurement_.current2, "current2/F");

    otree_->Branch("iotaPumpEnabled", &measurement_.iotaPumpEnabled, "iotaPumpEnabled/I");
    otree_->Branch("iotaSetPressure", &measurement_.iotaSetPressure, "iotaSetPressure/F");
    otree_->Branch("iotaSetFlow", &measurement_.iotaSetFlow, "iotaSetFlow/F");
    otree_->Branch("iotaActPressure", &measurement_.iotaActPressure, "iotaActPressure/F");
    otree_->Branch("iotaActFlow", &measurement_.iotaActFlow, "iotaActFlow/F");

    otree_->Branch("arduinoPressureA", &measurement_.arduinoPressureA, "arduinoPressureA/F");
    otree_->Branch("arduinoPressureB", &measurement_.arduinoPressureB, "arduinoPressureB/F");

    ologtree_ = new TTree("thermoLog", "thermoLog");
    ologtree_->Branch("uTime", &log_.uTime, "uTime/i");
    ologtree_->Branch("datime", &log_.datime, 1024, 2);
    ologtree_->Branch("message", &log_.message);

    QString line;
    QTextStream in(&file);
    while (!in.atEnd()) {
        line = in.readLine();
        processLine(line);
    }

    ofile_->Write();
    delete ofile_;
}
