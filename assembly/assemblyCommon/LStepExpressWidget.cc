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

#include <LStepExpressWidget.h>
#include <nqlogger.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>

LStepExpressWidget::LStepExpressWidget(LStepExpressModel* model, QWidget* parent)
 : QWidget(parent)

 , model_(model)

 , lstepCheckBox_   (nullptr)
 , joystickCheckBox_(nullptr)
 , posCtrlCheckBox_ (nullptr)

 , buttonOrigin_(nullptr)
 , buttonCalibrate_(nullptr)
 , buttonEmergencyStop_(nullptr)
 , buttonClearQueue_(nullptr)
 , buttonErrorQuit_(nullptr)
 , buttonRestart_(nullptr)

 , axisWidget_X_(nullptr)
 , axisWidget_Y_(nullptr)
 , axisWidget_Z_(nullptr)
 , axisWidget_A_(nullptr)

 , axisControlWidget_(nullptr)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->setLayout(layout);

    QHBoxLayout* hlayout = new QHBoxLayout();
    layout->addLayout(hlayout);

    // Motion Stage Settings
    QGroupBox* mot_settings_box = new QGroupBox(tr("Motion Stage Settings"));
    hlayout->addWidget(mot_settings_box);

    mot_settings_box->setStyleSheet("QGroupBox { font-weight: bold; } ");

    QGridLayout* mot_settings_grid = new QGridLayout;
    mot_settings_box->setLayout(mot_settings_grid);

    lstepCheckBox_ = new QCheckBox("Enable Controller");
    mot_settings_grid->addWidget(lstepCheckBox_, 0, 0);

    posCtrlCheckBox_ = new QCheckBox("Enable Position Controller");
    mot_settings_grid->addWidget(posCtrlCheckBox_, 0, 1);

    buttonRestart_ = new QPushButton("Restart LStep");
    mot_settings_grid->addWidget(buttonRestart_, 1, 0);

    buttonErrorQuit_ = new QPushButton("Error Quit");
    mot_settings_grid->addWidget(buttonErrorQuit_, 1, 1);

    joystickCheckBox_ = new QCheckBox("Enable Joystick");
    mot_settings_grid->addWidget(joystickCheckBox_, 0, 2);

    // Motion Stage Basic Commands (e.g. Calibrate)
    QGridLayout* mot_basicmoves_grid = new QGridLayout;
    hlayout->addLayout(mot_basicmoves_grid);

    buttonCalibrate_ = new QPushButton("Calibrate");
    mot_basicmoves_grid->addWidget(buttonCalibrate_, 0, 0);

    buttonOrigin_ = new QPushButton("Origin");
    mot_basicmoves_grid->addWidget(buttonOrigin_, 1, 0);

    buttonEmergencyStop_ = new QPushButton("Emergency Stop");
    mot_basicmoves_grid->addWidget(buttonEmergencyStop_, 0, 1);

    buttonClearQueue_ = new QPushButton("Clear Motion Queue");
    mot_basicmoves_grid->addWidget(buttonClearQueue_, 1, 1);

    // AXIS
    axisControlWidget_ = new QWidget(this);
    layout->addWidget(axisControlWidget_);

    QGridLayout* axisLayout = new QGridLayout(axisControlWidget_);
    axisControlWidget_->setLayout(axisLayout);

    // Add all the axes displays
    axisWidget_X_ = new LStepExpressAxisWidget(model_, 0);
    axisWidget_Y_ = new LStepExpressAxisWidget(model_, 1);
    axisWidget_Z_ = new LStepExpressAxisWidget(model_, 2);
    axisWidget_A_ = new LStepExpressAxisWidget(model_, 3);

    axisLayout->addWidget(axisWidget_X_, 0, 0);
    axisLayout->addWidget(axisWidget_Y_, 0, 1);
    axisLayout->addWidget(axisWidget_Z_, 0, 2);
    axisLayout->addWidget(axisWidget_A_, 0, 3);

    connect(lstepCheckBox_   , SIGNAL(toggled(bool)), model_, SLOT(setDeviceEnabled(bool)));
    connect(joystickCheckBox_, SIGNAL(toggled(bool)), model_, SLOT(setJoystickEnabled(bool)));
    connect(posCtrlCheckBox_ , SIGNAL(toggled(bool)), model_, SLOT(setPositionControllerEnabled(bool)));

    connect(model_, SIGNAL(deviceStateChanged(State)), this, SLOT(lstepStateChanged(State)));
    connect(model_, SIGNAL(controlStateChanged(bool)), this, SLOT(controlStateChanged(bool)));
    connect(model_, SIGNAL(informationChanged())     , this, SLOT(updateWidgets()));
    connect(model_, SIGNAL(motionStarted())          , this, SLOT(motionStarted()));
    connect(model_, SIGNAL(motionFinished())         , this, SLOT(motionFinished()));

    connect(buttonOrigin_       , SIGNAL(clicked()), model_, SLOT(moveAbsolute()));
    connect(buttonCalibrate_    , SIGNAL(clicked()), model_, SLOT(calibrate()));
    connect(buttonEmergencyStop_, SIGNAL(clicked()), model_, SLOT(emergencyStop()));
    connect(buttonClearQueue_   , SIGNAL(clicked()), this  , SIGNAL(clearQueue_request()));
    connect(buttonErrorQuit_    , SIGNAL(clicked()), model_, SLOT(errorQuit()));
    connect(buttonRestart_      , SIGNAL(clicked()), this  , SLOT(restart()));

    this->lstepStateChanged(model_->getDeviceState());

    NQLog("LStepExpressWidget", NQLog::Debug) << "constructed";
}

