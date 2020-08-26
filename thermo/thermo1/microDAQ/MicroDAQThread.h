#ifndef MICRODAQTHREAD_H
#define MICRODAQTHREAD_H

#include <QObject>
#include <QThread>

#include "MicroDAQModel.h"

class MicroDAQThread : public QThread
{
public:
    explicit MicroDAQThread(MicroDAQModel* daqModel,
                            QObject *parent = 0);
public:
    void run();

protected:
    MicroDAQModel* daqModel_;
};

#endif // MICRODAQTHREAD_H
