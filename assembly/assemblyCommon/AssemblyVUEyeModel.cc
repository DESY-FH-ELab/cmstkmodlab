#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyVUEyeModel.h"

AssemblyVUEyeModel::AssemblyVUEyeModel(int updateInterval,
                                       QObject *parent)
    : QObject(parent),
      updateInterval_(updateInterval)
{
    timer_ = new QTimer(this);
    timer_->setInterval(updateInterval_ * 1000);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));

    timer_->start();
}

AssemblyVUEyeModel::~AssemblyVUEyeModel()
{
    NQLog("AssemblyVUEyeModel") << "delete";
}

size_t AssemblyVUEyeModel::getCameraCount() const
{
    return cameras_.size();
}

AssemblyVUEyeCamera * AssemblyVUEyeModel::getCamera(size_t idx)
{
    if (idx >= this->getCameraCount()) return 0;
    return cameras_.at(idx);
}

AssemblyVUEyeCamera * AssemblyVUEyeModel::getCameraByID(unsigned int id)
{
    for (QVector<AssemblyVUEyeCamera*>::iterator it = cameras_.begin();
         it != cameras_.end();
         ++it) {
        if ((*it)->getCameraID()==id) return *it;
    }

    return 0;
}

void AssemblyVUEyeModel::clear()
{
    for (QVector<AssemblyVUEyeCamera*>::iterator it = cameras_.begin();
         it != cameras_.end();
         ++it) {
        delete *it;
    }
    cameras_.clear();

    for (QVector<QThread*>::iterator it = threads_.begin();
         it != threads_.end();
         ++it) {
        (*it)->wait(1000);
        delete *it;
    }
    threads_.clear();
}

