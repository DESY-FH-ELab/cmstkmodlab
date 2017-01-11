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
  int nTotalImages, nAcquiredImages;
  vector<double> x_vals, y_vals;
  double step_distance;

protected:
    double imageVariance(cv::Mat img_input, cv::Rect rectangle);

public slots:
  void run_scan(double, int);
    void write_image(cv::Mat, cv::Rect);

    
signals:
    void getImage();
    void moveRelative(double, double, double, double);
    void updateScanImage(cv::Mat);
    void make_graph(const string);
    void updateText(double);

};


#endif // ASSEMBLYAUTOFOCUS_H
