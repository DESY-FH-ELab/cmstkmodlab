#ifndef THERMODAQSTREAMER_H
#define THERMODAQSTREAMER_H

#include <QObject>

#include <ThermoDAQModel.h>

class ThermoDAQStreamer : public QObject
{
    Q_OBJECT
public:
    explicit ThermoDAQStreamer(ThermoDAQModel* model, QObject *parent = 0);

protected slots:
    void handleDAQMessage(const QString& message);

protected:
  ThermoDAQModel* model_;
};

#endif // THERMODAQSTREAMER_H
