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

#ifndef THERMODAQSTREAMREADER_H
#define THERMODAQSTREAMREADER_H

#include <vector>
#include <string>

#include <QObject>
#include <QStringList>
#include <QXmlStreamReader>

#include <TGraph.h>
#include <TDatime.h>

class ThermoDAQStreamReader : public QObject
{
    Q_OBJECT
public:
    explicit ThermoDAQStreamReader(QStringList arguments, QObject* parent);

public slots:
    void run();

signals:
    void finished();

protected:
    QStringList arguments_;
    void process();
    void processLine(QString& line);

    void processHuberTemperature(QXmlStreamReader& xml);
    void processKeithleyState(QXmlStreamReader& xml);
    void processKeithleyTemperature(QXmlStreamReader& xml);
    void processPfeifferPressure(QXmlStreamReader& xml);
    void processHamegSetup(QXmlStreamReader& xml);
    void processHamegSetvalues(QXmlStreamReader& xml);
    void processHamegValues(QXmlStreamReader& xml);
    void processLog(QXmlStreamReader& xml);
    void processDAQStarted(QXmlStreamReader& xml);
    void processIotaSetup(QXmlStreamReader& xml);
    void processIotaValues(QXmlStreamReader& xml);
    void processArduinoPressure(QXmlStreamReader& xml);

    int activeT[10];
    std::vector<TGraph*> graphsT[10];
    TGraph * graphT[10];
    int graphTn[10];
    
    TDatime offset;
    uint minUTime;
    uint maxUTime;
    float minT;
    float maxT;
};

#endif // THERMODAQSTREAMREADER_H
