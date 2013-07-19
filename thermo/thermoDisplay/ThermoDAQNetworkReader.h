#ifndef THERMODAQNETWORKREADER_H
#define THERMODAQNETWORKREADER_H

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
} Measurement_t;

class ThermoDAQNetworkReader : public QObject
{
    Q_OBJECT
public:
    explicit ThermoDAQNetworkReader(QObject* parent);

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

    Measurement_t measurement_;
};

#endif // THERMODAQNETWORKREADER_H
