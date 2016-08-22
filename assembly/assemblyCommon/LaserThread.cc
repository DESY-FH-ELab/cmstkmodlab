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
  std::cout<<"destructor laserthread"<<std::endl;
}

void LaserThread::run()
{
  //    NQLog("LaserThread ", NQLog::Debug) << "run";
    exec();
}







