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
#include <AssemblyZFocusFinder.h>
#include <AssemblyImageController.h>
#include <AssemblyImageView.h>
#include <AssemblyThresholder.h>
#include <AssemblyThresholderView.h>
#include <AssemblyObjectFinderPatRec.h>
#include <AssemblyObjectFinderPatRecView.h>
#include <AssemblyObjectFinderPatRecThread.h>
#include <AssemblyObjectAligner.h>
#include <AssemblyObjectAlignerView.h>
#include <AssemblyAssembly.h>
#include <AssemblyAssemblyView.h>
#include <AssemblyAssemblyV2.h>
#include <AssemblyAssemblyV2View.h>
#include <AssemblyMultiPickupTester.h>
#include <AssemblyToolboxView.h>
#include <AssemblySmartMotionManager.h>
#include <AssemblyParametersView.h>
#include <AssemblyHardwareControlView.h>
#include <AssemblyStopwatchWidget.h>
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
#include <VRelayCardModel.h>
#include <RelayCardManager.h>
#include <Metrology.h>
#include <MetrologyView.h>


#include <QMainWindow>
#include <QString>
#include <QTabWidget>
#include <QDir>
#include <QTimer>
#include <QToolBar>
#include <QScrollArea>
#include <QLineSeries>
#include <QtCharts>

class AssemblyMainWindow : public QMainWindow
{
 Q_OBJECT

 public:

  explicit AssemblyMainWindow(const QString& outputdir_path, const QString& logfile_path, QWidget* parent=nullptr);
  virtual ~AssemblyMainWindow()
  {
      disconnect_otherSlots();
  }
  void switchAndUpdate_alignment_tab(bool);

 public slots:

  void  enable_images();
  void disable_images();
  void     get_image ();

  void    connect_images();
  void disconnect_images();

  void changeState_autofocus(const int);

  void start_objectAligner(const AssemblyObjectAligner::Configuration&);
  void disconnect_objectAligner();

  void start_metrology(const Metrology::Configuration&);
  void disconnect_metrology();

  void start_multiPickupTest(const AssemblyMultiPickupTester::Configuration&);
  void disconnect_multiPickupTest();

  void disconnect_otherSlots();

  void testTimer();

  void displayInfo_activeTab();

  void quit_thread(QThread*, const QString&) const;
  void quit();

  void update_alignment_tab_psp();
  void update_alignment_tab_pss();

  void update_stage_position();

  void update_vacuum_information(const int, const SwitchState);

  void warn_on_stage_limits(const double target_pos, const char axis, const double limit_pos_lower, const double limit_pos_upper);

 signals:

  void images_ON();
  void images_OFF();

  void image_request();

  void changeExposureTime(double);

  void updateVacuumChannelsStatus();

  void autofocus_ON();
  void autofocus_OFF();

  void objectAligner_connected();
  void objectAligner_disconnected();

  void metrology_connected();
  void metrology_disconnected();

  void multiPickupTest_disconnected();

  void display_info();

  void set_alignmentMode_PSP_request();
  void set_alignmentMode_PSS_request();

 protected slots:

  void liveUpdate();

  void messageBox_restartMotionStage();

  void restore_autofocus_settings();

 protected:

  // Low-Level Controllers (Motion, Camera, Vacuum)
  VRelayCardModel* relayCardModel_;
  RelayCardManager* relayCardManager_;

  LStepExpressModel*          motion_model_;
  LStepExpressMotionManager*  motion_manager_;
  LStepExpressMotionView*     motion_manager_view_;
  LStepExpressMotionThread*   motion_thread_;
  LStepExpressSettings*       motionSettings_;
  LStepExpressSettingsWidget* motionSettingsWidget_;

  std::vector<QLabel*>        stage_values_;

  int vacuum_basepl_;
  int vacuum_pickup_;
  int vacuum_spacer_;
  QLabel* PU_status_;
  QLabel* SP_status_;
  QLabel* BP_status_;

  AssemblyUEyeModel_t*      camera_model_;
  AssemblyUEyeCameraThread* camera_thread_;
//  AssemblyUEyeWidget*      camera_widget_;
  AssemblyVUEyeCamera*      camera_;
  unsigned int              camera_ID_;

  // High-Level Controllers
  AssemblyImageController*    image_ctr_;
  AssemblyZFocusFinder*       zfocus_finder_;
  AssemblyThresholder*        thresholder_;
  AssemblyObjectAligner*      aligner_;
  Metrology*                  metrology_;
  AssemblyAssembly*           assembly_;
  AssemblyAssemblyV2*         assemblyV2_;
  AssemblyMultiPickupTester*  multipickup_tester_;
  AssemblySubassemblyPickup*  subassembly_pickup_;

  AssemblyObjectFinderPatRec*       finder_;
  AssemblyObjectFinderPatRecThread* finder_thread_;

  AssemblySmartMotionManager* smart_motion_;


  // Views
  QToolBar*   toolBar_;
  QTabWidget* main_tab;
  QTabWidget* tabWidget_;

//  AssemblyUEyeSnapShooter* finderView_;
//  AssemblyUEyeSnapShooter* edgeView_;
//  AssemblyUEyeSnapShooter* rawView_;
  AssemblyImageView* image_view_;
  AssemblyThresholderView* thresholder_view_;
  AssemblyObjectFinderPatRecView* finder_view_;
  AssemblyObjectAlignerView* aligner_view_;
  AssemblyAssemblyView* assembly_view_;
  MetrologyView* metrology_view_;
  AssemblyAssemblyV2View* assemblyV2_view_;
  AssemblyToolboxView* toolbox_view_;
  AssemblyParametersView* params_view_;
  AssemblyHardwareControlView* hwctr_view_;

  AssemblyStopwatchWidget* stopwatch_wid_;

  QPushButton* button_mainEmergencyStop_;
  QPushButton* button_info_;
  QCheckBox* autofocus_checkbox_;

  // flags
  bool images_enabled_;
  bool aligner_connected_;
  bool metrology_connected_;

  // timing
  double testTimerCount_;
  QTimer* liveTimer_;

  int idx_alignment_tab;
  int idx_module_tab;
  int idx_manual_tab;
};

#endif // ASSEMBLYMAINWINDOW_H
