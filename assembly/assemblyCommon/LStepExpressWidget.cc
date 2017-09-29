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

#include <nqlogger.h>

#include "LStepExpressWidget.h"

LStepExpressWidget::LStepExpressWidget(LStepExpressModel* model,
                                       QWidget *parent)
    : QWidget(parent),
      model_(model)
{
    
    NQLog("LStepExpressWidget::LStepExpressWidget")  ;

    
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);

    QHBoxLayout* hlayout = new QHBoxLayout(this);
    layout->addLayout(hlayout);

    lstepCheckBox_ = new QCheckBox("Enable Controller", this);
    hlayout->addWidget(lstepCheckBox_);

    joystickCheckBox_ = new QCheckBox("Enable Joystick", this);
    hlayout->addWidget(joystickCheckBox_);

    buttonOrigin_ = new QPushButton("Origin", this);
    hlayout->addWidget(buttonOrigin_);

    axisControlWidget_= new QWidget(this);
    layout->addWidget(axisControlWidget_);

    buttonCalibrate_ = new QPushButton("Calibrate", this);
    hlayout->addWidget(buttonCalibrate_);

    buttonEmergencyStop_ = new QPushButton("Emergency stop", this);
    hlayout->addWidget(buttonEmergencyStop_);

    QGridLayout* axisLayout = new QGridLayout(axisControlWidget_);
    axisControlWidget_->setLayout(axisLayout);

    // Add all the axes displays
    for (unsigned int i=0;i<4;++i) {
        axisLayout->addWidget(new LStepExpressAxisWidget(model_, i, this),
                              0, i);
    }

    connect(lstepCheckBox_, SIGNAL(toggled(bool)),
            model_, SLOT(setDeviceEnabled(bool)));

    connect(joystickCheckBox_, SIGNAL(toggled(bool)),
            model_, SLOT(setJoystickEnabled(bool)));

    connect(buttonOrigin_, SIGNAL(clicked()),
            model_, SLOT(moveAbsolute()));

    connect(model_, SIGNAL(deviceStateChanged(State)),
            this, SLOT(lstepStateChanged(State)));

    connect(model_, SIGNAL(controlStateChanged(bool)),
            this, SLOT(controlStateChanged(bool)));

    connect(model_, SIGNAL(informationChanged()),
            this, SLOT(updateWidgets()));

    connect(model_, SIGNAL(motionStarted()),
            this, SLOT(motionStarted()));

    connect(model_, SIGNAL(motionFinished()),
            this, SLOT(motionFinished()));

    connect(buttonCalibrate_, SIGNAL(clicked()),
	    model_, SLOT(calibrate()));

    connect(buttonEmergencyStop_, SIGNAL(clicked()),
	model_, SLOT(emergencyStop()));

    lstepStateChanged(model_->getDeviceState());

    lstepCheckBox_->setCheckable(true);
    lstepCheckBox_->setChecked(true);

}

LStepExpressWidget::~LStepExpressWidget()
{
    if(lstepCheckBox_){delete lstepCheckBox_; lstepCheckBox_ = NULL;}
    if(joystickCheckBox_){delete joystickCheckBox_; joystickCheckBox_ = NULL;}
    if(buttonOrigin_){delete buttonOrigin_; buttonOrigin_ = NULL;}
    if(buttonCalibrate_){delete buttonCalibrate_; buttonCalibrate_ = NULL;}
    if(buttonEmergencyStop_){delete buttonEmergencyStop_; buttonEmergencyStop_ = NULL;}
}

