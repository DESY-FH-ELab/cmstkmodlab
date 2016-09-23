#include <iostream>

#include <QGridLayout>
#include <QFormLayout>

#include <nqlogger.h>

#include "NanotecSMCI36Widget.h"

NanotecSMCI36StepModeWidget::NanotecSMCI36StepModeWidget(NanotecSMCI36Model* model,
                                                         QWidget *parent)
  : QComboBox(parent),
    model_(model)
{
  // No user editable text
  setEditable(false);

  const std::vector<std::pair<int,std::string>>& modes = model_->getStepModeNames();

  for (std::vector<std::pair<int,std::string>>::const_iterator it = modes.begin();
       it!=modes.end();
       ++it) {
    addItem(it->second.c_str(), QVariant(it->first));
  }

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(indexChanged(int)));

  updateInfo();
}

void NanotecSMCI36StepModeWidget::indexChanged(int index)
{
  // NQLog("NanotecSMCI36StepModeWidget", NQLog::Debug) << "indexChanged()";

  int userValue = itemData(index).toInt();

  if (model_->getStepMode()!=userValue) {
    model_->setStepMode(userValue);
  }
}

void NanotecSMCI36StepModeWidget::updateInfo()
{
  NQLog("NanotecSMCI36StepModeWidget", NQLog::Debug) << "updateInfo()";

  int mode = model_->getStepMode();
  int index = findData(mode);
  if (index!=currentIndex()) {
    setCurrentIndex(index);
  }
}

NanotecSMCI36ErrorCorrectionModeWidget::NanotecSMCI36ErrorCorrectionModeWidget(NanotecSMCI36Model* model,
                                                                               QWidget *parent)
  : QComboBox(parent),
    model_(model)
{
  // No user editable text
  setEditable(false);

  const std::vector<std::pair<int,std::string>>& modes = model_->getErrorCorrectionModeNames();

  for (std::vector<std::pair<int,std::string>>::const_iterator it = modes.begin();
       it!=modes.end();
       ++it) {
    addItem(it->second.c_str(), QVariant(it->first));
  }

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(indexChanged(int)));

  updateInfo();
}

void NanotecSMCI36ErrorCorrectionModeWidget::indexChanged(int index)
{
  // NQLog("NanotecSMCI36ErrorCorrectionModeWidget", NQLog::Debug) << "indexChanged()";

  int userValue = itemData(index).toInt();

  if (model_->getErrorCorrectionMode()!=userValue) {
    model_->setErrorCorrectionMode(userValue);
  }
}

void NanotecSMCI36ErrorCorrectionModeWidget::updateInfo()
{
  NQLog("NanotecSMCI36ErrorCorrectionModeWidget", NQLog::Debug) << "updateInfo()";

  int mode = model_->getErrorCorrectionMode();
  int index = findData(mode);
  if (index!=currentIndex()) {
    setCurrentIndex(index);
  }
}

NanotecSMCI36RampModeWidget::NanotecSMCI36RampModeWidget(NanotecSMCI36Model* model,
                                                         QWidget *parent)
  : QComboBox(parent),
    model_(model)
{
  // No user editable text
  setEditable(false);

  const std::vector<std::pair<int,std::string>>& modes = model_->getRampModeNames();

  for (std::vector<std::pair<int,std::string>>::const_iterator it = modes.begin();
       it!=modes.end();
       ++it) {
    addItem(it->second.c_str(), QVariant(it->first));
  }

  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(indexChanged(int)));

  updateInfo();
}

void NanotecSMCI36RampModeWidget::indexChanged(int index)
{
  // NQLog("NanotecSMCI36RampModeWidget", NQLog::Debug) << "indexChanged()";

  int userValue = itemData(index).toInt();

  if (model_->getRampMode()!=userValue) {
    model_->setRampMode(userValue);
  }
}

