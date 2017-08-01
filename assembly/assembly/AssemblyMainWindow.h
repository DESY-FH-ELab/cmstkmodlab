/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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
  AssemblyUEyeSnapShooter* edgeView_;
  AssemblyUEyeSnapShooter* rawView_;
  AssemblyModuleAssembler* assembleView_;
    

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

  double testTimerCount_;

  QTimer *liveTimer_;
};

#endif // ASSEMBLYMAINWINDOW_H
