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

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(indexChanged(int)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

void NanotecSMCI36StepModeWidget::indexChanged(int index)
{
  int userValue = itemData(index).toInt();

  if (model_->getStepMode()!=userValue) {
    model_->setStepMode(userValue);
  }
}

/**
  Updates the GUI according to the new state of the controller
 */
void NanotecSMCI36StepModeWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36StepModeWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);
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

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(indexChanged(int)));

  updateDeviceState(model_->getDeviceState());
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

/**
  Updates the GUI according to the new state of the controller
 */
void NanotecSMCI36ErrorCorrectionModeWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36ErrorCorrectionModeWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);
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

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(indexChanged(int)));

  updateDeviceState(model_->getDeviceState());
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

/**
  Updates the GUI according to the new state of the controller
 */
void NanotecSMCI36RampModeWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36RampModeWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);
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
    if (it->first==VNanotecSMCI36::smciAbsolutePositioning ||
        it->first==VNanotecSMCI36::smciExternalRefRun) {
    addItem(it->second.c_str(), QVariant(it->first));
    }
  }

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(indexChanged(int)));

  updateDeviceState(model_->getDeviceState());
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

/**
  Updates the GUI according to the new state of the controller
 */
void NanotecSMCI36PositioningModeWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36PositioningModeWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);
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

