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
#include <Log.h>

ImageController::ImageController(AssemblyVUEyeCamera* camera, ZFocusFinder* zfocus_finder, QObject* parent) :
  QObject(parent),
  zfocus_finder_(zfocus_finder),
  is_enabled_(false),
  autofocus_is_enabled_(false)
{
  if(!camera)
  {
    const std::string log("initialization error: null pointer to AssemblyVUEyeCamera object");
    Log::KILL("ImageController::ImageController -- "+log);
  }

  if(zfocus_finder_)
  {
    if(zfocus_finder_->camera() != camera)
    {
      Log::KILL("ImageController::ImageController -- target input camera differs from ZFocusFinder.camera");

      return;
    }
  }

  camera_manager_ = new AssemblyUEyeCameraManager(camera);

  connect(this           , SIGNAL(open_camera())   , camera_manager_, SLOT(open()));
  connect(camera_manager_, SIGNAL(camera_opened()) , this           , SLOT(enable_camera()));

  connect(this           , SIGNAL(close_camera())  , camera_manager_, SLOT(close()));
  connect(camera_manager_, SIGNAL(camera_closed()) , this           , SLOT(disable_camera()));

  connect(this           , SIGNAL(image())                , camera_manager_, SLOT(acquire_image()));
  connect(camera_manager_, SIGNAL(image_acquired(cv::Mat)), this           , SLOT(retrieve_image(cv::Mat)));
}

ImageController::~ImageController() {}

void ImageController::enable()
{
  is_enabled_ = true;

  emit open_camera();
}

void ImageController::disable()
{
  is_enabled_ = false;

  emit close_camera();
}

void ImageController::acquire_image()
{
  emit image();
}

void ImageController::enable_camera()
{
  emit camera_enabled();
}

void ImageController::disable_camera()
{
  emit camera_disabled();
}

void ImageController::retrieve_image(const cv::Mat& a_mat)
{
  emit image_acquired(a_mat);
}

void ImageController::enable_AutoFocus()
{
  if(zfocus_finder_ == 0)
  {
    NQLog("ImageController::ImageController") << "ZFocusFinder not initialized, auto-focusing not enabled";

    return;
  }

  disconnect(this           , SIGNAL(image())                , camera_manager_, SLOT(acquire_image()));
  disconnect(camera_manager_, SIGNAL(image_acquired(cv::Mat)), this           , SLOT(retrieve_image(cv::Mat)));

  connect   (this           , SIGNAL(image())                , zfocus_finder_ , SLOT(execute()));
  connect   (zfocus_finder_ , SIGNAL(image_acquired(cv::Mat)), this           , SLOT(retrieve_image(cv::Mat)));

  autofocus_is_enabled_ = true;

  NQLog("ImageController::enable_AutoFocus") << "connected ZFocusFinder";

  return;
}

void ImageController::disable_AutoFocus()
{
  if(zfocus_finder_ == 0)
  {
    NQLog("ImageController::disable_AutoFocus") << "ZFocusFinder not initialized, no action taken";

    return;
  }

  connect   (this           , SIGNAL(image())                , camera_manager_, SLOT(acquire_image()));
  connect   (camera_manager_, SIGNAL(image_acquired(cv::Mat)), this           , SLOT(retrieve_image(cv::Mat)));

  disconnect(this           , SIGNAL(image())                , zfocus_finder_ , SLOT(acquire_image()));
  disconnect(zfocus_finder_ , SIGNAL(image_acquired(cv::Mat)), this           , SLOT(retrieve_image(cv::Mat)));

  autofocus_is_enabled_ = false;

  NQLog("ImageController::disable_AutoFocus") << "disconnected ZFocusFinder";

  return;
}
