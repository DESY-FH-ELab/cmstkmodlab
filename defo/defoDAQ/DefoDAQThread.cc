#include <QMutex>

#include <nqlogger.h>

#include "DefoDAQThread.h"

DefoDAQThread::DefoDAQThread(DefoDAQModel* daqModel,
                         QObject *parent)
  : QThread(parent),
    daqModel_(daqModel)
{

}

void DefoDAQThread::run()
{
    NQLog("DefoDAQThread", NQLog::Spam) << "started";
    exec();
}
