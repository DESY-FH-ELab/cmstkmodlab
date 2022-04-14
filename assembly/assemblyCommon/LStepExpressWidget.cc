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

 , mot_settings_box_(nullptr)
 , lstepCheckBox_   (nullptr)
 , joystickCheckBox_(nullptr)
 , posCtrlCheckBox_ (nullptr)

 , buttonOrigin_(nullptr)
 , buttonCalibrate_(nullptr)
 , buttonEmergencyStop_(nullptr)
 , buttonClearQueue_(nullptr)
 , buttonRestart_(nullptr)
 , buttonErrorQuit_(nullptr)

 , axisWidget_X_(nullptr)
 , axisWidget_Y_(nullptr)
 , axisWidget_Z_(nullptr)
 , axisWidget_A_(nullptr)

 , axisControlWidget_(nullptr)

 , motionSettings_locked_(false)

 , restart_timer_(nullptr)
 , restart_step_(0)
 , restart_attempts_(0)
 , restart_completed_(false)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->setLayout(layout);

    QHBoxLayout* hlayout = new QHBoxLayout();
    layout->addLayout(hlayout);

    // Motion Stage Settings
    mot_settings_box_ = new QGroupBox(tr("Motion Stage Settings"));
    mot_settings_box_->setStyleSheet("QGroupBox { font-weight: bold; } ");
    hlayout->addWidget(mot_settings_box_);

    QGridLayout* mot_settings_grid = new QGridLayout;
    mot_settings_box_->setLayout(mot_settings_grid);

    lstepCheckBox_ = new QCheckBox("Enable Controller");
    mot_settings_grid->addWidget(lstepCheckBox_, 0, 0);

    posCtrlCheckBox_ = new QCheckBox("Enable Position Controller");
    mot_settings_grid->addWidget(posCtrlCheckBox_, 0, 1);

    joystickCheckBox_ = new QCheckBox("Enable Joystick");
    mot_settings_grid->addWidget(joystickCheckBox_, 0, 2);

    // Motion Stage Basic Commands (e.g. Calibrate)
    QGridLayout* mot_basicfunc_grid = new QGridLayout;
    hlayout->addLayout(mot_basicfunc_grid);

    buttonCalibrate_ = new QPushButton("Calibrate");
    mot_basicfunc_grid->addWidget(buttonCalibrate_, 0, 0);

    buttonOrigin_ = new QPushButton("Origin");
    mot_basicfunc_grid->addWidget(buttonOrigin_, 1, 0);

    buttonRestart_ = new QPushButton("Restart Motion Stage");
    mot_basicfunc_grid->addWidget(buttonRestart_, 2, 0);

    buttonEmergencyStop_ = new QPushButton("Emergency Stop");
    mot_basicfunc_grid->addWidget(buttonEmergencyStop_, 0, 1);

    buttonClearQueue_ = new QPushButton("Clear Motion Queue");
    mot_basicfunc_grid->addWidget(buttonClearQueue_, 1, 1);

    buttonErrorQuit_ = new QPushButton("Error Quit");
    mot_basicfunc_grid->addWidget(buttonErrorQuit_, 2, 1);

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

    //connect(buttonOrigin_       , SIGNAL(clicked()), model_, SLOT(moveAbsolute()));
    connect(buttonOrigin_       , SIGNAL(clicked()), this  , SIGNAL(moveToOrigin_request())); //New signal/slot to connect the Origin button with the manager (where XYA/Z priorities are implemented) rather than directly via the model
    connect(buttonCalibrate_    , SIGNAL(clicked()), this, SLOT(confirmCalibrate())); //Changed: clicking 'Calibrate' pops-up a GUI message, and the calibration is only performed upon confirmation from the user
    connect(this, SIGNAL(startCalibrate()), model_, SLOT(calibrate()));
    connect(buttonEmergencyStop_, SIGNAL(clicked()), model_, SLOT(emergencyStop()));
    connect(buttonClearQueue_   , SIGNAL(clicked()), this  , SIGNAL(clearQueue_request()));
    connect(buttonRestart_      , SIGNAL(clicked()), this  , SLOT(restart()));
    connect(buttonErrorQuit_    , SIGNAL(clicked()), model_, SLOT(errorQuit()));

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
    if(buttonRestart_      ){ delete buttonRestart_      ; buttonRestart_       = nullptr; }
    if(buttonErrorQuit_    ){ delete buttonErrorQuit_    ; buttonErrorQuit_     = nullptr; }

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
    buttonRestart_      ->setEnabled(newState == READY);
    buttonErrorQuit_    ->setEnabled(newState == READY);

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
      buttonRestart_      ->setEnabled(false);
      buttonErrorQuit_    ->setEnabled(false);

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
  if(model_->getDeviceState() != READY)
  {
    NQLog("LStepExpressWidget", NQLog::Critical) << "restart [step=" << restart_step_ << "]"
       << ": logic error, motion stage NOT READY (try to initialize LStepExpressModel by ticking the \"Enable Controller\" box)";

    return;
  }

  if(restart_step_ == 0)
  {
    if(restart_timer_ != nullptr)
    {
      delete restart_timer_;

      restart_timer_ = nullptr;

      NQLog("LStepExpressWidget", NQLog::Critical) << "restart [step=" << restart_step_ << "]"
         << ": logic error, restart-timer was already initialized (disabled now), restart procedure stopped";

      return;
    }

    NQLog("LStepExpressWidget", NQLog::Spam) << "restart [step=" << restart_step_ << "]"
       << ": dedicated timer started";

    // disable widget (prevent further actions)
    this->setEnabled(false);

    // start restart timer
    restart_timer_ = new QTimer(this);
    restart_timer_->setInterval(2 * std::max(model_->updateInterval(), model_->motionUpdateInterval()));

    connect(restart_timer_, SIGNAL(timeout()), this, SLOT(restart()));

    restart_timer_->start();

    // proceed to next step
    ++restart_step_;
  }
  else if(restart_step_ == 1)
  {
    NQLog("LStepExpressWidget", NQLog::Spam) << "restart [step=" << restart_step_ << "]"
       << ": switching OFF motion stage axes and position-controller";

    // switch axes OFF
    if(axisWidget_X_->enabledCheckBox()->isChecked() == true){ axisWidget_X_->enabledCheckBox()->setChecked(false); }
    if(axisWidget_Y_->enabledCheckBox()->isChecked() == true){ axisWidget_Y_->enabledCheckBox()->setChecked(false); }
    if(axisWidget_Z_->enabledCheckBox()->isChecked() == true){ axisWidget_Z_->enabledCheckBox()->setChecked(false); }
    if(axisWidget_A_->enabledCheckBox()->isChecked() == true){ axisWidget_A_->enabledCheckBox()->setChecked(false); }

    // disable position controller
    if(posCtrlCheckBox_->isChecked() == true){ posCtrlCheckBox_->setChecked(false); }

    // proceed to next step
    ++restart_step_;
  }
  else if(restart_step_ == 2)
  {
    NQLog("LStepExpressWidget", NQLog::Spam) << "restart [step=" << restart_step_ << "]"
       << ": switching motion stage axes back ON";

    // switch axes back ON
    if(axisWidget_X_->enabledCheckBox()->isChecked() == false){ axisWidget_X_->enabledCheckBox()->setChecked(true); }
    if(axisWidget_Y_->enabledCheckBox()->isChecked() == false){ axisWidget_Y_->enabledCheckBox()->setChecked(true); }
    if(axisWidget_Z_->enabledCheckBox()->isChecked() == false){ axisWidget_Z_->enabledCheckBox()->setChecked(true); }
    if(axisWidget_A_->enabledCheckBox()->isChecked() == false){ axisWidget_A_->enabledCheckBox()->setChecked(true); }

    // proceed to next step
    ++restart_step_;
  }
  else if(restart_step_ == 3)
  {
    const auto x_status  = model_->getAxisStatusText(0);
    const auto y_status  = model_->getAxisStatusText(1);
    const auto z_status  = model_->getAxisStatusText(2);
    const auto a_status  = model_->getAxisStatusText(3);

    const bool x_ready = (x_status == "@");
    const bool y_ready = (y_status == "@");
    const bool z_ready = (z_status == "@");
    const bool a_ready = (a_status == "@");

    const auto x_enabled = model_->getAxisEnabled(0);
    const auto y_enabled = model_->getAxisEnabled(1);
    const auto z_enabled = model_->getAxisEnabled(2);
    const auto a_enabled = model_->getAxisEnabled(3);

    const bool axes_ready   = (x_ready   && y_ready   && z_ready   && a_ready  );
    const bool axes_enabled = (x_enabled && y_enabled && z_enabled && a_enabled);

    if(axes_ready && axes_enabled)
    {
      restart_completed_ = true;

      NQLog("LStepExpressWidget", NQLog::Spam) << "restart [step=" << restart_step_ << "]"
         << ": axes READY (status"
         << ": x=" << x_status
         << ", y=" << y_status
         << ", z=" << z_status
         << ", a=" << a_status
         << ") and ENABLED";

      // proceed to next step
      ++restart_step_;
    }
    else
    {
      restart_completed_ = false;

      if(axes_ready == false)
      {
        NQLog("LStepExpressWidget", NQLog::Spam) << "restart [step=" << restart_step_ << "]"
           << ": axes NOT READY (status"
           << ": x=" << x_status
           << ", y=" << y_status
           << ", z=" << z_status
           << ", a=" << a_status
           << ")";

        if(restart_attempts_ >= RESTART_MAX_ATTEMPTS_)
        {
          // proceed to next step
          ++restart_step_;
        }
        else
        {
          ++restart_attempts_;

          NQLog("LStepExpressWidget", NQLog::Spam) << "restart [step=" << restart_step_ << "]"
             << ": calling LStepExpressModel::errorQuit (attempt #" << restart_attempts_ << ")";

          model_->errorQuit();

          // back to step #1
          restart_step_ = 1;
        }
      }
      else //if(axes_ready == false)
      {
        NQLog("LStepExpressWidget", NQLog::Spam) << "restart [step=" << restart_step_ << "]"
           << ": axes NOT READY (enabled"
           << ": x=" << x_enabled
           << ", y=" << y_enabled
           << ", z=" << z_enabled
           << ", a=" << a_enabled
           << ")";

        if(restart_attempts_ >= RESTART_MAX_ATTEMPTS_)
        {
          // proceed to next step
          ++restart_step_;
        }
        else
        {
          ++restart_attempts_;

          // switch ON axes
          if(x_enabled == false)
          {
            NQLog("LStepExpressWidget", NQLog::Spam) << "restart [step=" << restart_step_ << "]"
               << ": switching ON axis #0 (X)";

            axisWidget_X_->enabledCheckBox()->setChecked(true);
          }

          if(y_enabled == false)
          {
            NQLog("LStepExpressWidget", NQLog::Spam) << "restart [step=" << restart_step_ << "]"
               << ": switching ON axis #1 (Y)";

            axisWidget_Y_->enabledCheckBox()->setChecked(true);
          }

          if(z_enabled == false)
          {
            NQLog("LStepExpressWidget", NQLog::Spam) << "restart [step=" << restart_step_ << "]"
               << ": switching ON axis #2 (Z)";

            axisWidget_Z_->enabledCheckBox()->setChecked(true);
          }

          if(a_enabled == false)
          {
            NQLog("LStepExpressWidget", NQLog::Spam) << "restart [step=" << restart_step_ << "]"
               << ": switching ON axis #3 (A)";

            axisWidget_A_->enabledCheckBox()->setChecked(true);
          }

          // back to step #3
          restart_step_ = 3;
        }
      }
    }
  }
  else if(restart_step_ == 4)
  {
    if(restart_completed_)
    {
      NQLog("LStepExpressWidget", NQLog::Message) << "restart [step=" << restart_step_ << "]"
         << ": restart completed successfully [attempts=" << restart_attempts_ << "], axes status"
         << " (x=" << model_->getAxisStatusText(0)
         << ", y=" << model_->getAxisStatusText(1)
         << ", z=" << model_->getAxisStatusText(2)
         << ", a=" << model_->getAxisStatusText(3)
         << ")";
    }
    else
    {
      NQLog("LStepExpressWidget", NQLog::Critical) << "restart [step=" << restart_step_ << "]"
         << ": restart procedure failed (stopped after " << restart_attempts_ << " calls to LStepExpressModel::errorQuit)";
    }

    // enable position controller
    if(posCtrlCheckBox_->isChecked() == false){ posCtrlCheckBox_->setChecked(true); }

    // stop timer
    restart_timer_->stop();

    disconnect(restart_timer_, SIGNAL(timeout()), this, SLOT(restart()));

    if(restart_timer_){ delete restart_timer_; restart_timer_ = nullptr; }

    // re-enable widget
    if(restart_completed_) this->setEnabled(true);

    restart_attempts_ = 0;

    restart_step_ = 0;

    if(!restart_completed_)
    {
      QMessageBox* msgBox = new QMessageBox;
      msgBox->setStyleSheet("QLabel{min-width: 400px;}");
      msgBox->setInformativeText("Restart of motion stage failed!\nWould you like to try again?");
      msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox->setDefaultButton(QMessageBox::Yes);
      int ret = msgBox->exec();
      switch(ret)
      {
	case QMessageBox::No: break;
        case QMessageBox::Yes:
          restart_step_ = 0;
          this->restart();
        default: return;
      }
    }

    // reset restart variables
    restart_completed_ = false;

    NQLog("LStepExpressWidget", NQLog::Spam) << "restart"
       << ": emitting signal \"restart_completed\"";

    emit restart_completed();
  }
}

