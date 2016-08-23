#include <QApplication>
#include <iostream>
#include <nqlogger.h>

#include "LaserThread.h"

LaserThread::LaserThread(QObject *parent)
    : QThread(parent)
{

}

LaserThread::~LaserThread()
{
}

void LaserThread::run()
{
  //    NQLog("LaserThread ", NQLog::Debug) << "run";
    exec();
}







