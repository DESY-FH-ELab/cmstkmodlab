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

    axisControlWidget_= new QWidget(this);
    layout->addWidget(axisControlWidget_);

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

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void LStepExpressWidget::lstepStateChanged(State newState)
{
    NQLog("LStepExpressWidget", NQLog::Debug) << "lStepStateChanged(State newState) " << newState;

    lstepCheckBox_->setChecked(newState == READY || newState == INITIALIZING);

    joystickCheckBox_->setEnabled(newState == READY);
    if (newState == READY) joystickCheckBox_->setChecked(model_->getJoystickEnabled());

    axisControlWidget_->setEnabled(newState == READY);
}

/// Updates the GUI when the controler is enabled/disabled.
void LStepExpressWidget::controlStateChanged(bool enabled)
{
    NQLog("LStepExpressWidget", NQLog::Debug) << "controlStateChanged(bool enabled) " << enabled;

    if (enabled) {
        lstepStateChanged(model_->getDeviceState());
    } else {
        lstepCheckBox_->setEnabled(false);
        joystickCheckBox_->setEnabled(false);
        axisControlWidget_->setEnabled(false);
    }
}

void LStepExpressWidget::motionStarted()
{
    NQLog("LStepExpressWidget", NQLog::Debug) << "motionStarted()";
}

void LStepExpressWidget::motionFinished()
{
    NQLog("LStepExpressWidget", NQLog::Debug) << "motionFinished()";
}

LStepExpressAxisWidget::LStepExpressAxisWidget(LStepExpressModel* model,
                                               unsigned int axis,
                                               QWidget *parent)
    : QWidget(parent),
      model_(model),
      axis_(axis)
{
    layout_ = new QFormLayout(this);
    setLayout(layout_);

    enabledCheckBox_ = new QCheckBox(model_->getAxisName(axis), this);
    layout_->addRow(enabledCheckBox_);

    joystickCheckBox_ = new QCheckBox("joystick", this);
    layout_->addRow(joystickCheckBox_);

    statusLabel_ = new QLabel("-", this);
    statusLabel_->setAlignment(Qt::AlignHCenter);
    statusLabel_->setFont(QFont("Helvetica", 24));
    layout_->addRow(statusLabel_);

    positionLabel_ = new QLabel("0.0000 mm", this);
    positionLabel_->setAlignment(Qt::AlignHCenter);
    positionLabel_->setFont(QFont("Helvetica", 18));
    layout_->addRow(positionLabel_);

    connect(enabledCheckBox_, SIGNAL(clicked(bool)),
            this, SLOT(enabledCheckBoxToggled(bool)));

    connect(joystickCheckBox_, SIGNAL(clicked(bool)),
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

void LStepExpressAxisWidget::updateWidgets()
{
    bool axis = model_->getAxisEnabled(axis_);

    NQLog("LStepExpressAxisWidget", NQLog::Debug) << "updateWidgets() " << axis;

    if (axis && model_->getJoystickEnabled()) {
        joystickCheckBox_->setEnabled(true);
    } else {
        joystickCheckBox_->setEnabled(false);
    }

    NQLog("LStepExpressAxisWidget", NQLog::Debug) << "updateWidgets() " << model_->getJoystickAxisEnabled(axis);

    enabledCheckBox_->setChecked(axis);
    joystickCheckBox_->setChecked(model_->getJoystickAxisEnabled(axis));
}

void LStepExpressAxisWidget::updateMotionWidgets()
{
    NQLog("LStepExpressAxisWidget", NQLog::Debug) << "updateMotionWidgets()";

    statusLabel_->setText(model_->getAxisStatusText(axis_));
    positionLabel_->setText(QString::number(model_->getPosition(axis_), 'f', 4) + " mm");
}

void LStepExpressAxisWidget::lStepStateChanged(State newState)
{
    NQLog("LStepExpressAxisWidget", NQLog::Debug) << "lStepStateChanged(State newState) " << newState;
    NQLog("LStepExpressAxisWidget", NQLog::Debug) << "                             axis " << model_->getAxisEnabled(axis_);

    if (newState == READY || newState == INITIALIZING) {
        enabledCheckBox_->setEnabled(true);
        updateWidgets();
    } else {
        enabledCheckBox_->setEnabled(false);
        joystickCheckBox_->setEnabled(false);
    }
}

void LStepExpressAxisWidget::controlStateChanged(bool enabled)
{
    NQLog("LStepExpressAxisWidget", NQLog::Debug) << "controlStateChanged(bool enabled) " << enabled;

    if (enabled) {
        lStepStateChanged(model_->getDeviceState());
    } else {
        enabledCheckBox_->setEnabled(false);
    }
}

void LStepExpressAxisWidget::enabledCheckBoxToggled(bool enabled)
{
    NQLog("LStepExpressAxisWidget", NQLog::Spam) << "enabledCheckBoxToggled(bool enabled) " << enabled;

    model_->setAxisEnabled(axis_, enabled);
}

void LStepExpressAxisWidget::joystickCheckBoxToggled(bool enabled)
{
    NQLog("LStepExpressAxisWidget", NQLog::Spam) << "joystickCheckBoxToggled(bool enabled) " << enabled;

    model_->setJoystickAxisEnabled(axis_, enabled);
}

void LStepExpressAxisWidget::motionStarted()
{
    NQLog("LStepExpressAxisWidget", NQLog::Debug) << "motionStarted()";
}

void LStepExpressAxisWidget::motionFinished()
{
    NQLog("LStepExpressAxisWidget", NQLog::Debug) << "motionFinished()";
}
