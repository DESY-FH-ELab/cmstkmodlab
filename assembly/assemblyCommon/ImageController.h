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

#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H

#include <AssemblyVUEyeCamera.h>
#include <ZFocusFinder.h>

#include <QObject>

#include <opencv2/opencv.hpp>

// \brief ImageController: CONTROLLER
//          Wrapper class for camera model including
//          auto-focusing (vertical movement of the motion stage)
class ImageController : public QObject
{
 Q_OBJECT

  public:

    explicit ImageController(AssemblyVUEyeCamera*, ZFocusFinder* zff=0, QObject* parent=0);
    virtual ~ImageController();

    bool is_enabled()           const { return is_enabled_; }
    bool autofocus_is_enabled() const { return autofocus_is_enabled_; }

    AssemblyVUEyeCamera* camera_manager() const { return camera_manager_; }

  protected:

    AssemblyVUEyeCamera* camera_manager_;
    ZFocusFinder*        zfocus_finder_;

    bool is_enabled_;
    bool autofocus_is_enabled_;

  public slots:

    void  enable();
    void disable();

    void acquire_image();

    void  enable_camera();
    void disable_camera();

    void retrieve_image(const cv::Mat&);

    void  enable_AutoFocus();
    void disable_AutoFocus();

  protected slots:

  signals:

    void  open_camera();
    void   use_camera();
    void close_camera();

    void run_autofocus();

    void image();

    void camera_enabled();
    void camera_disabled();

    void image_acquired(const cv::Mat&);
};

#endif // IMAGECONTROLLER_H
