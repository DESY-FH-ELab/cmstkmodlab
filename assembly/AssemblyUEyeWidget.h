#ifndef ASSEMBLYUEYEWIDGET_H
#define ASSEMBLYUEYEWIDGET_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include <DeviceState.h>

#ifdef USE_FAKEIO
#include "AssemblyUEyeFakeModel.h"
typedef AssemblyUEyeFakeModel AssemblyUEyeModel_t;
#else
#include "AssemblyUEyeModel.h"
typedef AssemblyUEyeModel AssemblyUEyeModel_t;
#endif

class AssemblyUEyeWidget : public QWidget
{
    Q_OBJECT
public:
  explicit AssemblyUEyeWidget(AssemblyUEyeModel_t* model,
                              QWidget *parent = 0);

protected:

  AssemblyUEyeModel_t* model_;

public slots:

  void updateCameraInformation(unsigned int);
};

#endif // JULABOWIDGET_H
