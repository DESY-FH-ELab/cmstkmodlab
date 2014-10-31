#include <iostream>
#include <cmath>

#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <TDatime.h>
#include <TMultiGraph.h>
#include <TH1F.h>
#include <TCanvas.h>

#include "ThermoDAQStreamReader.h"

ThermoDAQStreamReader::ThermoDAQStreamReader(QStringList arguments,
                                             QObject* parent) :
    QObject(parent),
    arguments_(arguments)
{
    for (int i=0;i<10;++i) activeT[i] = 0;
    
    minUTime = 0;
    maxUTime = 0;
    minT = 1000;
    maxT = -1000;
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

    if (minUTime==0) {
      minUTime = dt.toTime_t();
      offset.Set(minUTime);
    }
    uint utime = dt.toTime_t()-minUTime;
    maxUTime = std::max(maxUTime, utime);

    /*
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.bathTemperature = xml.attributes().value("bath").toString().toFloat();
    measurement_.workingTemperature = xml.attributes().value("work").toString().toFloat();
    measurement_.circulator = xml.attributes().value("circulator").toString().toInt();
    */
}

void ThermoDAQStreamReader::processKeithleyState(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    int idx = xml.attributes().value("sensor").toString().toInt();

    if (minUTime==0) {
      minUTime = dt.toTime_t();
      offset.Set(minUTime);
    }
    uint utime = dt.toTime_t()-minUTime;
    maxUTime = std::max(maxUTime, utime);

    activeT[idx] = xml.attributes().value("state").toString().toInt();
    
    if (activeT[idx]==1) {
        graphT[idx] = new TGraph();
        graphsT[idx].push_back(graphT[idx]);
        graphTn[idx] = 0;
    }
}

void ThermoDAQStreamReader::processKeithleyTemperature(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);
    int idx = xml.attributes().value("sensor").toString().toInt();

    if (minUTime==0) {
      minUTime = dt.toTime_t();
      offset.Set(minUTime);
    }
    uint utime = dt.toTime_t()-minUTime;
    maxUTime = std::max(maxUTime, utime);

    if (activeT[idx]==1) {
       float t = xml.attributes().value("temperature").toString().toFloat();
       minT = std::min(minT, t);
       maxT = std::max(maxT, t);
       graphT[idx]->SetPoint(graphTn[idx]++, utime, t);
    }
}

void ThermoDAQStreamReader::processPfeifferPressure(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);

    /*
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.gaugeStatus1 = xml.attributes().value("s1").toString().toInt();
    measurement_.gaugePressure1 = xml.attributes().value("p1").toString().toFloat();
    measurement_.gaugeStatus2 = xml.attributes().value("s2").toString().toInt();
    measurement_.gaugePressure2 = xml.attributes().value("p2").toString().toFloat();
    */
}

void ThermoDAQStreamReader::processHamegSetup(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);

    /*
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.powerRemote = xml.attributes().value("remote").toString().toInt();
    measurement_.powerOn = xml.attributes().value("outputs").toString().toInt();
    measurement_.cv1 = xml.attributes().value("CV1").toString().toInt();
    measurement_.cv2 = xml.attributes().value("CV2").toString().toInt();
    */
}

void ThermoDAQStreamReader::processHamegSetvalues(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);

    /*
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.setVoltage1 = xml.attributes().value("V1").toString().toFloat();
    measurement_.setCurrent1 = xml.attributes().value("C1").toString().toFloat();
    measurement_.setVoltage2 = xml.attributes().value("V2").toString().toFloat();
    measurement_.setCurrent2 = xml.attributes().value("C2").toString().toFloat();
    */
}

void ThermoDAQStreamReader::processHamegValues(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);

    /*
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.voltage1 = xml.attributes().value("V1").toString().toFloat();
    measurement_.current1 = xml.attributes().value("C1").toString().toFloat();
    measurement_.voltage2 = xml.attributes().value("V2").toString().toFloat();
    measurement_.current2 = xml.attributes().value("C2").toString().toFloat();
    */
}

void ThermoDAQStreamReader::processIotaSetup(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);

    /*
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.iotaPumpEnabled = xml.attributes().value("enabled").toString().toInt();
    measurement_.iotaSetPressure = xml.attributes().value("pressure").toString().toFloat();
    measurement_.iotaSetFlow = xml.attributes().value("flow").toString().toFloat();
    */
}

void ThermoDAQStreamReader::processIotaValues(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);

    /*
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.iotaActPressure = xml.attributes().value("pressure").toString().toFloat();
    measurement_.iotaActFlow = xml.attributes().value("flow").toString().toFloat();
    */
}

void ThermoDAQStreamReader::processArduinoPressure(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);

    /*
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurement_.arduinoPressureA = xml.attributes().value("pA").toString().toFloat();
    measurement_.arduinoPressureB = xml.attributes().value("pB").toString().toFloat();
    */
}

void ThermoDAQStreamReader::processLog(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::TextDate);

    /*
    log_.uTime = dt.toTime_t();
    log_.datime = TDatime(measurement_.uTime);

    log_.message = xml.readElementText(QXmlStreamReader::IncludeChildElements).toStdString();

    ologtree_->Fill();
    */
}

void ThermoDAQStreamReader::processDAQStarted(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::ISODate);

    /*
    measurement_.uTime = dt.toTime_t();
    measurement_.datime = TDatime(measurement_.uTime);

    measurementValid_ = true;
    */
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
}

void ThermoDAQStreamReader::process()
{
    if (arguments_.size()!=3) return;

    QFile file(arguments_.at(1));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QString line;
    QTextStream in(&file);
    while (!in.atEnd()) {
        line = in.readLine();
        processLine(line);
    }
    
    TMultiGraph * mgr = new TMultiGraph();
    for (int i=0;i<10;++i) {
       for (std::vector<TGraph*>::iterator it = graphsT[i].begin();
            it!=graphsT[i].end();
            ++it) {
            mgr->Add(*it);
        }
    }
    
    TCanvas * c = new TCanvas("c", "c", 700, 500);
    TH1F* frame = c->DrawFrame(0, minT-1, maxUTime, maxT+1);
    frame->GetXaxis()->SetTimeDisplay(1);
    frame->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{%d/%m/%y}");
    frame->GetXaxis()->SetLabelOffset(0.025);
    frame->GetXaxis()->SetNdivisions(505);
    frame->GetXaxis()->SetTimeOffset(offset.Convert());
    mgr->Draw("LP");
    c->Print(arguments_.at(2).toStdString().c_str());
}
