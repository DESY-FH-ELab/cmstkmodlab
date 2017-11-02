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

#include <AssemblyUEyeCameraThread.h>
#include <nqlogger.h>

#include <QMutex>

AssemblyUEyeCameraThread::AssemblyUEyeCameraThread(AssemblyUEyeModel_t* model, QObject* parent) :
  QThread(parent),
  model_(model)
{
  model_->moveToThread(this);

  NQLog("AssemblyUEyeCameraThread", NQLog::Debug) << "constructed";
}

AssemblyUEyeCameraThread::~AssemblyUEyeCameraThread()
{
  NQLog("AssemblyUEyeCameraThread", NQLog::Debug) << "destructed";
}

void AssemblyUEyeCameraThread::run()
{
  NQLog("AssemblyUEyeCameraThread", NQLog::Debug) << "run";

  this->exec();

  return;
}
