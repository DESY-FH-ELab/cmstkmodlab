#ifndef ASSEMBLYUEYESNAPSHOOTER_H
#define ASSEMBLYUEYESNAPSHOOTER_H

#include <QWidget>
#include <QScrollArea>

#ifdef USE_FAKEIO
#include "AssemblyUEyeFakeCamera.h"
typedef AssemblyUEyeFakeCamera AssemblyUEyeCamera_t;
#else
#include "AssemblyUEyeCamera.h"
typedef AssemblyUEyeCamera AssemblyUEyeCamera_t;
#endif

#include <AssemblyUEyeView.h>

class AssemblyUEyeSnapShooter : public QWidget
{
    Q_OBJECT
public:

  explicit AssemblyUEyeSnapShooter(QWidget *parent = 0);

    void connectCamera(AssemblyUEyeCamera_t* camera);
    void disconnectCamera(AssemblyUEyeCamera_t* camera);

protected:

  AssemblyUEyeCamera_t* camera_;

  QScrollArea *scrollArea_;
  AssemblyUEyeView *imageView_;

  bool takeSnapShot_;

public slots:

  void snapShot();
  void imageAcquired(const QImage&);
};

#endif // ASSEMBLYUEYEWIDGET_H