void LStepExpressWidget::updateWidgets()
{
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void LStepExpressWidget::lstepStateChanged(State newState)
{
  //    NQLog("LStepExpressWidget", NQLog::Spam)<< "lStepStateChanged(State newState) " << newState  ;

    lstepCheckBox_->setChecked(newState == READY || newState == INITIALIZING);

    joystickCheckBox_->setEnabled(newState == READY);

    buttonOrigin_->setEnabled(newState == READY);

    buttonCalibrate_->setEnabled(newState == READY);

    axisControlWidget_->setEnabled(newState == READY);

    buttonEmergencyStop_->setEnabled(newState == READY);
}

/// Updates the GUI when the controler is enabled/disabled.
void LStepExpressWidget::controlStateChanged(bool enabled)
{
  //    NQLog("LStepExpressWidget", NQLog::Spam)<< "controlStateChanged(bool enabled) " << enabled  ;

    if (enabled) {
        lstepStateChanged(model_->getDeviceState());
    } else {
        lstepCheckBox_->setEnabled(false);
        joystickCheckBox_->setEnabled(false);
        buttonOrigin_->setEnabled(false);
        buttonCalibrate_->setEnabled(false);
        axisControlWidget_->setEnabled(false);
        buttonEmergencyStop_->setEnabled(false);
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


LStepExpressAxisWidget::LStepExpressAxisWidget(LStepExpressModel* model,
                                               unsigned int axis,
                                               QWidget *parent)
    : QWidget(parent),
      model_(model),
      axis_(axis),
      axisDimensionName_("usteps")
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

    connect(enabledCheckBox_, SIGNAL(toggled(bool)),
            this, SLOT(enabledCheckBoxToggled(bool)));

    connect(joystickCheckBox_, SIGNAL(toggled(bool)),
            this, SLOT(joystickCheckBoxToggled(bool)));

    connect(model_, SIGNAL(deviceStateChanged(State)),
            this, SLOT(lStepStateChanged(State)));

    connect(model_, SIGNAL(controlStateChanged(bool)),
            this, SLOT(controlStateChanged(bool)));

    connect(model_, SIGNAL(informationChanged()),
            this, SLOT(updateWidgets()));

    connect(model_, SIGNAL(motionInformationChanged()),
            this, SLOT(updateMotionWidgets()));

    connect(model_, SIGNAL(motionStarted()),
            this, SLOT(motionStarted()));

    connect(model_, SIGNAL(motionFinished()),
            this, SLOT(motionFinished()));

}

LStepExpressAxisWidget::~LStepExpressAxisWidget()
{
    if(layout_){delete layout_; layout_ = NULL;}
    if(enabledCheckBox_){ delete enabledCheckBox_; enabledCheckBox_ = NULL;}
    if(joystickCheckBox_){ delete joystickCheckBox_; joystickCheckBox_ = NULL;}
    if(statusLabel_){ delete statusLabel_; statusLabel_ = NULL;}
    if(positionLabel_){delete positionLabel_; positionLabel_ = NULL;}
}

void LStepExpressAxisWidget::updateWidgets()
{
  //    NQLog("LStepExpressAxisWidget ", NQLog::Spam)<< "updateWidgets()"  ;
    
    bool axis = model_->getAxisEnabled(axis_);

    //    NQLog("LStepExpressAxisWidget ", NQLog::Spam)<< "updateWidgets() , axis enabled?  "<<axis<<" joystick enabled? =  "<<model_->getJoystickEnabled()  ;

    if (axis && model_->getJoystickEnabled()) {
        joystickCheckBox_->setEnabled(true);
    } else {
        joystickCheckBox_->setEnabled(false);
    }

    //    NQLog("LStepExpressAxisWidget ", NQLog::Spam)<< "updateWidgets() JOYstick ----------->" << model_->getJoystickAxisEnabled(axis_)  ;

    if(axis){QString temp = model_->getAxisDimensionShortName(axis_); if(axisDimensionName_ != temp){axisDimensionName_ = temp; updateMotionWidgets();}}
}

void LStepExpressAxisWidget::updateMotionWidgets()
{
  //    NQLog("LStepExpressAxisWidget ", NQLog::Spam)<< "updateMotionWidgets()"  ;
    statusLabel_->setText(model_->getAxisStatusText(axis_));
    positionLabel_->setText(QString::number(model_->getPosition(axis_), 'f', 4) + " " + axisDimensionName_);
}

void LStepExpressAxisWidget::lStepStateChanged(State newState)
{
  //    NQLog("LStepExpressAxisWidget ", NQLog::Spam)<< "lStepStateChanged(State newState) " << newState  ;
  //NQLog("LStepExpressAxisWidget ", NQLog::Spam)<< "                             axis " << model_->getAxisEnabled(axis_)  ;

    if (newState == READY || newState == INITIALIZING) {
      enabledCheckBox_->setEnabled(true);
      joystickCheckBox_->setEnabled(model_->getJoystickEnabled());
      updateWidgets();
    } else {
      enabledCheckBox_->setEnabled(false);
      joystickCheckBox_->setEnabled(false);
    }
}

void LStepExpressAxisWidget::controlStateChanged(bool enabled)
{
  //    NQLog("LStepExpressAxisWidget ", NQLog::Spam)<< "controlStateChanged(bool enabled) " << enabled  ;

    if (enabled) {
        lStepStateChanged(model_->getDeviceState());
    } else {
        enabledCheckBox_->setEnabled(false);
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

void LStepExpressAxisWidget::motionStarted()
{
  //    NQLog("LStepExpressAxisWidget ", NQLog::Spam)<< "motionStarted()"  ;
}

void LStepExpressAxisWidget::motionFinished()
{
  //    NQLog("LStepExpressAxisWidget ", NQLog::Spam)<< "motionFinished()"  ;
}
