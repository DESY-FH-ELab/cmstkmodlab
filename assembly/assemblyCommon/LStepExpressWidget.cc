#include <nqlogger.h>

#include "LStepExpressWidget.h"

LStepExpressWidget::LStepExpressWidget(LStepExpressModel* model,
                                       QWidget *parent)
    : QWidget(parent),
      model_(model)
{
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

    /*
    spyTimer_ = new QTimer(this);
    spyTimer_->setInterval(500);
    spyTimer_->start();
    connect(spyTimer_, SIGNAL(timeout()), this, SLOT(printSpyInformation()));
    */

    spyLstepCheckBox_ = new QSignalSpy(lstepCheckBox_, SIGNAL(toggled(bool)));
    spyJoystickCheckBox_ = new QSignalSpy(joystickCheckBox_, SIGNAL(toggled(bool)));
    spyButtonOrigin_ = new QSignalSpy(buttonOrigin_, SIGNAL(clicked()));
    spyButtonCalibrate_ = new QSignalSpy(buttonCalibrate_, SIGNAL(clicked()));
    spyButtonEmergencyStop_ = new QSignalSpy(buttonEmergencyStop_, SIGNAL(clicked()));

    connect(lstepCheckBox_, SIGNAL(toggled(bool)),
            this, SLOT(printSpyInformation()));

    connect(joystickCheckBox_, SIGNAL(toggled(bool)),
            this, SLOT(printSpyInformation()));

    connect(buttonOrigin_, SIGNAL(clicked()),
            this, SLOT(printSpyInformation()));

    connect(buttonCalibrate_, SIGNAL(clicked()),
	this, SLOT(printSpyInformation()));

    connect(buttonEmergencyStop_, SIGNAL(clicked()),
	this, SLOT(printSpyInformation()));
}

void LStepExpressWidget::printSpyInformation()
{

    //    std::cout<<"LStepExpressWidget"<< "printSpyInformation";
    for(int i = 0; i < spyLstepCheckBox_->size(); i++){
        std::cout<<"SPY LStepExpressWidget"<< "lstepCheckBox_, signal toggled("<<(spyLstepCheckBox_->value(i))[0].toBool()<<")"  <<std::endl;
    }
    spyLstepCheckBox_->clear();
    for(int i = 0; i < spyJoystickCheckBox_->size(); i++){
        std::cout<<"SPY LStepExpressWidget"<< "joystickCheckBox_, signal toggled("<<(spyJoystickCheckBox_->value(i))[0].toBool()<<")"  <<std::endl;
    }
    spyJoystickCheckBox_->clear();
    for(int i = 0; i < spyButtonOrigin_->size(); i++){
        std::cout<<"SPY LStepExpressWidget"<< "ButtonOrigin_, signal clicked()"  <<std::endl;
    }
    spyButtonOrigin_->clear();
    for(int i = 0; i < spyButtonCalibrate_->size(); i++){
        std::cout<<"SPY LStepExpressWidget"<< "ButtonCalibrate_, signal clicked()"  <<std::endl;
    }
    spyButtonCalibrate_->clear();
    for(int i = 0; i < spyButtonEmergencyStop_->size(); i++){
        std::cout<<"SPY LStepExpressWidget"<< "ButtonEmergencyStop_, signal clicked()"  <<std::endl;
    }
    spyButtonEmergencyStop_->clear();
}

