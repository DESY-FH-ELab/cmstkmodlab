#ifndef ASSEMBLYUEYECAMERAWIDGET_H
#define ASSEMBLYUEYECAMERAWIDGET_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QToolBox>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QPushButton>

#include <QProgressBar>


#include <DeviceState.h>

#include "AssemblyVUEyeCamera.h"

class AssemblyUEyeCameraWidget : public QToolBox
{
    Q_OBJECT
public:
    explicit AssemblyUEyeCameraWidget(AssemblyVUEyeCamera *camera,
                                      QWidget *parent = 0);

protected:

    AssemblyVUEyeCamera* camera_;


public slots:

    void cameraInformationChanged();
};

class AssemblyUEyeCameraGeneralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AssemblyUEyeCameraGeneralWidget(AssemblyVUEyeCamera* camera,
                                             QWidget *parent = 0);


    
protected:

    AssemblyVUEyeCamera* camera_;

    QLabel* nameLabel_;
    QLabel* fullNameLabel_;
    QLabel* cameraIDLabel_;
    QLabel* deviceIDLabel_;
    QLabel* sensorIDLabel_;
    QLabel* serialNumberLabel_;
    QLabel* statusLabel_;
    QLabel* idLabel_;
    QLabel* versionLabel_;
    QLabel* dateLabel_;
    


public slots:

    void cameraInformationChanged();
};

class AssemblyUEyeCameraSensorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AssemblyUEyeCameraSensorWidget(AssemblyVUEyeCamera* camera,
                                            QWidget *parent = 0);

protected:

    AssemblyVUEyeCamera* camera_;

    QLabel* sensorNameLabel_;
    QLabel* colorModeLabel_;
    QLabel* maxResolutionLabel_;
    QLabel* masterGainLabel_;
    QLabel* rgbGainLabel_;
    QLabel* globalShutterLabel_;
    QLabel* pixelSizeLabel_;

public slots:
    void cameraInformationChanged();
};


class AssemblyUEyeCameraSettingsCalibrater : public QPushButton
{
    Q_OBJECT
    
public:
    explicit AssemblyUEyeCameraSettingsCalibrater(AssemblyVUEyeCamera* camera,
                                                  QWidget *parent = 0);
    
public slots:
    void onCalibrate();
    
protected:
    
    AssemblyVUEyeCamera* camera_;
    
signals:
    void changeExposureTime(double);
    void acquireImage();
    
};



class AssemblyUEyeCameraSettingsPicker : public QPushButton
{
    Q_OBJECT
    
public:
    explicit AssemblyUEyeCameraSettingsPicker(AssemblyVUEyeCamera* camera,
                                                  QWidget *parent = 0);
    
    public slots:
//    void onCalibrate();
    
protected:
    
    AssemblyVUEyeCamera* camera_;
    
signals:
//   void updateStatus(QString, double);
 //   void acquireImage();
    
};


class AssemblyUEyeCameraSettingsStatus : public QProgressBar
{
    Q_OBJECT
    
public:
    explicit AssemblyUEyeCameraSettingsStatus(AssemblyVUEyeCamera* camera,
                                              QWidget *parent = 0);
    
    public slots:
       void updateStatus(QString, double);
    
protected:
    
    AssemblyVUEyeCamera* camera_;
    
signals:
    //   void changeExposureTime(double);
    //   void acquireImage();
    
};



class AssemblyUEyeCameraMarkerFinderResult
{
    
public:
    AssemblyUEyeCameraMarkerFinderResult();
    double x, y, angle;
};





class AssemblyUEyeCameraPixelClockWidget : public QComboBox
{
    Q_OBJECT
public:
    explicit AssemblyUEyeCameraPixelClockWidget(AssemblyVUEyeCamera* camera,
                                            QWidget *parent = 0);

public slots:

    void pixelClockListChanged(unsigned int);
    void pixelClockChanged(unsigned int);
    void currentItemChanged(int idx);

protected:

    AssemblyVUEyeCamera* camera_;

signals:

    void changePixelClock(unsigned int);
};

class AssemblyUEyeCameraExposureTimeSlider : public QSlider
{
    Q_OBJECT
public:
    explicit AssemblyUEyeCameraExposureTimeSlider(AssemblyVUEyeCamera* camera,
                                            QWidget *parent = 0);

public slots:

    void exposureTimeRangeChanged(double);
    void exposureTimeChanged(double);
    void currentValueChanged(int);

protected:

    AssemblyVUEyeCamera* camera_;

signals:

    void changeExposureTime(double);
};

class AssemblyUEyeCameraExposureTimeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AssemblyUEyeCameraExposureTimeWidget(AssemblyVUEyeCamera* camera,
                                            QWidget *parent = 0);

public slots:

    void exposureTimeRangeChanged(double);
    void exposureTimeChanged(double);

protected:

    AssemblyVUEyeCamera* camera_;

    AssemblyUEyeCameraExposureTimeSlider* slider_;
    QLabel* minLabel_;
    QLabel* maxLabel_;
    QLabel* currentLabel_;

signals:

    void changeExposureTime(double);
};

class AssemblyUEyeCameraSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AssemblyUEyeCameraSettingsWidget(AssemblyVUEyeCamera* camera,
                                            QWidget *parent = 0);
    void updateCoordinates(double, double, double);
    double x_,y_,angle_;


protected:

    AssemblyVUEyeCamera* camera_;
    
    QLineEdit* x_coor;
    QLineEdit* y_coor;
    QLineEdit* ang;

public slots:

    void cameraInformationChanged();
    void updateResult(double, double, double);

};


class AssemblyUEyeCameraSettingsMotionInterface : public QPushButton
{
    Q_OBJECT
    
public:
    explicit AssemblyUEyeCameraSettingsMotionInterface(AssemblyVUEyeCamera* camera,
                                                       QWidget *parent = 0, AssemblyUEyeCameraSettingsWidget * settings =0);
    AssemblyUEyeCameraSettingsWidget * settings;
    double local_x, local_y, local_angle;
    public slots:
        void returntoOrigin();
        void catchResult(double, double,double);
       // void gotoPickup(double, double,double);


protected:
    AssemblyVUEyeCamera* camera_;
    
signals:
     void moveAbsolute(double, double, double, double);
    
};






#endif // ASSEMBLYUEYECAMERAWIDGET_H
