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

#include <LStepExpressMotionThread.h>
#include <nqlogger.h>

LStepExpressMotionThread::LStepExpressMotionThread(LStepExpressMotionManager* motion_manager, QObject* parent) :
  QThread(parent),
  motion_manager_(motion_manager)
{
  if(motion_manager_ == nullptr)
  {
    NQLog("LStepExpressMotionThread", NQLog::Fatal) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }

  motion_manager_->myMoveToThread(this);

//  connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));

  NQLog("LStepExpressMotionThread", NQLog::Debug) << "constructed";
}

LStepExpressMotionThread::~LStepExpressMotionThread()
{
  NQLog("LStepExpressMotionThread", NQLog::Debug) << "destructed";
}

void LStepExpressMotionThread::run()
{
  NQLog("LStepExpressMotionThread", NQLog::Debug) << "run";

  this->exec();

  return;
}
