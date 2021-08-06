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

#ifndef ASSEMBLYZFOCUSFINDER_H
#define ASSEMBLYZFOCUSFINDER_H

#include <AssemblyVUEyeCamera.h>
#include <LStepExpressMotionManager.h>

#include <QObject>
#include <QString>

#include <vector>
#include <string>

#include <opencv2/opencv.hpp>

class AssemblyZFocusFinder : public QObject
{
 Q_OBJECT

  public:

    explicit AssemblyZFocusFinder(const QString&, const AssemblyVUEyeCamera*, const LStepExpressMotionManager*, QObject* parent=nullptr);

    const AssemblyVUEyeCamera*       camera_manager() const { return camera_manager_; }
    const LStepExpressMotionManager* motion_manager() const { return motion_manager_; }

    double zrange() const { return focus_zrange_; }
    int    points() const { return focus_pointN_; }

    struct focus_info
    {
      double z_position;
      double focus_disc;
    };

  protected:

    QString output_dir_prepath_;

    const AssemblyVUEyeCamera*       camera_manager_;
    const LStepExpressMotionManager* motion_manager_;

    static int exe_counter_;

    bool motion_enabled_;

    bool   focus_completed_;
    int    focus_pointN_max_;
    int    focus_pointN_;
    double focus_zrange_max_;
    double focus_zrange_;
    double focus_stepsize_min_;

    double zposi_init_;

    int zrelm_index_;

    std::string output_dir_;

    std::vector<double>     v_zrelm_vals_;
    std::vector<focus_info> v_focus_vals_;

    double image_focus_value(const cv::Mat&);

  public slots:

    void  enable_motion();
    void disable_motion();

    void update_focus_config(const double, const int);

    void acquire_image();

    void test_focus();

    void process_image(const cv::Mat&);

    void emergencyStop();

  signals:

    void next_zpoint();

    void focus_config_request();
    void updated_focus_config();

    void focus(const double, const double, const double, const double);

    void image_acquired(const cv::Mat&);

    void show_zscan(const QString&);

    void text_update_request(const double);

    void emergencyStopped();

    void sig_update_progBar(int);
};

#endif // ASSEMBLYZFOCUSFINDER_H
