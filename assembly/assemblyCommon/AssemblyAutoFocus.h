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
  cv::Mat img;
  QPushButton* button1;
  QPushButton* button2;
  QLineEdit * lE1;
  QLineEdit * lE2;

  QCheckBox *checkbox;

public slots:
  void imageAcquired(const cv::Mat&);
  void updateImage(int,std::string);
  void configure_scan();
  void make_graph(const string);
  void updateText(double);
  void go_to_focal_point();
    
signals:
  void run_scan(double,int);
  void moveRelative(double,double,double,double);
  void graph_made(const cv::Mat&);

};


#endif // ASSEMBLYAUTOFOCUS_H
