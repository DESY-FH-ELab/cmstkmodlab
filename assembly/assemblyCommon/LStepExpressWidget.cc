#include <nqlogger.h>

#include "LStepExpressWidget.h"

LStepExpressWidget::LStepExpressWidget(LStepExpressModel* model,
                                       QWidget *parent)
    : QWidget(parent),
      model_(model)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);

    lstepCheckBox_ = new QCheckBox("Enable Controler", this);
    layout->addWidget(lstepCheckBox_);

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

    connect(model_, SIGNAL(deviceStateChanged(State)),
            this, SLOT(lstepStateChanged(State)));

    connect(model_, SIGNAL(controlStateChanged(bool)),
            this, SLOT(controlStateChanged(bool)));

    lstepStateChanged(model_->getDeviceState());
}

/// Updates the GUI when the Keithley multimeter is enabled/disabled.
void LStepExpressWidget::lstepStateChanged(State newState)
{
    // NQLog("LStepExpressWidget", NQLog::Debug) << "lStepStateChanged(State newState) " << newState;

    lstepCheckBox_->setChecked(newState == READY || newState == INITIALIZING);
    axisControlWidget_->setEnabled(newState == READY);
}

/// Updates the GUI when the controler is enabled/disabled.
void LStepExpressWidget::controlStateChanged(bool enabled)
{
    // NQLog("LStepExpressWidget", NQLog::Debug) << "controlStateChanged(bool enabled) " << enabled;

    if (enabled) {
        lstepStateChanged(model_->getDeviceState());
    } else {
        lstepCheckBox_->setEnabled(false);
        axisControlWidget_->setEnabled(false);
    }
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

    positionLabel_ = new QLabel("0.0000", this);
    layout_->addRow("position", positionLabel_);

    connect(enabledCheckBox_, SIGNAL(clicked(bool)),
            this, SLOT(enabledCheckBoxToggled(bool)));

    connect(model_, SIGNAL(deviceStateChanged(State)),
            this, SLOT(lStepStateChanged(State)));

    connect(model_, SIGNAL(controlStateChanged(bool)),
            this, SLOT(controlStateChanged(bool)));

    connect(model_, SIGNAL(informationChanged()),
            this, SLOT(updateWidgets()));

    connect(model_, SIGNAL(motionInformationChanged()),
            this, SLOT(updateMotionWidgets()));
}

void LStepExpressAxisWidget::updateWidgets()
{
    bool axis = model_->getAxisEnabled(axis_);

    NQLog("LStepExpressAxisWidget", NQLog::Debug) << "updateWidgets() " << axis;

    enabledCheckBox_->setChecked(axis);
}

void LStepExpressAxisWidget::updateMotionWidgets()
{
    // NQLog("LStepExpressAxisWidget", NQLog::Debug) << "updateMotionWidgets()";

    positionLabel_->setText(QString::number(model_->getPosition(axis_), 'f', 4));
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
    }
}

void LStepExpressAxisWidget::controlStateChanged(bool enabled)
{
    // NQLog("LStepExpressAxisWidget", NQLog::Debug) << "controlStateChanged(bool enabled) " << enabled;

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