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

#ifndef AUTOFOCUSVIEW_H
#define AUTOFOCUSVIEW_H

#include <AssemblyUEyeView.h>
#include <AssemblyVUEyeModel.h>
#include <AssemblySensorMarkerFinder.h>
#include <AssemblyVUEyeCamera.h>
#include <LStepExpressModel.h>
#include <LStepExpressMotionManager.h>
#include <AssemblyScanner.h>

#include <string>

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

#include <opencv2/opencv.hpp>

class AutoFocusView : public QWidget
{
 Q_OBJECT

  public:

    explicit AutoFocusView(AssemblyScanner*, QWidget *parent=0);

    void    connectImageProducer(const QObject*, const char*);
    void disconnectImageProducer(const QObject*, const char*);

    AssemblyScanner * cmdr_zscan;

  protected:

    void keyReleaseEvent(QKeyEvent*);

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

    void update_autofocus_parameters();

    void imageAcquired(const cv::Mat&);
    void updateImage(int, std::string);
    void make_graph(const std::string);
    void updateText(double);
    void go_to_focal_point();

  signals:

    void autofocus_parameters(const double, const int);
    void moveRelative(double,double,double,double);
    void graph_made(const cv::Mat&);
};

#endif // AUTOFOCUSVIEW_H
