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

#ifndef SNAPSHOTCONTROLLER_H
#define SNAPSHOTCONTROLLER_H

#include <AssemblyVUEyeCamera.h>
#include <ZFocusFinder.h>

#include <QObject>

#include <opencv2/opencv.hpp>

/*! \brief SnapshotController: CONTROLLER
 *           Wrapper class for camera model including
 *           auto-focusing (vertical movement of the motion stage)
 */
class SnapshotController : public QObject
{
 Q_OBJECT

  public:

    explicit SnapshotController(AssemblyVUEyeCamera*, ZFocusFinder* zff=0, QObject* parent=0);
    virtual ~SnapshotController();

    bool is_enabled() const { return is_enabled_; }
    bool autofocus_is_enabled() const { return autofocus_is_enabled_; }

  protected:

    AssemblyVUEyeCamera* camera_;
    ZFocusFinder* zfocus_finder_;

    bool is_enabled_;
    bool autofocus_is_enabled_;

    void crosscheck_inputs() const;

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

    void camera_enabled();
    void camera_disabled();

    void image_acquired(const cv::Mat&);
};

#endif // SNAPSHOTCONTROLLER_H
