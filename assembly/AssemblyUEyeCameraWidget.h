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

#include <DeviceState.h>

#ifdef USE_FAKEIO
#include "AssemblyUEyeFakeCamera.h"
typedef AssemblyUEyeFakeCamera AssemblyUEyeCamera_t;
#else
#include "AssemblyUEyeCamera.h"
typedef AssemblyUEyeCamera AssemblyUEyeCamera_t;
#endif

class AssemblyUEyeCameraWidget : public QToolBox
{
    Q_OBJECT
public:
    explicit AssemblyUEyeCameraWidget(AssemblyUEyeCamera_t* camera,
                                      QWidget *parent = 0);

protected:

    AssemblyUEyeCamera_t* camera_;

public slots:

    void cameraInformationChanged();
};

class AssemblyUEyeCameraGeneralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AssemblyUEyeCameraGeneralWidget(AssemblyUEyeCamera_t* camera,
                                             QWidget *parent = 0);

protected:

    AssemblyUEyeCamera_t* camera_;

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

#endif // ASSEMBLYUEYECAMERAWIDGET_H
