#include <iostream>

#include <QDateTime>
#include <QTextStream>

#include <nqlogger.h>

#include "DefoDAQNetworkReader.h"

DefoDAQNetworkReader::DefoDAQNetworkReader(QObject* parent) :
    QObject(parent)
{

}

void DefoDAQNetworkReader::run(QString& buffer)
{
    process(buffer);
    NQLog("DefoDAQNetworkReader", NQLog::Debug) << "run(QString& buffer)";
    emit finished();
}

void DefoDAQNetworkReader::processJulaboTemperature(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

    measurement_.bathTemperature = xml.attributes().value("bath").toString().toFloat();
    measurement_.workingTemperature = xml.attributes().value("work").toString().toFloat();
    measurement_.circulator = xml.attributes().value("circulator").toString().toInt();
}

void DefoDAQNetworkReader::processKeithleyState(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

    int idx = xml.attributes().value("sensor").toString().toInt();
    measurement_.channelActive[idx] = xml.attributes().value("state").toString().toInt();
}

void DefoDAQNetworkReader::processKeithleyTemperature(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    measurement_.dt = QDateTime::fromString(time, Qt::ISODate);

    int idx = xml.attributes().value("sensor").toString().toInt();
    measurement_.temperature[idx] = xml.attributes().value("temperature").toString().toFloat();
}

void DefoDAQNetworkReader::processLine(QString& line)
{
    QXmlStreamReader xml(line);
    while (!xml.atEnd()) {
        xml.readNextStartElement();
        if (xml.isStartElement()) {
            if (xml.name()=="JulaboTemperature") {
                processJulaboTemperature(xml);
            }
            if (xml.name()=="KeithleyState") {
                processKeithleyState(xml);
            }
            if (xml.name()=="KeithleyTemperature") {
                processKeithleyTemperature(xml);
            }
        }
    }
}

void DefoDAQNetworkReader::process(QString& buffer)
{
    QString line;
    QTextStream in(&buffer);
    while (!in.atEnd()) {
        line = in.readLine();
        processLine(line);
    }
}
