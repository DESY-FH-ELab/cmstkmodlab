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

#include <AssemblyUEyeCameraManager.h>
#include <nqlogger.h>
#include <Log.h>

AssemblyUEyeCameraManager::AssemblyUEyeCameraManager(AssemblyVUEyeCamera* camera, QObject *parent) :
   QObject(parent),
   camera_(camera)
{
  if(!this->camera())
  {
    NQLog("AssemblyUEyeCameraManager::AssemblyUEyeCameraManager") << "camera initialization failed";

    const std::string log("initialization error: null pointer to AssemblyVUEyeCamera object");
    Log::KILL("AssemblyUEyeCameraManager::AssemblyUEyeCameraManager -- "+log);
  }

  connect(this, SIGNAL( open_camera()), camera_, SLOT(open()));
  connect(this, SIGNAL(close_camera()), camera_, SLOT(close()));

  connect(this, SIGNAL(update_cameraInfo()), camera_, SLOT(updateInformation()));
  connect(this, SIGNAL(acquire_image())    , camera_, SLOT(acquireImage()));

  connect(camera_, SIGNAL(imageAcquired(cv::Mat)), this, SLOT(image_acquired(cv::Mat)));
}

AssemblyUEyeCameraManager::~AssemblyUEyeCameraManager()
{
}
