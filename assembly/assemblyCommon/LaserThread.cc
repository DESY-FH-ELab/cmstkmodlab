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
    std::cout<<"LaserThread " << "run"<<std::endl;
    exec();
}







