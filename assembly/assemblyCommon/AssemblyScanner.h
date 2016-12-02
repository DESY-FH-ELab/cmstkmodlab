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

#include <TGraph.h>
#include <TCanvas.h>


//vision
#include <AssemblyUEyeView.h>
#include "AssemblyVUEyeModel.h"
#include "AssemblySensorMarkerFinder.h"
#include "AssemblyVUEyeCamera.h"

//motion
#include "LStepExpressModel.h"
#include "LStepExpressMotionManager.h"



class AssemblyScanner : public QObject
{
  Q_OBJECT

public:
  AssemblyVUEyeModel *uEyeModel_;
  AssemblyVUEyeCamera* camera_l;
  LStepExpressModel* lStepExpressModel_;
  LStepExpressMotionManager* motionManager_;
  TGraph *gr;
  TCanvas *canvas;

  explicit AssemblyScanner(AssemblyVUEyeModel *uEyeModel_);

  double local_range, local_steps, local_delay;
  int nTotalImages, nAcquiredImages;

protected:
    double imageVariance(cv::Mat img_input);

public slots:
  void run_scan(double, int);
  void enable_autofocus(int);
  void write_image(cv::Mat);

    
signals:
    void getImage();
    void updateScanImage(cv::Mat);

};


#endif // ASSEMBLYAUTOFOCUS_H
