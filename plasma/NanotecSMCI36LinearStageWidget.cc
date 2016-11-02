#include <iostream>

#include <QGridLayout>
#include <QFormLayout>

#include <ApplicationConfig.h>
#include <nqlogger.h>

#include "NanotecSMCI36LinearStageWidget.h"

NanotecSMCI36LinearStageStatusWidget::NanotecSMCI36LinearStageStatusWidget(NanotecSMCI36LinearStageModel* model,
                                                                           QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QFormLayout *formLayout = new QFormLayout;
  formLayout->setSpacing(0);
  formLayout->setMargin(0);
  formLayout->setContentsMargins(0, 0, 0, 0);
  formLayout->setVerticalSpacing(2);
  formLayout->setHorizontalSpacing(8);
  setLayout(formLayout);

  status_ = new QLabel("", this);
  formLayout->addRow("status", status_);

  position_ = new QLCDNumber(10, this);
  position_->setMode(QLCDNumber::Dec);
  position_->setSegmentStyle(QLCDNumber::Filled);
  position_->setMinimumHeight(60);
  position_->setFrameStyle(0);
  formLayout->addRow("position [mm]", position_);

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

void NanotecSMCI36LinearStageStatusWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36LinearStageStatusWidget::controlStateChanged(bool enabled)
{

}

void NanotecSMCI36LinearStageStatusWidget::updateInfo()
{
  //NQLog("NanotecSMCI36StatusWidget", NQLog::Debug) << "updateInfo()";

  status_->setText(QString::number(model_->getStatus()));
  position_->display(QString::number(model_->getPosition(), 'f', 4));
}

NanotecSMCI36LinearStageMovementWidget::NanotecSMCI36LinearStageMovementWidget(NanotecSMCI36LinearStageModel* model,
                                                                               QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->setMargin(0);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  QGridLayout* gridLayout = new QGridLayout();

  QLabel *label;

  label = new QLabel("position [mm]", this);
  gridLayout->addWidget(label, 0, 0, Qt::AlignVCenter);

  label = new QLabel("speed [mm/s]", this);
  gridLayout->addWidget(label, 0, 1, Qt::AlignVCenter);

  position_ = new QDoubleSpinBox(this);
  position_->setMinimum(model_->getMinimumPosition());
  position_->setMaximum(model_->getMaximumPosition());
  position_->setDecimals(4);
  position_->setSingleStep(10);
  gridLayout->addWidget(position_, 1, 0, Qt::AlignVCenter);

  speed_ = new QDoubleSpinBox(this);
  speed_->setMinimum(model_->getSpeedLimits().first);
  speed_->setMaximum(model_->getSpeedLimits().second);
  speed_->setDecimals(4);
  speed_->setSingleStep(0.1);
  connect(speed_, SIGNAL(valueChanged(double)),
          model_, SLOT(setSpeed(double)));
  gridLayout->addWidget(speed_, 1, 1, Qt::AlignVCenter);

  moveButton_ = new QPushButton("Move", this);
  connect(moveButton_, SIGNAL(clicked()),
          this, SLOT(moveButtonClicked()));
  gridLayout->addWidget(moveButton_, 1, 2);

  refRunButton_ = new QPushButton("Reference Run", this);
  connect(refRunButton_, SIGNAL(clicked()),
          model_, SLOT(requestReferenceRun()));
  gridLayout->addWidget(refRunButton_, 2, 0, 1, 3);

  stopButton_ = new QPushButton("Stop", this);
  connect(stopButton_, SIGNAL(clicked()),
          model_, SLOT(requestStop()));
  gridLayout->addWidget(stopButton_, 3, 0, 1, 3);

  quickstopButton_ = new QPushButton("Quick Stop", this);
  connect(quickstopButton_, SIGNAL(clicked()),
          model_, SLOT(requestQuickStop()));
  gridLayout->addWidget(quickstopButton_, 4, 0, 1, 3);

  resetErrorButton_ = new QPushButton("Reset Position Error", this);
  connect(resetErrorButton_, SIGNAL(clicked()),
          model_, SLOT(requestResetError()));
  gridLayout->addWidget(resetErrorButton_, 5, 0, 1, 3);

  layout->addLayout(gridLayout);

  layout->addStretch(1);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));
  connect(model_, SIGNAL(limitsChanged()),
          this, SLOT(limitsChanged()));
  connect(this, SIGNAL(requestMove(double)),
          model_, SLOT(requestMove(double)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

void NanotecSMCI36LinearStageMovementWidget::moveButtonClicked()
{
  emit requestMove(position_->value());
}

void NanotecSMCI36LinearStageMovementWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36LinearStageMovementWidget::controlStateChanged(bool enabled)
{
  position_->setEnabled(enabled);
  speed_->setEnabled(enabled);
}

void NanotecSMCI36LinearStageMovementWidget::updateInfo()
{
  //NQLog("NanotecSMCI36Widget", NQLog::Debug) << "updateInfo()";

  position_->setValue(model_->getPosition());
  speed_->setValue(model_->getSpeed());
}

void NanotecSMCI36LinearStageMovementWidget::limitsChanged()
{
  speed_->setMinimum(model_->getSpeedLimits().first);
  speed_->setMaximum(model_->getSpeedLimits().second);
}

NanotecSMCI36LinearStageWidget::NanotecSMCI36LinearStageWidget(NanotecSMCI36LinearStageModel* model,
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
void NanotecSMCI36LinearStageWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36LinearStageWidget::controlStateChanged(bool /* enabled */)
{

}

void NanotecSMCI36LinearStageWidget::updateInfo()
{
  //NQLog("NanotecSMCI36Widget", NQLog::Debug) << "updateInfo()";
}
