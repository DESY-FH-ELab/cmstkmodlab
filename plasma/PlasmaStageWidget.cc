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

#include <iostream>

#include <QGridLayout>
#include <QFormLayout>

#include <ApplicationConfig.h>
#include <nqlogger.h>

#include "PlasmaStageWidget.h"

PlasmaStageOutputStateWidget::PlasmaStageOutputStateWidget(NanotecSMCI36LinearStageModel* model,
                                                           int pin,
                                                           QWidget *parent)
  : QLabel(parent),
    model_(model),
    pin_(pin)
{
  buttonRed_ = QPixmap(QString(Config::CMSTkModLabBasePath.c_str()) + "/share/common/button_red.png");
  buttonGreen_ = QPixmap(QString(Config::CMSTkModLabBasePath.c_str()) + "/share/common/button_green.png");
  buttonGrey_ = QPixmap(QString(Config::CMSTkModLabBasePath.c_str()) + "/share/common/button_grey.png");

  setPixmap(buttonGrey_);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(setOutputPinState(int, bool)),
          model_, SLOT(setOutputPinState(int, bool)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
  Updates the GUI according to the new state of the controller
 */
void PlasmaStageOutputStateWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void PlasmaStageOutputStateWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);

  if (enabled) {
    bool state = model_->getOutputPinState(pin_);
    if (state) {
      setPixmap(buttonGreen_);
    } else {
      setPixmap(buttonRed_);
    }
  } else {
    setPixmap(buttonGrey_);
  }
}

void PlasmaStageOutputStateWidget::mousePressEvent(QMouseEvent* /* event */)
{
  bool ready = model_->isReady();

  if (isEnabled() && ready) {
    emit setOutputPinState(pin_, true);
  }
}

void PlasmaStageOutputStateWidget::mouseReleaseEvent(QMouseEvent* /* event */)
{
  bool ready = model_->isReady();

  if (isEnabled() && ready) {
    emit setOutputPinState(pin_, false);
  }
}

void PlasmaStageOutputStateWidget::updateInfo()
{
  // NQLog("NanotecSMCI36LinearStageOutputStateWidget", NQLog::Debug) << "updateInfo()";

  if (isEnabled()) {
    bool state = model_->getOutputPinState(pin_);
    if (state) {
      setPixmap(buttonGreen_);
    } else {
      setPixmap(buttonRed_);
    }
  } else {
    setPixmap(buttonGrey_);
  }
}

PlasmaStageIOWidget::PlasmaStageIOWidget(NanotecSMCI36LinearStageModel* model,
                                         QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QGridLayout* layout = new QGridLayout();
  setLayout(layout);

  QLabel *label;
  PlasmaStageOutputStateWidget *stateOutputWidget;
  NanotecSMCI36LinearStageInputStateWidget *stateInputWidget;

  label = new QLabel("Start Plasma", this);
  layout->addWidget(label, 0, 0, Qt::AlignLeft);

  stateOutputWidget = new PlasmaStageOutputStateWidget(model_, 3, this);
  layout->addWidget(stateOutputWidget, 0, 1, Qt::AlignHCenter | Qt::AlignVCenter);

  label = new QLabel("Stop Plasma", this);
  layout->addWidget(label, 1, 0, Qt::AlignLeft);

  stateOutputWidget = new PlasmaStageOutputStateWidget(model_, 2, this);
  layout->addWidget(stateOutputWidget, 1, 1, Qt::AlignHCenter | Qt::AlignVCenter);

  label = new QLabel("Primary Gas Alarm", this);
  layout->addWidget(label, 0, 3, Qt::AlignVCenter);

  stateInputWidget = new NanotecSMCI36LinearStageInputStateWidget(model_, 1, this);
  layout->addWidget(stateInputWidget, 0, 4, Qt::AlignHCenter | Qt::AlignVCenter);

  label = new QLabel("Secondary Gas Alarm", this);
  layout->addWidget(label, 1, 3, Qt::AlignVCenter);

  stateInputWidget = new NanotecSMCI36LinearStageInputStateWidget(model_, 2, this);
  layout->addWidget(stateInputWidget, 1, 4, Qt::AlignHCenter | Qt::AlignVCenter);

  label = new QLabel("Tuning Alarm", this);
  layout->addWidget(label, 2, 3, Qt::AlignVCenter);

  stateInputWidget = new NanotecSMCI36LinearStageInputStateWidget(model_, 4, this);
  layout->addWidget(stateInputWidget, 2, 4, Qt::AlignHCenter | Qt::AlignVCenter);

  label = new QLabel("Plasma On", this);
  layout->addWidget(label, 3, 3, Qt::AlignVCenter);

  stateInputWidget = new NanotecSMCI36LinearStageInputStateWidget(model_, 3, this);
  layout->addWidget(stateInputWidget, 3, 4, Qt::AlignHCenter | Qt::AlignVCenter);

  layout->setColumnStretch(2, 1);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
  Updates the GUI according to the new state of the controller
 */
void PlasmaStageIOWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void PlasmaStageIOWidget::controlStateChanged(bool /* enabled */)
{

}

void PlasmaStageIOWidget::updateInfo()
{
  //NQLog("NanotecSMCI36LinearStageIOWidget", NQLog::Debug) << "updateInfo()";
}

PlasmaStageWidget::PlasmaStageWidget(NanotecSMCI36LinearStageModel* model,
                                     QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(8);
  layout->setMargin(0);
  layout->setContentsMargins(4, 4, 4, 4);
  setLayout(layout);

  stageStatus_ = new NanotecSMCI36LinearStageStatusWidget(model_, this);
  layout->addWidget(stageStatus_);

  stageMovement_ = new NanotecSMCI36LinearStageMovementWidget(model_, this);
  layout->addWidget(stageMovement_);

  stageIO_ = new PlasmaStageIOWidget(model_, this);
  layout->addWidget(stageIO_);

  layout->addStretch(1);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
  Updates the GUI according to the new state of the controller
 */
void PlasmaStageWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void PlasmaStageWidget::controlStateChanged(bool /* enabled */)
{

}

void PlasmaStageWidget::updateInfo()
{
  //NQLog("NanotecSMCI36Widget", NQLog::Debug) << "updateInfo()";
}
