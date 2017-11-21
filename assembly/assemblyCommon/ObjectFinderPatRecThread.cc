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

#include <ObjectFinderPatRecThread.h>
#include <nqlogger.h>

#include <QMutex>

ObjectFinderPatRecThread::ObjectFinderPatRecThread(ObjectFinderPatRec* finder, QObject* parent) :
  QThread(parent),
  finder_(finder)
{
  if(finder_ == NULL)
  {
    NQLog("ObjectFinderPatRecThread", NQLog::Fatal) << "initialization error"
       << ": null pointer to ObjectFinderPatRec object, exiting constructor";

    return;
  }

  finder_->moveToThread(this);

//  connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));

  NQLog("ObjectFinderPatRecThread", NQLog::Debug) << "constructed";
}

ObjectFinderPatRecThread::~ObjectFinderPatRecThread()
{
  NQLog("ObjectFinderPatRecThread", NQLog::Debug) << "destructed";
}

void ObjectFinderPatRecThread::run()
{
  NQLog("ObjectFinderPatRecThread", NQLog::Debug) << "run";

  this->exec();

  return;
}
