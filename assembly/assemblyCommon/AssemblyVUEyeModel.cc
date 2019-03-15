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

#include <AssemblyVUEyeModel.h>
#include <nqlogger.h>

AssemblyVUEyeModel::AssemblyVUEyeModel(int updateInterval, QObject* parent) :
  QObject(parent),
  updateInterval_(updateInterval)
{
    timer_ = new QTimer(this);
    timer_->setInterval(updateInterval_ * 1000);

    connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));

    timer_->start();

    NQLog("AssemblyVUEyeModel", NQLog::Debug) << "constructed";
}

AssemblyVUEyeModel::~AssemblyVUEyeModel()
{
    NQLog("AssemblyVUEyeModel", NQLog::Debug) << "destructed";
}

size_t AssemblyVUEyeModel::getCameraCount() const
{
    return cameras_.size();
}

AssemblyVUEyeCamera* AssemblyVUEyeModel::getCamera(size_t idx)
{
    NQLog("AssemblyVUEyeModel", NQLog::Debug) << "getCamera(" << idx <<" )";

    if(idx >= this->getCameraCount()){ return 0; }

    return cameras_.at(idx);
}

AssemblyVUEyeCamera* AssemblyVUEyeModel::getCameraByID(unsigned int id)
{
    NQLog("AssemblyVUEyeModel", NQLog::Debug) << "getCameraByID(" << id << ")";

    for(QVector<AssemblyVUEyeCamera*>::iterator it=cameras_.begin(); it!=cameras_.end(); ++it)
    {
      if((*it)->getCameraID()==id){ return *it; }
    }

    return 0;
}

void AssemblyVUEyeModel::clear()
{
    for(QVector<AssemblyVUEyeCamera*>::iterator it=cameras_.begin(); it!=cameras_.end(); ++it)
    {
        delete *it;
    }
    cameras_.clear();

    NQLog("AssemblyVUEyeModel", NQLog::Debug) << "clear: collection of camera(s) cleared";

    for(QVector<QThread*>::iterator it = threads_.begin(); it != threads_.end(); ++it)
    {
        (*it)->wait(1000);
        delete *it;
    }
    threads_.clear();

    NQLog("AssemblyVUEyeModel", NQLog::Debug) << "clear: collection of thread(s) cleared";

    return;
}
