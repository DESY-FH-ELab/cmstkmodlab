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

#ifndef ASSEMBLYIMAGECONTROLLER_H
#define ASSEMBLYIMAGECONTROLLER_H

#include <AssemblyVUEyeCamera.h>
#include <AssemblyZFocusFinder.h>

#include <QObject>

#include <opencv2/opencv.hpp>

// \brief AssemblyImageController: CONTROLLER
//         Manager to use camera model
//         with option to apply auto-focusing
//         (vertical movement of the motion stage, see AssemblyZFocusFinder)
class AssemblyImageController : public QObject
{
 Q_OBJECT

  public:

    explicit AssemblyImageController(const AssemblyVUEyeCamera*, const AssemblyZFocusFinder* zff=nullptr, QObject* parent=nullptr);
    virtual ~AssemblyImageController();

    bool is_enabled()           const { return is_enabled_; }
    bool autofocus_is_enabled() const { return autofocus_is_enabled_; }

    const AssemblyVUEyeCamera* camera_manager() const { return camera_manager_; }

  protected:

    const AssemblyVUEyeCamera*  camera_manager_;
    const AssemblyZFocusFinder* zfocus_finder_;

    bool is_enabled_;
    bool autofocus_is_enabled_;
    bool autofocus_to_be_disabled_;

  public slots:

    void  enable();
    void disable();

    void acquire_image();

    void  enable_camera();
    void disable_camera();

    void retrieve_image(const cv::Mat&);

    void  enable_autofocus();
    void disable_autofocus();

    void acquire_autofocused_image();
    void restore_autofocus_settings();

  signals:

    void  open_camera();
    void   use_camera();
    void close_camera();

    void run_autofocus();

    void image();

    void camera_enabled();
    void camera_disabled();

    void image_acquired(const cv::Mat&);
    void image_acquired();

    void autofocused_image_request();
    void autofocused_image_acquired();
};

#endif // ASSEMBLYIMAGECONTROLLER_H
