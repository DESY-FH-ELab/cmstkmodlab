#ifndef THERMODAQSTREAMREADER_H
#define THERMODAQSTREAMREADER_H

#include <QObject>
#include <QStringList>
#include <QXmlStreamReader>

class ThermoDAQLogReader : public QObject
{
    Q_OBJECT
public:
    explicit ThermoDAQLogReader(QStringList arguments, QObject* parent);

public slots:
    void run();

signals:
    void finished();

protected:
    QStringList arguments_;
    void process();
    void processLine(QString& line);

    void processLog(QXmlStreamReader& xml);
};

#endif // THERMODAQSTREAMREADER_H
