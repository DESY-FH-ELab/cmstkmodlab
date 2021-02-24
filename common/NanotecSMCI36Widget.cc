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

#include "NanotecSMCI36Widget.h"

NanotecSMCI36ErrorCorrectionModeWidget::NanotecSMCI36ErrorCorrectionModeWidget(NanotecSMCI36Model* model,
                                                                               QWidget *parent)
  : QComboBox(parent),
    model_(model)
{
  // No user editable text
  setEditable(false);

  for (const auto& kv : model_->getErrorCorrectionModeNames()) {
    addItem(kv.second.c_str(), QVariant(kv.first));
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
  bool ready = model_->isReady();

  bool theEnabled;
  if (enabled) {
    if (ready) {
      theEnabled = true;
    } else {
      theEnabled = false;
    }
  } else {
    theEnabled = false;
  }

  setEnabled(theEnabled);
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

  for (const auto& kv : model_->getRampModeNames()) {
    addItem(kv.second.c_str(), QVariant(kv.first));
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
  bool ready = model_->isReady();

  bool theEnabled;
  if (enabled) {
    if (ready) {
      theEnabled = true;
    } else {
      theEnabled = false;
    }
  } else {
    theEnabled = false;
  }

  setEnabled(theEnabled);
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

  for (const auto& kv : model_->getPositioningModeNames()) {
    if (kv.first==VNanotecSMCI36::smciRelativePositioning ||
        kv.first==VNanotecSMCI36::smciAbsolutePositioning ||
        kv.first==VNanotecSMCI36::smciExternalRefRun) {
      addItem(kv.second.c_str(), QVariant(kv.first));
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
  bool ready = model_->isReady();

  bool theEnabled;
  if (enabled) {
    if (ready) {
      theEnabled = true;
    } else {
      theEnabled = false;
    }
  } else {
    theEnabled = false;
  }

  setEnabled(theEnabled);
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
  formLayout->setSpacing(0);
  formLayout->setMargin(0);
  formLayout->setContentsMargins(0, 0, 0, 0);
  formLayout->setVerticalSpacing(2);
  formLayout->setHorizontalSpacing(8);
  setLayout(formLayout);

  status_ = new QLabel("", this);
  status_->setFont(QFont("Helvetica", 18, QFont::DemiBold));
  formLayout->addRow("status", status_);

  controllerSteps_ = new QLabel("", this);
  formLayout->addRow("controller position", controllerSteps_);

  encoderSteps_ = new QLabel("", this);
  formLayout->addRow("encoder position", encoderSteps_);

  stepMode_ = new QLabel("", this);
  formLayout->addRow("step mode", stepMode_);

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

  status_->setText(model_->getStatusText());
  controllerSteps_->setText(QString::number(model_->getControllerPosition()));
  encoderSteps_->setText(QString::number(model_->getEncoderPosition()));
  stepMode_->setText(QString(model_->getStepModeName().c_str()));
}

NanotecSMCI36SettingsWidget::NanotecSMCI36SettingsWidget(NanotecSMCI36Model* model, QWidget *parent)
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

  errorCorrectionMode_ = new NanotecSMCI36ErrorCorrectionModeWidget(model_, this);
  formLayout->addRow("error correction mode", errorCorrectionMode_);

  maxEncoderDeviation_ = new QSpinBox(this);
  maxEncoderDeviation_->setMinimum(4);
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
  bool ready = model_->isReady();

  bool theEnabled;
  if (enabled) {
    if (ready) {
      theEnabled = true;
    } else {
      theEnabled = false;
    }
  } else {
    theEnabled = false;
  }

  maxEncoderDeviation_->setEnabled(theEnabled);
}

void NanotecSMCI36SettingsWidget::updateInfo()
{
  //NQLog("NanotecSMCI36SettingsWidget", NQLog::Debug) << "updateInfo()";

  maxEncoderDeviation_->setValue(model_->getMaxEncoderDeviation());
}

NanotecSMCI36MovementWidget::NanotecSMCI36MovementWidget(NanotecSMCI36Model* model, QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->setMargin(0);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  QFormLayout *formLayout = new QFormLayout;
  formLayout->setSpacing(0);
  formLayout->setMargin(0);
  formLayout->setContentsMargins(0, 0, 0, 0);
  formLayout->setVerticalSpacing(2);
  formLayout->setHorizontalSpacing(8);

  positioningMode_ = new NanotecSMCI36PositioningModeWidget(model_, this);
  formLayout->addRow("positioning mode", positioningMode_);

  direction_ = new QCheckBox(this);
  connect(direction_, SIGNAL(clicked(bool)),
          model_, SLOT(setDirection(bool)));
  formLayout->addRow("direction", direction_);

  travelDistance_ = new QSpinBox(this);
  travelDistance_->setMinimum(-100000);
  travelDistance_->setMaximum(+100000);
  connect(travelDistance_, SIGNAL(valueChanged(int)),
          model_, SLOT(setTravelDistance(int)));
  formLayout->addRow("travel distance [steps]", travelDistance_);

  minFrequency_ = new QSpinBox(this);
  minFrequency_->setMinimum(model_->getMinFrequencyLimits().first);
  minFrequency_->setMaximum(model_->getMinFrequencyLimits().second);
  connect(minFrequency_, SIGNAL(valueChanged(int)),
          model_, SLOT(setMinFrequency(int)));
  formLayout->addRow("min. frequency [Hz]", minFrequency_);

  maxFrequency_ = new QSpinBox(this);
  maxFrequency_->setMinimum(model_->getMaxFrequencyLimits().first);
  maxFrequency_->setMaximum(model_->getMaxFrequencyLimits().second);
  connect(maxFrequency_, SIGNAL(valueChanged(int)),
          model_, SLOT(setMaxFrequency(int)));
  formLayout->addRow("max. frequency [Hz]", maxFrequency_);

  maxFrequency2_ = new QSpinBox(this);
  maxFrequency2_->setMinimum(model_->getMaxFrequency2Limits().first);
  maxFrequency2_->setMaximum(model_->getMaxFrequency2Limits().second);
  connect(maxFrequency2_, SIGNAL(valueChanged(int)),
          model_, SLOT(setMaxFrequency2(int)));
  formLayout->addRow("max. frequency 2 [Hz]", maxFrequency2_);

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

  updateDeviceState(model_->getDeviceState());
  updateInfo();
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
  bool ready = model_->isReady();

  bool theEnabled;
  if (enabled) {
    if (ready) {
      theEnabled = true;
    } else {
      theEnabled = false;
    }
  } else {
    theEnabled = false;
  }

  direction_->setEnabled(theEnabled);

  travelDistance_->setEnabled(theEnabled);
  minFrequency_->setEnabled(theEnabled);
  maxFrequency_->setEnabled(theEnabled);
  maxFrequency2_->setEnabled(theEnabled);

  start_->setEnabled(theEnabled);

  stop_->setEnabled(enabled);
  quickstop_->setEnabled(enabled);
  resetPositionError_->setEnabled(enabled);
}

void NanotecSMCI36MovementWidget::updateInfo()
{
  //NQLog("NanotecSMCI36Widget", NQLog::Debug) << "updateInfo()";

  direction_->setChecked(model_->getDirection());

  travelDistance_->setValue(model_->getTravelDistance());
  minFrequency_->setValue(model_->getMinFrequency());
  maxFrequency_->setValue(model_->getMaxFrequency());
  maxFrequency2_->setValue(model_->getMaxFrequency2());
}

NanotecSMCI36InputStateWidget::NanotecSMCI36InputStateWidget(NanotecSMCI36Model* model,
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

  if (enabled) {
    bool state = model_->getInputPinState(pin_);
    if (state) {
      setPixmap(buttonGreen_);
    } else {
      setPixmap(buttonRed_);
    }
  } else {
    setPixmap(buttonGrey_);
  }
}

void NanotecSMCI36InputStateWidget::updateInfo()
{
  // NQLog("NanotecSMCI36InputStateWidget", NQLog::Debug) << "updateInfo()";

  bool state = model_->getInputPinState(pin_);
  if (state) {
    setPixmap(buttonGreen_);
  } else {
    setPixmap(buttonRed_);
  }
}

NanotecSMCI36OutputStateWidget::NanotecSMCI36OutputStateWidget(NanotecSMCI36Model* model,
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

  connect(this, SIGNAL(toggleOutputPin(int)),
          model_, SLOT(toggleOutputPin(int)));

  updateDeviceState(model_->getDeviceState());
  updateInfo();
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

void NanotecSMCI36OutputStateWidget::mousePressEvent(QMouseEvent* /* event */)
{
  bool ready = model_->isReady();

  if (isEnabled() && ready) {
    emit toggleOutputPin(pin_);
  }
}

void NanotecSMCI36OutputStateWidget::updateInfo()
{
  // NQLog("NanotecSMCI36OutputStateWidget", NQLog::Debug) << "updateInfo()";

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

NanotecSMCI36InputWidget::NanotecSMCI36InputWidget(NanotecSMCI36Model* model,
                                                   int pin,
                                                   QWidget *parent)
  : QWidget(parent),
    model_(model),
    pin_(pin)
{
  QGridLayout* layout = new QGridLayout();
  setLayout(layout);

  QLabel *label;

  for (int i=1;i<7;++i) {
    label = new QLabel(QString("%1").arg(i), this);
    layout->addWidget(label, 0, i, Qt::AlignHCenter | Qt::AlignVCenter);
  }
  label = new QLabel("input", this);
  layout->addWidget(label, 1, 0, Qt::AlignVCenter);

  for (int i=1;i<7;++i) {
    NanotecSMCI36InputStateWidget *stateWidget = new NanotecSMCI36InputStateWidget(model_, i, this);
    layout->addWidget(stateWidget, 1, i, Qt::AlignHCenter | Qt::AlignVCenter);
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

void NanotecSMCI36InputWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36InputWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);
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
  QGridLayout* layout = new QGridLayout();
  setLayout(layout);

  QLabel *label;

  for (int i=1;i<4;++i) {
    label = new QLabel(QString("%1").arg(i), this);
    layout->addWidget(label, 0, i, Qt::AlignHCenter | Qt::AlignVCenter);
  }
  label = new QLabel("output", this);
  layout->addWidget(label, 1, 0, Qt::AlignVCenter);

  for (int i=1;i<4;++i) {
    NanotecSMCI36OutputStateWidget *stateWidget = new NanotecSMCI36OutputStateWidget(model_, i, this);
    layout->addWidget(stateWidget, 2, i, Qt::AlignHCenter | Qt::AlignVCenter);
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

void NanotecSMCI36OutputWidget::updateDeviceState(State newState)
{
  bool ready = (newState == READY);

  controlStateChanged(ready);
}

/// Updates the GUI when the controller is enabled/disabled.
void NanotecSMCI36OutputWidget::controlStateChanged(bool enabled)
{
  setEnabled(enabled);
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

  QLabel *label;

  for (int i=1;i<7;++i) {
    label = new QLabel(QString("%1").arg(i), this);
    layout->addWidget(label, 0, i, Qt::AlignHCenter | Qt::AlignVCenter);
  }
  label = new QLabel("input", this);
  layout->addWidget(label, 1, 0, Qt::AlignVCenter);

  label = new QLabel("output", this);
  layout->addWidget(label, 2, 0, Qt::AlignVCenter);

  for (int i=1;i<7;++i) {
    NanotecSMCI36InputStateWidget *stateWidget = new NanotecSMCI36InputStateWidget(model_, i, this);
    layout->addWidget(stateWidget, 1, i, Qt::AlignHCenter | Qt::AlignVCenter);
  }

  for (int i=1;i<4;++i) {
    NanotecSMCI36OutputStateWidget *stateWidget = new NanotecSMCI36OutputStateWidget(model_, i, this);
    layout->addWidget(stateWidget, 2, i, Qt::AlignHCenter | Qt::AlignVCenter);
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

NanotecSMCI36Widget::NanotecSMCI36Widget(NanotecSMCI36Model* model, QWidget *parent)
  : QWidget(parent),
    model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(8);
  layout->setMargin(0);
  layout->setContentsMargins(4, 4, 4, 4);
  setLayout(layout);

  smci36CheckBox_ = new QCheckBox("Enable controller", this);
  layout->addWidget(smci36CheckBox_);

  scmi36Status_ = new NanotecSMCI36StatusWidget(model_, this);
  layout->addWidget(scmi36Status_);

  scmi36Settings_ = new NanotecSMCI36SettingsWidget(model_, this);
  layout->addWidget(scmi36Settings_);

  scmi36Movement_ = new NanotecSMCI36MovementWidget(model_, this);
  layout->addWidget(scmi36Movement_);

  smci36IO_ = new NanotecSMCI36IOWidget(model_, this);
  layout->addWidget(smci36IO_);

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
