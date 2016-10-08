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

    int motorID_X = config->getValue("SMCI36_MotorID_X", 1);

    std::vector<std::string> ports = config->getValueVector("SMCI36_Ports");
    for (std::vector<std::string>::const_iterator it = ports.begin();
         it != ports.end();
         ++it) {
      NQLog("PlasmaMainWindow") << "SMCI36 PORT: " << *it;

      NanotecSMCI36Model * model = new NanotecSMCI36Model(it->c_str(),
                                                          0.5, 1.0, this);

      if (model->getMotorID()==motorID_X) {
        smci36ModelX_ = model;
      }
    }

    if (smci36ModelX_) {
      smci36ModelX_->setPitch(config->getValue("SMCI36_Pitch_X",
                                               1.25));
      smci36ModelX_->setStepMode(config->getValue("SMCI36_StepMode_X",
                                                  (int)VNanotecSMCI36::smci04MicroStepsPerFullStep));
      smci36ModelX_->setRampMode(config->getValue("SMCI36_RampMode_X",
                                                  (int)VNanotecSMCI36::smciJerkFreeRamp));
      smci36ModelX_->setPositioningMode(config->getValue("SMCI36_PositioningMode_X",
                                                         (int)VNanotecSMCI36::smciAbsolutePositioning));
      smci36ModelX_->setErrorCorrectionMode(config->getValue("SMCI36_ErrorCorrectionMode_X",
                                                             (int)VNanotecSMCI36::smciErrCorrectionAfterTravel));
      smci36ModelX_->setMaxEncoderDeviation(config->getValue("SMCI36_MaxEncoderDeviation_X",
                                                             8));

      smci36ModelX_->setMinPositionInMM(config->getValue("SMCI36_MinPositionInMM_X",
                                                         0));
      smci36ModelX_->setMaxPositionInMM(config->getValue("SMCI36_MaxPositionInMM_X",
                                                         500));

      smci36ModelX_->updateInformation2();
    }

    tabWidget_ = new QTabWidget(this);
    tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    if (smci36ModelX_) {
      NanotecSMCI36Widget* smci36Widget_ = new NanotecSMCI36Widget(smci36ModelX_, tabWidget_);
      smci36Widget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
      tabWidget_->addTab(smci36Widget_, "stage");
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
