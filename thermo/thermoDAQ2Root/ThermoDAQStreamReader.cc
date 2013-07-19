#include <iostream>

#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include "ThermoDAQStreamReader.h"

ThermoDAQStreamReader::ThermoDAQStreamReader(QStringList arguments,
                                             QObject* parent) :
    QObject(parent),
    arguments_(arguments),
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

    measurement_.bathTemperature = xml.attributes().value("bath").toString().toFloat();
    measurement_.workingTemperature = xml.attributes().value("work").toString().toFloat();
    measurement_.circulator = xml.attributes().value("circulator").toString().toInt();
}

void ThermoDAQStreamReader::processKeithleyState(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();

    int idx = xml.attributes().value("sensor").toString().toInt();
    measurement_.channelActive[idx] = xml.attributes().value("state").toString().toInt();
}

void ThermoDAQStreamReader::processKeithleyTemperature(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();

    int idx = xml.attributes().value("sensor").toString().toInt();
    measurement_.temperature[idx] = xml.attributes().value("temperature").toString().toFloat();
}

void ThermoDAQStreamReader::processPfeifferPressure(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();

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

    measurement_.voltage1 = xml.attributes().value("V1").toString().toFloat();
    measurement_.current1 = xml.attributes().value("C1").toString().toFloat();
    measurement_.voltage2 = xml.attributes().value("V2").toString().toFloat();
    measurement_.current2 = xml.attributes().value("C2").toString().toFloat();
}

void ThermoDAQStreamReader::processLog(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    log_.uTime = dt.toTime_t();
    log_.message = xml.text().toString().toStdString();

    ologtree_->Fill();
}

void ThermoDAQStreamReader::processDAQStarted(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    measurement_.uTime = dt.toTime_t();

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
    if (arguments_.size()!=2) return;

    QFile file(arguments_.at(1));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QFileInfo fi(file);
    QString base = fi.baseName();
    std::string rootname = base.toStdString();
    rootname += ".root";

    ofile_ = new TFile(rootname.c_str(), "RECREATE");

    otree_ = new TTree("thermoDAQ", "thermoDAQ");

    otree_->Branch("uTime", &measurement_.uTime, "uTime/I");

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

    ologtree_ = new TTree("thermoLog", "thermoLog");
    ologtree_->Branch("uTime", &log_.uTime, "uTime/I");
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
