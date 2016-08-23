#include <QApplication>
#include <iostream>
#include <nqlogger.h>

#include "LStepExpressMotionThread.h"

LStepExpressMotionThread::LStepExpressMotionThread(QObject *parent)
    : QThread(parent)
{

}

void LStepExpressMotionThread::run()
{
  //    NQLog("LStepExpressMotionThread ", NQLog::Debug) << "run";
    exec();
}
