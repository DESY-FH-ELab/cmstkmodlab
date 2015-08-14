#ifndef ASSEMBLYUEYEWIDGET_H
#define ASSEMBLYUEYEWIDGET_H

#include <QTabWidget>

#include <DeviceState.h>

#ifdef USE_FAKEIO
#include "AssemblyUEyeFakeModel.h"
typedef AssemblyUEyeFakeModel AssemblyUEyeModel_t;
#include "AssemblyUEyeFakeCamera.h"
typedef AssemblyUEyeFakeCamera AssemblyUEyeCamera_t;
#else
#include "AssemblyUEyeModel.h"
typedef AssemblyUEyeModel AssemblyUEyeModel_t;
#include "AssemblyUEyeCamera.h"
typedef AssemblyUEyeCamera AssemblyUEyeCamera_t;
#endif

class AssemblyUEyeWidget : public QTabWidget
{
    Q_OBJECT
public:
  explicit AssemblyUEyeWidget(AssemblyUEyeModel_t* model,
                              QWidget *parent = 0);

protected:

  AssemblyUEyeModel_t* model_;

public slots:

  void cameraCountChanged(unsigned int);
};

#endif // ASSEMBLYUEYEWIDGET_H
