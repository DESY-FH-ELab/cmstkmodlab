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

#include <SnapshotController.h>

#include <QApplication>

#include <nqlogger.h>
#include <Log.h>

SnapshotController:: SnapshotController(AssemblyVUEyeCamera* camera, ZFocusFinder* zfocus_finder, QObject* parent)
 : QObject(parent),
   camera_(camera),
   zfocus_finder_(zfocus_finder),
   is_enabled_(false),
   autofocus_is_enabled_(false)
{
  if(!camera_)
  {
    NQLog("SnapshotController::SnapshotController") << "camera initialization failed";

    const std::string log("initialization error: null pointer to AssemblyVUEyeCamera object");
    Log::KILL("SnapshotController::SnapshotController -- "+log);
  }

  crosscheck_inputs();

  connect(this   , SIGNAL( open_camera()), camera_, SLOT(open        ()));
  connect(this   , SIGNAL(  use_camera()), camera_, SLOT(acquireImage()));
  connect(this   , SIGNAL(close_camera()), camera_, SLOT(close       ()));

  connect(camera_, SIGNAL(cameraOpened        ()), this, SLOT(enable_camera        ()));
  connect(camera_, SIGNAL(imageAcquired(cv::Mat)), this, SLOT(retrieve_image(cv::Mat)));
  connect(camera_, SIGNAL(cameraClosed        ()), this, SLOT(disable_camera       ()));
}

SnapshotController::~SnapshotController() {}

void SnapshotController::enable()
{
  is_enabled_ = true;

  emit open_camera();
}

void SnapshotController::disable()
{
  is_enabled_ = false;

  emit close_camera();
}

void SnapshotController::acquire_image()
{
  emit use_camera();
}

void SnapshotController::enable_camera()
{
  emit camera_enabled();
}

void SnapshotController::disable_camera()
{
  emit camera_disabled();
}

void SnapshotController::retrieve_image(const cv::Mat& a_mat)
{
  emit image_acquired(a_mat);
}

void SnapshotController::enable_AutoFocus()
{
  if(zfocus_finder_ == 0)
  {
    NQLog("SnapshotController::SnapshotController") << "ZFocusFinder not initialized, auto-focusing not enabled";

    return;
  }

  crosscheck_inputs();

//!!!!  connect   (zfocus_ctr_       , SIGNAL(moveRelative(double, double, double, double))        , motionManager_, SLOT(moveRelative(double, double, double, double)));
//!!!!  connect   (lStepExpressModel_, SIGNAL(motionFinished())                                    , camera_       , SLOT(acquireImage()));
//!!!!  connect   (camera_           , SIGNAL(imageAcquired(cv::Mat))                              , finder_       , SLOT(findMarker_templateMatching(cv::Mat, cv::Mat)));
//!!!!  connect   (finder_           , SIGNAL(getImageBlur(cv::Mat, cv::Rect))                     , cmdr_zscan    , SLOT(write_image(cv::Mat, cv::Rect)));
//!!!!  connect   (cmdr_zscan        , SIGNAL(make_graph(std::vector<double>, std::vector<double>)), autoFocusView_, SLOT(make_graph(std::vector<double>, std::vector<double>)));
//!!!!  connect   (cmdr_zscan        , SIGNAL(updateText(double))                                  , autoFocusView_, SLOT(updateText(double)));

/*                                                                                                                                                                                                                  
      connect   (cmdr_zscan        , SIGNAL(moveRelative(double, double, double, double))        , motionManager_, SLOT(moveRelative(double, double, double, double)));                                             
      connect   (lStepExpressModel_, SIGNAL(motionFinished())                                    , camera_       , SLOT(acquireImage()));                                                                           
      connect   (camera_           , SIGNAL(imageAcquired(cv::Mat))                              , finder_       , SLOT(findMarker_templateMatching(cv::Mat, cv::Mat)));                                            
      connect   (finder_           , SIGNAL(getImageBlur(cv::Mat, cv::Rect))                     , cmdr_zscan    , SLOT(write_image(cv::Mat, cv::Rect)));                                                           
      connect   (cmdr_zscan        , SIGNAL(make_graph(std::vector<double>, std::vector<double>)), autoFocusView_, SLOT(make_graph(std::vector<double>, std::vector<double>)));                                     
      connect   (cmdr_zscan        , SIGNAL(updateText(double))                                  , autoFocusView_, SLOT(updateText(double)));                                                                       
                                                                                                                                                                                                                   */

  autofocus_is_enabled_ = true;

  NQLog("SnapshotController::enable_AutoFocus") << "signal-slot connections enabled";

  return;
}

void SnapshotController::disable_AutoFocus()
{
/*                                                                                                                                                                                                                  
      disconnect(cmdr_zscan        , SIGNAL(moveRelative(double, double, double, double))        , motionManager_, SLOT(moveRelative(double, double, double, double)));                                             
      disconnect(lStepExpressModel_, SIGNAL(motionFinished())                                    , camera_       , SLOT(acquireImage()));                                                                           
      disconnect(camera_           , SIGNAL(imageAcquired(cv::Mat))                              , finder_       , SLOT(findMarker_templateMatching(cv::Mat, cv::Mat)));                                            
      disconnect(finder_           , SIGNAL(getImageBlur(cv::Mat, cv::Rect))                     , cmdr_zscan    , SLOT(write_image(cv::Mat, cv::Rect)));                                                           
      disconnect(cmdr_zscan        , SIGNAL(make_graph(std::vector<double>, std::vector<double>)), autoFocusView_, SLOT(make_graph(std::vector<double>, std::vector<double>)));                                     
      disconnect(cmdr_zscan        , SIGNAL(updateText(double))                                  , autoFocusView_, SLOT(updateText(double)));                                                                       
                                                                                                                                                                                                                    
      NQLog("AssemblyMainWindow::changeState_AutoFocus") << "signal-slot connections disabled";                                                                                                                     
*/

  autofocus_is_enabled_ = false;

  NQLog("SnapshotController::disable_AutoFocus") << "signal-slot connections disabled";

  return;
}

void SnapshotController::crosscheck_inputs() const
{
  if(zfocus_finder_->camera() != camera_)
  {
    Log::KILL("SnapshotController::SnapshotController -- SnapshotController.camera differs from ZFocusFinder.camera");

    return;
  }

  return;
}
