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

#include <AssemblyUEyeView.h>

#include <QWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QKeyEvent>
#include <QString>

#include <opencv2/opencv.hpp>

class AssemblyAutoFocus : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyAutoFocus(QWidget* parent=0);

  void    connectImageProducer(const QObject* sender, const char* signal);
  void disconnectImageProducer(const QObject* sender, const char* signal);

 protected:

  void keyReleaseEvent(QKeyEvent* event);

  QScrollArea *scrollArea_1;
  AssemblyUEyeView *imageView_1;

  QScrollArea *scrollArea_2;
  AssemblyUEyeView *imageView_2;

  QPushButton* button1;
  QPushButton* button2;
  QLineEdit * lE1;
  QLineEdit * lE2;

  QCheckBox* checkbox;

 public slots:

  void configure_scan();
  void make_graph(const QString&);
  void updateText(double);
  void go_to_focal_point();
  void updateImage(const int, const QString&);

 signals:
  void run_scan(double,int);
  void moveRelative(double,double,double,double);
  void graph_made(const cv::Mat&);
};

#endif // ASSEMBLYAUTOFOCUS_H
