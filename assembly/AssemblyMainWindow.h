#ifndef ASSEMBLYMAINWINDOW_H
#define ASSEMBLYMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QTimer>
#include <QToolBar>
#include <QScrollArea>

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

class AssemblyMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit AssemblyMainWindow(QWidget *parent = 0);

public slots:

  void quit();

  void testTimer();

  void test();

  void onOpenCamera();
  void onCloseCamera();
  void onSnapShot();

  void cameraOpened();
  void cameraClosed();
  void imageAcquired(const QImage&);

signals:

  void openCamera();
  void closeCamera();
  void acquireImage();

protected:

  QDir currentDir_;

  QToolBar* toolBar_;

  QTabWidget* tabWidget_;

  QScrollArea* scrollArea_;
  AssemblyUEyeView* imageView_;

  AssemblyUEyeModel_t* uEyeModel_;
  AssemblyUEyeCameraThread* cameraThread_;
  AssemblyUEyeWidget* uEyeWidget_;

  AssemblyUEyeCamera_t * camera_;

  double testTimerCount_;
  QTimer *testTimer_;
};

#endif // ASSEMBLYMAINWINDOW_H
