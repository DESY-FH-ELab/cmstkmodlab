#ifndef ASSEMBLYAUTOFOCUS_H
#define ASSEMBLYAUTOFOCUS_H

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


#include "AssemblyScanner.h"

class AssemblyAutoFocus : public QWidget
{
  Q_OBJECT

public:
  explicit AssemblyAutoFocus(AssemblyScanner* cmdr_zscan, QWidget *parent = 0);
  void connectImageProducer(const QObject* sender, const char* signal);
  void disconnectImageProducer(const QObject* sender, const char* signal);

  AssemblyScanner * cmdr_zscan;

 
protected:

  void keyReleaseEvent(QKeyEvent *event);

  QScrollArea *scrollArea_1;
  AssemblyUEyeView *imageView_1;

  QScrollArea *scrollArea_2;
  AssemblyUEyeView *imageView_2;
    
  cv::Mat image_;
  QPushButton* button1;
  QLineEdit * lE1;
  QCheckBox *checkbox;



public slots:

  void imageAcquired(const cv::Mat&);
  void updateImage(int,std::string);
  void updateText(int,double, double, double);
  void configure_scan();

 
signals:
  void run_scan(double,int);
  void moveRelative(double,double,double,double);
};


#endif // ASSEMBLYAUTOFOCUS_H
