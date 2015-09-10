#ifndef DEFODAQNETWORKREADER_H
#define DEFODAQNETWORKREADER_H

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
} Measurement_t;

class DefoDAQNetworkReader : public QObject
{
    Q_OBJECT
public:
    explicit DefoDAQNetworkReader(QObject* parent);

    const Measurement_t& getMeasurement() const { return measurement_; }

public slots:
    void run(QString& buffer);

signals:
    void finished();

protected:
    void process(QString& buffer);
    void processLine(QString& line);

    void processJulaboTemperature(QXmlStreamReader& xml);
    void processKeithleyState(QXmlStreamReader& xml);
    void processKeithleyTemperature(QXmlStreamReader& xml);

    Measurement_t measurement_;
};

#endif // DEFODAQNETWORKREADER_H
