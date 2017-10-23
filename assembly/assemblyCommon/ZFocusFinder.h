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

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QString>
#include <QDateTime>
#include <QTimer>

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

    double local_range, local_steps, local_delay;
    double x_meas, y_meas, z_meas;
    double x_pickup, y_pickup, z_pickup;
    double z_prepickup;
    int iteration, iterations;

    int nTotalImages, nAcquiredImages, step;
    std::vector<double> x_vals, y_vals;
    std::vector<double> xpre_vec,ypre_vec,thetapre_vec;
    std::vector<double> xpost_vec,ypost_vec,thetapost_vec;
    double step_distance;

    AssemblyVUEyeCamera* camera() const { return camera_manager_->camera(); }

  protected:

    AssemblyUEyeCameraManager* camera_manager_;
    LStepExpressMotionManager* motion_manager_;

    double image_focus_value(const cv::Mat&);

    unsigned int bestimage_index_;

    unsigned int image_counter_;

    bool motion_enabled_;
    bool autofocus_completed_;

    double max_deltaZ_;

    QString output_dir_;

    std::vector<double> v_zrelm_vals_;

    std::vector<double> v_zposi_vals_;
    std::vector<double> v_focus_vals_;

  public slots:

    void acquire_image();

    void process_image (const cv::Mat&); //!!, cv::Rect);

    void  enable_motion();
    void disable_motion();

    void run_scan(double, int);
    void write_image(const cv::Mat&); //!!, cv::Rect);
    void process_step();
//!!    void run_precisionestimation(double, double, double, double, double, double, int);
//!!    void fill_positionvectors(int , double, double, double);
//!!    void toggleVacuum();

  signals:

    void focus(double, double, double, double);

    void image_tmp();
    void image_final();

    void image_acquired(const cv::Mat&);

    void getImage();
    void moveAbsolute(double, double, double, double);
    void updateScanImage(cv::Mat);
    void make_graph(const std::string);
    void updateText(double);
    void nextStep();
    void acquireImage();
    void makeDummies(int, double,double,double);
    void showHistos(int, QString);
    void toggleVacuum(int);
};

#endif // ZFOCUSFINDER_H
