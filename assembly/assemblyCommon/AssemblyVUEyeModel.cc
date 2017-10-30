/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblyVUEyeModel.h"

AssemblyVUEyeModel::AssemblyVUEyeModel(int updateInterval, QObject *parent)
  : QObject(parent), updateInterval_(updateInterval)
{
    timer_ = new QTimer(this);
    timer_->setInterval(updateInterval_ * 1000);

    connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));

    timer_->start();
}

AssemblyVUEyeModel::~AssemblyVUEyeModel()
{
    NQLog("AssemblyVUEyeModel::~AssemblyVUEyeModel") << "delete";
}

size_t AssemblyVUEyeModel::getCameraCount() const
{
    return cameras_.size();
}

AssemblyVUEyeCamera* AssemblyVUEyeModel::getCamera(size_t idx)
{
    if(idx >= this->getCameraCount()){ return 0; }

    return cameras_.at(idx);
}

AssemblyVUEyeCamera* AssemblyVUEyeModel::getCameraByID(unsigned int id)
{
    for(QVector<AssemblyVUEyeCamera*>::iterator it=cameras_.begin(); it!=cameras_.end(); ++it)
    {
        if((*it)->getCameraID()==id){ return *it; }
    }

    return 0;
}

void AssemblyVUEyeModel::clear()
{
    NQLog("AssemblyVUEyeModel::clear") << "clearing collection of camera(s)";

    for(QVector<AssemblyVUEyeCamera*>::iterator it=cameras_.begin(); it!=cameras_.end(); ++it)
    {
        delete *it;
    }
    cameras_.clear();

    NQLog("AssemblyVUEyeModel::clear") << "clearing collection of thread(s)";

    for(QVector<QThread*>::iterator it = threads_.begin(); it != threads_.end(); ++it)
    {
        (*it)->wait(1000);
        delete *it;
    }
    threads_.clear();

    return;
}
