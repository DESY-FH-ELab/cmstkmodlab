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

#include <AssemblyImageController.h>
#include <nqlogger.h>
#include <Util.h>

AssemblyImageController::AssemblyImageController(const AssemblyVUEyeCamera* camera, const AssemblyZFocusFinder* zfocus_finder, QObject* parent) :
  QObject(parent),
  camera_manager_(camera),
  zfocus_finder_(zfocus_finder),
  is_enabled_(false),
  autofocus_is_enabled_(false)
{
  if(camera_manager_ == nullptr)
  {
    NQLog("AssemblyImageController", NQLog::Fatal) << "initialization error"
       << ": null pointer to AssemblyVUEyeCamera object, exiting constructor";

    return;
  }

  if(zfocus_finder_ != nullptr && zfocus_finder_->camera_manager() != camera_manager_)
  {
    NQLog("AssemblyImageController", NQLog::Fatal) << "initialization error"
       << ": target input camera differs from camera of AssemblyZFocusFinder, exiting constructor";

    return;
  }

  connect(this           , SIGNAL(open_camera())  , camera_manager_, SLOT(open()));
  connect(camera_manager_, SIGNAL(cameraOpened()) , this           , SLOT(enable_camera()));

  connect(this           , SIGNAL(close_camera()) , camera_manager_, SLOT(close()));
  connect(camera_manager_, SIGNAL(cameraClosed()) , this           , SLOT(disable_camera()));

  connect(this           , SIGNAL(image())               , camera_manager_, SLOT(acquireImage()));
  connect(camera_manager_, SIGNAL(imageAcquired(cv::Mat)), this           , SLOT(retrieve_image(cv::Mat)));

  NQLog("AssemblyImageController", NQLog::Debug) << "constructed";
}

AssemblyImageController::~AssemblyImageController()
{
}

void AssemblyImageController::enable()
{
  is_enabled_ = true;

  NQLog("AssemblyImageController", NQLog::Spam) << "enable"
     << ": emitting signal \"open_camera\"";

  emit open_camera();
}

void AssemblyImageController::disable()
{
  is_enabled_ = false;

  NQLog("AssemblyImageController", NQLog::Spam) << "disable"
     << ": emitting signal \"close_camera\"";

  emit close_camera();
}

void AssemblyImageController::acquire_image()
{
  NQLog("AssemblyImageController", NQLog::Spam) << "acquire_image"
     << ": emitting signal \"image\"";

  emit image();
}

void AssemblyImageController::enable_camera()
{
  NQLog("AssemblyImageController", NQLog::Spam) << "enable_camera"
     << ": emitting signal \"camera_enabled\"";

  emit camera_enabled();
}

void AssemblyImageController::disable_camera()
{
  NQLog("AssemblyImageController", NQLog::Spam) << "disable_camera"
     << ": emitting signal \"camera_disabled\"";

  emit camera_disabled();
}

void AssemblyImageController::retrieve_image(const cv::Mat& a_mat)
{
  NQLog("AssemblyImageController", NQLog::Message) << "retrieve_image"
     << ": emitting signal \"image_acquired\"";

  emit image_acquired(a_mat);
}

void AssemblyImageController::enable_AutoFocus()
{
  if(zfocus_finder_ == 0)
  {
    NQLog("AssemblyImageController", NQLog::Warning) << "enable_AutoFocus"
       << ": AssemblyZFocusFinder not initialized, auto-focusing not enabled";

    return;
  }

  disconnect(this           , SIGNAL(image())                , camera_manager_, SLOT(acquireImage()));
  disconnect(camera_manager_, SIGNAL(imageAcquired(cv::Mat)) , this           , SLOT(retrieve_image(cv::Mat)));

  connect   (this           , SIGNAL(image())                , zfocus_finder_ , SLOT(acquire_image()));
  connect   (zfocus_finder_ , SIGNAL(image_acquired(cv::Mat)), this           , SLOT(retrieve_image(cv::Mat)));

  autofocus_is_enabled_ = true;

  NQLog("AssemblyImageController", NQLog::Message) << "enable_AutoFocus"
     << ": connected AssemblyZFocusFinder";

  return;
}

void AssemblyImageController::disable_AutoFocus()
{
  if(zfocus_finder_ == 0)
  {
    NQLog("AssemblyImageController", NQLog::Warning) << "disable_AutoFocus"
       << ": AssemblyZFocusFinder not initialized, no action taken";

    return;
  }

  connect   (this           , SIGNAL(image())                , camera_manager_, SLOT(acquireImage()));
  connect   (camera_manager_, SIGNAL(imageAcquired(cv::Mat)) , this           , SLOT(retrieve_image(cv::Mat)));

  disconnect(this           , SIGNAL(image())                , zfocus_finder_ , SLOT(acquire_image()));
  disconnect(zfocus_finder_ , SIGNAL(image_acquired(cv::Mat)), this           , SLOT(retrieve_image(cv::Mat)));

  autofocus_is_enabled_ = false;

  NQLog("AssemblyImageController", NQLog::Message) << "disable_AutoFocus"
     << ": disconnected AssemblyZFocusFinder";

  return;
}
