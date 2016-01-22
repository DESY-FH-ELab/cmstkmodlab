#include <nqlogger.h>

#include "ApplicationConfig.h"

#include "LStepExpressSettingsWidget.h"

LStepExpressSettingsCheckBox::LStepExpressSettingsCheckBox(LStepExpressSettings* settings,
                                                           const QString& key,
                                                           QWidget * parent)
    : QCheckBox(parent),
      settings_(settings),
      key_(key)
{
    connect(this, SIGNAL(stateChanged(int)),
            this, SLOT(statusChanged(int)));

    connect(this, SIGNAL(valueChanged(QString,bool)),
            settings_, SLOT(valueChanged(QString,bool)));
}

void LStepExpressSettingsCheckBox::statusChanged(int /* state */)
{
    emit valueChanged(key_, isChecked());
}

LStepExpressSettingsTripleCheckBox::LStepExpressSettingsTripleCheckBox(LStepExpressSettings* settings,
                                                                       const QString& key,
                                                                       QWidget * parent)
    : QWidget(parent),
      settings_(settings),
      key_(key)
{
    QHBoxLayout * layout = new QHBoxLayout(this);
    setLayout(layout);

    box_[0] = new QCheckBox(this);
    layout->addWidget(box_[0]);
    box_[1] = new QCheckBox(this);
    layout->addWidget(box_[1]);
    box_[2] = new QCheckBox(this);
    layout->addWidget(box_[2]);

    connect(box_[0], SIGNAL(stateChanged(int)),
            this, SLOT(statusChanged(int)));
    connect(box_[1], SIGNAL(stateChanged(int)),
            this, SLOT(statusChanged(int)));
    connect(box_[2], SIGNAL(stateChanged(int)),
            this, SLOT(statusChanged(int)));

    connect(this, SIGNAL(valueChanged(QString,bool,bool,bool)),
            settings_, SLOT(valueChanged(QString,bool,bool,bool)));
}

void LStepExpressSettingsTripleCheckBox::statusChanged(int /* state */)
{
    emit valueChanged(key_, box_[0]->isChecked(), box_[1]->isChecked(), box_[2]->isChecked());
}

LStepExpressSettingsIntSpinBox::LStepExpressSettingsIntSpinBox(LStepExpressSettings* settings,
                                                               const QString& key,
                                                               int min, int max,
                                                               QWidget * parent)
    : QSpinBox(parent),
      settings_(settings),
      key_(key)
{
    setMinimum(min);
    setMaximum(max);
    setSingleStep(min);

    connect(this, SIGNAL(valueChanged(int)),
            this, SLOT(handleValueChanged(int)));

    connect(this, SIGNAL(valueChanged(QString,int)),
            settings_, SLOT(valueChanged(QString,int)));
}

void LStepExpressSettingsIntSpinBox::wheelEvent(QWheelEvent * event)
{
    event->accept();
}

void LStepExpressSettingsIntSpinBox::handleValueChanged(int value)
{
    emit valueChanged(key_, value);
}

LStepExpressSettingsDoubleSpinBox::LStepExpressSettingsDoubleSpinBox(LStepExpressSettings* settings,
                                                                     const QString& key,
                                                                     double min, double max,
                                                                     QWidget * parent)
    : QDoubleSpinBox(parent),
      settings_(settings),
      key_(key)
{
    setMinimum(min);
    setMaximum(max);
    setSingleStep(min);
    setDecimals(4);

    connect(this, SIGNAL(valueChanged(double)),
            this, SLOT(handleValueChanged(double)));

    connect(this, SIGNAL(valueChanged(QString,double)),
            settings_, SLOT(valueChanged(QString,double)));
}

void LStepExpressSettingsDoubleSpinBox::wheelEvent(QWheelEvent * event)
{
    event->accept();
}

void LStepExpressSettingsDoubleSpinBox::handleValueChanged(double value)
{
    emit valueChanged(key_, value);
}

