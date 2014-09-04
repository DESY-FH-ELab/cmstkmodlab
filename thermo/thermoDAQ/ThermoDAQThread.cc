#include <QMutex>

#include <nqlogger.h>

#include "ThermoDAQThread.h"

ThermoDAQThread::ThermoDAQThread(ThermoDAQModel* daqModel,
                         QObject *parent) :
    QThread(parent),
    daqModel_(daqModel)
{

}

void ThermoDAQThread::run()
{
    NQLog("ThermoDAQThread", NQLog::Spam) << "started";
    exec();
}
