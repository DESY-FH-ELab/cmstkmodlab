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
   camera_(camera),
   model_connected_(false)
{
  if(!this->camera())
  {
    NQLog("AssemblyUEyeCameraManager::AssemblyUEyeCameraManager") << "camera initialization failed";

    const std::string log("initialization error: null pointer to AssemblyVUEyeCamera object");
    Log::KILL("AssemblyUEyeCameraManager::AssemblyUEyeCameraManager -- "+log);
  }

  this->connect_model();
}

AssemblyUEyeCameraManager::~AssemblyUEyeCameraManager()
{
}

void AssemblyUEyeCameraManager::connect_model()
{
  if(model_connected_ == false)
  {
    connect(this   , SIGNAL(open_camera()) , camera_, SLOT(open()));
    connect(camera_, SIGNAL(cameraOpened()), this   , SLOT(enable_camera()));

    connect(this   , SIGNAL(close_camera()), camera_, SLOT(close()));
    connect(camera_, SIGNAL(cameraClosed()), this   , SLOT(disable_camera()));

    connect(this   , SIGNAL(call_camera())         , camera_, SLOT(acquireImage()));
    connect(camera_, SIGNAL(imageAcquired(cv::Mat)), this   , SLOT(process_image(cv::Mat)));

    model_connected_ = true;
  }

  return;
}

void AssemblyUEyeCameraManager::disconnect_model()
{
  if(model_connected_ == true)
  {
    disconnect(this   , SIGNAL(open_camera()) , camera_, SLOT(open()));
    disconnect(camera_, SIGNAL(cameraOpened()), this   , SLOT(enable_camera()));

    disconnect(this   , SIGNAL(close_camera()), camera_, SLOT(close()));
    disconnect(camera_, SIGNAL(cameraClosed()), this   , SLOT(disable_camera()));

    disconnect(this   , SIGNAL(call_camera())         , camera_, SLOT(acquireImage()));
    disconnect(camera_, SIGNAL(imageAcquired(cv::Mat)), this   , SLOT(process_image(cv::Mat)));

    model_connected_ = false;
  }

  return;
}
