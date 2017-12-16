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

#include <AssemblyObjectFinderPatRecThread.h>
#include <nqlogger.h>

AssemblyObjectFinderPatRecThread::AssemblyObjectFinderPatRecThread(AssemblyObjectFinderPatRec* finder, QObject* parent) :
  QThread(parent),
  finder_(finder)
{
  if(finder_ == nullptr)
  {
    NQLog("AssemblyObjectFinderPatRecThread", NQLog::Fatal) << "initialization error"
       << ": null pointer to ObjectFinderPatRec object, exiting constructor";

    return;
  }

  finder_->moveToThread(this);

//  connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));

  NQLog("AssemblyObjectFinderPatRecThread", NQLog::Debug) << "constructed";
}

AssemblyObjectFinderPatRecThread::~AssemblyObjectFinderPatRecThread()
{
  NQLog("AssemblyObjectFinderPatRecThread", NQLog::Debug) << "destructed";
}

void AssemblyObjectFinderPatRecThread::run()
{
  NQLog("AssemblyObjectFinderPatRecThread", NQLog::Spam) << "run";

  this->exec();

  return;
}
