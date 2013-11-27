#include <QMutex>

#include "ThermoDAQThread.h"

ThermoDAQThread::ThermoDAQThread(ThermoDAQModel* daqModel,
                         QObject *parent) :
    QThread(parent),
    daqModel_(daqModel)
{

}

void ThermoDAQThread::run()
{
    std::cout << "DAQ thread started" << std::endl;
    exec();
}
