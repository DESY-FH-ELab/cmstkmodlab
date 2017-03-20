#ifndef ASSEMBLYMODULEASSEMBLER_H
#define ASSEMBLYMODULEASSEMBLER_H

#include <string>

#include <opencv2/opencv.hpp>

#include <QWidget>
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
//#include <AssemblyVUEyeCamera.h>
//#include <AssemblyVMarkerFinder.h>
#include <AssemblyUEyeView.h>
#include "AssemblyVUEyeModel.h"
#include "AssemblySensorMarkerFinder.h"

#include "AssemblyVUEyeCamera.h"

//motion
#include "LStepExpressModel.h"
//#include "LStepExpressSettings.h"
#include "LStepExpressMotionManager.h"
//#include "LStepExpressMotionThread.h"

//#include "LStepExpressSettingsWidget.h"
//#include "LStepExpressWidget.h"
//#include "LStepExpressJoystickWidget.h"

//relay card for vacuum control
//#include "ConradModel.h"


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

  explicit AssemblyModuleAssembler(AssemblyVUEyeModel *uEyeModel_, AssemblySensorMarkerFinder * finder_,LStepExpressModel* lStepExpressModel_,
                                   QWidget *parent = 0);
  void connectImageProducer(const QObject* sender, const char* signal);
  void disconnectImageProducer(const QObject* sender, const char* signal);

  double pickup_position;
  QLineEdit * lE1;
  QLineEdit * lE2;
  QLineEdit * lE3;
  QLineEdit * lE4;
  QLineEdit * lE5;
  QLineEdit * lE6;


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
  void updateImage(int,QString);
  void updateText(int,double, double, double);
  void startMacro(double, double, double, double, double, double, int);

signals:

  void moveAbsolute(double,double,double,double);
  void launchPrecisionEstimation(double, double, double, double, double, double, int);
  void launchSandwitchAssembly(double, double, double, double, double, double, double, double, double);


};


class AssemblyPrecisionEstimator : public QWidget
{
    Q_OBJECT
public:
    
    explicit AssemblyPrecisionEstimator(QWidget *parent = 0, std::string text ="Estimate Assembly Precision",
                                        std::string measurement_position = "0.0,0.0,0.0", std::string pickup_position = "0.0,0.0,0.0", int iterations = 1 );
    
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


class AssemblySandwitchAssembler : public QWidget
{
    Q_OBJECT
public:
    
    explicit AssemblySandwitchAssembler(QWidget *parent = 0, std::string text ="Assemble sandwitch",
                                        std::string assembly_position = "0.0,0.0,0.0", std::string bottom_part_position = "0.0,0.0,0.0", std::string top_part_position = "0.0,0.0,0.0");
    
    //double local_x, local_y, local_z, local_a;
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
signals:
    void launchSandwitchAssembly(double, double, double, double, double, double, double, double, double);
};


class AssemblyAttacher : public QWidget
{
  Q_OBJECT

public:

  explicit AssemblyAttacher(std::string ="test", double x =0.0, double y =0.0, double z  =0.0, double a  =0.0);
  double local_x, local_y, local_z, local_a;
  QPushButton* button1;
  QLineEdit *lineEdit1;

protected:

public slots:

  void moveRelative();
    
signals:

  void moveRelative(double,double,double,double);
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

class AssemblyCommander : public QWidget
{
  Q_OBJECT

public:

  explicit AssemblyCommander(QWidget *parent = 0, std::string ="test",
                             double x =0.0, double y =0.0, double z  =0.0,
                             double a  =0.0);

  double local_x, local_y, local_z, local_a;
  QPushButton* button1;
  QLineEdit *lineEdit1;

protected:

public slots:

  void goToTarget();

signals:

  void moveAbsolute(double,double,double,double);
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
                                 double a  = 0.0, AssemblySensorMarkerFinder* finder_ = 0);

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
  int objectmode, labmode;


  QVBoxLayout *vbox1,*vbox2 ;

protected:

  std::string cacheDirectory1_;
  std::string cacheDirectory2_;

public slots:

  void foundsensor(int);
  void detectPatRecMode();

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
