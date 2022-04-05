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

#ifndef THERMODAQSTREAMREADER_H
#define THERMODAQSTREAMREADER_H

#include <string>

#include <QObject>
#include <QStringList>
#include <QXmlStreamReader>
#include <QDateTime>

typedef struct {
    unsigned int   uTime;
    QDateTime      datime;

    float          bathTemperature;
    float          workingTemperature;
    int            circulator;

    int            channelActive[10];
    float          temperature[10];

    int            gaugeStatus1;
    float          gaugePressure1;
    int            gaugeStatus2;
    float          gaugePressure2;

    int            powerRemote;
    int            powerOn;
    int            cv1;
    int            cv2;
    float          setVoltage1;
    float          setCurrent1;
    float          setVoltage2;
    float          setCurrent2;
    float          voltage1;
    float          current1;
    float          voltage2;
    float          current2;

    int            iotaPumpEnabled;
    float          iotaSetPressure;
    float          iotaSetFlow;
    float          iotaActPressure;
    float          iotaActFlow;

    float          arduinoPressureA;
    float          arduinoPressureB;
} Measurement_t;

typedef struct {
    unsigned int   uTime;
    QDateTime      datime;

    std::string    message;
} Log_t;

class ThermoDAQStreamReader : public QObject
{
    Q_OBJECT
public:
    explicit ThermoDAQStreamReader(const QString &filename,
        QObject* parent);

public slots:
    void run();

signals:
    void finished();

protected:

    const QString filename_;

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

    bool measurementValid_;
    Measurement_t measurement_;
    Log_t         log_;
};

#endif // THERMODAQSTREAMREADER_H
