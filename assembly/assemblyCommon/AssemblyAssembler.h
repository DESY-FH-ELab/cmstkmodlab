#ifndef ASSEMBLYASSEMBLER_H
#define ASSEMBLYASSEMBLER_H

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




#include <math.h>       /* atan */


using namespace std;


class AssemblyAssembler : public QObject
{
  Q_OBJECT

    

public:
  LStepExpressModel* lStepExpressModel_;
  LStepExpressMotionManager* motionManager_;
    
  double marker_x, marker_y, marker_z, marker_theta;


  explicit AssemblyAssembler(LStepExpressModel* lStepExpressModel_);

  //double local_range, local_steps, local_delay;
  double x_assembly, y_assembly, z_assembly;
  double x_bottom, y_bottom, z_bottom;
  double x_top, y_top, z_top;
  double z_prepickup_distance, z_spacer_thickness, z_sensor_thickness;
  double platform_rotation;
    
  int nTotalImages, nAcquiredImages, step, alignment_step;
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
  void run_sandwitchassembly(double, double, double, double, double, double, double, double, double);
  void process_step();
  void run_alignment(int, double, double, double);

  void fill_positionvectors(int , double, double, double);

    
signals:
    void getImage();
    void moveRelative(double, double, double, double);
    void moveAbsolute(double, double, double, double);
    void updateScanImage(cv::Mat);
    void make_graph(const string);
    void updateText(double);
    void nextStep();
    void nextAlignmentStep();
    void acquireImage();
    void makeDummies(int, double,double,double);
    void showHistos(int, QString);
    void toggleVacuum(int);

};


#endif // ASSEMBLYASSEMBLER_H
