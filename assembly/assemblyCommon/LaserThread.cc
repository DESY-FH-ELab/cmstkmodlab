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

#include <LaserThread.h>
#include <nqlogger.h>

LaserThread::LaserThread(QObject* parent) : QThread(parent)
{
  NQLog("LaserThread ", NQLog::Debug) << "constructed";
}

LaserThread::~LaserThread()
{
  NQLog("LaserThread ", NQLog::Debug) << "destructed";
}

void LaserThread::run()
{
  NQLog("LaserThread ", NQLog::Debug) << "run";

  this->exec();

  return;
}







