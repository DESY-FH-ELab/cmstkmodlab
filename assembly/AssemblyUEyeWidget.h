#ifndef ASSEMBLYUEYEWIDGET_H
#define ASSEMBLYUEYEWIDGET_H

#include <QTabWidget>

#include <DeviceState.h>

#include "AssemblyVUEyeModel.h"
#include "AssemblyVUEyeCamera.h"
#include "AssemblyVUEyePixelClock.h"

class AssemblyUEyeWidget : public QTabWidget
{
    Q_OBJECT
public:
  explicit AssemblyUEyeWidget(AssemblyVUEyeModel *model,
                              QWidget *parent = 0);

protected:

  AssemblyVUEyeModel* model_;

public slots:

  void cameraCountChanged(unsigned int);
};

#endif // ASSEMBLYUEYEWIDGET_H
