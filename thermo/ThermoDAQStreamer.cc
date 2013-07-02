#include <iostream>

#include "ThermoDAQStreamer.h"

ThermoDAQStreamer::ThermoDAQStreamer(ThermoDAQModel* model,
                                     QObject *parent) :
    QObject(parent),
    model_(model)
{
    connect(model_, SIGNAL(daqMessage(QString)),
            this, SLOT(handleDAQMessage(QString)));
}

void ThermoDAQStreamer::handleDAQMessage(const QString& message)
{
    std::cout << message.toStdString() << std::endl;
}
