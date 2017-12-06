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

#include <AssemblyImageControllerThread.h>
#include <nqlogger.h>

#include <QMutex>

AssemblyImageControllerThread::AssemblyImageControllerThread(AssemblyImageController* image_c, QObject* parent) :
  QThread(parent),
  image_ctr_(image_c)
{
  if(image_ctr_ == NULL)
  {
    NQLog("AssemblyImageControllerThread", NQLog::Fatal) << "initialization error"
       << ": null pointer to AssemblyImageController object, exiting constructor";

    return;
  }

  image_ctr_->moveToThread(this);

//  connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));

  NQLog("AssemblyImageControllerThread", NQLog::Debug) << "constructed";
}

AssemblyImageControllerThread::~AssemblyImageControllerThread()
{
  NQLog("AssemblyImageControllerThread", NQLog::Debug) << "destructed";
}

void AssemblyImageControllerThread::run()
{
  NQLog("AssemblyImageControllerThread", NQLog::Spam) << "run";

  this->exec();

  return;
}
