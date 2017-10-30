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

#ifndef ASSEMBLYMODULEASSEMBLER_H
#define ASSEMBLYMODULEASSEMBLER_H

#include <string>

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QFormLayout>
#include <QScrollArea>
#include <QKeyEvent>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QRadioButton>

//vision
#include <AssemblyUEyeView.h>
#include <AssemblyVUEyeCamera.h>
#include <MarkerFinderPatRec.h>

#include "AssemblyVUEyeCamera.h"

//motion
#include "LStepExpressModel.h"
//#include "LStepExpressSettings.h"
#include "LStepExpressMotionManager.h"

//#include "LStepExpressSettingsWidget.h"
//#include "LStepExpressWidget.h"
//#include "LStepExpressJoystickWidget.h"

#include <ConradModel.h>

class AssemblyVacuumToggler : public QWidget
{
  Q_OBJECT

public:
    
  explicit AssemblyVacuumToggler(QWidget *parent = 0, std::string ="test");

  QPushButton* button1;
  QLineEdit *lineEdit1;
  //ConradModel * cnrd1;

  std::vector <QRadioButton*> valves;
  std::vector <QLabel*> labels;
      
  QLabel* ql1;
  QLabel* ql2;
  QLabel* ql3;
  QLabel* ql4;

  QRadioButton *radio1;
  QRadioButton *radio2;
  QRadioButton *radio3;
  QRadioButton *radio4;
    
  bool state;
    
protected:
    
public slots:
  void toggleVacuum();
  void updateVacuumChannelState(int, bool);
  void enableVacuumButton();
  void disableVacuumButton();
    
  
signals:
  void toggleVacuum(int);
};


class AssemblyModuleAssembler : public QWidget
{
 Q_OBJECT

public:
  explicit AssemblyModuleAssembler(AssemblyVUEyeCamera* camera, MarkerFinderPatRec* finder_, LStepExpressModel* lStepExpressModel_, QWidget* parent=0);
  void connectImageProducer(const QObject* sender, const char* signal);
  void disconnectImageProducer(const QObject* sender, const char* signal);

  double pickup_position;
  QLineEdit * lE1;
  QLineEdit * lE2;
  QLineEdit * lE3;
  QLineEdit * lE4;
  QLineEdit * lE5;
  QLineEdit * lE6;

  ConradModel * cnrd1;

  AssemblyVUEyeCamera * camera_;
  AssemblyVacuumToggler* toggle1;  //to connect vacuum signals in MainWindow

protected:

  void keyReleaseEvent(QKeyEvent *event);

  QScrollArea *scrollArea_1;
  AssemblyUEyeView *imageView_1;

  QScrollArea *scrollArea_2;
  AssemblyUEyeView *imageView_2;

  QScrollArea *scrollArea_3;
  AssemblyUEyeView *imageView_3;

  QScrollArea *scrollArea_4;
  AssemblyUEyeView *imageView_4;

  QScrollArea *scrollArea_5;
  AssemblyUEyeView *imageView_5;

  QScrollArea *scrollArea_6;
  AssemblyUEyeView *imageView_6;

  cv::Mat image_;

public slots:

  void snapShot();
  void imageAcquired(const cv::Mat&);
  void gotoPickup();
  void updateImage(const int, const QString&);
  void updateText(const int, const double, const double, const double);
  void startMacro(double, double, double, double, double, double, int);

signals:

  void moveAbsolute(double,double,double,double);
  void launchPrecisionEstimation(double, double, double, double, double, double, int);
  void launchSandwichAssembly(double, double, double, double, double, double, double, double, double);
  void launchAlignment(int, double, double, double );
};



class AssemblyPrecisionEstimator : public QWidget
{
    Q_OBJECT
public:
    
    explicit AssemblyPrecisionEstimator(QWidget *parent = 0, std::string text ="Estimate Assembly Precision",
                                        std::string measurement_position = "0.0,0.0,0.0", std::string pickup_position = "0.0,0.0,0.0", int iterations = 1 , ConradModel * conradModel_ = 0);
    
    double local_x, local_y, local_z, local_a;
    QPushButton* button1;
    
    QLabel * label1;
    QLabel * label2;
    QLabel * label3;
    
