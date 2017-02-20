#ifndef ASSEMBLYMAINWINDOW_H
#define ASSEMBLYMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QTimer>
#include <QToolBar>
#include <QScrollArea>

#include "AssemblyVUEyeCamera.h"

#ifdef NOUEYE
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
#include "AssemblyModuleAssembler.h"
#include "AssemblyAutoFocus.h"
#include "AssemblyScanner.h"


#include "AssemblyMarkerFinderThread.h"
#include "AssemblySensorMarkerFinder.h"
#include "AssemblySensorMarkerFinderWidget.h"



//motion
#include <ApplicationConfig.h>
#include "LStepExpressModel.h"
#include "LStepExpressSettings.h"
#include "LStepExpressMotionManager.h"
#include "LStepExpressMotionThread.h"
#include "LStepExpressSettingsWidget.h"
#include "LStepExpressWidget.h"
#include "LStepExpressJoystickWidget.h"

#include "LStepExpressMeasurement.h"
#include "LStepExpressMeasurementWidget.h"
#include "LStepExpressPositionWidget.h"
#include "LStepExpressStatusWindow.h"

//conrad relay card
#include "ConradModel.h"
#include "ConradManager.h"


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
  void enableAutoFocus(int);
  void enablePrecisionEstimation(int);

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
  AssemblyUEyeSnapShooter* edgeView_;
  AssemblyUEyeSnapShooter* rawView_;
  AssemblyModuleAssembler* assembleView_;
  AssemblyAutoFocus* autoFocusView_;
  AssemblyScanner*  cmdr_zscan;
  QCheckBox *checkbox1;
  QCheckBox *checkbox2;

    
  AssemblyUEyeModel_t* uEyeModel_;
  AssemblyUEyeCameraThread* cameraThread_;
  AssemblyUEyeWidget* uEyeWidget_;

  AssemblyVUEyeCamera * camera_;

  AssemblyMarkerFinderThread* finderThread_;
  AssemblySensorMarkerFinder* finder_;
  AssemblySensorMarkerFinderWidget* finderWidget_;
    

    
  LStepExpressModel* lStepExpressModel_;
  LStepExpressMotionManager* motionManager_;
  LStepExpressSettings* lStepExpressSettings_;
  LStepExpressMotionThread* motionThread_;
    
  ConradModel * conradModel_;
  ConradManager* conradManager_;

  double testTimerCount_;

  QTimer *liveTimer_;
};

#endif // ASSEMBLYMAINWINDOW_H
