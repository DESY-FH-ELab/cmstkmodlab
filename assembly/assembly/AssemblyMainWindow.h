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
#include <AssemblyThresholdTuner.h>
#include <AssemblyModuleAssembler.h>
#include <AssemblyAutoFocus.h>
#include <AssemblyImageController.h>
#include <AssemblyZFocusFinder.h>
#include <AssemblyObjectFinderPatRec.h>
#include <AssemblyObjectFinderPatRecThread.h>
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
    explicit AssemblyMainWindow(const unsigned int camera_ID=10, QWidget* parent=0);

  public slots:

    void  enable_images();
    void disable_images();
    void     get_image ();

    void    connect_images();
    void disconnect_images();

    void changeState_AutoFocus       (int);
//  void changeState_SandwichAssembly(int);
    void changeState_Alignment       (int);

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

    void multipickupNpatrec_connected();
    void multipickupNpatrec_disconnected();

  protected slots:

    void liveUpdate();

  protected:

    // model(s) and model-manager(s)
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
//    AssemblyUEyeWidget*      camera_widget_;
    AssemblyVUEyeCamera*      camera_;
    unsigned int              camera_ID_;

    // view(s)
    QToolBar*   toolBar_;
    QTabWidget* tabWidget_;

//    AssemblyUEyeSnapShooter* finderView_;
//    AssemblyUEyeSnapShooter* edgeView_;
//    AssemblyUEyeSnapShooter* rawView_;
    AssemblyAutoFocus*       autoFocusView_;
    AssemblyThresholdTuner*  thresholdTunerView_;
    AssemblyModuleAssembler* assembleView_;

    QCheckBox* checkbox1;
//    QCheckBox* checkbox3;
    QCheckBox* checkbox4;

    // controller(s)
    AssemblyImageController*    image_ctr_;

    AssemblyZFocusFinder*       zfocus_finder_;

    AssemblyObjectFinderPatRec*       object_finder_;
    AssemblyObjectFinderPatRecThread* object_finder_thread_;

    AssemblyMultiPickupTester*  multipickup_;

    AssemblyAssembler*          module_assembler_;

    // timing
    double testTimerCount_;

    QTimer* liveTimer_;
};

#endif // ASSEMBLYMAINWINDOW_H