    QLineEdit *lineEdit1;
    QLineEdit *lineEdit2;
    QLineEdit *lineEdit3;

    
    protected:
    
    public slots:
    void recordPosition(double,double,double);
    void run();
signals:
    void moveAbsolute(double,double,double,double);
    void locateMarker();
    void launchPrecisionEstimation(double, double, double, double, double, double, int);
};


class AssemblySandwichAssembler : public QWidget
{
    Q_OBJECT
public:
    
    explicit AssemblySandwichAssembler(QWidget *parent = 0, std::string text ="Assemble sandwich",
                                        std::string assembly_position = "0.0,0.0,0.0", std::string bottom_part_position = "0.0,0.0,0.0", std::string top_part_position = "0.0,0.0,0.0");
    
    //double local_x, local_y, local_z, local_a;
    QPushButton* button0;
    QPushButton* button1;
    
    QLabel * label1;
    QLabel * label2;
    QLabel * label3;
    
    QLineEdit *lineEdit1;
    QLineEdit *lineEdit2;
    QLineEdit *lineEdit3;

    
    protected:
    
    public slots:
    void run();
    void run_alignment();

    signals:
    void launchSandwichAssembly(double, double, double, double, double, double, double, double, double);
    void launchAlignment(int, double, double, double );
};

class MoveWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit MoveWidget(const QString&, const QString&, const bool move_relative=false, QWidget* parent=0);
  virtual ~MoveWidget() {}

  void useMoveRelative(const bool b=false){ moveRelative_ = b; }

 protected:

  bool moveRelative_;

  QFormLayout* layout_;
  QPushButton* button_;
  QLineEdit*   liedit_;

 public slots:

  void execute();
    
 signals:

  void moveAbsolute(const double, const double, const double, const double);
  void moveRelative(const double, const double, const double, const double);
};

class AssemblyMountChecker : public QWidget
{
  Q_OBJECT

public:

  explicit AssemblyMountChecker(QWidget *parent = 0, std::string ="test",
                                double x =0.0, double y =0.0, double z  =0.0,
                                double a  =0.0, int =0);

  double local_x, local_y, local_z, local_a;
  QPushButton* button1;
  QLineEdit *lineEdit1;

protected:

public slots:

  void checkMount();

signals:

  void moveAbsolute(double,double,double,double);
  void moveRelative(double,double,double,double);
  void locateCorner(int);
  void reportCornerLocation(int);
};

class AssemblyAligner : public QWidget
{
  Q_OBJECT      
public:

  explicit AssemblyAligner(QWidget *parent = 0, std::string ="test",
                           double a  = 0.0);

  double local_x, local_y, local_z, local_a;
  QPushButton* button1;
  QLineEdit *lineEdit1;

protected:

public slots:

  void align();
  void setDown();

signals:

  void moveRelative(double,double,double,double);
  void locateSetdowncorner(int);
};

class AssemblySensorLocator : public QWidget
{
Q_OBJECT

 public:

  explicit AssemblySensorLocator(QWidget *parent = 0, std::string ="test",
                                 double a  = 0.0, MarkerFinderPatRec* finder_ = 0);

  double local_x, local_y, local_z, local_a;
  QPushButton* button1;
  QLineEdit *lineEdit1;
  QLabel* ql;
  QGroupBox *groupBox1, *groupBox2;
  QRadioButton *radio1;
  QRadioButton *radio2;
  QRadioButton *radio3;
  QRadioButton *radio31;
  QRadioButton *radio4;
  QRadioButton *radio5;

  QVBoxLayout *vbox1,*vbox2 ;

 protected:

  std::string cacheDirectory1_;
  std::string cacheDirectory2_;

 public slots:

  void foundsensor(int);
  void detectPatRecMode();
  void locatePickup(const cv::Mat&) {} //!!

 signals:

  void runObjectDetection(int, int);
  void updateImage(int, std::string);
  void foundSensor(int);
  void acquireImage();
  void sendPosition(int, double, double, double);
  void locatePickupCorner_circleSeed(int);
  void locatePickupCorner_templateMatching(cv::Mat,cv::Mat);
};

#endif // ASSEMBLYMODULEASSEMBLER_H
