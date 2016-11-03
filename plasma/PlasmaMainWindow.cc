#include <stdlib.h>

#include <string>
#include <iostream>

#include <QGroupBox>
#include <QFileDialog>
#include <QApplication>
#include <QPalette>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "PlasmaMainWindow.h"

PlasmaMainWindow::PlasmaMainWindow(QWidget *parent)
  : QMainWindow(parent),
    smci36ModelX_(0)
{
    ApplicationConfig* config = ApplicationConfig::instance();

    int driveAddress_X = config->getValue("SMCI36_DriveAddress_X", 1);

    std::vector<std::string> ports = config->getValueVector<std::string>("SMCI36_Ports");
    for (std::vector<std::string>::const_iterator it = ports.begin();
         it != ports.end();
         ++it) {
      NQLog("PlasmaMainWindow") << "SMCI36 PORT: " << *it;

      NanotecSMCI36Model * model = new NanotecSMCI36Model(it->c_str(),
                                                          0.1, 5.0, this);

      if (model->getDriveAddress()==driveAddress_X) {
        smci36ModelX_ = model;
        stageX_ = new NanotecSMCI36LinearStageModel(smci36ModelX_, this);
      }
    }

    if (smci36ModelX_ && stageX_) {
      smci36ModelX_->setMotorID(
          config->getValue("SMCI36_MotorID_X",
                           0)
      );
      smci36ModelX_->setPhaseCurrent(
          config->getValue("SMCI36_PhaseCurrent_X",
                           50)
      );
      smci36ModelX_->setStandStillPhaseCurrent(
          config->getValue("SMCI36_StandStillPhaseCurrent_X",
                           10)
      );
      smci36ModelX_->setStepMode(
          config->getValue("SMCI36_StepMode_X",
                           (int)VNanotecSMCI36::smci04MicroStepsPerFullStep)
      );
      smci36ModelX_->setRampMode(
          config->getValue("SMCI36_RampMode_X",
                           (int)VNanotecSMCI36::smciJerkFreeRamp)
      );
      smci36ModelX_->setErrorCorrectionMode(
          config->getValue("SMCI36_ErrorCorrectionMode_X",
                           (int)VNanotecSMCI36::smciErrCorrectionAfterTravel)
      );
      smci36ModelX_->setMaxEncoderDeviation(
          config->getValue("SMCI36_MaxEncoderDeviation_X",
                           4)
      );
      smci36ModelX_->setDirection(
          config->getValue("SMCI36_Direction_X",
                           0)
      );
      smci36ModelX_->setEncoderDirection(
          config->getValue("SMCI36_EncoderDirection_X",
                           0)
      );
      smci36ModelX_->setMinFrequency(
          config->getValue("SMCI36_MinFrequency_X",
                           40)
      );
      smci36ModelX_->setQuickstopRampHzPerSecond(
          config->getValue("SMCI36_QuickstopRampHzPerSecond_X",
                           3000000)
      );
      smci36ModelX_->setAccelerationRampHzPerSecond(
          config->getValue("SMCI36_AccelerationRampHzPerSecond_X",
                           50000)
      );
      smci36ModelX_->setDecelerationRampHzPerSecond(
          config->getValue("SMCI36_DecelerationRampHzPerSecond_X",
                           50000)
      );

      smci36ModelX_->setInputPinFunction(1, config->getValue<int>("SMCI36_InputPin1Function_X", 1));
      smci36ModelX_->setInputPinFunction(2, config->getValue<int>("SMCI36_InputPin2Function_X", 2));
      smci36ModelX_->setInputPinFunction(3, config->getValue<int>("SMCI36_InputPin3Function_X", 3));
      smci36ModelX_->setInputPinFunction(4, config->getValue<int>("SMCI36_InputPin4Function_X", 4));
      smci36ModelX_->setInputPinFunction(5, config->getValue<int>("SMCI36_InputPin5Function_X", 5));
      smci36ModelX_->setInputPinFunction(6, config->getValue<int>("SMCI36_InputPin6Function_X", 7));

      smci36ModelX_->setOutputPinFunction(1, config->getValue<int>("SMCI36_OutputPin1Function_X", 1));
      smci36ModelX_->setOutputPinFunction(2, config->getValue<int>("SMCI36_OutputPin2Function_X", 2));
      smci36ModelX_->setOutputPinFunction(3, config->getValue<int>("SMCI36_OutputPin3Function_X", 3));

      smci36ModelX_->setIOMask(
          config->getValue<unsigned int>("SMCI36_IOMask_X",
                                         0x107003F)
      );
      smci36ModelX_->setReversePolarityMask(
          config->getValue<unsigned int>("SMCI36_ReversePolarityMask_X",
                                         0x107003F)
      );

      stageX_->setPitch(config->getValue<double>("Stage_Pitch_X",
                                                 0.35)
      );
      stageX_->setMinimumPosition(config->getValue<double>("Stage_MinPosition_X",
                                                           0.0)
      );
      stageX_->setMaximumPosition(config->getValue<double>("Stage_MaxPosition_X",
                                                           370.0)
      );

      smci36ModelX_->updateInformation2();
      stageX_->updateInformation();
    }

    tabWidget_ = new QTabWidget(this);

    QWidget * w;

    if (smci36ModelX_) {
      stageWidgetX_ = new NanotecSMCI36LinearStageWidget(stageX_, tabWidget_);
      tabWidget_->addTab(stageWidgetX_, "stage X");

      smci36WidgetX_ = new NanotecSMCI36Widget(smci36ModelX_, tabWidget_);
      tabWidget_->addTab(smci36WidgetX_, "controller X");
    }

    setCentralWidget(tabWidget_);
    updateGeometry();

    NQLog("PlasmaMainWindow") << "main window constructed";
}

void PlasmaMainWindow::quit()
{
    NQLog("PlasmaMainWindow") << "quit";

    delete smci36ModelX_;
}
