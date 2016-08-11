#include <nqlogger.h>

#include "ApplicationConfig.h"

#include "LStepExpressJoystickWidget.h"

LStepExpressJoystickWidget::LStepExpressJoystickWidget(LStepExpressModel* model,
                                                       QWidget *parent)
    : QWidget(parent),
      model_(model)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);

    axisControlWidget_= new QWidget(this);
    layout->addWidget(axisControlWidget_);

    QGridLayout* axisLayout = new QGridLayout(axisControlWidget_);
    axisControlWidget_->setLayout(axisLayout);

    // Add all the axes displays
    for (unsigned int i=0;i<4;++i) {
        axisLayout->addWidget(new LStepExpressJoystickAxisWidget(model_, i, this),
                              0, i);
    }

    connect(model_, SIGNAL(deviceStateChanged(State)),
            this, SLOT(lstepStateChanged(State)));

    connect(model_, SIGNAL(controlStateChanged(bool)),
            this, SLOT(controlStateChanged(bool)));

    connect(model_, SIGNAL(motionStarted()),
            this, SLOT(motionStarted()));

    connect(model_, SIGNAL(motionFinished()),
            this, SLOT(motionFinished()));

    lstepStateChanged(model_->getDeviceState());
}

LStepExpressJoystickWidget::~LStepExpressJoystickWidget()
{
}

void LStepExpressJoystickWidget::lstepStateChanged(State newState)
{

  std::cout<<"LStepExpressJoystickWidget " << "lStepStateChanged(State newState) " << newState    <<std::endl;

    axisControlWidget_->setEnabled(newState == READY);
}

/// Updates the GUI when the controler is enabled/disabled.
void LStepExpressJoystickWidget::controlStateChanged(bool enabled)
{
  std::cout<<"LStepExpressJoystickWidget " << "controlStateChanged(bool enabled) " << enabled    <<std::endl;

    if (enabled) {
        lstepStateChanged(model_->getDeviceState());
    } else {
        axisControlWidget_->setEnabled(false);
    }
}

void LStepExpressJoystickWidget::motionStarted()
{
    std::cout<<"LStepExpressJoystickWidget " << "motionStarted()"    <<std::endl;
}

void LStepExpressJoystickWidget::motionFinished()
{
    std::cout<<"LStepExpressJoystickWidget " << "motionFinished()"    <<std::endl;
}

LStepExpressJoystickAxisWidget::LStepExpressJoystickAxisWidget(LStepExpressModel* model,
                                                               unsigned int axis,
                                                               QWidget *parent)
    : QWidget(parent),
      model_(model),
      axis_(axis)
{
    layout_ = new QVBoxLayout(this);
    setLayout(layout_);

    stepBox_ = new QDoubleSpinBox(this);
    stepBox_->setSuffix(" usteps");
    stepBox_->setMinimum(0.0000);
    stepBox_->setMaximum(10.0000);
    stepBox_->setValue(1.0);
    layout_->addWidget(stepBox_);

    upButton_ = new QPushButton(QIcon(QString(Config::CMSTkModLabBasePath.c_str())+"/share/icon_UP.png"),
                                "", this);
    layout_->addWidget(upButton_);
    connect(upButton_, SIGNAL(clicked()),
            this, SLOT(upButtonClicked()));

    downButton_ = new QPushButton(QIcon(QString(Config::CMSTkModLabBasePath.c_str())+"/share/icon_DOWN.png"),
                                  "", this);
    layout_->addWidget(downButton_);
    connect(downButton_, SIGNAL(clicked()),
            this, SLOT(downButtonClicked()));

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

    spyUpButton_ = new QSignalSpy(upButton_, SIGNAL(clicked()));
    spyDownButton_ = new QSignalSpy(downButton_, SIGNAL(clicked()));

    connect(downButton_, SIGNAL(clicked()),
	this, SLOT(printSpyInformation()));

    connect(upButton_, SIGNAL(clicked()),
	this, SLOT(printSpyInformation()));
}

