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

#include <ImageController.h>
#include <nqlogger.h>
#include <Util.h>

ImageController::ImageController(AssemblyVUEyeCamera* camera, ZFocusFinder* zfocus_finder, QObject* parent) :
  QObject(parent),
  zfocus_finder_(zfocus_finder),
  is_enabled_(false),
  autofocus_is_enabled_(false)
{
  if(!camera)
  {
    NQLog("ImageController", NQLog::Fatal)
       << "initialization error: null pointer to AssemblyVUEyeCamera object";

    exit(1);
  }

  if(zfocus_finder_)
  {
    if(zfocus_finder_->camera() != camera)
    {
      NQLog("ImageController", NQLog::Fatal)
         << "target input camera differs from ZFocusFinder.camera";

      exit(1);
    }
  }

  camera_manager_ = new AssemblyUEyeCameraManager(camera);

  connect(this           , SIGNAL(open_camera())   , camera_manager_, SLOT(open()));
  connect(camera_manager_, SIGNAL(camera_opened()) , this           , SLOT(enable_camera()));

  connect(this           , SIGNAL(close_camera())  , camera_manager_, SLOT(close()));
  connect(camera_manager_, SIGNAL(camera_closed()) , this           , SLOT(disable_camera()));

  connect(this           , SIGNAL(image())                , camera_manager_, SLOT(acquire_image()));
  connect(camera_manager_, SIGNAL(image_acquired(cv::Mat)), this           , SLOT(retrieve_image(cv::Mat)));

  NQLog("ImageController", NQLog::Debug) << "constructed";
}

ImageController::~ImageController()
{
}

void ImageController::enable()
{
  is_enabled_ = true;

  NQLog("ImageController", NQLog::Debug) << "enable"
     << ": emitting signal \"open_camera\"";

  emit open_camera();
}

void ImageController::disable()
{
  is_enabled_ = false;

  NQLog("ImageController", NQLog::Debug) << "disable"
     << ": emitting signal \"close_camera\"";

  emit close_camera();
}

void ImageController::acquire_image()
{
  NQLog("ImageController", NQLog::Debug) << "acquire_image"
     << ": emitting signal \"image\"";

  emit image();
}

void ImageController::enable_camera()
{
  NQLog("ImageController", NQLog::Debug) << "enable_camera"
     << ": emitting signal \"camera_enabled\"";

  emit camera_enabled();
}

void ImageController::disable_camera()
{
  NQLog("ImageController", NQLog::Debug) << "disable_camera"
     << ": emitting signal \"camera_disabled\"";

  emit camera_disabled();
}

void ImageController::retrieve_image(const cv::Mat& a_mat)
{
  NQLog("ImageController", NQLog::Debug) << "retrieve_image"
     << ": emitting signal \"image_acquired\"";

  emit image_acquired(a_mat);
}

void ImageController::enable_AutoFocus()
{
  if(zfocus_finder_ == 0)
  {
    NQLog("ImageController", NQLog::Warning) << "enable_AutoFocus"
       << ": ZFocusFinder not initialized, auto-focusing not enabled";

    return;
  }

  disconnect(this           , SIGNAL(image())                , camera_manager_, SLOT(acquire_image()));
  disconnect(camera_manager_, SIGNAL(image_acquired(cv::Mat)), this           , SLOT(retrieve_image(cv::Mat)));

  connect   (this           , SIGNAL(image())                , zfocus_finder_ , SLOT(acquire_image()));
  connect   (zfocus_finder_ , SIGNAL(image_acquired(cv::Mat)), this           , SLOT(retrieve_image(cv::Mat)));

  autofocus_is_enabled_ = true;

  NQLog("ImageController", NQLog::Message) << "enable_AutoFocus"
     << ": connected ZFocusFinder";

  return;
}

void ImageController::disable_AutoFocus()
{
  if(zfocus_finder_ == 0)
  {
    NQLog("ImageController", NQLog::Warning) << "disable_AutoFocus"
       << ": ZFocusFinder not initialized, no action taken";

    return;
  }

  connect   (this           , SIGNAL(image())                , camera_manager_, SLOT(acquire_image()));
  connect   (camera_manager_, SIGNAL(image_acquired(cv::Mat)), this           , SLOT(retrieve_image(cv::Mat)));

  disconnect(this           , SIGNAL(image())                , zfocus_finder_ , SLOT(acquire_image()));
  disconnect(zfocus_finder_ , SIGNAL(image_acquired(cv::Mat)), this           , SLOT(retrieve_image(cv::Mat)));

  autofocus_is_enabled_ = false;

  NQLog("ImageController", NQLog::Message) << "disable_AutoFocus"
     << ": disconnected ZFocusFinder";

  return;
}
