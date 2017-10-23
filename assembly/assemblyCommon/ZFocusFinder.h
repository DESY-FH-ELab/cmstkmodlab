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

#ifndef ZFOCUSFINDER_H
#define ZFOCUSFINDER_H

#include <AssemblyVUEyeCamera.h>
#include <AssemblyUEyeCameraManager.h>
#include <LStepExpressModel.h>
#include <LStepExpressMotionManager.h>

#include <QObject>
#include <QString>

#include <vector>
#include <string>

#include <opencv2/opencv.hpp>

class ZFocusFinder : public QObject
{
 Q_OBJECT

  public:

    QTimer* qt;
    LStepExpressModel* lStepExpressModel_;

    explicit ZFocusFinder(AssemblyVUEyeCamera*, LStepExpressModel* lStepExpressModel_, QObject* parent=0);

    AssemblyVUEyeCamera*       camera()         const { return camera_manager_->camera(); }
    LStepExpressMotionManager* motion_manager() const { return motion_manager_; }

  protected:

    AssemblyUEyeCameraManager* camera_manager_;
    LStepExpressMotionManager* motion_manager_;

    bool motion_enabled_;

    bool   focus_completed_;
    int    focus_pointN_;
    double focus_zsigma_;

    double zposi_init_;
    double zposi_min_;
    double zposi_max_;

    int                   zrelm_index_;
    std::vector<double> v_zrelm_vals_;

    std::vector<double> v_zposi_vals_;
    std::vector<double> v_focus_vals_;

    int image_counter_;

    QString output_dir_;

    static int exe_counter_;
    static int focus_pointN_max_;

    double image_focus_value(const cv::Mat&);

  public slots:

    void  enable_motion();
    void disable_motion();

    void update_focus_inputs(const double, const int);

    void execute();

    void acquire_image();

    void process_image(const cv::Mat&);

  signals:

    void initialized();

    void focus(double, double, double, double);

    void next_zpoint();

    void image_acquired(const cv::Mat&);

    void show_zscan(const std::string);

    void update_text(const double);
};

#endif // ZFOCUSFINDER_H
