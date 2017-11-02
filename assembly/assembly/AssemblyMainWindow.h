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
#include <ImageController.h>
#include <MarkerFinderPatRec.h>
#include <MarkerFinderPatRecThread.h>
#include <ZFocusFinder.h>

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

    void changeState_AutoFocus          (int);
    void changeState_PrecisionEstimation(int);
    void changeState_SandwichAssembly   (int);
    void changeState_Alignment          (int);

    void testTimer();

    void quit();

  signals:

    void images_ON();
    void images_OFF();

    void image();

    void updateThresholdLabel();

    void updateVacuumChannelsStatus();

    void AutoFocus_ON();
    void AutoFocus_OFF();

  protected slots:

    void liveUpdate();

  protected:

    QDir currentDir_;

    QToolBar* toolBar_;
    QTabWidget* tabWidget_;

//    AssemblyUEyeSnapShooter* finderView_;
//    AssemblyUEyeSnapShooter* edgeView_;
//    AssemblyUEyeSnapShooter* rawView_;
    AssemblyAutoFocus* autoFocusView_;
    AssemblyThresholdTuner* thresholdTunerView_;
    AssemblyModuleAssembler* assembleView_;

    QCheckBox* checkbox1;
    QCheckBox* checkbox2;
    QCheckBox* checkbox3;
    QCheckBox* checkbox4;

    LStepExpressModel*          motion_model_;
    LStepExpressMotionManager*  motion_manager_;
    LStepExpressMotionView*     motion_manager_view_;
    LStepExpressMotionThread*   motion_thread_;
    LStepExpressSettings*       motionSettings_;
    LStepExpressSettingsWidget* motionSettingsWidget_;

    AssemblyUEyeModel_t*      camera_model_;
    AssemblyUEyeCameraThread* camera_thread_;
//    AssemblyUEyeWidget*      camera_widget_;
    unsigned int              camera_ID_;
    AssemblyVUEyeCamera*      camera_;

    ZFocusFinder*       zfocus_finder_;

    MarkerFinderPatRec*       marker_finder_;
    MarkerFinderPatRecThread* marker_finder_thread_;

    ConradModel*       conradModel_;
    ConradManager*     conradManager_;

    AssemblyAssembler* module_assembler_;

    ImageController* image_ctr_;

    double testTimerCount_;

    QTimer* liveTimer_;
};

#endif // ASSEMBLYMAINWINDOW_H
