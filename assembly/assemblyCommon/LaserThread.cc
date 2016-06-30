#include <QApplication>

#include <nqlogger.h>

#include "LaserThread.h"

LaserThread::LaserThread(QObject *parent)
    : QThread(parent)
{

}

void LaserThread::run()
{
    NQLog("LaserThread", NQLog::Debug) << "run";
    exec();
}







