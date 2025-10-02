/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include "ThermoDAQLogReader.h"

ThermoDAQLogReader::ThermoDAQLogReader(QStringList arguments,
                                             QObject* parent) :
    QObject(parent),
    arguments_(arguments)
{

}

void ThermoDAQLogReader::run()
{
    process();
    emit finished();
}

void ThermoDAQLogReader::processLog(QXmlStreamReader& xml)
{
    QString time = xml.attributes().value("time").toString();
    QDateTime dt = QDateTime::fromString(time, Qt::TextDate);
    std::string message = xml.readElementText(QXmlStreamReader::IncludeChildElements).toStdString();
    std::cout << dt.toString().toStdString() << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << message << std::endl << std::endl;
}

void ThermoDAQLogReader::processLine(QString& line)
{
    QXmlStreamReader xml(line);
    while (!xml.atEnd()) {
        xml.readNextStartElement();
        if (xml.isStartElement()) {
            if (xml.name()=="Log") {
                processLog(xml);
            }
        }
    }
}

void ThermoDAQLogReader::process()
{
    if (arguments_.size()!=2) return;

    QFile file(arguments_.at(1));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QString line;
    QTextStream in(&file);
    while (!in.atEnd()) {
        line = in.readLine();
        processLine(line);
    }
}