void LStepExpressWidget::lockMotionSettings(const bool disable)
{
  motionSettings_locked_ = disable;

  mot_settings_box_->setDisabled(disable);

  if(lstepCheckBox_   ){ lstepCheckBox_   ->setDisabled(disable); }
  if(joystickCheckBox_){ joystickCheckBox_->setDisabled(disable); }
  if(posCtrlCheckBox_ ){ posCtrlCheckBox_ ->setDisabled(disable); }

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
  if(motionSettings_locked_ == false)
  {
    if(lstepCheckBox_   ){ lstepCheckBox_   ->setEnabled(enable); }
    if(joystickCheckBox_){ joystickCheckBox_->setEnabled(enable); }
    if(posCtrlCheckBox_ ){ posCtrlCheckBox_ ->setEnabled(enable); }
  }

  if(buttonOrigin_       ){ buttonOrigin_       ->setEnabled(enable); }
  if(buttonCalibrate_    ){ buttonCalibrate_    ->setEnabled(enable); }
  if(buttonEmergencyStop_){ buttonEmergencyStop_->setEnabled(enable); }
  if(buttonClearQueue_   ){ buttonClearQueue_   ->setEnabled(enable); }
  if(buttonRestart_      ){ buttonRestart_      ->setEnabled(enable); }
  if(buttonErrorQuit_    ){ buttonErrorQuit_    ->setEnabled(enable); }

  if(axisWidget_X_){ axisWidget_X_->enableMotionTools(enable); }
  if(axisWidget_Y_){ axisWidget_Y_->enableMotionTools(enable); }
  if(axisWidget_Z_){ axisWidget_Z_->enableMotionTools(enable); }
  if(axisWidget_A_){ axisWidget_A_->enableMotionTools(enable); }
}

