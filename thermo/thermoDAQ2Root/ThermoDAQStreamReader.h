#ifndef THERMODAQSTREAMREADER_H
#define THERMODAQSTREAMREADER_H

#include <string>

#include <QObject>
#include <QStringList>
#include <QXmlStreamReader>

#include <TFile.h>
#include <TTree.h>

typedef struct {
    unsigned int   uTime;

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
} Measurement_t;

typedef struct {
    unsigned int   uTime;
    std::string    message;
} Log_t;

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

    bool measurementValid_;
    Measurement_t measurement_;
    Log_t         log_;

    TFile *ofile_;
    TTree *otree_;
    TTree *ologtree_;
};

#endif // THERMODAQSTREAMREADER_H
