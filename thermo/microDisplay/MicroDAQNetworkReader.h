#ifndef MICRODAQNETWORKREADER_H
#define MICRODAQNETWORKREADER_H

#include <string>

#include <QObject>
#include <QStringList>
#include <QXmlStreamReader>
#include <QDateTime>

typedef struct {
    QDateTime      dt;

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

    bool           iotaPumpEnabled;
    float          iotaActPressure;
    float          iotaSetPressure;
    float          iotaActFlow;
    float          iotaSetFlow;

    float          arduinoPressureA;
    float          arduinoPressureB;
} Measurement_t;

class MicroDAQNetworkReader : public QObject
{
    Q_OBJECT
public:
    explicit MicroDAQNetworkReader(QObject* parent);

    const Measurement_t& getMeasurement() const { return measurement_; }

public slots:
    void run(QString& buffer);

signals:
    void finished();

protected:
    void process(QString& buffer);
    void processLine(QString& line);

    void processHuberTemperature(QXmlStreamReader& xml);
    void processKeithleyState(QXmlStreamReader& xml);
    void processKeithleyTemperature(QXmlStreamReader& xml);
    void processPfeifferPressure(QXmlStreamReader& xml);
    void processHamegSetup(QXmlStreamReader& xml);
    void processHamegSetvalues(QXmlStreamReader& xml);
    void processHamegValues(QXmlStreamReader& xml);
    void processIotaSetup(QXmlStreamReader& xml);
    void processIotaValues(QXmlStreamReader& xml);
    void processArduinoPressure(QXmlStreamReader& xml);

    Measurement_t measurement_;
};

#endif // MICRODAQNETWORKREADER_H
