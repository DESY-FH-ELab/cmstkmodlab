#ifndef ASSEMBLYPLASMACLEANER_H
#define ASSEMBLYPLASMACLEANER_H

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>
#include <QLineEdit>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QPainter>
#include <QColor>

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



class LightWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool on READ isOn WRITE setOn)
public:
    LightWidget(const QColor &color, QWidget *parent = 0)
    : QWidget(parent), m_color(color), m_on(false) {}
    
    bool isOn() const
    { return m_on; }
    void setOn(bool on)
    {
        if (on == m_on)
            return;
        m_on = on;
        update();
    }
    
    public slots:
    void turnOff() { setOn(false); }
    void turnOn() { setOn(true); }
    
protected:
    virtual void paintEvent(QPaintEvent *)
    {
        if (!m_on)
            return;
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(m_color);
        painter.drawEllipse(0, 0, width()/4, height());
    }
    
private:
    QColor m_color;
    bool m_on;
};




class AssemblyPlasmaCleaner : public QWidget
{
    Q_OBJECT
public:

    explicit AssemblyPlasmaCleaner(QWidget *parent = 0);

protected:


public slots:
    
signals:
    void moveAbsolute(double,double,double,double);


};



class CleanerCommander : public QWidget
{
    Q_OBJECT
public:
    
  explicit CleanerCommander(QWidget *parent = 0, std::string ="test", double target_speed =0.0, double max_accel =0.0, double scan_distance = 0.0, double steps_per_mm = 0.0);

    QPushButton* button1;
    QLineEdit *lineEdit1;
    QLabel *ql_0;
    QLabel *ql_1;
    QLabel *ql_2;
    QLabel *ql_3;
    QLabel *ql_4;
    QLineEdit *lineEdit2;
    QLineEdit *lineEdit3;
    QLineEdit *lineEdit4;
    QProgressBar *progressBar;
    LightWidget* light;
    ArduinoMotor * motor;
    std::string str;
    float progress, sec, scan_time;

    std::string target_speed_str;
    std::string max_accel_str;
    std::string scan_distance_str;
    std::string steps_per_mm_str; 
   
    
protected:
    
    
public slots:
  void sendCommand();
  void update();
 
signals:
   void moveAbsolute(double,double,double,double);
    
    
};


#endif //
