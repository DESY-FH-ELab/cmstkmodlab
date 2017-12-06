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

#include <AssemblyMultiPickupTesterThread.h>
#include <nqlogger.h>

#include <QMutex>

AssemblyMultiPickupTesterThread::AssemblyMultiPickupTesterThread(AssemblyMultiPickupTester* tester, QObject* parent) :
  QThread(parent),
  tester_(tester)
{
  if(tester_ == NULL)
  {
    NQLog("AssemblyMultiPickupTesterThread", NQLog::Fatal) << "initialization error"
       << ": null pointer to AssemblyMultiPickupTester object, exiting constructor";

    return;
  }

  tester_->moveToThread(this);

//  connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));

  NQLog("AssemblyMultiPickupTesterThread", NQLog::Debug) << "constructed";
}

AssemblyMultiPickupTesterThread::~AssemblyMultiPickupTesterThread()
{
  NQLog("AssemblyMultiPickupTesterThread", NQLog::Debug) << "destructed";
}

void AssemblyMultiPickupTesterThread::run()
{
  NQLog("AssemblyMultiPickupTesterThread", NQLog::Spam) << "run";

  this->exec();

  return;
}
