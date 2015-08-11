#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyUEyeModel.h"

AssemblyUEyeModel::AssemblyUEyeModel(int updateInterval,
                                QObject *parent)
    : QObject(),
      updateInterval_(updateInterval)
{
    timer_ = new QTimer(this);
    timer_->setInterval(updateInterval_ * 1000);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));

    updateInformation();
    //timer_->start();
}

void AssemblyUEyeModel::updateInformation()
{
    NQLog("AssemblyUEyeModel") << "updateInformation";
}
