#ifndef ASSEMBLYMODULEASSEMBLER_H
#define ASSEMBLYMODULEASSEMBLER_H

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>

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

//relay card
#include "../../devices/Conrad/ConradCommunication.h"
#include "../../devices/Conrad/ConradController.h"



class AssemblyModuleAssembler : public QWidget
{
    Q_OBJECT
    
public:

    explicit AssemblyModuleAssembler(QWidget *parent = 0);
    void connectImageProducer(const QObject* sender, const char* signal);
    void disconnectImageProducer(const QObject* sender, const char* signal);
    double pickup_position;
    QLineEdit * lE1;
    QLineEdit * lE2;
    QLineEdit * lE3;
    QLineEdit * lE4;
    QLineEdit * lE5;
    QLineEdit * lE6;
    
    ApplicationConfig* config;
    LStepExpressModel* lStepExpressModel_;
    LStepExpressMotionManager* motionManager_;
    
    
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
    void updateImage(int,std::string);
    void updateText(int,double, double, double);

    
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
    ConradController * cnrd1;

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
    
    explicit AssemblyAttacher(AssemblyModuleAssembler *parent = 0, std::string ="test", double drop =0.0, int =0);
    double local_drop;
    QPushButton* button1;
    QLineEdit *lineEdit1;

    
protected:
    
public slots:
   void dropAttach();
    
signals:
    void moveRelative(double,double,double,double);
    
};



class AssemblyMountChecker : public QWidget
{
    Q_OBJECT
public:
    
    explicit AssemblyMountChecker(QWidget *parent = 0, std::string ="test", double x =0.0, double y =0.0, double z  =0.0, double a  =0.0, int =0);
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
    
    explicit AssemblyCommander(QWidget *parent = 0, std::string ="test", double x =0.0, double y =0.0, double z  =0.0, double a  =0.0, int =0);
    double local_x, local_y, local_z, local_a;
    QPushButton* button1;
    QLineEdit *lineEdit1;
    
    
protected:
    public slots:
      void goToTarget();
    
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


protected:
    public slots:
        void allign();
        void setDown();
    
signals:
    void moveRelative(double,double,double,double);
    void locateSetdowncorner(int);
    
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
    QGroupBox *groupBox;
    QRadioButton *radio1;
    QRadioButton *radio2;
    QRadioButton *radio3;
    QVBoxLayout *vbox;
protected:
    public slots:
        void locateSensor_circleSeed(int);
        void locateSensor_templateMatching(int);
        void foundsensor(int);
        void locatePickup();

    
signals:
    void updateImage(int, std::string);
    void foundSensor(int);
    void sendPosition(int, double, double, double);
    void locatePickupCorner_circleSeed(int);
    void locatePickupCorner_templateMatching(int);
};



#endif //