LStepExpressSettingsWidget::LStepExpressSettingsWidget(LStepExpressSettings* settings,
                                                       QWidget *parent)
    : QWidget(parent),
      settings_(settings)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(4, 4, 4, 4);
    setLayout(vlayout);

    setMinimumWidth(900);

    mainToolBox_ = new QToolBox(this);
    vlayout->addWidget(mainToolBox_);

    generalToolBox_ = new QWidget(mainToolBox_);
    fillGeneralToolBox();
    mainToolBox_->addItem(generalToolBox_, "General");

    axisToolBox_ = new QWidget(mainToolBox_);
    fillAxisToolBox();
    mainToolBox_->addItem(axisToolBox_, "Axis");

    motorToolBox_ = new QWidget(mainToolBox_);
    fillMotorToolBox();
    mainToolBox_->addItem(motorToolBox_, "Motor");

    encoderToolBox_ = new QWidget(mainToolBox_);
    fillEncoderToolBox();
    mainToolBox_->addItem(encoderToolBox_, "Encoder");

    QWidget * buttons = new QWidget(this);
    QHBoxLayout* hlayout = new QHBoxLayout(this);
    hlayout->setContentsMargins(4, 4, 4, 4);
    buttons->setLayout(hlayout);

    readFromDeviceButton_ = new QPushButton("Read From Device", buttons);
    connect(readFromDeviceButton_, SIGNAL(clicked(bool)),
            this, SLOT(readFromDeviceClicked()));
    hlayout->addWidget(readFromDeviceButton_);

    readFromFileButton_ = new QPushButton("Read From File", buttons);
    connect(readFromFileButton_, SIGNAL(clicked(bool)),
            this, SLOT(readFromFileClicked()));
    hlayout->addWidget(readFromFileButton_);

    writeToDeviceButton_ = new QPushButton("Write To Device", buttons);
    connect(writeToDeviceButton_, SIGNAL(clicked(bool)),
            this, SLOT(writeToDeviceClicked()));
    hlayout->addWidget(writeToDeviceButton_);

    writeToFileButton_ = new QPushButton("Write To File", buttons);
    connect(writeToFileButton_, SIGNAL(clicked(bool)),
            this, SLOT(writeToFileClicked()));
    hlayout->addWidget(writeToFileButton_);

    vlayout->addWidget(buttons);
}

void LStepExpressSettingsWidget::fillGeneralToolBox()
{
    QGridLayout *layout = new QGridLayout(generalToolBox_);
    generalToolBox_->setLayout(layout);

    layout->addWidget(new QLabel("Axis Count", generalToolBox_), 0, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "AxisCount", 0, 4, generalToolBox_), 0, 1);

    layout->addWidget(new QLabel("eQep Inputs", generalToolBox_), 1, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "EQEP", 0, 5, generalToolBox_), 1, 1);

    layout->addWidget(new QLabel("TTL Outputs", generalToolBox_), 2, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "TTLOut", generalToolBox_), 2, 1);

    layout->addWidget(new QLabel("X", generalToolBox_), 3, 1);
    layout->addWidget(new QLabel("Y", generalToolBox_), 3, 2);
    layout->addWidget(new QLabel("Z", generalToolBox_), 3, 3);
    layout->addWidget(new QLabel("A", generalToolBox_), 3, 4);

    layout->addWidget(new QLabel("Configured", generalToolBox_), 4, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-Configured", generalToolBox_), 4, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-Configured", generalToolBox_), 4, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-Configured", generalToolBox_), 4, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-Configured", generalToolBox_), 4, 4);

    layout->addWidget(new QWidget(generalToolBox_), 5, 0);
    layout->setRowStretch(5, 100);
}