void LStepExpressWidget::disableMotionTools(const bool disable)
{
  this->enableMotionTools(!disable);
}

//-- Changed: clicking 'Calibrate' pops-up a GUI message, and the calibration is only performed upon confirmation from the user
void LStepExpressWidget::confirmCalibrate()
{
    QMessageBox* msgBox = new QMessageBox;
    msgBox->setInformativeText("Recalibrate the motion stage ?");
    msgBox->setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox->setDefaultButton(QMessageBox::No);
    int ret = msgBox->exec();
    switch(ret)
    {
      case QMessageBox::No: return; //Exit
      case QMessageBox::Yes: break; //Continue function execution
      default: return; //Exit
    }

	NQLog("LStepExpressWidget", NQLog::Spam) << "confirmCalibrate"
     << ": emitting signal \"startCalibrate\"";

    emit startCalibrate(); //Emit signal to start the actual MS calibration

    return;
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
      accelerationSpinBox_    ->setSuffix(QString(" " + model_->getAxisAccelerationShortName    (axis_)));
      decelerationSpinBox_    ->setSuffix(QString(" " + model_->getAxisAccelerationShortName    (axis_)));
      velocitySpinBox_        ->setSuffix(QString(" " + model_->getAxisVelocityShortName        (axis_)));

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
