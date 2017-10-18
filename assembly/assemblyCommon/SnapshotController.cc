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

#include "SnapshotController.h"

#include <QApplication>

#include <nqlogger.h>

SnapshotController:: SnapshotController() {}

SnapshotController::~SnapshotController() {}

//!!NQLog("SnapshotController") << "moveAbsolute";

void SnapshotController::enable()
{
  is_enabled_ = true;

  emit openCamera();
}

void SnapshotController::disable()
{
  is_enabled_ = false;

  emit closeCamera();
}

void SnapshotController::acquireImage()
{
  emit getImage();
}

void SnapshotController::cameraOpened()
{
  emit camera_is_opened();
}

void SnapshotController::cameraClosed()
{
  emit camera_is_closed();
}

void SnapshotController::retrieveImage(const cv::Mat& a_mat)
{
  emit imageAcquired(a_mat);
}