void LStepExpressSettingsWidget::fillAxisToolBox()
{
    QGridLayout *layout = new QGridLayout(axisToolBox_);
    axisToolBox_->setLayout(layout);

    layout->addWidget(new QLabel("X", axisToolBox_), 0, 1);
    layout->addWidget(new QLabel("Y", axisToolBox_), 0, 2);
    layout->addWidget(new QLabel("Z", axisToolBox_), 0, 3);
    layout->addWidget(new QLabel("A", axisToolBox_), 0, 4);

    layout->addWidget(new QLabel("Direction", axisToolBox_), 1, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-AxisDirection", axisToolBox_), 1, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-AxisDirection", axisToolBox_), 1, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-AxisDirection", axisToolBox_), 1, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-AxisDirection", axisToolBox_), 1, 4);

    layout->addWidget(new QLabel("Gear Denominator", axisToolBox_), 2, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-GearDenominator", 1, 1000000, axisToolBox_), 2, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-GearDenominator", 1, 1000000, axisToolBox_), 2, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-GearDenominator", 1, 1000000, axisToolBox_), 2, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-GearDenominator", 1, 1000000, axisToolBox_), 2, 4);

    layout->addWidget(new QLabel("Gear Numerator", axisToolBox_), 3, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-GearNumerator", 1, 1000000, axisToolBox_), 3, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-GearNumerator", 1, 1000000, axisToolBox_), 3, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-GearNumerator", 1, 1000000, axisToolBox_), 3, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-GearNumerator", 1, 1000000, axisToolBox_), 3, 4);

    layout->addWidget(new QLabel("Spindle Pitch", axisToolBox_), 4, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-SpindlePitch", 0.0001, 68.0, axisToolBox_), 4, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-SpindlePitch", 0.0001, 68.0, axisToolBox_), 4, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-SpindlePitch", 0.0001, 68.0, axisToolBox_), 4, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-SpindlePitch", 0.0001, 68.0, axisToolBox_), 4, 4);

    layout->addWidget(new QLabel("Swap Limit Switch", axisToolBox_), 5, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-SwapLimitSwitch", axisToolBox_), 5, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-SwapLimitSwitch", axisToolBox_), 5, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-SwapLimitSwitch", axisToolBox_), 5, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-SwapLimitSwitch", axisToolBox_), 5, 4);

    layout->addWidget(new QLabel("Limit Switch Polarity", axisToolBox_), 6, 0);
    layout->addWidget(new LStepExpressSettingsTripleCheckBox(settings_, "X-LimitSwitchPolarity", axisToolBox_), 6, 1);
    layout->addWidget(new LStepExpressSettingsTripleCheckBox(settings_, "Y-LimitSwitchPolarity", axisToolBox_), 6, 2);
    layout->addWidget(new LStepExpressSettingsTripleCheckBox(settings_, "Z-LimitSwitchPolarity", axisToolBox_), 6, 3);
    layout->addWidget(new LStepExpressSettingsTripleCheckBox(settings_, "A-LimitSwitchPolarity", axisToolBox_), 6, 4);

    layout->addWidget(new QLabel("Acceleration", axisToolBox_), 7, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-Acceleration", 0.01, 20.0, axisToolBox_), 7, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-Acceleration", 0.01, 20.0, axisToolBox_), 7, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-Acceleration", 0.01, 20.0, axisToolBox_), 7, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-Acceleration", 0.01, 20.0, axisToolBox_), 7, 4);

    layout->addWidget(new QLabel("Acceleration Jerk", axisToolBox_), 8, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-AccelerationJerk", 1, 1000000, axisToolBox_), 8, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-AccelerationJerk", 1, 1000000, axisToolBox_), 8, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-AccelerationJerk", 1, 1000000, axisToolBox_), 8, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-AccelerationJerk", 1, 1000000, axisToolBox_), 8, 4);

    layout->addWidget(new QLabel("Deceleration", axisToolBox_), 9, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-Deceleration", 0.01, 20.0, axisToolBox_), 9, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-Deceleration", 0.01, 20.0, axisToolBox_), 9, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-Deceleration", 0.01, 20.0, axisToolBox_), 9, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-Deceleration", 0.01, 20.0, axisToolBox_), 9, 4);

    layout->addWidget(new QLabel("Deceleration Jerk", axisToolBox_), 10, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-DecelerationJerk", 1, 1000000, axisToolBox_), 10, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-DecelerationJerk", 1, 1000000, axisToolBox_), 10, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-DecelerationJerk", 1, 1000000, axisToolBox_), 10, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-DecelerationJerk", 1, 1000000, axisToolBox_), 10, 4);

    layout->addWidget(new QLabel("Velocity", axisToolBox_), 11, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-Velocity", 0.0, 100.0, axisToolBox_), 11, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-Velocity", 0.0, 100.0, axisToolBox_), 11, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-Velocity", 0.0, 100.0, axisToolBox_), 11, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-Velocity", 0.0, 100.0, axisToolBox_), 11, 4);

    layout->addWidget(new QWidget(generalToolBox_), 12, 0);
    layout->setRowStretch(12, 100);
}

