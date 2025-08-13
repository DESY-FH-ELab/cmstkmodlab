/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <nqlogger.h>

#include <AssemblyObjectFinderPatRecThread.h>

AssemblyObjectFinderPatRecThread::AssemblyObjectFinderPatRecThread(AssemblyObjectFinderPatRec* const finder, QObject* parent) :
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

  NQLog("AssemblyObjectFinderPatRecThread", NQLog::Debug) << "constructed";
}

AssemblyObjectFinderPatRecThread::~AssemblyObjectFinderPatRecThread()
{
  NQLog("AssemblyObjectFinderPatRecThread", NQLog::Debug) << "destructed";
}

void AssemblyObjectFinderPatRecThread::run()
{
  NQLog("AssemblyObjectFinderPatRecThread", NQLog::Debug) << "run";

  this->exec();

  return;
}
