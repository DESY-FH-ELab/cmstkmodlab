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

#include "AssemblyUEyeModel.h"

class AssemblyUEyeWidget : public QWidget
{
    Q_OBJECT
public:
  explicit AssemblyUEyeWidget(AssemblyUEyeModel* model,
                              QWidget *parent = 0);

protected:

  AssemblyUEyeModel* model_;

public slots:

  void updateCameraInformation(unsigned int);
};

#endif // JULABOWIDGET_H