LStepExpressJoystickAxisWidget::~LStepExpressJoystickAxisWidget()
{
    if(layout_){delete layout_; layout_ = NULL;}
    if(upButton_){delete upButton_; upButton_ = NULL;}
    if(downButton_){delete downButton_; downButton_ = NULL;}
    if(stepBox_){delete stepBox_; stepBox_ = NULL;}
    if(spyUpButton_){delete spyUpButton_ ; spyUpButton_ = NULL;}
    if(spyDownButton_){delete spyDownButton_; spyDownButton_ = NULL;}
}

void LStepExpressJoystickAxisWidget::printSpyInformation()
{
    for(int i = 0; i < spyUpButton_->size(); i++){
        std::cout<<"SPY LStepExpressJoystickAxisWidget " << "upButton_, signal clicked()"    <<std::endl;
    }
    spyUpButton_->clear();
    for(int i = 0; i < spyDownButton_->size(); i++){
        std::cout<<"SPY LStepExpressJoystickAxisWidget " << "downButton_, signal clicked()"    <<std::endl;
    }
    spyDownButton_->clear();
}

void LStepExpressJoystickAxisWidget::updateWidgets()
{
    bool axis = model_->getAxisEnabled(axis_);

    std::cout<<"LStepExpressJoystickAxisWidget " << "updateWidgets() " << axis    <<std::endl;

    stepBox_->setSuffix(QString(" ") + model_->getAxisDimensionShortName(axis_));

    if (model_->getAxisEnabled(axis_)) {
      std::cout<<"LStepExpressJoystickAxisWidget " << "updateWidgets() " << axis << "axis is enabled, enable buttons"    <<std::endl;
        stepBox_->setEnabled(true);
        upButton_->setEnabled(true);
        downButton_->setEnabled(true);
    } else {
        stepBox_->setEnabled(false);
        upButton_->setEnabled(false);
        downButton_->setEnabled(false);
    }
}

void LStepExpressJoystickAxisWidget::updateMotionWidgets()
{
    // std::cout<<"LStepExpressJoystickAxisWidget " << "updateMotionWidgets()"    <<std::endl;
}

void LStepExpressJoystickAxisWidget::lStepStateChanged(State newState)
{
    // std::cout<<"LStepExpressJoystickAxisWidget " << "lStepStateChanged(State newState) " << newState    <<std::endl;
    // std::cout<<"LStepExpressJoystickAxisWidget " << "                             axis " << model_->getAxisEnabled(axis_)    <<std::endl;

    if (newState == READY || newState == INITIALIZING) {
        if (model_->getAxisEnabled(axis_)) {
            stepBox_->setEnabled(true);
            upButton_->setEnabled(true);
            downButton_->setEnabled(true);
        } else {
            stepBox_->setEnabled(false);
            upButton_->setEnabled(false);
            downButton_->setEnabled(false);
        }
    } else {
        stepBox_->setEnabled(false);
        upButton_->setEnabled(false);
        downButton_->setEnabled(false);
    }
}

void LStepExpressJoystickAxisWidget::controlStateChanged(bool enabled)
{
    // std::cout<<"LStepExpressJoystickAxisWidget " << "controlStateChanged(bool enabled) " << enabled    <<std::endl;

    if (enabled) {
        lStepStateChanged(model_->getDeviceState());
    } else {
        upButton_->setEnabled(false);
        downButton_->setEnabled(false);
    }
}

void LStepExpressJoystickAxisWidget::upButtonClicked()
{
    // std::cout<<"LStepExpressJoystickAxisWidget " << "upButtonClicked()"    <<std::endl;

    std::vector<double> values{ 0.0, 0.0, 0.0, 0.0 };
    values[axis_] = stepBox_->value();

    model_->moveRelative(values);
}

void LStepExpressJoystickAxisWidget::downButtonClicked()
{
    // std::cout<<"LStepExpressJoystickAxisWidget " << "downButtonClicked()"    <<std::endl;

    std::vector<double> values{ 0.0, 0.0, 0.0, 0.0 };
    values[axis_] = -stepBox_->value();

    model_->moveRelative(values);
}

void LStepExpressJoystickAxisWidget::motionStarted()
{
    std::cout<<"LStepExpressJoystickAxisWidget " << "motionStarted()"    <<std::endl;
}

void LStepExpressJoystickAxisWidget::motionFinished()
{
    std::cout<<"LStepExpressJoystickAxisWidget " << "motionFinished()"    <<std::endl;
}
