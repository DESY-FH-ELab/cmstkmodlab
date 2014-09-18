#include <QMutex>

#include <nqlogger.h>

#include "MicroDAQThread.h"

MicroDAQThread::MicroDAQThread(MicroDAQModel* daqModel,
                               QObject *parent) :
    QThread(parent),
    daqModel_(daqModel)
{

}

void MicroDAQThread::run()
{
    NQLog("MicroDAQThread", NQLog::Spam) << "started";
    exec();
}
