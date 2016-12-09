#ifndef ASSEMBLYSCANNER_H
#define ASSEMBLYSCANNER_H

#include <string>

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

//vision
#include <AssemblyUEyeView.h>
#include "AssemblyVUEyeModel.h"
#include "AssemblySensorMarkerFinder.h"
#include "AssemblyVUEyeCamera.h"

//motion
#include "LStepExpressModel.h"
#include "LStepExpressMotionManager.h"


using namespace std;


class AssemblyScanner : public QObject
{
  Q_OBJECT

public:
  AssemblyVUEyeModel *uEyeModel_;
  AssemblyVUEyeCamera* camera_l;
  LStepExpressModel* lStepExpressModel_;
  LStepExpressMotionManager* motionManager_;

  explicit AssemblyScanner(AssemblyVUEyeModel *uEyeModel_, LStepExpressModel* lStepExpressModel_);

  double local_range, local_steps, local_delay;
  int nTotalImages, nAcquiredImages;
  vector<double> x_vals, y_vals;
  double step_distance;

protected:
    double imageVariance(cv::Mat img_input);

public slots:
  void run_scan(double, int);
  void enable_autofocus(int);
  void write_image(cv::Mat);

    
signals:
    void getImage();
    void moveRelative(double, double, double, double);
    void updateScanImage(cv::Mat);
    void make_graph(vector<double>, vector<double>);
};


#endif // ASSEMBLYAUTOFOCUS_H
