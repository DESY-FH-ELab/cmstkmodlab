#include <iostream>

#include <QDateTime>
#include <QTextStream>

#include <nqlogger.h>

#include "MicroDAQNetworkReader.h"

MicroDAQNetworkReader::MicroDAQNetworkReader(QObject* parent) :
    QObject(parent)
{

}

void MicroDAQNetworkReader::run(QString& buffer)
{
    process(buffer);
    NQLog("MicroDAQNetworkReader", NQLog::Debug) << "run(QString& buffer)";
    emit finished();
}

void MicroDAQNetworkReader::processHuberTemperature(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

    measurement_.bathTemperature = xml.attributes().value("bath").toString().toFloat();
    measurement_.workingTemperature = xml.attributes().value("work").toString().toFloat();
    measurement_.circulator = xml.attributes().value("circulator").toString().toInt();
}

void MicroDAQNetworkReader::processKeithleyState(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

    int idx = xml.attributes().value("sensor").toString().toInt();
    measurement_.channelActive[idx] = xml.attributes().value("state").toString().toInt();
}

void MicroDAQNetworkReader::processKeithleyTemperature(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

    int idx = xml.attributes().value("sensor").toString().toInt();
    measurement_.temperature[idx] = xml.attributes().value("temperature").toString().toFloat();
}

void MicroDAQNetworkReader::processPfeifferPressure(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

    measurement_.gaugeStatus1 = xml.attributes().value("s1").toString().toInt();
    measurement_.gaugePressure1 = xml.attributes().value("p1").toString().toFloat();
    measurement_.gaugeStatus2 = xml.attributes().value("s2").toString().toInt();
    measurement_.gaugePressure2 = xml.attributes().value("p2").toString().toFloat();
}

void MicroDAQNetworkReader::processHamegSetup(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

    measurement_.powerRemote = xml.attributes().value("remote").toString().toInt();
    measurement_.powerOn = xml.attributes().value("outputs").toString().toInt();
    measurement_.cv1 = xml.attributes().value("CV1").toString().toInt();
    measurement_.cv2 = xml.attributes().value("CV2").toString().toInt();
}

void MicroDAQNetworkReader::processHamegSetvalues(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

    measurement_.setVoltage1 = xml.attributes().value("V1").toString().toFloat();
    measurement_.setCurrent1 = xml.attributes().value("C1").toString().toFloat();
    measurement_.setVoltage2 = xml.attributes().value("V2").toString().toFloat();
    measurement_.setCurrent2 = xml.attributes().value("C2").toString().toFloat();
}

void MicroDAQNetworkReader::processHamegValues(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

    measurement_.voltage1 = xml.attributes().value("V1").toString().toFloat();
    measurement_.current1 = xml.attributes().value("C1").toString().toFloat();
    measurement_.voltage2 = xml.attributes().value("V2").toString().toFloat();
    measurement_.current2 = xml.attributes().value("C2").toString().toFloat();
}
void MicroDAQNetworkReader::processIotaSetup(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

    measurement_.iotaPumpEnabled = xml.attributes().value("enabled").toString().toInt();
    measurement_.iotaSetPressure = xml.attributes().value("pressure").toString().toFloat();
    measurement_.iotaSetFlow = xml.attributes().value("flow").toString().toFloat();
}

void MicroDAQNetworkReader::processIotaValues(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

    measurement_.iotaActPressure = xml.attributes().value("pressure").toString().toFloat();
    measurement_.iotaActFlow = xml.attributes().value("flow").toString().toFloat();
}

void MicroDAQNetworkReader::processArduinoPressure(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

    measurement_.arduinoPressureA = xml.attributes().value("pA").toString().toFloat();
    measurement_.arduinoPressureB = xml.attributes().value("pB").toString().toFloat();
}

void MicroDAQNetworkReader::processLine(QString& line)
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
        }
    }
}

void MicroDAQNetworkReader::process(QString& buffer)
{
    QString line;
    QTextStream in(&buffer);
    while (!in.atEnd()) {
        line = in.readLine();
        processLine(line);
    }
}
