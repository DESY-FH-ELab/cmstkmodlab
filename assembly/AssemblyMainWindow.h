#ifndef ASSEMBLYMAINWINDOW_H
#define ASSEMBLYMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QTimer>
#include <QToolBar>
#include <QScrollArea>

#include "AssemblyVUEyeCamera.h"

#ifdef USE_FAKEIO
#include "AssemblyUEyeFakeModel.h"
typedef AssemblyUEyeFakeModel AssemblyUEyeModel_t;
#else
#include "AssemblyUEyeModel.h"
typedef AssemblyUEyeModel AssemblyUEyeModel_t;
#endif

#include "AssemblyUEyeCameraThread.h"
#include "AssemblyUEyeWidget.h"
#include "AssemblyUEyeView.h"
#include "AssemblyUEyeSnapShooter.h"

class AssemblyMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit AssemblyMainWindow(QWidget *parent = 0);

public slots:

  void quit();

  void testTimer();

  void onOpenCamera();
  void onCloseCamera();
  void onSnapShot();

  void cameraOpened();
  void cameraClosed();

signals:

  void openCamera();
  void closeCamera();
  void acquireImage();

protected slots:

  void liveUpdate();

protected:

  QDir currentDir_;

  QToolBar* toolBar_;

  QTabWidget* tabWidget_;

  AssemblyUEyeSnapShooter* finderView_;
  AssemblyUEyeSnapShooter* rawView_;

  AssemblyUEyeModel_t* uEyeModel_;
  AssemblyUEyeCameraThread* cameraThread_;
  AssemblyUEyeWidget* uEyeWidget_;

  AssemblyVUEyeCamera * camera_;

  double testTimerCount_;
  QTimer *testTimer_;

  QTimer *liveTimer_;
};

#endif // ASSEMBLYMAINWINDOW_H