NanotecSMCI36StatusWidget::NanotecSMCI36StatusWidget(NanotecSMCI36Model* model, QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QFormLayout *formLayout = new QFormLayout;
  formLayout->setVerticalSpacing(2);
  setLayout(formLayout);

  status_ = new QLabel("         ", this);
  formLayout->addRow("status", status_);

  controllerSteps_ = new QLabel("         ", this);
  formLayout->addRow("controller steps", controllerSteps_);

  encoderSteps_ = new QLabel("         ", this);
  formLayout->addRow("encoder steps", encoderSteps_);

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
void NanotecSMCI36StatusWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36StatusWidget::controlStateChanged(bool /* enabled */)
{

}

void NanotecSMCI36StatusWidget::updateInfo()
{
  //NQLog("NanotecSMCI36StatusWidget", NQLog::Debug) << "updateInfo()";

  unsigned int status = model_->getStatus();
  status_->setText(QString::number(status));

  controllerSteps_->setText(QString("%1 mm (%2)").arg(model_->getControllerPosition(), 20, 'f', 3)
                                                 .arg(model_->getControllerSteps()));
  encoderSteps_->setText(QString("%1 mm (%2)").arg(model_->getEncoderPosition(), 20, 'f', 3)
                                              .arg(model_->getEncoderSteps()));
}

NanotecSMCI36SettingsWidget::NanotecSMCI36SettingsWidget(NanotecSMCI36Model* model, QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QFormLayout *formLayout = new QFormLayout;
  formLayout->setVerticalSpacing(2);
  setLayout(formLayout);

  pitch_ = new QDoubleSpinBox(this);
  pitch_->setDecimals(6);
  pitch_->setMinimum(0.001);
  pitch_->setMaximum(1);
  connect(pitch_, SIGNAL(valueChanged(double)),
          model_, SLOT(setPitch(double)));
  formLayout->addRow("pitch [mm/full step]", pitch_);

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
void NanotecSMCI36SettingsWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36SettingsWidget::controlStateChanged(bool enabled)
{
  pitch_->setEnabled(enabled);
  motorID_->setEnabled(enabled);

  maxEncoderDeviation_->setEnabled(enabled);
}

void NanotecSMCI36SettingsWidget::updateInfo()
{
  //NQLog("NanotecSMCI36SettingsWidget", NQLog::Debug) << "updateInfo()";

  pitch_->setValue(model_->getPitch());
  motorID_->setValue(model_->getMotorID());
  maxEncoderDeviation_->setValue(model_->getMaxEncoderDeviation());
}

NanotecSMCI36MovementWidget::NanotecSMCI36MovementWidget(NanotecSMCI36Model* model, QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(0);
  setLayout(layout);

  stageMode_ = new QCheckBox("stage mode", this);
  layout->addWidget(stageMode_);
  stageMode_->setChecked(true);
  connect(stageMode_, SIGNAL(clicked(bool)),
          this, SLOT(stageModeChanged(bool)));

  QFormLayout *formLayout = new QFormLayout;
  formLayout->setVerticalSpacing(2);

  positioningMode_ = new NanotecSMCI36PositioningModeWidget(model_, this);
  formLayout->addRow("positioning mode", positioningMode_);

  /*
  direction_ = new QCheckBox(this);
  connect(direction_, SIGNAL(clicked(bool)),
          model_, SLOT(setDirection(bool)));
  formLayout->addRow("direction", direction_);
  */

  layout->addLayout(formLayout);

  modeTabs_ = new QTabWidget(this);

  QWidget * widget = new QWidget(modeTabs_);

  formLayout = new QFormLayout;
  widget->setLayout(formLayout);
  formLayout->setVerticalSpacing(2);

  travelDistance_ = new QDoubleSpinBox(widget);
  travelDistance_->setMinimum(-100000);
  travelDistance_->setMaximum(+100000);
  connect(travelDistance_, SIGNAL(valueChanged(double)),
          model_, SLOT(setTravelDistance(double)));
  formLayout->addRow("travel distance [steps]", travelDistance_);

  minFrequency_ = new QDoubleSpinBox(widget);
  minFrequency_->setMinimum(1);
  minFrequency_->setMaximum(160000);
  connect(minFrequency_, SIGNAL(valueChanged(double)),
          model_, SLOT(setMinFrequency(double)));
  formLayout->addRow("min. speed [steps/s]", minFrequency_);

  maxFrequency_ = new QDoubleSpinBox(widget);
  maxFrequency_->setMinimum(1);
  maxFrequency_->setMaximum(512000);
  connect(maxFrequency_, SIGNAL(valueChanged(double)),
          model_, SLOT(setMaxFrequency(double)));
  formLayout->addRow("max. speed [steps/s]", maxFrequency_);

  maxFrequency2_ = new QDoubleSpinBox(widget);
  maxFrequency2_->setMinimum(1);
  maxFrequency2_->setMaximum(512000);
  connect(maxFrequency2_, SIGNAL(valueChanged(double)),
          model_, SLOT(setMaxFrequency2(double)));
  formLayout->addRow("max. speed 2 [steps/s]", maxFrequency2_);

  modeTabs_->addTab(widget, "step");

  widget = new QWidget(modeTabs_);

  formLayout = new QFormLayout;
  widget->setLayout(formLayout);
  formLayout->setVerticalSpacing(2);

  travelDistanceInMM_ = new QDoubleSpinBox(widget);
  travelDistanceInMM_->setMinimum(-100000);
  travelDistanceInMM_->setMaximum(+100000);
  connect(travelDistanceInMM_, SIGNAL(valueChanged(double)),
          model_, SLOT(setTravelDistanceInMM(double)));
  formLayout->addRow("travel distance [mm]", travelDistanceInMM_);

  minSpeed_ = new QDoubleSpinBox(widget);
  minSpeed_->setMinimum(1);
  minSpeed_->setMaximum(160000);
  connect(minSpeed_, SIGNAL(valueChanged(double)),
          model_, SLOT(setMinSpeed(double)));
  formLayout->addRow("min. speed [mm/s]", minSpeed_);

  maxSpeed_ = new QDoubleSpinBox(widget);
  maxSpeed_->setMinimum(1);
  maxSpeed_->setMaximum(160000);
  connect(maxSpeed_, SIGNAL(valueChanged(double)),
          model_, SLOT(setMaxSpeed(double)));
  formLayout->addRow("max. speed [mm/s]", maxSpeed_);

  maxSpeed2_ = new QDoubleSpinBox(widget);
  maxSpeed2_->setMinimum(1);
  maxSpeed2_->setMaximum(160000);
  connect(maxSpeed2_, SIGNAL(valueChanged(double)),
          model_, SLOT(setMaxSpeed(double)));
  formLayout->addRow("max. speed 2 [mm/s]", maxSpeed2_);

  minPosition_ = new QLabel(widget);
  formLayout->addRow("min. position [mm]", minPosition_);

  maxPosition_ = new QLabel(widget);
  formLayout->addRow("max. position [mm]", maxPosition_);

  modeTabs_->addTab(widget, "mm");

  layout->addWidget(modeTabs_);

  modeTabs_->setCurrentIndex(1);

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
  connect(model_, SIGNAL(positionModeChanged(int)),
          this, SLOT(positionModeChanged(int)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

void NanotecSMCI36MovementWidget::stageModeChanged(bool)
{
  bool ready = (model_->getDeviceState() == READY);
  controlStateChanged(ready);
}

void NanotecSMCI36MovementWidget::positionModeChanged(int mode)
{
  if (mode==VNanotecSMCI36::smciExternalRefRun) {
    maxSpeed_->setMaximum(model_->getMaxSpeedForRefRun());
    maxSpeed2_->setMaximum(model_->getMaxSpeedForRefRun());
  } else {
    maxSpeed_->setMaximum(model_->getMaxSpeedForOperation());
    maxSpeed2_->setMaximum(model_->getMaxSpeedForOperation());
  }
}

/**
  Updates the GUI according to the new state of the controller
 */
void NanotecSMCI36MovementWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36MovementWidget::controlStateChanged(bool enabled)
{
  stageMode_->setEnabled(enabled);

  // direction_->setEnabled(enabled);

  /*
  if (!stageMode_->isChecked()) {
    travelDistance_->setEnabled(enabled);
    minFrequency_->setEnabled(enabled);
    maxFrequency_->setEnabled(enabled);
    maxFrequency2_->setEnabled(enabled);

    travelDistanceInMM_->setEnabled(!enabled);
    minSpeed_->setEnabled(!enabled);
    maxSpeed_->setEnabled(!enabled);
    maxSpeed2_->setEnabled(!enabled);
  } else {
    travelDistance_->setEnabled(!enabled);
    minFrequency_->setEnabled(!enabled);
    maxFrequency_->setEnabled(!enabled);
    maxFrequency2_->setEnabled(!enabled);

    travelDistanceInMM_->setEnabled(enabled);
    minSpeed_->setEnabled(enabled);
    maxSpeed_->setEnabled(enabled);
    maxSpeed2_->setEnabled(enabled);
  }
  */

  start_->setEnabled(enabled);
  stop_->setEnabled(enabled);
  quickstop_->setEnabled(enabled);
  resetPositionError_->setEnabled(enabled);
}

void NanotecSMCI36MovementWidget::updateInfo()
{
  //NQLog("NanotecSMCI36Widget", NQLog::Debug) << "updateInfo()";

  // direction_->setChecked(model_->getDirection());

  travelDistance_->setValue(model_->getTravelDistance());
  minFrequency_->setValue(model_->getMinFrequency());
  maxFrequency_->setValue(model_->getMaxFrequency());
  maxFrequency2_->setValue(model_->getMaxFrequency2());

  travelDistanceInMM_->setValue(model_->getTravelDistanceInMM());
  minSpeed_->setValue(model_->getMinSpeed());
  maxSpeed_->setValue(model_->getMaxSpeed());
  maxSpeed2_->setValue(model_->getMaxSpeed2());

  minPosition_->setText(QString::number(model_->getMinPositionInMM()));
  maxPosition_->setText(QString::number(model_->getMaxPositionInMM()));
}

NanotecSMCI36Widget::NanotecSMCI36Widget(NanotecSMCI36Model* model, QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(0);
  setLayout(layout);

  smci36CheckBox_ = new QCheckBox("Enable controller", this);
  layout->addWidget(smci36CheckBox_);

  scmi36Status_ = new NanotecSMCI36StatusWidget(model_, this);
  layout->addWidget(scmi36Status_);

  scmi36Settings_ = new NanotecSMCI36SettingsWidget(model_, this);
  layout->addWidget(scmi36Settings_);

  scmi36Movement_ = new NanotecSMCI36MovementWidget(model_, this);
  layout->addWidget(scmi36Movement_);

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(smci36CheckBox_, SIGNAL(toggled(bool)),
          model_, SLOT(setDeviceEnabled(bool)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

/**
  Updates the GUI according to the new state of the controller
 */
void NanotecSMCI36Widget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);
  smci36CheckBox_->setChecked(ready);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36Widget::controlStateChanged(bool /* enabled */)
{

}

void NanotecSMCI36Widget::updateInfo()
{
  //NQLog("NanotecSMCI36Widget", NQLog::Debug) << "updateInfo()";
}

NanotecSMCI36InputReversePolarityWidget::NanotecSMCI36InputReversePolarityWidget(NanotecSMCI36Model* model,
                                                                   int pin,
                                                                   QWidget *parent)
  : QCheckBox(parent),
    model_(model),
    pin_(pin)
{
  setText("rev. pol.");

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(stateChanged(int)),
          this, SLOT(polarityChanged(int)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

void NanotecSMCI36InputReversePolarityWidget::polarityChanged(int /* polarity */)
{
  // NQLog("NanotecSMCI36PositioningModeWidget", NQLog::Debug) << "indexChanged()";

  if (model_->getInputPolarity(pin_)!=isChecked()) {
    model_->setInputPolarity(pin_, isChecked());
  }
}

/**
  Updates the GUI according to the new state of the controller
 */
void NanotecSMCI36InputReversePolarityWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36InputReversePolarityWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);
}

void NanotecSMCI36InputReversePolarityWidget::updateInfo()
{
  // NQLog("NanotecSMCI36InputReversePolarityWidget", NQLog::Debug) << "updateInfo()";

  bool checked = model_->getInputPolarity(pin_);
  if (isChecked()!=checked) {
    setChecked(checked);
  }
}

NanotecSMCI36OutputReversePolarityWidget::NanotecSMCI36OutputReversePolarityWidget(NanotecSMCI36Model* model,
                                                                                   int pin,
                                                                                   QWidget *parent)
  : QCheckBox(parent),
    model_(model),
    pin_(pin)
{
  setText("rev. pol.");

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(stateChanged(int)),
          this, SLOT(polarityChanged(int)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

void NanotecSMCI36OutputReversePolarityWidget::polarityChanged(int /* polarity */)
{
  // NQLog("NanotecSMCI36OutputReversePolarityWidget", NQLog::Debug) << "indexChanged()";

  if (model_->getOutputPolarity(pin_)!=isChecked()) {
    model_->setOutputPolarity(pin_, isChecked());
  }
}

/**
  Updates the GUI according to the new state of the controller
 */
void NanotecSMCI36OutputReversePolarityWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36OutputReversePolarityWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);
}

void NanotecSMCI36OutputReversePolarityWidget::updateInfo()
{
  // NQLog("NanotecSMCI36OutputReversePolarityWidget", NQLog::Debug) << "updateInfo()";

  bool checked = model_->getOutputPolarity(pin_);
  if (isChecked()!=checked) {
    setChecked(checked);
  }
}

NanotecSMCI36InputStateWidget::NanotecSMCI36InputStateWidget(NanotecSMCI36Model* model,
                                                             int pin,
                                                             QWidget *parent)
  : QLabel(parent),
    model_(model),
    pin_(pin)
{
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
void NanotecSMCI36InputStateWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36InputStateWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);
}

void NanotecSMCI36InputStateWidget::updateInfo()
{
  // NQLog("NanotecSMCI36InputStateWidget", NQLog::Debug) << "updateInfo()";

  bool state = model_->getInputPinState(pin_);
  if (state) {
    setText("on");
  } else {
    setText("off");
  }
}

NanotecSMCI36OutputStateWidget::NanotecSMCI36OutputStateWidget(NanotecSMCI36Model* model,
                                                               int pin,
                                                               QWidget *parent)
  : QCheckBox(parent),
    model_(model),
    pin_(pin)
{
  setText("");

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(stateChanged(int)),
          this, SLOT(outputChanged(int)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

void NanotecSMCI36OutputStateWidget::outputChanged(int /* polarity */)
{
  // NQLog("NanotecSMCI36OutputStateWidget", NQLog::Debug) << "indexChanged()";

  if (model_->getOutputPinState(pin_)!=isChecked()) {
    model_->setOutputPinState(pin_, isChecked());
  }
}

/**
  Updates the GUI according to the new state of the controller
 */
void NanotecSMCI36OutputStateWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36OutputStateWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);
}

void NanotecSMCI36OutputStateWidget::updateInfo()
{
  // NQLog("NanotecSMCI36OutputStateWidget", NQLog::Debug) << "updateInfo()";

  bool checked = model_->getOutputPinState(pin_);
  if (isChecked()!=checked) {
    setChecked(checked);
  }
}

NanotecSMCI36InputFunctionWidget::NanotecSMCI36InputFunctionWidget(NanotecSMCI36Model* model,
                                                                   int pin,
                                                                   QWidget *parent)
  : QComboBox(parent),
    model_(model),
    pin_(pin)
{
  // No user editable text
  setEditable(false);

  const std::vector<std::pair<int,std::string>>& modes = model_->getInputPinFunctionNames();

  for (std::vector<std::pair<int,std::string>>::const_iterator it = modes.begin();
       it!=modes.end();
       ++it) {
    addItem(it->second.c_str(), QVariant(it->first));
  }

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(indexChanged(int)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

void NanotecSMCI36InputFunctionWidget::indexChanged(int index)
{
  // NQLog("NanotecSMCI36InputFunctionWidget", NQLog::Debug) << "indexChanged()";

  int userValue = itemData(index).toInt();

  if (model_->getInputPinFunction(pin_)!=userValue) {
    model_->setInputPinFunction(pin_, userValue);
  }
}

/**
  Updates the GUI according to the new state of the controller
 */
void NanotecSMCI36InputFunctionWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36InputFunctionWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);
}

void NanotecSMCI36InputFunctionWidget::updateInfo()
{
  // NQLog("NanotecSMCI36InputFunctionWidget", NQLog::Debug) << "updateInfo()";

  int function = model_->getInputPinFunction(pin_);
  int index = findData(function);
  if (index!=currentIndex()) {
    setCurrentIndex(index);
  }
}

NanotecSMCI36OutputFunctionWidget::NanotecSMCI36OutputFunctionWidget(NanotecSMCI36Model* model,
                                                                     int pin,
                                                                     QWidget *parent)
  : QComboBox(parent),
    model_(model),
    pin_(pin)
{
  // No user editable text
  setEditable(false);

  const std::vector<std::pair<int,std::string>>& modes = model_->getOutputPinFunctionNames();

  for (std::vector<std::pair<int,std::string>>::const_iterator it = modes.begin();
       it!=modes.end();
       ++it) {
    addItem(it->second.c_str(), QVariant(it->first));
  }

  // Connect all the signals
  connect(model_, SIGNAL(deviceStateChanged(State)),
          this, SLOT(updateDeviceState(State)));
  connect(model_, SIGNAL(controlStateChanged(bool)),
          this, SLOT(controlStateChanged(bool)));
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  connect(this, SIGNAL(currentIndexChanged(int)),
          this, SLOT(indexChanged(int)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
}

void NanotecSMCI36OutputFunctionWidget::indexChanged(int index)
{
  // NQLog("NanotecSMCI36OutputFunctionWidget", NQLog::Debug) << "indexChanged()";

  int userValue = itemData(index).toInt();

  if (model_->getOutputPinFunction(pin_)!=userValue) {
    model_->setOutputPinFunction(pin_, userValue);
  }
}

/**
  Updates the GUI according to the new state of the controller
 */
void NanotecSMCI36OutputFunctionWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36OutputFunctionWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);
}

void NanotecSMCI36OutputFunctionWidget::updateInfo()
{
  // NQLog("NanotecSMCI36OutputFunctionWidget", NQLog::Debug) << "updateInfo()";

  int function = model_->getInputPinFunction(pin_);
  int index = findData(function);
  if (index!=currentIndex()) {
    setCurrentIndex(index);
  }
}

NanotecSMCI36InputWidget::NanotecSMCI36InputWidget(NanotecSMCI36Model* model,
                                                   int pin,
                                                   QWidget *parent)
  : QWidget(parent),
    model_(model),
    pin_(pin)
{
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setSpacing(4);
  setLayout(layout);

  QLabel *label = new QLabel(QString("input %1").arg(pin_), this);
  layout->addWidget(label);

  NanotecSMCI36InputFunctionWidget *functionWidget = new NanotecSMCI36InputFunctionWidget(model_, pin_, this);
  layout->addWidget(functionWidget);

  NanotecSMCI36InputReversePolarityWidget *polarityWidget = new NanotecSMCI36InputReversePolarityWidget(model_, pin_, this);
  layout->addWidget(polarityWidget);

  NanotecSMCI36InputStateWidget *stateWidget = new NanotecSMCI36InputStateWidget(model_, pin_, this);
  layout->addWidget(stateWidget);

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
void NanotecSMCI36InputWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36InputWidget::controlStateChanged(bool /* enabled */)
{
  // setEnabled(enabled);
}

void NanotecSMCI36InputWidget::updateInfo()
{
  // NQLog("NanotecSMCI36InputWidget", NQLog::Debug) << "updateInfo()";
}

NanotecSMCI36OutputWidget::NanotecSMCI36OutputWidget(NanotecSMCI36Model* model,
                                                     int pin,
                                                     QWidget *parent)
  : QWidget(parent),
    model_(model),
    pin_(pin)
{
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setSpacing(4);
  setLayout(layout);

  QLabel *label = new QLabel(QString("output %1").arg(pin_), this);
  layout->addWidget(label);

  NanotecSMCI36OutputFunctionWidget *functionWidget = new NanotecSMCI36OutputFunctionWidget(model_, pin_, this);
  layout->addWidget(functionWidget);

  NanotecSMCI36OutputReversePolarityWidget *polarityWidget = new NanotecSMCI36OutputReversePolarityWidget(model_, pin_, this);
  layout->addWidget(polarityWidget);

  NanotecSMCI36OutputStateWidget *stateWidget = new NanotecSMCI36OutputStateWidget(model_, pin_, this);
  layout->addWidget(stateWidget);

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
void NanotecSMCI36OutputWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36OutputWidget::controlStateChanged(bool /* enabled */)
{
  // setEnabled(enabled);
}

void NanotecSMCI36OutputWidget::updateInfo()
{
  // NQLog("NanotecSMCI36OutputWidget", NQLog::Debug) << "updateInfo()";
}

NanotecSMCI36IOWidget::NanotecSMCI36IOWidget(NanotecSMCI36Model* model, QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QGridLayout* layout = new QGridLayout();
  setLayout(layout);

  int row = 0;
  for (int i=1;i<7;++i) {
    QLabel *label = new QLabel(QString("input %1").arg(i), this);
    layout->addWidget(label, row, 0);

    NanotecSMCI36InputFunctionWidget *functionWidget = new NanotecSMCI36InputFunctionWidget(model_, i, this);
    layout->addWidget(functionWidget, row, 1);

    NanotecSMCI36InputReversePolarityWidget *polarityWidget = new NanotecSMCI36InputReversePolarityWidget(model_, i, this);
    layout->addWidget(polarityWidget, row, 2);

    NanotecSMCI36InputStateWidget *stateWidget = new NanotecSMCI36InputStateWidget(model_, i, this);
    layout->addWidget(stateWidget, row, 3);

    row++;
  }

  for (int i=1;i<4;++i) {
    QLabel *label = new QLabel(QString("output %1").arg(i), this);
    layout->addWidget(label, row, 0);

    NanotecSMCI36OutputFunctionWidget *functionWidget = new NanotecSMCI36OutputFunctionWidget(model_, i, this);
    layout->addWidget(functionWidget, row, 1);

    NanotecSMCI36OutputReversePolarityWidget *polarityWidget = new NanotecSMCI36OutputReversePolarityWidget(model_, i, this);
    layout->addWidget(polarityWidget, row, 2);

    NanotecSMCI36OutputStateWidget *stateWidget = new NanotecSMCI36OutputStateWidget(model_, i, this);
    layout->addWidget(stateWidget, row, 3);

    row++;
  }

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
void NanotecSMCI36IOWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36IOWidget::controlStateChanged(bool /* enabled */)
{

}

void NanotecSMCI36IOWidget::updateInfo()
{
  //NQLog("NanotecSMCI36IOWidget", NQLog::Debug) << "updateInfo()";
}