void NanotecSMCI36RampModeWidget::updateInfo()
{
  NQLog("NanotecSMCI36RampModeWidget", NQLog::Debug) << "updateInfo()";

  int mode = model_->getRampMode();
  int index = findData(mode);
  if (index!=currentIndex()) {
    setCurrentIndex(index);
  }
}

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
          this, SLOT(indexChanged(int)));

  updateInfo();
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

  int mode = model_->getPositioningMode();
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
  formLayout->setVerticalSpacing(2);

  pitch_ = new QDoubleSpinBox(this);
  pitch_->setMinimum(1);
  pitch_->setMaximum(1000000);
  connect(pitch_, SIGNAL(valueChanged(double)),
          model_, SLOT(setPitch(double)));
  formLayout->addRow("pitch [full steps/mm]", pitch_);

  status_ = new QLabel("         ", this);
  formLayout->addRow("status", status_);

  motorID_ = new QSpinBox(this);
  motorID_->setMinimum(0);
  motorID_->setMaximum(2147483647);
  connect(motorID_, SIGNAL(valueChanged(int)),
          model_, SLOT(setMotorID(int)));
  formLayout->addRow("motor ID", motorID_);

  stepMode_ = new NanotecSMCI36StepModeWidget(model_, this);
  formLayout->addRow("step mode", stepMode_);

  errorCorrectionMode_ = new NanotecSMCI36ErrorCorrectionModeWidget(model_, this);
  formLayout->addRow("error correction mode", errorCorrectionMode_);

  maxEncoderDeviation_ = new QSpinBox(this);
  maxEncoderDeviation_->setMinimum(0);
  maxEncoderDeviation_->setMaximum(250);
  connect(maxEncoderDeviation_, SIGNAL(valueChanged(int)),
          model_, SLOT(setMaxEncoderDeviation(int)));
  formLayout->addRow("max encoder deviation", maxEncoderDeviation_);

  rampMode_ = new NanotecSMCI36RampModeWidget(model_, this);
  formLayout->addRow("ramp mode", rampMode_);

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
  formLayout->addRow("travel distance [steps]", travelDistance_);

  minFrequency_ = new QDoubleSpinBox(this);
  minFrequency_->setMinimum(1);
  minFrequency_->setMaximum(160000);
  connect(minFrequency_, SIGNAL(valueChanged(double)),
          model_, SLOT(setMinFrequency(double)));
  formLayout->addRow("min. speed [steps/s]", minFrequency_);

  maxFrequency_ = new QDoubleSpinBox(this);
  maxFrequency_->setMinimum(1);
  maxFrequency_->setMaximum(512000);
  connect(maxFrequency_, SIGNAL(valueChanged(double)),
          model_, SLOT(setMaxFrequency(double)));
  formLayout->addRow("max. speed [steps/s]", maxFrequency_);

  maxFrequency2_ = new QDoubleSpinBox(this);
  maxFrequency2_->setMinimum(1);
  maxFrequency2_->setMaximum(512000);
  connect(maxFrequency2_, SIGNAL(valueChanged(double)),
          model_, SLOT(setMaxFrequency2(double)));
  formLayout->addRow("max. speed 2 [steps/s]", maxFrequency2_);

  travelDistanceInMM_ = new QDoubleSpinBox(this);
  travelDistanceInMM_->setMinimum(-100000);
  travelDistanceInMM_->setMaximum(+100000);
  connect(travelDistanceInMM_, SIGNAL(valueChanged(double)),
          model_, SLOT(setTravelDistanceInMM(double)));
  formLayout->addRow("travel distance [mm]", travelDistanceInMM_);

  minSpeed_ = new QDoubleSpinBox(this);
  minSpeed_->setMinimum(1);
  minSpeed_->setMaximum(160000);
  connect(minSpeed_, SIGNAL(valueChanged(double)),
          model_, SLOT(setMinSpeed(double)));
  formLayout->addRow("min. speed [mm/s]", minSpeed_);

  maxSpeed_ = new QDoubleSpinBox(this);
  maxSpeed_->setMinimum(1);
  maxSpeed_->setMaximum(160000);
  connect(maxSpeed_, SIGNAL(valueChanged(double)),
          model_, SLOT(setMaxSpeed(double)));
  formLayout->addRow("max. speed [mm/s]", maxSpeed_);

  maxSpeed2_ = new QDoubleSpinBox(this);
  maxSpeed2_->setMinimum(1);
  maxSpeed2_->setMaximum(160000);
  connect(maxSpeed2_, SIGNAL(valueChanged(double)),
          model_, SLOT(setMaxSpeed(double)));
  formLayout->addRow("max. speed 2 [mm/s]", maxSpeed2_);

  layout->addLayout(formLayout);

  QHBoxLayout* hlayout = new QHBoxLayout();

  start_ = new QPushButton("start", this);
  connect(start_, SIGNAL(clicked()),
          model_, SLOT(start()));
  hlayout->addWidget(start_);

  stop_ = new QPushButton("stop", this);
  connect(stop_, SIGNAL(clicked()),
          model_, SLOT(stop()));
  hlayout->addWidget(stop_);

  quickstop_ = new QPushButton("quick stop", this);
  connect(quickstop_, SIGNAL(clicked()),
          model_, SLOT(stop()));
  hlayout->addWidget(quickstop_);

  resetPositionError_ = new QPushButton("reset position error", this);
  connect(resetPositionError_, SIGNAL(clicked()),
          model_, SLOT(resetPositionError()));
  hlayout->addWidget(resetPositionError_);

  layout->addLayout(hlayout);

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

  controlStateChanged(ready);
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void NanotecSMCI36Widget::controlStateChanged(bool enabled)
{
  stageMode_->setEnabled(enabled);

  pitch_->setEnabled(enabled);
  status_->setEnabled(enabled);
  motorID_->setEnabled(enabled);

  maxEncoderDeviation_->setEnabled(enabled);

  direction_->setEnabled(enabled);

    travelDistance_->setEnabled(enabled);
    minFrequency_->setEnabled(enabled);
    maxFrequency_->setEnabled(enabled);
    maxFrequency2_->setEnabled(enabled);

    travelDistanceInMM_->setEnabled(!enabled);
    minSpeed_->setEnabled(!enabled);
    maxSpeed_->setEnabled(!enabled);
    maxSpeed2_->setEnabled(!enabled);
  }

  start_->setEnabled(enabled);
  stop_->setEnabled(enabled);
  quickstop_->setEnabled(enabled);
  resetPositionError_->setEnabled(enabled);
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
  maxEncoderDeviation_->setValue(model_->getMaxEncoderDeviation());

  controllerSteps_->setText(QString::number(model_->getControllerSteps()));
  encoderSteps_->setText(QString::number(model_->getEncoderSteps()));

  direction_->setChecked(model_->getDirection());

  travelDistance_->setValue(model_->getTravelDistance());
  minFrequency_->setValue(model_->getMinFrequency());
  maxFrequency_->setValue(model_->getMaxFrequency());
  maxFrequency2_->setValue(model_->getMaxFrequency2());

  travelDistanceInMM_->setValue(model_->getTravelDistanceInMM());
  minSpeed_->setValue(model_->getMinSpeed());
  maxSpeed_->setValue(model_->getMaxSpeed());
  maxSpeed2_->setValue(model_->getMaxSpeed2());
}
