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

#ifndef ASSEMBLYASSEMBLER_H
#define ASSEMBLYASSEMBLER_H

#include <LStepExpressMotionManager.h>

#include <string>
#include <vector>

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

#include <opencv2/opencv.hpp>

class AssemblyAssembler : public QObject
{
 Q_OBJECT

  public:
    explicit AssemblyAssembler(LStepExpressMotionManager*, QObject* parent=0);
    virtual ~AssemblyAssembler() {}

    double marker_x, marker_y, marker_z, marker_theta;

//    double local_range, local_steps, local_delay;
    double x_assembly, y_assembly, z_assembly;
    double x_bottom, y_bottom, z_bottom;
    double x_top, y_top, z_top;
    double z_prepickup_distance, z_spacer_thickness, z_sensor_thickness;
    double platform_rotation;

    int nTotalImages, nAcquiredImages, step, alignment_step;
    std::vector<double> x_vals, y_vals;
    std::vector<double> xpre_vec,ypre_vec,thetapre_vec;
    std::vector<double> xpost_vec,ypost_vec,thetapost_vec;
    double step_distance;

  protected:

    LStepExpressMotionManager* motion_manager_;

    double imageVariance(cv::Mat img_input, cv::Rect rectangle);

    bool motion_enabled_;

  public slots:

    void run_scan(double, int);
    void write_image(cv::Mat, cv::Rect);
    void run_sandwitchassembly(double, double, double, double, double, double, double, double, double);
    void process_step();
    void run_alignment(int, double, double, double);
    void launch_next_alignment_step();
    void fill_positionvectors(int, double, double, double);

    void  enable_motion();
    void disable_motion();

    void move_relative(const double, const double, const double, const double);

    void stop_motion();

  signals:

    void getImage();
    void moveRelative(const double, const double, const double, const double);
    void moveAbsolute(const double, const double, const double, const double);
    void updateScanImage(cv::Mat);
    void make_graph(const std::string);
    void updateText(double);
    void nextStep();
    void nextAlignmentStep(int, double, double, double);
    void acquireImage();
    void makeDummies(int, double,double,double);
    void showHistos(int, QString);
    void toggleVacuum(int);

    void motion_finished();
};

#endif // ASSEMBLYASSEMBLER_H