LStepExpressWidget::~LStepExpressWidget()
{
    if(lstepCheckBox_      ){ delete lstepCheckBox_      ; lstepCheckBox_       = nullptr; }
    if(joystickCheckBox_   ){ delete joystickCheckBox_   ; joystickCheckBox_    = nullptr; }
    if(posCtrlCheckBox_    ){ delete posCtrlCheckBox_    ; posCtrlCheckBox_     = nullptr; }
    if(buttonOrigin_       ){ delete buttonOrigin_       ; buttonOrigin_        = nullptr; }
    if(buttonCalibrate_    ){ delete buttonCalibrate_    ; buttonCalibrate_     = nullptr; }
    if(buttonEmergencyStop_){ delete buttonEmergencyStop_; buttonEmergencyStop_ = nullptr; }
    if(buttonClearQueue_   ){ delete buttonClearQueue_   ; buttonClearQueue_    = nullptr; }
    if(buttonErrorQuit_    ){ delete buttonErrorQuit_    ; buttonErrorQuit_     = nullptr; }
    if(buttonRestart_      ){ delete buttonRestart_      ; buttonRestart_       = nullptr; }

    NQLog("LStepExpressWidget", NQLog::Debug) << "destructed";
}

void LStepExpressWidget::updateWidgets()
{
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void LStepExpressWidget::lstepStateChanged(State newState)
{
    NQLog("LStepExpressWidget", NQLog::Debug) << "lStepStateChanged(" << newState << ")";

    lstepCheckBox_->setChecked(newState == READY || newState == INITIALIZING);

    joystickCheckBox_   ->setEnabled(newState == READY);
    posCtrlCheckBox_    ->setEnabled(newState == READY);
    buttonOrigin_       ->setEnabled(newState == READY);
    buttonCalibrate_    ->setEnabled(newState == READY);
    buttonEmergencyStop_->setEnabled(newState == READY);
    buttonClearQueue_   ->setEnabled(newState == READY);
    buttonErrorQuit_    ->setEnabled(newState == READY);
    buttonRestart_      ->setEnabled(newState == READY);

    axisControlWidget_  ->setEnabled(newState == READY);
}

/// Updates the GUI when the controler is enabled/disabled.
void LStepExpressWidget::controlStateChanged(bool enabled)
{
    NQLog("LStepExpressWidget", NQLog::Debug) << "controlStateChanged(" << enabled << ")";

    if(enabled)
    {
      lstepStateChanged(model_->getDeviceState());
    }
    else
    {
      lstepCheckBox_      ->setEnabled(false);
      joystickCheckBox_   ->setEnabled(false);
      posCtrlCheckBox_    ->setEnabled(false);
      buttonOrigin_       ->setEnabled(false);
      buttonCalibrate_    ->setEnabled(false);
      buttonEmergencyStop_->setEnabled(false);
      buttonClearQueue_   ->setEnabled(false);
      buttonErrorQuit_    ->setEnabled(false);
      buttonRestart_      ->setEnabled(false);

      axisControlWidget_  ->setEnabled(false);
    }
}

void LStepExpressWidget::motionStarted()
{
//    NQLog("LStepExpressWidget", NQLog::Spam)<< "motionStarted()"  ;
}

void LStepExpressWidget::motionFinished()
{
//    NQLog("LStepExpressWidget", NQLog::Spam)<< "motionFinished()"  ;
}

void LStepExpressWidget::enableMotionControllers()
{
  lstepCheckBox_->setChecked(true);

  NQLog("LStepExpressWidget", NQLog::Spam) << "enableMotionControllers"
     << ": emitting signal \"MotionControllers_enabled\"";

  emit MotionControllers_enabled();
}

void LStepExpressWidget::restart()
{
  if(posCtrlCheckBox_->isEnabled() == true)
  {
    posCtrlCheckBox_->setEnabled(false);
  }

  if(model_)
  {
    model_->setPositionControllerEnabled(false);
    model_->restart();
  }

  posCtrlCheckBox_->setEnabled(true);
  posCtrlCheckBox_->setChecked(true);
}

void LStepExpressWidget::lockMotionSettings(const bool disable)
{
  motionSettings_locked_ = disable;

  if(lstepCheckBox_   ){ lstepCheckBox_   ->setDisabled(disable); }
  if(joystickCheckBox_){ joystickCheckBox_->setDisabled(disable); }
  if(posCtrlCheckBox_ ){ posCtrlCheckBox_ ->setDisabled(disable); }
  if(buttonErrorQuit_ ){ buttonErrorQuit_ ->setDisabled(disable); }
  if(buttonRestart_   ){ buttonRestart_   ->setDisabled(disable); }

  if(axisWidget_X_){ axisWidget_X_->lockMotionSettings(disable); }
  if(axisWidget_Y_){ axisWidget_Y_->lockMotionSettings(disable); }
  if(axisWidget_Z_){ axisWidget_Z_->lockMotionSettings(disable); }
  if(axisWidget_A_){ axisWidget_A_->lockMotionSettings(disable); }
}

void LStepExpressWidget::unlockMotionSettings()
{
  this->lockMotionSettings(false);
}

void LStepExpressWidget::enableMotionTools(const bool enable)
{
  if(buttonOrigin_   ){ buttonOrigin_   ->setEnabled(enable); }
  if(buttonCalibrate_){ buttonCalibrate_->setEnabled(enable); }

  if(motionSettings_locked_ == false)
  {
    if(lstepCheckBox_   ){ lstepCheckBox_   ->setEnabled(enable); }
    if(joystickCheckBox_){ joystickCheckBox_->setEnabled(enable); }
    if(posCtrlCheckBox_ ){ posCtrlCheckBox_ ->setEnabled(enable); }
    if(buttonErrorQuit_ ){ buttonErrorQuit_ ->setEnabled(enable); }
    if(buttonRestart_   ){ buttonRestart_   ->setEnabled(enable); }
  }

  if(axisWidget_X_){ axisWidget_X_->enableMotionTools(enable); }
  if(axisWidget_Y_){ axisWidget_Y_->enableMotionTools(enable); }
  if(axisWidget_Z_){ axisWidget_Z_->enableMotionTools(enable); }
  if(axisWidget_A_){ axisWidget_A_->enableMotionTools(enable); }
}

void LStepExpressWidget::disableMotionTools()
{
  this->enableMotionTools(false);
}
// ============================================================================

LStepExpressAxisWidget::LStepExpressAxisWidget(LStepExpressModel* model, unsigned int axis, QWidget* parent)
 : QWidget(parent)
 , model_(model)
 , axis_(axis)
 , axisDimensionName_("usteps")
 , motionTools_enabled_(true)
 , motionSettings_locked_(false)
{
    layout_ = new QFormLayout(this);
    setLayout(layout_);

    enabledCheckBox_ = new QCheckBox(model_->getAxisName(axis), this);
    layout_->addRow(enabledCheckBox_);

    joystickCheckBox_ = new QCheckBox("joystick", this);
    layout_->addRow(joystickCheckBox_);

    statusLabel_ = new QLabel("-", this);
    statusLabel_->setAlignment(Qt::AlignHCenter);
    statusLabel_->setFont(QFont("Helvetica", 32));
    layout_->addRow(statusLabel_);

    positionLabel_ = new QLabel("0000.0000 usteps", this);
    positionLabel_->setAlignment(Qt::AlignHCenter);
    positionLabel_->setFont(QFont("Helvetica", 16));
    layout_->addRow(positionLabel_);

    velocitySpinBox_ = new QDoubleSpinBox(this);
    velocitySpinBox_->setDecimals(1);
    velocitySpinBox_->setSuffix(" usteps/s");
    layout_->addRow("v", velocitySpinBox_);

    accelerationSpinBox_ = new QDoubleSpinBox(this);
    accelerationSpinBox_->setDecimals(1);
    accelerationSpinBox_->setSuffix(" usteps/s^2");
    layout_->addRow("a", accelerationSpinBox_);

    decelerationSpinBox_ = new QDoubleSpinBox(this);
    decelerationSpinBox_->setDecimals(1);
    decelerationSpinBox_->setSuffix(" usteps/s^2");
    layout_->addRow("d", decelerationSpinBox_);

    accelerationJerkSpinBox_ = new QDoubleSpinBox(this);
    accelerationJerkSpinBox_->setDecimals(1);
    accelerationJerkSpinBox_->setSuffix(" usteps/s^3");
    layout_->addRow("ja", accelerationJerkSpinBox_);

    decelerationJerkSpinBox_ = new QDoubleSpinBox(this);
    decelerationJerkSpinBox_->setDecimals(1);
    decelerationJerkSpinBox_->setSuffix(" usteps/s^3");
    layout_->addRow("jd", decelerationJerkSpinBox_);

    buttonWriteParameter_ = new QPushButton("Write", this);
    layout_->addRow(buttonWriteParameter_);

    connect(enabledCheckBox_ , SIGNAL(toggled(bool)), this, SLOT(enabledCheckBoxToggled(bool)));
    connect(joystickCheckBox_, SIGNAL(toggled(bool)), this, SLOT(joystickCheckBoxToggled(bool)));

    connect(velocitySpinBox_, SIGNAL(valueChanged(double)), this, SLOT(setVelocity(double)));
    connect(accelerationSpinBox_, SIGNAL(valueChanged(double)), this, SLOT(setAcceleration(double)));
    connect(decelerationSpinBox_, SIGNAL(valueChanged(double)), this, SLOT(setDeceleration(double)));
    connect(accelerationJerkSpinBox_, SIGNAL(valueChanged(double)), this, SLOT(setAccelerationJerk(double)));
    connect(decelerationJerkSpinBox_, SIGNAL(valueChanged(double)), this, SLOT(setDecelerationJerk(double)));
    connect(buttonWriteParameter_, SIGNAL(clicked()), this, SLOT(writeParameter()));

    connect(model_, SIGNAL(deviceStateChanged(State)) , this, SLOT(lStepStateChanged(State)));
    connect(model_, SIGNAL(controlStateChanged(bool)) , this, SLOT(controlStateChanged(bool)));
    connect(model_, SIGNAL(informationChanged())      , this, SLOT(updateWidgets()));
    connect(model_, SIGNAL(motionInformationChanged()), this, SLOT(updateMotionWidgets()));
    connect(model_, SIGNAL(motionStarted())           , this, SLOT(motionStarted()));
    connect(model_, SIGNAL(motionFinished())          , this, SLOT(motionFinished()));
}

LStepExpressAxisWidget::~LStepExpressAxisWidget()
{
    if(layout_)          { delete layout_;           layout_           = nullptr; }
    if(enabledCheckBox_) { delete enabledCheckBox_;  enabledCheckBox_  = nullptr; }
    if(joystickCheckBox_){ delete joystickCheckBox_; joystickCheckBox_ = nullptr; }
    if(statusLabel_)     { delete statusLabel_;      statusLabel_      = nullptr; }
    if(positionLabel_)   { delete positionLabel_;    positionLabel_    = nullptr; }
}

void LStepExpressAxisWidget::updateWidgets()
{
    NQLog("LStepExpressAxisWidget", NQLog::Debug) << "updateWidgets";

    const bool axis = model_->getAxisEnabled(axis_);

    NQLog("LStepExpressAxisWidget", NQLog::Debug) << "updateWidgets"
       << ": axis " << axis_ << " enabled="<< axis;

    double value;

    value = model_->getAccelerationJerk(axis_);
    if (value!=accelerationJerkSpinBox_->value()) accelerationJerkSpinBox_->setValue(value);

    value = model_->getDecelerationJerk(axis_);
    if (value!=decelerationJerkSpinBox_->value()) decelerationJerkSpinBox_->setValue(value);

    value = model_->getAcceleration(axis_);
    if (value!=accelerationSpinBox_->value()) accelerationSpinBox_->setValue(value);

    value = model_->getDeceleration(axis_);
    if (value!=decelerationSpinBox_->value()) decelerationSpinBox_->setValue(value);

    value = model_->getVelocity(axis_);
    if (value!=velocitySpinBox_->value()) velocitySpinBox_->setValue(value);

    const QString temp = model_->getAxisDimensionShortName(axis_);
    if(axisDimensionName_ != temp)
    {
      axisDimensionName_ = temp;

      accelerationJerkSpinBox_->setSuffix(QString(" " + model_->getAxisAccelerationJerkShortName(axis_)));
      decelerationJerkSpinBox_->setSuffix(QString(" " + model_->getAxisAccelerationJerkShortName(axis_)));
      accelerationSpinBox_->setSuffix(QString(" " + model_->getAxisAccelerationShortName(axis_)));
      decelerationSpinBox_->setSuffix(QString(" " + model_->getAxisAccelerationShortName(axis_)));
      velocitySpinBox_->setSuffix(QString(" " + model_->getAxisVelocityShortName(axis_)));

      positionLabel_->setText(QString::number(model_->getPosition(axis_), 'f', 4)+" "+axisDimensionName_);
    }

    if(motionTools_enabled_)
    {
      if(axis && model_->getJoystickEnabled())
      {
        joystickCheckBox_->setEnabled(true);
      }
      else
      {
        joystickCheckBox_->setEnabled(false);
      }
    }

    if(axis)
    {
      this->updateMotionWidgets();

      NQLog("LStepExpressAxisWidget", NQLog::Debug) << "updateWidgets"
          << ": updated motion widgets";
    }
}

void LStepExpressAxisWidget::updateMotionWidgets()
{
    NQLog("LStepExpressAxisWidget", NQLog::Debug) << "updateMotionWidgets";

    statusLabel_  ->setText(model_->getAxisStatusText(axis_));
    positionLabel_->setText(QString::number(model_->getPosition(axis_), 'f', 4)+" "+axisDimensionName_);
}

void LStepExpressAxisWidget::lStepStateChanged(State newState)
{
    NQLog("LStepExpressAxisWidget", NQLog::Debug) <<  "lStepStateChanged(" << newState << ")";

    if(motionTools_enabled_)
    {
      if((newState == READY) || (newState == INITIALIZING))
      {
         enabledCheckBox_->setEnabled(true);
        joystickCheckBox_->setEnabled(model_->getJoystickEnabled());
        updateWidgets();
      }
      else
      {
         enabledCheckBox_->setEnabled(false);
        joystickCheckBox_->setEnabled(false);
      }
    }
    else
    {
      NQLog("LStepExpressAxisWidget", NQLog::Debug) <<  "lStepStateChanged(" << newState << ")"
         << ": motions tools disabled, no action taken";
    }
}

void LStepExpressAxisWidget::controlStateChanged(bool enabled)
{
    NQLog("LStepExpressAxisWidget", NQLog::Debug) << "controlStateChanged(" << enabled << ")";

    if(motionTools_enabled_)
    {
      if(enabled)
      {
        lStepStateChanged(model_->getDeviceState());
      }
      else
      {
        enabledCheckBox_->setEnabled(false);
      }
    }
}

void LStepExpressAxisWidget::enabledCheckBoxToggled(bool enabled)
{
  model_->setAxisEnabled(axis_, enabled);
}

void LStepExpressAxisWidget::joystickCheckBoxToggled(bool enabled)
{
  model_->setJoystickAxisEnabled(axis_, enabled);
}

void LStepExpressAxisWidget::setVelocity(double /* value */)
{
  //model_->setVelocity(axis_, value);
}

void LStepExpressAxisWidget::setAcceleration(double /* value */)
{
  //model_->setAcceleration(axis_, value);
}

void LStepExpressAxisWidget::setDeceleration(double /* value */)
{
  //model_->setDeceleration(axis_, value);
}

void LStepExpressAxisWidget::setAccelerationJerk(double /* value */)
{
  //model_->setAccelerationJerk(axis_, value);
}

void LStepExpressAxisWidget::setDecelerationJerk(double /* value */)
{
  //model_->setDecelerationJerk(axis_, value);
}

void LStepExpressAxisWidget::writeParameter()
{
  NQLog("LStepExpressAxisWidget", NQLog::Spam) << "writeParameter";

  model_->setAccelerationJerk(axis_, accelerationJerkSpinBox_->value());
  model_->setDecelerationJerk(axis_, decelerationJerkSpinBox_->value());
  model_->setAcceleration(axis_, accelerationSpinBox_->value());
  model_->setDeceleration(axis_, decelerationSpinBox_->value());
  model_->setVelocity(axis_, velocitySpinBox_->value());
}

void LStepExpressAxisWidget::motionStarted()
{
//  NQLog("LStepExpressAxisWidget", NQLog::Spam) << "motionStarted";
}

void LStepExpressAxisWidget::motionFinished()
{
//  NQLog("LStepExpressAxisWidget", NQLog::Spam) << "motionFinished";
}

void LStepExpressAxisWidget::enableMotionTools(const bool enable)
{
  motionTools_enabled_ = enable;

  if(enabledCheckBox_){ enabledCheckBox_->setEnabled(enable); }

  if(joystickCheckBox_ && model_->getJoystickEnabled())
  {
    joystickCheckBox_->setEnabled(enable);
  }

  if(motionSettings_locked_ == false)
  {
    if(buttonWriteParameter_){ buttonWriteParameter_->setEnabled(enable); }
  }
}

void LStepExpressAxisWidget::disableMotionTools()
{
  this->enableMotionTools(false);
}

void LStepExpressAxisWidget::lockMotionSettings(const bool disable)
{
  motionSettings_locked_ = disable;

  if(velocitySpinBox_        ){ velocitySpinBox_        ->setDisabled(disable); }
  if(accelerationSpinBox_    ){ accelerationSpinBox_    ->setDisabled(disable); }
  if(decelerationSpinBox_    ){ decelerationSpinBox_    ->setDisabled(disable); }
  if(accelerationJerkSpinBox_){ accelerationJerkSpinBox_->setDisabled(disable); }
  if(decelerationJerkSpinBox_){ decelerationJerkSpinBox_->setDisabled(disable); }
  if(buttonWriteParameter_   ){ buttonWriteParameter_   ->setDisabled(disable); }
}

void LStepExpressAxisWidget::unlockMotionSettings()
{
  this->lockMotionSettings(false);
}
// ============================================================================
