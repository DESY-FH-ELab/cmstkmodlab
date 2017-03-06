#ifndef ASSEMBLYSCANNER_H
#define ASSEMBLYSCANNER_H

#include <string>
#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>

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


//motion
#include "LStepExpressModel.h"
#include "LStepExpressMotionManager.h"


using namespace std;


class AssemblyScanner : public QObject
{
  Q_OBJECT

public:
  LStepExpressModel* lStepExpressModel_;
  LStepExpressMotionManager* motionManager_;
    


  explicit AssemblyScanner(LStepExpressModel* lStepExpressModel_);

  double local_range, local_steps, local_delay;
  double x_meas, y_meas, z_meas;
  double x_pickup, y_pickup, z_pickup;
  double z_prepickup;
  int iteration, iterations;
    
  int nTotalImages, nAcquiredImages, step;
  vector<double> x_vals, y_vals;
  vector<double> xpre_vec,ypre_vec,thetapre_vec;
  vector<double> xpost_vec,ypost_vec,thetapost_vec;
  double step_distance;
  std::ofstream outfile;

protected:
    double imageVariance(cv::Mat img_input, cv::Rect rectangle);

public slots:
  void run_scan(double, int);
  void write_image(cv::Mat, cv::Rect);
  void run_precisionestimation(double, double, double, double, double, double, int);
  void process_step();
  void fill_positionvectors(int , double, double, double);

    
signals:
    void getImage();
    void moveRelative(double, double, double, double);
    void moveAbsolute(double, double, double, double);
    void updateScanImage(cv::Mat);
    void make_graph(const string);
    void updateText(double);
    void nextStep();
    void acquireImage();
    void makeDummies(int, double,double,double);
    void showHistos(int, QString);
    void toggleVacuum(int);

};


#endif // ASSEMBLYAUTOFOCUS_H
