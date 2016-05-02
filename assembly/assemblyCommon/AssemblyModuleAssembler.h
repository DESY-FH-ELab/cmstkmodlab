#ifndef ASSEMBLYMODULEASSEMBLER_H
#define ASSEMBLYMODULEASSEMBLER_H

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>

#include <AssemblyVUEyeCamera.h>
#include <AssemblyVMarkerFinder.h>
#include <AssemblyUEyeView.h>


//motion
#include <ApplicationConfig.h>
#include "LStepExpressModel.h"
#include "LStepExpressSettings.h"
#include "LStepExpressMotionManager.h"
#include "LStepExpressMotionThread.h"
#include "LStepExpressSettingsWidget.h"
#include "LStepExpressWidget.h"
#include "LStepExpressJoystickWidget.h"



class AssemblyModuleAssembler : public QWidget
{
    Q_OBJECT
public:

    explicit AssemblyModuleAssembler(QWidget *parent = 0);
    void connectImageProducer(const QObject* sender, const char* signal);
    void disconnectImageProducer(const QObject* sender, const char* signal);
    double pickup_position;
    //QPushButton* button1 =  new QPushButton(qname, this);
    
    
    
protected:

    void keyReleaseEvent(QKeyEvent *event);

    QScrollArea *scrollArea_;
    AssemblyUEyeView *imageView_;

    cv::Mat image_;

public slots:

    void snapShot();
    void imageAcquired(const cv::Mat&);
    void gotoPickup();
    void updateImage(std::string);
    
signals:
    void moveAbsolute(double,double,double,double);


};



class AssemblyVacuumToggler : public QWidget
{
    Q_OBJECT
public:
    
    explicit AssemblyVacuumToggler(QWidget *parent = 0, std::string ="test", double a  = 0.0, int =0);
    double local_x, local_y, local_z, local_a;
    QPushButton* button1;
    QLineEdit *lineEdit1;
    QLabel* ql;
    bool state;
    
protected:
    
public slots:
    void toggleVacuum();
    
signals:

    
};





class AssemblyAttacher : public QWidget
{
    Q_OBJECT
public:
    
    explicit AssemblyAttacher(QWidget *parent = 0, std::string ="test", double drop =0.0, int =0);
    double local_drop;
    QPushButton* button1;
    QLineEdit *lineEdit1;

    
protected:
    
public slots:
   void dropAttach();
    
signals:
    void moveRelative(double,double,double,double);
    
};








class AssemblyCommander : public QWidget
{
    Q_OBJECT
public:
    
    explicit AssemblyCommander(QWidget *parent = 0, std::string ="test", double x =0.0, double y =0.0, double z  =0.0, double a  =0.0, int =0);
    double local_x, local_y, local_z, local_a;
    QPushButton* button1;
    QLineEdit *lineEdit1;
 //   void connectImageProducer(const QObject* sender, const char* signal);
  //  void disconnectImageProducer(const QObject* sender, const char* signal);
    
  //  double pickup_position;
    
    
protected:
    
   // void keyReleaseEvent(QKeyEvent *event);
    
   // QScrollArea *scrollArea_;
   // AssemblyUEyeView *imageView_;
    
    //cv::Mat image_;
    
public slots:
    
    //void snapShot();
    //void imageAcquired(const cv::Mat&);
    void goToTarget();
    void updateText(double,double,double);

    
signals:
   void moveAbsolute(double,double,double,double);
    
    
};




class AssemblyAlligner : public QWidget
{
    Q_OBJECT
public:
    
    explicit AssemblyAlligner(QWidget *parent = 0, std::string ="test", double a  = 0.0, int =0);
    double local_x, local_y, local_z, local_a;
    QPushButton* button1;
    QLineEdit *lineEdit1;
    //   void connectImageProducer(const QObject* sender, const char* signal);
    //  void disconnectImageProducer(const QObject* sender, const char* signal);
    
    //  double pickup_position;
    
    
protected:
    
    // void keyReleaseEvent(QKeyEvent *event);
    
    // QScrollArea *scrollArea_;
    // AssemblyUEyeView *imageView_;
    
    //cv::Mat image_;
    
    public slots:
    
    //void snapShot();
    //void imageAcquired(const cv::Mat&);
    void allign();
   // void locateSensor();
    
signals:
    void moveRelative(double,double,double,double);
    
    
};




class AssemblySensorLocator : public QWidget
{
    Q_OBJECT
public:
    
    explicit AssemblySensorLocator(QWidget *parent = 0, std::string ="test", double a  = 0.0, int =0);
    double local_x, local_y, local_z, local_a;
    QPushButton* button1;
    QLineEdit *lineEdit1;
    QLabel* ql;
    
    //   void connectImageProducer(const QObject* sender, const char* signal);
    //  void disconnectImageProducer(const QObject* sender, const char* signal);
    
    //  double pickup_position;
    
    
protected:
    
    // void keyReleaseEvent(QKeyEvent *event);
    
    // QScrollArea *scrollArea_;
    // AssemblyUEyeView *imageView_;
    
    //cv::Mat image_;
    
    public slots:
    
    //void snapShot();
    //void imageAcquired(const cv::Mat&);
    void locateSensor();
    void foundsensor(int);
    
signals:
 //   void moveRelative(double,double,double,double);
    void updateImage(std::string);
    void foundSensor(int);
    void sendPosition(double, double, double);

    
};








#endif //