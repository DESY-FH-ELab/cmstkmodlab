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

#include <AssemblyVUEyeCamera.h>
#include <AssemblyUEyeCameraThread.h>
#ifdef NOUEYE
#include <AssemblyUEyeFakeModel.h>
typedef AssemblyUEyeFakeModel AssemblyUEyeModel_t;
#else
#include <AssemblyUEyeModel.h>
typedef AssemblyUEyeModel AssemblyUEyeModel_t;
#endif
#include <AssemblyUEyeWidget.h>
#include <AssemblyUEyeView.h>
#include <AssemblyUEyeSnapShooter.h>
#include <AssemblyAutoFocusView.h>
#include <AssemblyAssemblyView.h>
#include <AssemblyRegistryView.h>
#include <AssemblyZFocusFinder.h>
#include <AssemblyImageController.h>
#include <AssemblyImageView.h>
#include <AssemblyImageThresholder.h>
#include <AssemblyImageThresholderView.h>
#include <AssemblyObjectFinderPatRec.h>
#include <AssemblyObjectFinderPatRecView.h>
#include <AssemblyObjectFinderPatRecThread.h>
#include <AssemblyObjectAligner.h>
#include <AssemblyMultiPickupTester.h>
#include <AssemblyAssembler.h>
#include <LStepExpressModel.h>
#include <LStepExpressMotionManager.h>
#include <LStepExpressMotionView.h>
#include <LStepExpressMotionThread.h>
#include <LStepExpressSettings.h>
#include <LStepExpressSettingsWidget.h>
#include <LStepExpressWidget.h>
#include <LStepExpressJoystickWidget.h>
#include <LStepExpressMeasurement.h>
#include <LStepExpressMeasurementWidget.h>
#include <LStepExpressPositionWidget.h>
#include <LStepExpressStatusWindow.h>
#include <ConradModel.h>
#include <ConradManager.h>

#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QTimer>
#include <QToolBar>
#include <QScrollArea>

class AssemblyMainWindow : public QMainWindow
{
 Q_OBJECT

 public:

  explicit AssemblyMainWindow(const unsigned int camera_ID=10, QWidget* parent=nullptr);
  virtual ~AssemblyMainWindow() {}

 public slots:

  void  enable_images();
  void disable_images();
  void     get_image ();

  void    connect_images();
  void disconnect_images();

  void changeState_AutoFocus       (const int);
//  void changeState_SandwichAssembly(const int);
  void changeState_Alignment       (const int);

//!!  void    connect_objectAligner(const AssemblyObjectAligner::Configuration&);
//!!  void disconnect_objectAligner();

  void    connect_multipickupNpatrec(const AssemblyMultiPickupTester::Configuration&);
  void disconnect_multipickupNpatrec();

  void testTimer();

  void quit_thread(QThread*, const std::string&) const;
  void quit();

 signals:

  void images_ON();
  void images_OFF();

  void image_request();

  void updateVacuumChannelsStatus();

  void AutoFocus_ON();
  void AutoFocus_OFF();

  void objectAligner_connected();
  void objectAligner_disconnected();

  void multipickupNpatrec_connected();
  void multipickupNpatrec_disconnected();

 protected slots:

  void liveUpdate();

 protected:

  // Model(s) and Model-Manager(s)
  ConradModel*   conradModel_;
  ConradManager* conradManager_;

  LStepExpressModel*          motion_model_;
  LStepExpressMotionManager*  motion_manager_;
  LStepExpressMotionView*     motion_manager_view_;
  LStepExpressMotionThread*   motion_thread_;
  LStepExpressSettings*       motionSettings_;
  LStepExpressSettingsWidget* motionSettingsWidget_;

  AssemblyUEyeModel_t*      camera_model_;
  AssemblyUEyeCameraThread* camera_thread_;
//  AssemblyUEyeWidget*      camera_widget_;
  AssemblyVUEyeCamera*      camera_;
  unsigned int              camera_ID_;

  // View(s)
  QToolBar*   toolBar_;
  QTabWidget* tabWidget_;

//  AssemblyUEyeSnapShooter* finderView_;
//  AssemblyUEyeSnapShooter* edgeView_;
//  AssemblyUEyeSnapShooter* rawView_;
  AssemblyImageView* img_view_;
  AssemblyImageThresholderView* thresholdView_;
  AssemblyAutoFocusView* autoFocusView_;
  AssemblyAssemblyView*  assemblyView_;
  AssemblyObjectFinderPatRecView* finder_view_;
  AssemblyRegistryView*  registryView_;

  QCheckBox* checkbox1;
  QCheckBox* checkbox2;
//  QCheckBox* checkbox3;

  // Controller(s)
  AssemblyImageController*    image_ctr_;
  AssemblyZFocusFinder*       zfocus_finder_;
  AssemblyImageThresholder*   img_thresholder_;
  AssemblyObjectFinderPatRec* finder_;
  AssemblyObjectAligner*      aligner_;
  AssemblyMultiPickupTester*  multipickup_;
  AssemblyAssembler*          module_assembler_;

  // thread(s)
  AssemblyObjectFinderPatRecThread* finder_thread_;

  // timing
  double testTimerCount_;

  QTimer* liveTimer_;
};

#endif // ASSEMBLYMAINWINDOW_H
