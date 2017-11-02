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

#include <MarkerFinderPatRecThread.h>
#include <nqlogger.h>

#include <QMutex>

MarkerFinderPatRecThread::MarkerFinderPatRecThread(MarkerFinderPatRec* finder, QObject* parent) :
  QThread(parent),
  finder_(finder)
{
  finder_->moveToThread(this);

  NQLog("MarkerFinderPatRecThread", NQLog::Debug) << "constructed";
}

MarkerFinderPatRecThread::~MarkerFinderPatRecThread()
{
  NQLog("MarkerFinderPatRecThread", NQLog::Debug) << "destructed";
}

void MarkerFinderPatRecThread::run()
{
  NQLog("MarkerFinderPatRecThread", NQLog::Debug) << "run";

  this->exec();

  return;
}
