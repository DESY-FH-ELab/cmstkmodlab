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

#include <ImageControllerThread.h>
#include <nqlogger.h>

#include <QMutex>

ImageControllerThread::ImageControllerThread(ImageController* imagec, QObject* parent) :
  QThread(parent),
  imagec_(imagec)
{
  imagec_->moveToThread(this);

  NQLog("ImageControllerThread", NQLog::Debug) << "constructed";
}

ImageControllerThread::~ImageControllerThread()
{
  NQLog("ImageControllerThread", NQLog::Debug) << "destructed";
}

void ImageControllerThread::run()
{
  NQLog("ImageControllerThread", NQLog::Debug) << "run";

  this->exec();

  return;
}
