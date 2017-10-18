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

#include <string>

#include <opencv2/opencv.hpp>

class ZFocusFinder : public QObject
{
 Q_OBJECT

  public:

    QTimer * qt;
    LStepExpressModel* lStepExpressModel_;

    explicit ZFocusFinder(AssemblyVUEyeCamera*, LStepExpressModel* lStepExpressModel_=0, QObject* parent=0);

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

    AssemblyVUEyeCamera* camera() const { return camera_; }

  protected:

    AssemblyVUEyeCamera* camera_;

    double imageVariance(cv::Mat img_input, cv::Rect rectangle);

  public slots:

    void run_scan(double, int);
    void write_image(cv::Mat, cv::Rect);
    void process_step();
//    void run_precisionestimation(double, double, double, double, double, double, int);
//    void fill_positionvectors(int , double, double, double);
//    void toggleVacuum();

  signals:
    void getImage();
    void moveRelative(double, double, double, double);
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
