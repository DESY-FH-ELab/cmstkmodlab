#ifndef ASSEMBLYUEYEASSEMBLER_H
#define ASSEMBLYUEYEASSEMBLER_H

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>
#include <QLineEdit>


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



class AssemblyUEyeAssembler : public QWidget
{
    Q_OBJECT
public:

    explicit AssemblyUEyeAssembler(QWidget *parent = 0);
    void connectImageProducer(const QObject* sender, const char* signal);
    void disconnectImageProducer(const QObject* sender, const char* signal);

    double pickup_position;
    
    
protected:

    void keyReleaseEvent(QKeyEvent *event);

    QScrollArea *scrollArea_;
    AssemblyUEyeView *imageView_;

    cv::Mat image_;

public slots:

    void snapShot();
    void imageAcquired(const cv::Mat&);
    void gotoPickup();
    
signals:
    void moveAbsolute(double,double,double,double);


};




class AssemblyUEyeAssembleCommander : public QWidget
{
    Q_OBJECT
public:
    
    explicit AssemblyUEyeAssembleCommander(QWidget *parent = 0, std::string ="test", double x =0.0, double y =0.0, double z  =0.0, double a  =0.0, int =0);
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
    //void gotoPickup();
    
signals:
   // void moveAbsolute(double,double,double,double);
    
    
};




#endif // ASSEMBLYUEYEWIDGET_H
