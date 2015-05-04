#ifndef DEFODAQSTREAMREADER_H
#define DEFODAQSTREAMREADER_H

#include <string>

#include <QObject>
#include <QStringList>
#include <QXmlStreamReader>

#include <TFile.h>
#include <TTree.h>
#include <TDatime.h>

typedef struct {
    unsigned int   uTime;
    TDatime        datime;

    int            chillerState;
    float          bathTemperature;
    float          workingTemperature;
    int            circulator;

    int            channelActive[10];
    float          temperature[10];

    int            conradState[8];
} Measurement_t;

class DefoDAQStreamReader : public QObject
{
    Q_OBJECT
public:
    explicit DefoDAQStreamReader(QStringList arguments, QObject* parent);

public slots:
    void run();

signals:
    void finished();

protected:
    QStringList arguments_;
    void process();
    void processLine(QString& line);

    void processDAQStarted(QXmlStreamReader& xml);
    void processJulaboTemperature(QXmlStreamReader& xml);
    void processKeithleyState(QXmlStreamReader& xml);
    void processKeithleyTemperature(QXmlStreamReader& xml);
    void processConradState(QXmlStreamReader& xml);

    bool measurementValid_;
    Measurement_t measurement_;

    TFile *ofile_;
    TTree *otree_;
};

#endif // DEFODAQSTREAMREADER_H