void LStepExpressSettingsWidget::fillMotorToolBox()
{
    QGridLayout *layout = new QGridLayout(motorToolBox_);
    motorToolBox_->setLayout(layout);

    layout->addWidget(new QLabel("X", motorToolBox_), 0, 1);
    layout->addWidget(new QLabel("Y", motorToolBox_), 0, 2);
    layout->addWidget(new QLabel("Z", motorToolBox_), 0, 3);
    layout->addWidget(new QLabel("A", motorToolBox_), 0, 4);

    layout->addWidget(new QLabel("Current Reduction", motorToolBox_), 1, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorCurrentReduction", 0, 100, motorToolBox_), 1, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorCurrentReduction", 0, 100, motorToolBox_), 1, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorCurrentReduction", 0, 100, motorToolBox_), 1, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorCurrentReduction", 0, 100, motorToolBox_), 1, 4);

    layout->addWidget(new QLabel("Current Reduction Delay", motorToolBox_), 2, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorCurrentReductionDelay", 0, 10000, motorToolBox_), 2, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorCurrentReductionDelay", 0, 10000, motorToolBox_), 2, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorCurrentReductionDelay", 0, 10000, motorToolBox_), 2, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorCurrentReductionDelay", 0, 10000, motorToolBox_), 2, 4);

    layout->addWidget(new QLabel("Modulo Axis", motorToolBox_), 3, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-ModuloAxis", motorToolBox_), 3, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-ModuloAxis", motorToolBox_), 3, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-ModuloAxis", motorToolBox_), 3, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-ModuloAxis", motorToolBox_), 3, 4);

    layout->addWidget(new QLabel("Modulo Axis Mode", motorToolBox_), 4, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-ModuloAxisMode", 0, 4, motorToolBox_), 4, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-ModuloAxisMode", 0, 4, motorToolBox_), 4, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-ModuloAxisMode", 0, 4, motorToolBox_), 4, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-ModuloAxisMode", 0, 4, motorToolBox_), 4, 4);

    layout->addWidget(new QLabel("Type", motorToolBox_), 5, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorType", 0, 7, motorToolBox_), 5, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorType", 0, 7, motorToolBox_), 5, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorType", 0, 7, motorToolBox_), 5, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorType", 0, 7, motorToolBox_), 5, 4);

    layout->addWidget(new QLabel("Pole Scale", motorToolBox_), 6, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorPoleScale", 0, 10000, motorToolBox_), 6, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorPoleScale", 0, 10000, motorToolBox_), 6, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorPoleScale", 0, 10000, motorToolBox_), 6, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorPoleScale", 0, 10000, motorToolBox_), 6, 4);

    layout->addWidget(new QLabel("Pole Pairs", motorToolBox_), 7, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorPolePairs", 0, 10000, motorToolBox_), 7, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorPolePairs", 0, 10000, motorToolBox_), 7, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorPolePairs", 0, 10000, motorToolBox_), 7, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorPolePairs", 0, 10000, motorToolBox_), 7, 4);

    layout->addWidget(new QLabel("Pole Pair Resolution", motorToolBox_), 8, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorPolePairResolution", 0, 100000, motorToolBox_), 8, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorPolePairResolution", 0, 100000, motorToolBox_), 8, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorPolePairResolution", 0, 100000, motorToolBox_), 8, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorPolePairResolution", 0, 100000, motorToolBox_), 8, 4);

    layout->addWidget(new QLabel("Field Direction", motorToolBox_), 9, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-MotorFieldDirection", motorToolBox_), 9, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-MotorFieldDirection", motorToolBox_), 9, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-MotorFieldDirection", motorToolBox_), 9, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-MotorFieldDirection", motorToolBox_), 9, 4);

    layout->addWidget(new QLabel("Maximum Velocity", motorToolBox_), 10, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-MotorMaxVelocity", 0, 100000, motorToolBox_), 10, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-MotorMaxVelocity", 0, 100000, motorToolBox_), 10, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-MotorMaxVelocity", 0, 100000, motorToolBox_), 10, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-MotorMaxVelocity", 0, 100000, motorToolBox_), 10, 4);

    layout->addWidget(new QLabel("Brake Mode", motorToolBox_), 11, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorBrake", 0, 3, motorToolBox_), 11, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorBrake", 0, 3, motorToolBox_), 11, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorBrake", 0, 3, motorToolBox_), 11, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorBrake", 0, 3, motorToolBox_), 11, 4);

    layout->addWidget(new QLabel("Brake Switch On Delay", motorToolBox_), 12, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorBrakeSwitchOnDelay", -5000, 5000, motorToolBox_), 12, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorBrakeSwitchOnDelay", -5000, 5000, motorToolBox_), 12, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorBrakeSwitchOnDelay", -5000, 5000, motorToolBox_), 12, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorBrakeSwitchOnDelay", -5000, 5000, motorToolBox_), 12, 4);

    layout->addWidget(new QLabel("Brake Switch Off Delay", motorToolBox_), 13, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorBrakeSwitchOffDelay", -5000, 5000, motorToolBox_), 13, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorBrakeSwitchOffDelay", -5000, 5000, motorToolBox_), 13, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorBrakeSwitchOffDelay", -5000, 5000, motorToolBox_), 13, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorBrakeSwitchOffDelay", -5000, 5000, motorToolBox_), 13, 4);

    layout->addWidget(new QLabel("Temperature Sensor", motorToolBox_), 14, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-MotorTempSensor", motorToolBox_), 14, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-MotorTempSensor", motorToolBox_), 14, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-MotorTempSensor", motorToolBox_), 14, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-MotorTempSensor", motorToolBox_), 14, 4);

    layout->addWidget(new QLabel("Lower Temperature Limit", motorToolBox_), 15, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorTempSensorLowerLimit", 0, 17000, motorToolBox_), 15, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorTempSensorLowerLimit", 0, 17000, motorToolBox_), 15, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorTempSensorLowerLimit", 0, 17000, motorToolBox_), 15, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorTempSensorLowerLimit", 0, 17000, motorToolBox_), 15, 4);

    layout->addWidget(new QLabel("Upper Temperature Limit", motorToolBox_), 16, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorTempSensorUpperLimit", 0, 17000, motorToolBox_), 16, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorTempSensorUpperLimit", 0, 17000, motorToolBox_), 16, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorTempSensorUpperLimit", 0, 17000, motorToolBox_), 16, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorTempSensorUpperLimit", 0, 17000, motorToolBox_), 16, 4);

    layout->addWidget(new QLabel("Moment Constant", motorToolBox_), 17, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-MotorMomentConstant", 0, 50, motorToolBox_), 17, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-MotorMomentConstant", 0, 50, motorToolBox_), 17, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-MotorMomentConstant", 0, 50, motorToolBox_), 17, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-MotorMomentConstant", 0, 50, motorToolBox_), 17, 4);

    layout->addWidget(new QLabel("Moment Of Inertia", motorToolBox_), 18, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-MotorMomentOfInertia", 0, 100000, motorToolBox_), 18, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-MotorMomentOfInertia", 0, 100000, motorToolBox_), 18, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-MotorMomentOfInertia", 0, 100000, motorToolBox_), 18, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-MotorMomentOfInertia", 0, 100000, motorToolBox_), 18, 4);

    layout->addWidget(new QLabel("Current", motorToolBox_), 19, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-MotorCurrent", 0.5, 5.0, motorToolBox_), 19, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-MotorCurrent", 0.5, 5.0, motorToolBox_), 19, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-MotorCurrent", 0.5, 5.0, motorToolBox_), 19, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-MotorCurrent", 0.5, 5.0, motorToolBox_), 19, 4);

    layout->addWidget(new QWidget(generalToolBox_), 20, 0);
    layout->setRowStretch(20, 100);
}

