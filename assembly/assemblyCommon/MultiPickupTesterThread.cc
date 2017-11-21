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

#include <MultiPickupTesterThread.h>
#include <nqlogger.h>

#include <QMutex>

MultiPickupTesterThread::MultiPickupTesterThread(MultiPickupTester* tester, QObject* parent) :
  QThread(parent),
  tester_(tester)
{
  if(tester_ == NULL)
  {
    NQLog("MultiPickupTesterThread", NQLog::Fatal) << "initialization error"
       << ": null pointer to MultiPickupTester object, exiting constructor";

    return;
  }

  tester_->moveToThread(this);

//  connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));

  NQLog("MultiPickupTesterThread", NQLog::Debug) << "constructed";
}

MultiPickupTesterThread::~MultiPickupTesterThread()
{
  NQLog("MultiPickupTesterThread", NQLog::Debug) << "destructed";
}

void MultiPickupTesterThread::run()
{
  NQLog("MultiPickupTesterThread", NQLog::Debug) << "run";

  this->exec();

  return;
}
