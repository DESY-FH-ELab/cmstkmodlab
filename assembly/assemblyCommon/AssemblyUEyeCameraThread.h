#ifndef ASSEMBLYUEYECAMERATHREAD_H
#define ASSEMBLYUEYECAMERATHREAD_H

#include <QObject>
#include <QThread>

#ifdef NOUEYE
#include "AssemblyUEyeFakeModel.h"
typedef AssemblyUEyeFakeModel AssemblyUEyeModel_t;
#else
#include "AssemblyUEyeModel.h"
typedef AssemblyUEyeModel AssemblyUEyeModel_t;
#endif

class AssemblyUEyeCameraThread : public QThread
{
public:
    explicit AssemblyUEyeCameraThread(AssemblyUEyeModel_t* model,
                                      QObject *parent = 0);

    ~AssemblyUEyeCameraThread();

    void run();

protected:
    AssemblyUEyeModel_t* model_;
};

#endif // ASSEMBLYUEYECAMERATHREAD_H
