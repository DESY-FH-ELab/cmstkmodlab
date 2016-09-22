#include <iostream>

#include <QGridLayout>
#include <QFormLayout>

#include <nqlogger.h>

#include "NanotecSMCI36Widget.h"

NanotecSMCI36PositioningModeWidget::NanotecSMCI36PositioningModeWidget(NanotecSMCI36Model* model,
                                                                       QWidget *parent)
  : QComboBox(parent),
    model_(model)
{
  // No user editable text
  setEditable(false);

  const std::vector<std::pair<int,std::string>>& modes = model_->getPositioningModeNames();

  for (std::vector<std::pair<int,std::string>>::const_iterator it = modes.begin();
       it!=modes.end();
       ++it) {
    addItem(it->second.c_str(), QVariant(it->first));
  }

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(currentChanged(int)));
}

void NanotecSMCI36PositioningModeWidget::indexChanged(int index)
{
  // NQLog("NanotecSMCI36PositioningModeWidget", NQLog::Debug) << "indexChanged()";

  int userValue = itemData(index).toInt();

  if (model_->getPositioningMode()!=userValue) {
    model_->setPositioningMode(userValue);
  }
}

void NanotecSMCI36PositioningModeWidget::updateInfo()
{
  // NQLog("NanotecSMCI36PositioningModeWidget", NQLog::Debug) << "updateInfo()";

  unsigned int mode = model_->getPositioningMode();
  int index = findData(mode);
  if (index!=currentIndex()) {
    setCurrentIndex(index);
  }
}


NanotecSMCI36Widget::NanotecSMCI36Widget(NanotecSMCI36Model* model, QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout();
  setLayout(layout);

  smci36CheckBox_ = new QCheckBox("Enable controller", this);
  layout->addWidget(smci36CheckBox_);

  QFormLayout *formLayout = new QFormLayout;

  status_ = new QLabel("         ", this);
  formLayout->addRow("status", status_);

  motorID_ = new QSpinBox(this);
  motorID_->setMinimum(0);
  motorID_->setMaximum(2147483647);
  connect(motorID_, SIGNAL(valueChanged(int)),
          model_, SLOT(setMotorID(int)));
  formLayout->addRow("motor ID", motorID_);

  controllerSteps_ = new QLabel("         ", this);
  formLayout->addRow("controller steps", controllerSteps_);

  encoderSteps_ = new QLabel("         ", this);
  formLayout->addRow("encoder steps", encoderSteps_);

  positioningMode_ = new NanotecSMCI36PositioningModeWidget(model_, this);
  formLayout->addRow("positioning mode", positioningMode_);

  direction_ = new QCheckBox(this);
  connect(direction_, SIGNAL(clicked(bool)),
          model_, SLOT(setDirection(bool)));
  formLayout->addRow("direction", direction_);

  travelDistance_ = new QDoubleSpinBox(this);
  travelDistance_->setMinimum(-100000);
  travelDistance_->setMaximum(+100000);
  connect(travelDistance_, SIGNAL(valueChanged(double)),
          model_, SLOT(setTravelDistance(double)));
  formLayout->addRow("travel distance", travelDistance_);

  layout->addLayout(formLayout);

  start_ = new QPushButton("start", this);
  connect(start_, SIGNAL(clicked()),
          model_, SLOT(start()));
  layout->addWidget(start_);

  stop_ = new QPushButton("stop", this);
  connect(stop_, SIGNAL(clicked()),
          model_, SLOT(stop()));
  layout->addWidget(stop_);

  quickstop_ = new QPushButton("quick stop", this);
  connect(quickstop_, SIGNAL(clicked()),
          model_, SLOT(stop()));
  layout->addWidget(quickstop_);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(smci36CheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setDeviceEnabled(bool)));

  connect(motorID_, SIGNAL(valueChanged(int)),
          model_, SLOT(setMotorID(int)));

  // Set GUI according to the current chiller state
  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
  Updates the GUI according to the new state of the chiller chiller.
 */
void NanotecSMCI36Widget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);
  smci36CheckBox_->setChecked(ready);
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void NanotecSMCI36Widget::controlStateChanged(bool enabled)
{
  smci36CheckBox_->setEnabled(enabled);
  if (enabled) {
    //State state = model_->getDeviceState();
  }
}

/**
  Sets the values of all the subelements (except the global enablement)
  according to the model.
 */
void NanotecSMCI36Widget::updateInfo()
{
  //NQLog("NanotecSMCI36Widget", NQLog::Debug) << "updateInfo()";

  unsigned int status = model_->getStatus();
  status_->setText(QString::number(status));

  motorID_->setValue(model_->getMotorID());

  controllerSteps_->setText(QString::number(model_->getControllerSteps()));
  encoderSteps_->setText(QString::number(model_->getEncoderSteps()));

  direction_->setChecked(model_->getDirection());
  travelDistance_->setValue(model_->getTravelDistance());

}
