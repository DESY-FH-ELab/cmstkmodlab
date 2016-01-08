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

void LStepExpressJoystickWidget::lstepStateChanged(State newState)
{
    NQLog("LStepExpressJoystickWidget", NQLog::Debug) << "lStepStateChanged(State newState) " << newState;

    axisControlWidget_->setEnabled(newState == READY);
}

/// Updates the GUI when the controler is enabled/disabled.
void LStepExpressJoystickWidget::controlStateChanged(bool enabled)
{
    NQLog("LStepExpressJoystickWidget", NQLog::Debug) << "controlStateChanged(bool enabled) " << enabled;

    if (enabled) {
        lstepStateChanged(model_->getDeviceState());
    } else {
        axisControlWidget_->setEnabled(false);
    }
}

void LStepExpressJoystickWidget::motionStarted()
{
    NQLog("LStepExpressJoystickWidget", NQLog::Debug) << "motionStarted()";
}

void LStepExpressJoystickWidget::motionFinished()
{
    NQLog("LStepExpressJoystickWidget", NQLog::Debug) << "motionFinished()";
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
    stepBox_->setSuffix(" mm");
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
}

void LStepExpressJoystickAxisWidget::updateWidgets()
{
    bool axis = model_->getAxisEnabled(axis_);

    NQLog("LStepExpressJoystickAxisWidget", NQLog::Debug) << "updateWidgets() " << axis;

    if (model_->getAxisEnabled(axis_)) {
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
    // NQLog("LStepExpressJoystickAxisWidget", NQLog::Debug) << "updateMotionWidgets()";
}

void LStepExpressJoystickAxisWidget::lStepStateChanged(State newState)
{
    NQLog("LStepExpressJoystickAxisWidget", NQLog::Debug) << "lStepStateChanged(State newState) " << newState;
    NQLog("LStepExpressJoystickAxisWidget", NQLog::Debug) << "                             axis " << model_->getAxisEnabled(axis_);

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
    NQLog("LStepExpressJoystickAxisWidget", NQLog::Debug) << "controlStateChanged(bool enabled) " << enabled;

    if (enabled) {
        lStepStateChanged(model_->getDeviceState());
    } else {
        upButton_->setEnabled(false);
        downButton_->setEnabled(false);
    }
}

void LStepExpressJoystickAxisWidget::upButtonClicked()
{
    NQLog("LStepExpressJoystickAxisWidget", NQLog::Debug) << "upButtonClicked()";

    std::vector<double> values{ 0.0, 0.0, 0.0, 0.0 };
    values[axis_] = stepBox_->value();

    model_->moveRelative(values);
}

void LStepExpressJoystickAxisWidget::downButtonClicked()
{
    NQLog("LStepExpressJoystickAxisWidget", NQLog::Debug) << "downButtonClicked()";

    std::vector<double> values{ 0.0, 0.0, 0.0, 0.0 };
    values[axis_] = -stepBox_->value();

    model_->moveRelative(values);
}

void LStepExpressJoystickAxisWidget::motionStarted()
{
    NQLog("LStepExpressJoystickAxisWidget", NQLog::Debug) << "motionStarted()";
}

void LStepExpressJoystickAxisWidget::motionFinished()
{
    NQLog("LStepExpressJoystickAxisWidget", NQLog::Debug) << "motionFinished()";
}