void LStepExpressWidget::updateWidgets()
{
  std::cout<<"LStepExpressWidget"<< "updateWidgets() joystickenabled?"<<model_->getJoystickEnabled()  <<std::endl; 
  //  joystickCheckBox_->setChecked(model_->getJoystickEnabled());
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void LStepExpressWidget::lstepStateChanged(State newState)
{
    std::cout<<"LStepExpressWidget"<< "lStepStateChanged(State newState) " << newState  <<std::endl;

    lstepCheckBox_->setChecked(newState == READY || newState == INITIALIZING);

    joystickCheckBox_->setEnabled(newState == READY);
    //    if (newState == READY) joystickCheckBox_->setChecked(model_->getJoystickEnabled());

    buttonOrigin_->setEnabled(newState == READY);

    buttonCalibrate_->setEnabled(newState == READY);

    axisControlWidget_->setEnabled(newState == READY);

    buttonEmergencyStop_->setEnabled(newState == READY);
}

/// Updates the GUI when the controler is enabled/disabled.
void LStepExpressWidget::controlStateChanged(bool enabled)
{
    std::cout<<"LStepExpressWidget"<< "controlStateChanged(bool enabled) " << enabled  <<std::endl;

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
    std::cout<<"LStepExpressWidget"<< "motionStarted()"  <<std::endl;
}

void LStepExpressWidget::motionFinished()
{
    std::cout<<"LStepExpressWidget"<< "motionFinished()"  <<std::endl;
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

    /*
    connect(enabledCheckBox_, SIGNAL(clicked(bool)),
            this, SLOT(enabledCheckBoxToggled(bool)));

    connect(joystickCheckBox_, SIGNAL(clicked(bool)),
            this, SLOT(joystickCheckBoxToggled(bool)));
    */

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

    /*
    spyTimer_ = new QTimer(this);
    spyTimer_->setInterval(500);
    spyTimer_->start();
    connect(spyTimer_, SIGNAL(timeout()), this, SLOT(printSpyInformation()));
    */

    connect(enabledCheckBox_, SIGNAL(toggled(bool)),
            this, SLOT(printSpyInformation()));

    connect(joystickCheckBox_, SIGNAL(toggled(bool)),
            this, SLOT(printSpyInformation()));
}

void LStepExpressAxisWidget::printSpyInformation()
{
    //    std::cout<<"LStepExpressAxisWidget"<< "printSpyInformation() ";
    for(int i = 0; i < spyEnabledCheckBox_->size(); i++){
        std::cout<<"SPY LStepExpressAxisWidget"<< "enabledCheckBox_, signal toggled("<<(spyEnabledCheckBox_->value(i))[0].toBool()<<")"  <<std::endl;
    }
    spyEnabledCheckBox_->clear();
    for(int i = 0; i < spyJoystickCheckBox_->size(); i++){
        std::cout<<"SPY LStepExpressAxisWidget"<< "joystickCheckBox_, signal toggled("<<(spyJoystickCheckBox_->value(i))[0].toBool()<<")"  <<std::endl;
    }
    spyJoystickCheckBox_->clear();
}

void LStepExpressAxisWidget::updateWidgets()
{
    std::cout<<"LStepExpressAxisWidget"<< "updateWidgets()"  <<std::endl;
    
    bool axis = model_->getAxisEnabled(axis_);

    std::cout<<"LStepExpressAxisWidget"<< "updateWidgets() , axis enabled? "<<axis<<" joystick enabled? = "<<model_->getJoystickEnabled()  <<std::endl;

    if (axis && model_->getJoystickEnabled()) {
        joystickCheckBox_->setEnabled(true);
    } else {
        joystickCheckBox_->setEnabled(false);
    }

    std::cout<<"LStepExpressAxisWidget"<< "updateWidgets() JOYstick ----------->" << model_->getJoystickAxisEnabled(axis_)  <<std::endl;

    //enabledCheckBox_->setChecked(axis);
    //joystickCheckBox_->setChecked(model_->getJoystickAxisEnabled(axis_));

    //if(axis){axisDimensionName_ = model_->getAxisDimensionShortName(axis_);}

    if(axis){QString temp = model_->getAxisDimensionShortName(axis_); if(axisDimensionName_ != temp){axisDimensionName_ = temp; std::cout<<"LStepExpressAxisWidget, in updateWidgets, about to updateMotionWidgets"<<std::endl; updateMotionWidgets();}}
}

void LStepExpressAxisWidget::updateMotionWidgets()
{
    std::cout<<"LStepExpressAxisWidget"<< "updateMotionWidgets()"  <<std::endl;
    std::cout<<"LStepExpressAxisWidget"<< "updateMotionWidgets() status text = "<<model_->getAxisStatusText(axis_).toStdString()<<std::endl;
    std::cout<<"LStepExpressAxisWidget"<< "updateMotionWidgets() position = "<<model_->getPosition(axis_)  <<std::endl;
    statusLabel_->setText(model_->getAxisStatusText(axis_));
    positionLabel_->setText(QString::number(model_->getPosition(axis_), 'f', 4) + " " + axisDimensionName_);
}

void LStepExpressAxisWidget::lStepStateChanged(State newState)
{
    std::cout<<"LStepExpressAxisWidget"<< "lStepStateChanged(State newState) " << newState  <<std::endl;
    std::cout<<"LStepExpressAxisWidget"<< "                             axis " << model_->getAxisEnabled(axis_)  <<std::endl;

    if (newState == READY || newState == INITIALIZING) {
      //      enabledCheckBoxToggled(false);
      //enabledCheckBox_->setChecked(false);
      std::cout<<"LStepExpressAxisWidget"<< "lStepStateChanged(State newState) enable axis widgets"  <<std::endl;
      enabledCheckBox_->setEnabled(true);
      joystickCheckBox_->setEnabled(model_->getJoystickEnabled());
      //joystickCheckBox_->setEnabled(true);
      //joystickCheckBoxToggled(false);
      updateWidgets();
    } else {
      //enabledCheckBoxToggled(false);
      //enabledCheckBox_->setChecked(false);
      enabledCheckBox_->setEnabled(false);
      //joystickCheckBoxToggled(false);
      //joystickCheckBox_->setChecked(false);
      joystickCheckBox_->setEnabled(false);
      //joystickCheckBoxToggled(false);
    }
}

void LStepExpressAxisWidget::controlStateChanged(bool enabled)
{
    std::cout<<"LStepExpressAxisWidget"<< "controlStateChanged(bool enabled) " << enabled  <<std::endl;

    if (enabled) {
        lStepStateChanged(model_->getDeviceState());
    } else {
        enabledCheckBox_->setEnabled(false);
    }
}

void LStepExpressAxisWidget::enabledCheckBoxToggled(bool enabled)
{
    std::cout<<"LStepExpressAxisWidget"<< "enabledCheckBoxToggled(bool enabled) " << enabled  <<std::endl;

    model_->setAxisEnabled(axis_, enabled);
}

void LStepExpressAxisWidget::joystickCheckBoxToggled(bool enabled)
{
    std::cout<<"LStepExpressAxisWidget"<< "joystickCheckBoxToggled(bool enabled) " << enabled  <<std::endl;

    model_->setJoystickAxisEnabled(axis_, enabled);
}

void LStepExpressAxisWidget::motionStarted()
{
    std::cout<<"LStepExpressAxisWidget"<< "motionStarted()"  <<std::endl;
}

void LStepExpressAxisWidget::motionFinished()
{
    std::cout<<"LStepExpressAxisWidget"<< "motionFinished()"  <<std::endl;
}
