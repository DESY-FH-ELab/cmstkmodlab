#include <QMutex>

#include <nqlogger.h>

#include "AssemblyUEyeCameraThread.h"

AssemblyUEyeCameraThread::AssemblyUEyeCameraThread(AssemblyUEyeModel_t* model,
                                                   QObject *parent)
    : QThread(parent),
      model_(model)
{
    model_->moveToThread(this);

    NQLog("AssemblyUEyeCameraThread", NQLog::Spam) << "construct";
}

AssemblyUEyeCameraThread::~AssemblyUEyeCameraThread()
{
    NQLog("AssemblyUEyeCameraThread", NQLog::Spam) << "delete";
}

void AssemblyUEyeCameraThread::run()
{
    NQLog("AssemblyUEyeCameraThread", NQLog::Spam) << "started";
    exec();
}
