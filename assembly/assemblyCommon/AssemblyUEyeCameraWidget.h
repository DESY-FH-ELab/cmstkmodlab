#ifndef ASSEMBLYUEYECAMERAWIDGET_H
#define ASSEMBLYUEYECAMERAWIDGET_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QToolBox>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QSlider>

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

protected:

    AssemblyVUEyeCamera* camera_;

public slots:

    void cameraInformationChanged();
};

#endif // ASSEMBLYUEYECAMERAWIDGET_H