void LStepExpressSettingsWidget::fillEncoderToolBox()
{
    QGridLayout *layout = new QGridLayout(encoderToolBox_);
    encoderToolBox_->setLayout(layout);

    layout->addWidget(new QLabel("X", encoderToolBox_), 0, 1);
    layout->addWidget(new QLabel("Y", encoderToolBox_), 0, 2);
    layout->addWidget(new QLabel("Z", encoderToolBox_), 0, 3);
    layout->addWidget(new QLabel("A", encoderToolBox_), 0, 4);

    layout->addWidget(new QLabel("Direction", encoderToolBox_), 1, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-EncoderDirection", encoderToolBox_), 1, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-EncoderDirection", encoderToolBox_), 1, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-EncoderDirection", encoderToolBox_), 1, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-EncoderDirection", encoderToolBox_), 1, 4);

    layout->addWidget(new QLabel("Period", encoderToolBox_), 2, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-EncoderPeriod", 0.0, 20.0, encoderToolBox_), 2, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-EncoderPeriod", 0.0, 20.0, encoderToolBox_), 2, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-EncoderPeriod", 0.0, 20.0, encoderToolBox_), 2, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-EncoderPeriod", 0.0, 20.0, encoderToolBox_), 2, 4);

    layout->addWidget(new QLabel("Pole Pairs", encoderToolBox_), 3, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-EncoderPolePairs", 1, 1000000, encoderToolBox_), 3, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-EncoderPolePairs", 1, 1000000, encoderToolBox_), 3, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-EncoderPolePairs", 1, 1000000, encoderToolBox_), 3, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-EncoderPolePairs", 1, 1000000, encoderToolBox_), 3, 4);

    layout->addWidget(new QLabel("Assignment", encoderToolBox_), 4, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-EncoderToAxis", 0, 14, encoderToolBox_), 4, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-EncoderToAxis", 0, 14, encoderToolBox_), 4, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-EncoderToAxis", 0, 14, encoderToolBox_), 4, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-EncoderToAxis", 0, 14, encoderToolBox_), 4, 4);

    layout->addWidget(new QLabel("Type", encoderToolBox_), 5, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-EncoderType", 0, 12, encoderToolBox_), 5, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-EncoderType", 0, 12, encoderToolBox_), 5, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-EncoderType", 0, 12, encoderToolBox_), 5, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-EncoderType", 0, 12, encoderToolBox_), 5, 4);

    layout->addWidget(new QLabel("Position", encoderToolBox_), 6, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-EncoderPosition", encoderToolBox_), 6, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-EncoderPosition", encoderToolBox_), 6, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-EncoderPosition", encoderToolBox_), 6, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-EncoderPosition", encoderToolBox_), 6, 4);

    layout->addWidget(new QLabel("Reference", encoderToolBox_), 7, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-EncoderReference", encoderToolBox_), 7, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-EncoderReference", encoderToolBox_), 7, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-EncoderReference", encoderToolBox_), 7, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-EncoderReference", encoderToolBox_), 7, 4);

    layout->addWidget(new QLabel("Reference Polarity", encoderToolBox_), 8, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-EncoderReferencePolarity", encoderToolBox_), 8, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-EncoderReferencePolarity", encoderToolBox_), 8, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-EncoderReferencePolarity", encoderToolBox_), 8, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-EncoderReferencePolarity", encoderToolBox_), 8, 4);

    layout->addWidget(new QWidget(generalToolBox_), 9, 0);
    layout->setRowStretch(9, 100);
}

void LStepExpressSettingsWidget::readFromDeviceClicked()
{
    NQLog("LStepExpressSettingsWidget") << "readFromDeviceClicked()";

    settings_->readSettingsFromDevice();
}

void LStepExpressSettingsWidget::readFromFileClicked()
{
    NQLog("LStepExpressSettingsWidget") << "readFromFileClicked()";

}

void LStepExpressSettingsWidget::writeToDeviceClicked()
{
    NQLog("LStepExpressSettingsWidget") << "writeToDeviceClicked()";

    settings_->writeSettingsToDevice();
}

void LStepExpressSettingsWidget::writeToFileClicked()
{
    NQLog("LStepExpressSettingsWidget") << "writeToFileClicked()";

}
