#ifndef ASSEMBLYPLASMACLEANER_H
#define ASSEMBLYPLASMACLEANER_H

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

//relay card
#include "../../devices/Conrad/ConradCommunication.h"
#include "../../devices/Conrad/ConradController.h"

#include "../../devices/Arduino/ArduinoMotor.h"




class AssemblyPlasmaCleaner : public QWidget
{
    Q_OBJECT
public:

    explicit AssemblyPlasmaCleaner(QWidget *parent = 0);
    QLabel * ql;

protected:


public slots:

    
signals:
    void moveAbsolute(double,double,double,double);


};



class CleanerCommander : public QWidget
{
    Q_OBJECT
public:
    
    explicit CleanerCommander(QWidget *parent = 0, std::string ="test", double x =0.0, double y =0.0);
    double local_x, local_y, local_z, local_a;
    QPushButton* button1;
    QLineEdit *lineEdit1;
    ArduinoMotor * motor;
    std::string str;
    
protected:
    
    
public slots:
  void sendCommand();
    
signals:
   void moveAbsolute(double,double,double,double);
    
    
};






#endif //
