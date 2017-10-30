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

#include <QFileDialog>
#include <QMessageBox>

#include <nqlogger.h>

#include "ApplicationConfig.h"

#include "LStepExpressSettingsWidget.h"

LStepExpressSettingsComboBox::LStepExpressSettingsComboBox(LStepExpressSettings* settings,
                                                           const QString& key,
                                                           QWidget * parent)
    : QComboBox(parent),
      settings_(settings),
      key_(key)
{
//!!    connect(this, SIGNAL(valueChanged         (double)), this     , SLOT(valueChanged         (double)));
    connect(this, SIGNAL(valueChanged(QString, double)), settings_, SLOT(valueChanged(QString, double)));

    connect(settings_, SIGNAL(settingChanged(QString, QVariant)), this, SLOT(settingChanged(QString, QVariant)));
}

void LStepExpressSettingsComboBox::settingChanged(QString key, QVariant value)
{
    if (key!=key_) return;

    //    NQLog("LStepExpressSettingsComboBox", NQLog::Spam) << "settingChanged(" << key.toStdString() << ")"    ;

    setCurrentIndex(value.toInt());
}

LStepExpressSettingsCheckBox::LStepExpressSettingsCheckBox(LStepExpressSettings* settings,
                                                           const QString& key,
                                                           QWidget * parent)
    : QCheckBox(parent),
      settings_(settings),
      key_(key)
{
    connect(this, SIGNAL(stateChanged(int)),
            this, SLOT(statusChanged(int)));

    connect(this, SIGNAL(valueChanged(QString, bool)),
            settings_, SLOT(valueChanged(QString, bool)));

    connect(settings_, SIGNAL(settingChanged(QString, QVariant)),
            this, SLOT(settingChanged(QString, QVariant)));
}

void LStepExpressSettingsCheckBox::statusChanged(int /* state */)
{
    emit valueChanged(key_, isChecked());
}

void LStepExpressSettingsCheckBox::settingChanged(QString key, QVariant value)
{
    if (key!=key_) return;

    //    NQLog("LStepExpressSettingsCheckBox", NQLog::Spam) << "settingChanged(" << key.toStdString() << ")"    ;

    setChecked(value.toBool());
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

    connect(settings_, SIGNAL(settingChanged(QString, QVariant)),
            this, SLOT(settingChanged(QString, QVariant)));
}

void LStepExpressSettingsTripleCheckBox::statusChanged(int /* state */)
{
    emit valueChanged(key_, box_[0]->isChecked(), box_[1]->isChecked(), box_[2]->isChecked());
}

void LStepExpressSettingsTripleCheckBox::settingChanged(QString key, QVariant value)
{
    if (key!=key_) return;

    //    NQLog("LStepExpressSettingsTripleCheckBox", NQLog::Spam) << "settingChanged(" << key.toStdString() << ")"    ;

    QList<QVariant> list = value.toList();
    int i = 0;
    for (QList<QVariant>::Iterator it = list.begin();
         it!=list.end();
         ++it) {
        box_[i]->setChecked((*it).toInt());
        i++;
    }
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

    connect(this, SIGNAL(valueChanged(QString, int)),
            settings_, SLOT(valueChanged(QString, int)));

    connect(settings_, SIGNAL(settingChanged(QString, QVariant)),
            this, SLOT(settingChanged(QString, QVariant)));
}

void LStepExpressSettingsIntSpinBox::wheelEvent(QWheelEvent * event)
{
    event->accept();
}

void LStepExpressSettingsIntSpinBox::handleValueChanged(int value)
{
    emit valueChanged(key_, value);
}

void LStepExpressSettingsIntSpinBox::settingChanged(QString key, QVariant value)
{
    if (key!=key_) return;

    //    NQLog("LStepExpressSettingsIntSpinBox", NQLog::Spam) << "settingChanged(" << key.toStdString() << ")"    ;

    setValue(value.toInt());
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

    connect(settings_, SIGNAL(settingChanged(QString, QVariant)),
            this, SLOT(settingChanged(QString, QVariant)));
}

void LStepExpressSettingsDoubleSpinBox::wheelEvent(QWheelEvent * event)
{
    event->accept();
}

void LStepExpressSettingsDoubleSpinBox::handleValueChanged(double value)
{
    emit valueChanged(key_, value);
}

void LStepExpressSettingsDoubleSpinBox::settingChanged(QString key, QVariant value)
{
    if (key!=key_) return;

    //    NQLog("LStepExpressSettingsIntSpinBox ", NQLog::Spam) << "settingChanged(" << key.toStdString() << ")"    ;

    setValue(value.toDouble());
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

    joystickToolBox_ = new QWidget(mainToolBox_);
    fillJoystickToolBox();
    mainToolBox_->addItem(joystickToolBox_, "Joystick");

    calibrationToolBox_ = new QWidget(mainToolBox_);
    fillCalibrationToolBox();
    mainToolBox_->addItem(calibrationToolBox_, "Calibration");

    limitToolBox_ = new QWidget(mainToolBox_);
    fillLimitToolBox();
    mainToolBox_->addItem(limitToolBox_, "Limit");

    clockPulseToolBox_ = new QWidget(mainToolBox_);
    fillClockPulseToolBox();
    mainToolBox_->addItem(clockPulseToolBox_, "Clock Pulse");

    triggerToolBox_ = new QWidget(mainToolBox_);
    fillTriggerToolBox();
    mainToolBox_->addItem(triggerToolBox_, "Trigger");

    extraToolBox_ = new QWidget(mainToolBox_);
    fillExtraToolBox();
    mainToolBox_->addItem(extraToolBox_, "Extra");

    QWidget * buttons = new QWidget(this);

    QHBoxLayout* hlayout = new QHBoxLayout();
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

    applyButton_ = new QPushButton("Apply", buttons);
    connect(applyButton_, SIGNAL(clicked(bool)),
	this, SLOT(applyClicked()));
    hlayout->addWidget(applyButton_);

    resetButton_ = new QPushButton("Reset", buttons);
    connect(resetButton_, SIGNAL(clicked(bool)),
	this, SLOT(resetClicked()));
    hlayout->addWidget(resetButton_);

    writeToDeviceButton_ = new QPushButton("Write To Device", buttons);
    connect(writeToDeviceButton_, SIGNAL(clicked(bool)),
            this, SLOT(writeToDeviceClicked()));
    hlayout->addWidget(writeToDeviceButton_);

    writeToFileButton_ = new QPushButton("Write To File", buttons);
    connect(writeToFileButton_, SIGNAL(clicked(bool)),
            this, SLOT(writeToFileClicked()));
    hlayout->addWidget(writeToFileButton_);

    vlayout->addWidget(buttons);

    connect(settings_, SIGNAL(controlStateChanged(bool)),
            this, SLOT(controlStateChanged(bool)));

    readFromDeviceButton_->setEnabled(false);
    applyButton_->setEnabled(false);
    resetButton_->setEnabled(false);
    writeToDeviceButton_->setEnabled(false);

    //    connect(model_, SIGNAL(deviceStateChanged(State)),
    //        this, SLOT(lstepStateChanged(State)));

}

/*
/// Updates the GUI when the lstepcontroller is enabled/disabled.                                                                                                                                        
void LStepExpressSettingsWidget::lstepStateChanged(State newState)
{
    NQLog("LStepExpressSettingsWidget", NQLog::Spam) << "lStepStateChanged(State newState) " << newState;

    readFromDeviceButton_->setEnabled(newState == READY);

    //readFromFileButton_->setEnabled(newState == READY);

    applyButton_->setEnabled(newState == READY);

    resetButton_->setEnabled(newState == READY);

    writeToDeviceButton_->setEnabled(newState == READY);

    //writeToFileButton_->setEnabled(newState == READY);
}
*/

void LStepExpressSettingsWidget::fillGeneralToolBox()
{
    QGridLayout *layout = new QGridLayout(generalToolBox_);
    generalToolBox_->setLayout(layout);

    layout->addWidget(new QLabel("Baud Rate", generalToolBox_), 0, 0);
    LStepExpressSettingsComboBox *combo = new LStepExpressSettingsComboBox(settings_, "BaudRate", generalToolBox_);
    combo->addItem("9600"); combo->addItem("19200"); combo->addItem("38400"); combo->addItem("57600"); combo->addItem("115200");
    layout->addWidget(combo, 0, 1);

    layout->addWidget(new QLabel("Axis Count", generalToolBox_), 1, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "AxisCount", 0, 4, generalToolBox_), 1, 1);

    //    layout->addWidget(new QLabel("eQep Inputs", generalToolBox_), 1, 0);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "EQEP", 0, 5, generalToolBox_), 1, 1);

    //layout->addWidget(new QLabel("TTL Outputs", generalToolBox_), 2, 0);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "TTLOut", generalToolBox_), 2, 1);

    //layout->addWidget(new QLabel("X", generalToolBox_), 3, 1);
    //layout->addWidget(new QLabel("Y", generalToolBox_), 3, 2);
    //layout->addWidget(new QLabel("Z", generalToolBox_), 3, 3);
    //layout->addWidget(new QLabel("A", generalToolBox_), 3, 4);

    //layout->addWidget(new QLabel("Configured", generalToolBox_), 4, 0);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-Configured", generalToolBox_), 4, 1);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-Configured", generalToolBox_), 4, 2);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-Configured", generalToolBox_), 4, 3);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-Configured", generalToolBox_), 4, 4);

    layout->addWidget(new QWidget(generalToolBox_), 2, 0);
    layout->setRowStretch(2, 100);
}

void LStepExpressSettingsWidget::fillAxisToolBox()
{
    QGridLayout *layout = new QGridLayout(axisToolBox_);
    axisToolBox_->setLayout(layout);

    layout->addWidget(new QLabel("X", axisToolBox_), 0, 1);
    layout->addWidget(new QLabel("Y", axisToolBox_), 0, 2);
    layout->addWidget(new QLabel("Z", axisToolBox_), 0, 3);
    layout->addWidget(new QLabel("A", axisToolBox_), 0, 4);

    //    layout->addWidget(new QLabel("Enabled", axisToolBox_), 1, 0);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-EnableAxes", axisToolBox_), 1, 1);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-EnableAxes", axisToolBox_), 1, 2);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-EnableAxes", axisToolBox_), 1, 3);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-EnableAxes", axisToolBox_), 1, 4);

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

    layout->addWidget(new QLabel("Dimension", axisToolBox_), 12, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-Dimension", 0, 4, axisToolBox_), 12, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-Dimension", 0, 4, axisToolBox_), 12, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-Dimension", 0, 4, axisToolBox_), 12, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-Dimension", 0, 4, axisToolBox_), 12, 4);

    layout->addWidget(new QWidget(generalToolBox_), 13, 0);
    layout->setRowStretch(13, 100);
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

    //    layout->addWidget(new QLabel("Modulo Axis", motorToolBox_), 3, 0);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-ModuloAxis", motorToolBox_), 3, 1);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-ModuloAxis", motorToolBox_), 3, 2);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-ModuloAxis", motorToolBox_), 3, 3);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-ModuloAxis", motorToolBox_), 3, 4);

    //layout->addWidget(new QLabel("Modulo Axis Mode", motorToolBox_), 4, 0);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-ModuloAxisMode", 0, 4, motorToolBox_), 4, 1);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-ModuloAxisMode", 0, 4, motorToolBox_), 4, 2);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-ModuloAxisMode", 0, 4, motorToolBox_), 4, 3);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-ModuloAxisMode", 0, 4, motorToolBox_), 4, 4);

    layout->addWidget(new QLabel("Type", motorToolBox_), 3, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorType", 0, 7, motorToolBox_), 3, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorType", 0, 7, motorToolBox_), 3, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorType", 0, 7, motorToolBox_), 3, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorType", 0, 7, motorToolBox_), 3, 4);

    //layout->addWidget(new QLabel("Pole Scale", motorToolBox_), 6, 0);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorPoleScale", 0, 10000, motorToolBox_), 6, 1);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorPoleScale", 0, 10000, motorToolBox_), 6, 2);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorPoleScale", 0, 10000, motorToolBox_), 6, 3);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorPoleScale", 0, 10000, motorToolBox_), 6, 4);

    layout->addWidget(new QLabel("Pole Pairs", motorToolBox_), 4, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorPolePairs", 0, 10000, motorToolBox_), 4, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorPolePairs", 0, 10000, motorToolBox_), 4, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorPolePairs", 0, 10000, motorToolBox_), 4, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorPolePairs", 0, 10000, motorToolBox_), 4, 4);

    layout->addWidget(new QLabel("Pole Pair Resolution", motorToolBox_), 5, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorPolePairResolution", 0, 100000, motorToolBox_), 5, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorPolePairResolution", 0, 100000, motorToolBox_), 5, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorPolePairResolution", 0, 100000, motorToolBox_), 5, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorPolePairResolution", 0, 100000, motorToolBox_), 5, 4);

    layout->addWidget(new QLabel("Field Direction", motorToolBox_), 6, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-MotorFieldDirection", motorToolBox_), 6, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-MotorFieldDirection", motorToolBox_), 6, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-MotorFieldDirection", motorToolBox_), 6, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-MotorFieldDirection", motorToolBox_), 6, 4);

    layout->addWidget(new QLabel("Maximum Velocity", motorToolBox_), 7, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-MotorMaxVelocity", 0, 100000, motorToolBox_), 7, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-MotorMaxVelocity", 0, 100000, motorToolBox_), 7, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-MotorMaxVelocity", 0, 100000, motorToolBox_), 7, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-MotorMaxVelocity", 0, 100000, motorToolBox_), 7, 4);

    layout->addWidget(new QLabel("Brake Mode", motorToolBox_), 8, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorBrake", 0, 3, motorToolBox_), 8, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorBrake", 0, 3, motorToolBox_), 8, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorBrake", 0, 3, motorToolBox_), 8, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorBrake", 0, 3, motorToolBox_), 8, 4);

    layout->addWidget(new QLabel("Brake Switch On Delay", motorToolBox_), 9, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorBrakeSwitchOnDelay", -5000, 5000, motorToolBox_), 9, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorBrakeSwitchOnDelay", -5000, 5000, motorToolBox_), 9, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorBrakeSwitchOnDelay", -5000, 5000, motorToolBox_), 9, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorBrakeSwitchOnDelay", -5000, 5000, motorToolBox_), 9, 4);

    layout->addWidget(new QLabel("Brake Switch Off Delay", motorToolBox_), 10, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorBrakeSwitchOffDelay", -5000, 5000, motorToolBox_), 10, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorBrakeSwitchOffDelay", -5000, 5000, motorToolBox_), 10, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorBrakeSwitchOffDelay", -5000, 5000, motorToolBox_), 10, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorBrakeSwitchOffDelay", -5000, 5000, motorToolBox_), 10, 4);

    //layout->addWidget(new QLabel("Temperature Sensor", motorToolBox_), 14, 0);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-MotorTempSensor", motorToolBox_), 14, 1);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-MotorTempSensor", motorToolBox_), 14, 2);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-MotorTempSensor", motorToolBox_), 14, 3);
    //layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-MotorTempSensor", motorToolBox_), 14, 4);

    //layout->addWidget(new QLabel("Lower Temperature Limit", motorToolBox_), 15, 0);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorTempSensorLowerLimit", 0, 17000, motorToolBox_), 15, 1);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorTempSensorLowerLimit", 0, 17000, motorToolBox_), 15, 2);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorTempSensorLowerLimit", 0, 17000, motorToolBox_), 15, 3);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorTempSensorLowerLimit", 0, 17000, motorToolBox_), 15, 4);

    //layout->addWidget(new QLabel("Upper Temperature Limit", motorToolBox_), 16, 0);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-MotorTempSensorUpperLimit", 0, 17000, motorToolBox_), 16, 1);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-MotorTempSensorUpperLimit", 0, 17000, motorToolBox_), 16, 2);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-MotorTempSensorUpperLimit", 0, 17000, motorToolBox_), 16, 3);
    //layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-MotorTempSensorUpperLimit", 0, 17000, motorToolBox_), 16, 4);

    //layout->addWidget(new QLabel("Moment Constant", motorToolBox_), 17, 0);
    //layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-MotorMomentConstant", 0, 50, motorToolBox_), 17, 1);
    //layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-MotorMomentConstant", 0, 50, motorToolBox_), 17, 2);
    //layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-MotorMomentConstant", 0, 50, motorToolBox_), 17, 3);
    //layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-MotorMomentConstant", 0, 50, motorToolBox_), 17, 4);

    //layout->addWidget(new QLabel("Moment Of Inertia", motorToolBox_), 18, 0);
    //layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-MotorMomentOfInertia", 0, 100000, motorToolBox_), 18, 1);
    //layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-MotorMomentOfInertia", 0, 100000, motorToolBox_), 18, 2);
    //layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-MotorMomentOfInertia", 0, 100000, motorToolBox_), 18, 3);
    //layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-MotorMomentOfInertia", 0, 100000, motorToolBox_), 18, 4);

    layout->addWidget(new QLabel("Current", motorToolBox_), 11, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-MotorCurrent", 0.5, 5.0, motorToolBox_), 11, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-MotorCurrent", 0.5, 5.0, motorToolBox_), 11, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-MotorCurrent", 0.5, 5.0, motorToolBox_), 11, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-MotorCurrent", 0.5, 5.0, motorToolBox_), 11, 4);

    layout->addWidget(new QLabel("Stop input polarity", motorToolBox_), 12, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "StopInputPolarity", motorToolBox_), 12, 1);

    layout->addWidget(new QLabel("Stop deceleration", motorToolBox_), 13, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-StopDeceleration", 0.01, 20.0, motorToolBox_), 13, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-StopDeceleration", 0.01, 20.0, motorToolBox_), 13, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-StopDeceleration", 0.01, 20.0, motorToolBox_), 13, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-StopDeceleration", 0.01, 20.0, motorToolBox_), 13, 4);

    layout->addWidget(new QLabel("Stop deceleration jerk", motorToolBox_), 14, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-StopDecelerationJerk", 1, 10000, motorToolBox_), 14, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-StopDecelerationJerk", 1, 10000, motorToolBox_), 14, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-StopDecelerationJerk", 1, 10000, motorToolBox_), 14, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-StopDecelerationJerk", 1, 10000, motorToolBox_), 14, 4);

    layout->addWidget(new QLabel("Speed by Potentiometer", motorToolBox_), 15, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "SpeedByPotentioMeter", motorToolBox_), 15, 1);

    layout->addWidget(new QWidget(generalToolBox_), 16, 0);
    layout->setRowStretch(16, 100);
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

    layout->addWidget(new QLabel("Target window", encoderToolBox_), 9, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-TargetWindow", 0.0, 100000.0, encoderToolBox_), 9, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-TargetWindow", 0.0, 100000.0, encoderToolBox_), 9, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-TargetWindow", 0.0, 100000.0, encoderToolBox_), 9, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-TargetWindow", 0.0, 100000.0, encoderToolBox_), 9, 4);

    layout->addWidget(new QLabel("Kp band of position controller", encoderToolBox_), 10, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-PositionControlKPBand", 0.0, 4000.0, encoderToolBox_), 10, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-PositionControlKPBand", 0.0, 4000.0, encoderToolBox_), 10, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-PositionControlKPBand", 0.0, 4000.0, encoderToolBox_), 10, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-PositionControlKPBand", 0.0, 4000.0, encoderToolBox_), 10, 4);

    layout->addWidget(new QLabel("Position Controller time constant", encoderToolBox_), 11, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-PositionControlTimeConstant", 0, 100000, encoderToolBox_), 11, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-PositionControlTimeConstant", 0, 100000, encoderToolBox_), 11, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-PositionControlTimeConstant", 0, 100000, encoderToolBox_), 11, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-PositionControlTimeConstant", 0, 100000, encoderToolBox_), 11, 4);

    layout->addWidget(new QLabel("Position Controller axis enabled", encoderToolBox_), 12, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-PositionControlAxisEnable", encoderToolBox_), 12, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-PositionControlAxisEnable", encoderToolBox_), 12, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-PositionControlAxisEnable", encoderToolBox_), 12, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-PositionControlAxisEnable", encoderToolBox_), 12, 4);

    layout->addWidget(new QLabel("Position Controller on", encoderToolBox_), 13, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "PositionControlOn", encoderToolBox_), 13, 1);

    layout->addWidget(new QLabel("Deviation check/ range", encoderToolBox_), 14, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-DeviationRange", 0.0, 100000.0, encoderToolBox_), 14, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-DeviationRange", 0.0, 100000.0, encoderToolBox_), 14, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-DeviationRange", 0.0, 100000.0, encoderToolBox_), 14, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-DeviationRange", 0.0, 100000.0, encoderToolBox_), 14, 4);

    layout->addWidget(new QLabel("Deviation check/ time frame", encoderToolBox_), 15, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-DeviationTime", 0, 10000, encoderToolBox_), 15, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-DeviationTime", 0, 10000, encoderToolBox_), 15, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-DeviationTime", 0, 10000, encoderToolBox_), 15, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-DeviationTime", 0, 10000, encoderToolBox_), 15, 4);

    layout->addWidget(new QLabel("Deviation check/ axis enabled", encoderToolBox_), 16, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-DeviationCheck", encoderToolBox_), 16, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-DeviationCheck", encoderToolBox_), 16, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-DeviationCheck", encoderToolBox_), 16, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-DeviationCheck", encoderToolBox_), 16, 4);

    layout->addWidget(new QWidget(generalToolBox_), 17, 0);
    layout->setRowStretch(17, 100);
}

void LStepExpressSettingsWidget::fillJoystickToolBox()
{
    QGridLayout *layout = new QGridLayout(joystickToolBox_);
    joystickToolBox_->setLayout(layout);

    layout->addWidget(new QLabel("X", joystickToolBox_), 0, 1);
    layout->addWidget(new QLabel("Y", joystickToolBox_), 0, 2);
    layout->addWidget(new QLabel("Z", joystickToolBox_), 0, 3);
    layout->addWidget(new QLabel("A", joystickToolBox_), 0, 4);

    layout->addWidget(new QLabel("Speed", joystickToolBox_), 1, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-JoystickSpeed", -70.0, 70.0, joystickToolBox_), 1, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-JoystickSpeed", -70.0, 70.0, joystickToolBox_), 1, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-JoystickSpeed", -70.0, 70.0, joystickToolBox_), 1, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-JoystickSpeed", -70.0, 70.0, joystickToolBox_), 1, 4);

    layout->addWidget(new QLabel("Direction", joystickToolBox_), 2, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-JoystickDirection", joystickToolBox_), 2, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-JoystickDirection", joystickToolBox_), 2, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-JoystickDirection", joystickToolBox_), 2, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-JoystickDirection", joystickToolBox_), 2, 4);

    layout->addWidget(new QLabel("Maximum velocity", joystickToolBox_), 3, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-JoystickVelocity", 0.0, 70.0, joystickToolBox_), 3, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-JoystickVelocity", 0.0, 70.0, joystickToolBox_), 3, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-JoystickVelocity", 0.0, 70.0, joystickToolBox_), 3, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-JoystickVelocity", 0.0, 70.0, joystickToolBox_), 3, 4);

    layout->addWidget(new QLabel("Filter time", joystickToolBox_), 4, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-JoystickFilterTime", 0.0, 500000.0, joystickToolBox_), 4, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-JoystickFilterTime", 0.0, 500000.0, joystickToolBox_), 4, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-JoystickFilterTime", 0.0, 500000.0, joystickToolBox_), 4, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-JoystickFilterTime", 0.0, 500000.0, joystickToolBox_), 4, 4);

    layout->addWidget(new QLabel("Current reduction", joystickToolBox_), 5, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-JoystickCurrentReduction", joystickToolBox_), 5, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-JoystickCurrentReduction", joystickToolBox_), 5, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-JoystickCurrentReduction", joystickToolBox_), 5, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-JoystickCurrentReduction", joystickToolBox_), 5, 4);

    layout->addWidget(new QLabel("Minimum deviation", joystickToolBox_), 6, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-JoystickWindow", 0.0, 100.0, joystickToolBox_), 6, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-JoystickWindow", 0.0, 100.0, joystickToolBox_), 6, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-JoystickWindow", 0.0, 100.0, joystickToolBox_), 6, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-JoystickWindow", 0.0, 100.0, joystickToolBox_), 6, 4);

    layout->addWidget(new QLabel("Axis Assignment", joystickToolBox_), 7, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-JoystickAxisAssignment", 0, 3, joystickToolBox_), 7, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-JoystickAxisAssignment", 0, 3, joystickToolBox_), 7, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-JoystickAxisAssignment", 0, 3, joystickToolBox_), 7, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-JoystickAxisAssignment", 0, 3, joystickToolBox_), 7, 4);

    layout->addWidget(new QWidget(generalToolBox_), 8, 0);
    layout->setRowStretch(8, 100);
}

void LStepExpressSettingsWidget::fillCalibrationToolBox()
{
    QGridLayout *layout = new QGridLayout(calibrationToolBox_);
    calibrationToolBox_->setLayout(layout);

    layout->addWidget(new QLabel("X", calibrationToolBox_), 0, 1);
    layout->addWidget(new QLabel("Y", calibrationToolBox_), 0, 2);
    layout->addWidget(new QLabel("Z", calibrationToolBox_), 0, 3);
    layout->addWidget(new QLabel("A", calibrationToolBox_), 0, 4);

    layout->addWidget(new QLabel("Zero position offset", calibrationToolBox_), 1, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-CalibrationOffset", 0.0, 1600000, calibrationToolBox_), 1, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-CalibrationOffset", 0.0, 1600000, calibrationToolBox_), 1, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-CalibrationOffset", 0.0, 1600000, calibrationToolBox_), 1, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-CalibrationOffset", 0.0, 1600000, calibrationToolBox_), 1, 4);

    layout->addWidget(new QLabel("End position offset", calibrationToolBox_), 2, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-RMOffset", 0.0, 1600000, calibrationToolBox_), 2, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-RMOffset", 0.0, 1600000, calibrationToolBox_), 2, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-RMOffset", 0.0, 1600000, calibrationToolBox_), 2, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-RMOffset", 0.0, 1600000, calibrationToolBox_), 2, 4);

    layout->addWidget(new QLabel("Direction", calibrationToolBox_), 3, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-CalibrationDirection", calibrationToolBox_), 3, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-CalibrationDirection", calibrationToolBox_), 3, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-CalibrationDirection", calibrationToolBox_), 3, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-CalibrationDirection", calibrationToolBox_), 3, 4);

    layout->addWidget(new QLabel("Speed after limit trip", calibrationToolBox_), 4, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-CalibrationSpeedAfterTrip", 0.0, 70.0, calibrationToolBox_), 4, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-CalibrationSpeedAfterTrip", 0.0, 70.0, calibrationToolBox_), 4, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-CalibrationSpeedAfterTrip", 0.0, 70.0, calibrationToolBox_), 4, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-CalibrationSpeedAfterTrip", 0.0, 70.0, calibrationToolBox_), 4, 4);

    layout->addWidget(new QLabel("Acceleration", calibrationToolBox_), 5, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-CalibrationAcceleration", 0.01, 20.00, calibrationToolBox_), 5, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-CalibrationAcceleration", 0.01, 20.00, calibrationToolBox_), 5, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-CalibrationAcceleration", 0.01, 20.00, calibrationToolBox_), 5, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-CalibrationAcceleration", 0.01, 20.00, calibrationToolBox_), 5, 4);

    layout->addWidget(new QLabel("Adjust jerk", calibrationToolBox_), 6, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-CalibrationAdjustJerk", 0, 100000, calibrationToolBox_), 6, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-CalibrationAdjustJerk", 0, 100000, calibrationToolBox_), 6, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-CalibrationAdjustJerk", 0, 100000, calibrationToolBox_), 6, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-CalibrationAdjustJerk", 0, 100000, calibrationToolBox_), 6, 4);

    layout->addWidget(new QLabel("Velocity", calibrationToolBox_), 7, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-CalibrationVelocity", 0.0, 70.00, calibrationToolBox_), 7, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-CalibrationVelocity", 0.0, 70.00, calibrationToolBox_), 7, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-CalibrationVelocity", 0.0, 70.00, calibrationToolBox_), 7, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-CalibrationVelocity", 0.0, 70.00, calibrationToolBox_), 7, 4);

    layout->addWidget(new QWidget(generalToolBox_), 8, 0);
    layout->setRowStretch(8, 100);
}

void LStepExpressSettingsWidget::fillLimitToolBox()
{
    QGridLayout *layout = new QGridLayout(limitToolBox_);
    limitToolBox_->setLayout(layout);

    layout->addWidget(new QLabel("X", limitToolBox_), 0, 1);
    layout->addWidget(new QLabel("Y", limitToolBox_), 0, 2);
    layout->addWidget(new QLabel("Z", limitToolBox_), 0, 3);
    layout->addWidget(new QLabel("A", limitToolBox_), 0, 4);

    //FIX ME!
    layout->addWidget(new QLabel("Travel limits", limitToolBox_), 1, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-Limit", 0.0, 100000.00, limitToolBox_), 1, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-Limit", 0.0, 100000.00, limitToolBox_), 1, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-Limit", 0.0, 100000.00, limitToolBox_), 1, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-Limit", 0.0, 100000.00, limitToolBox_), 1, 4);

    layout->addWidget(new QLabel("Range monitoring", limitToolBox_), 2, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-RangeMonitoring", limitToolBox_), 2, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-RangeMonitoring", limitToolBox_), 2, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-RangeMonitoring", limitToolBox_), 2, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-RangeMonitoring", limitToolBox_), 2, 4);

    layout->addWidget(new QLabel("Hardware limit", limitToolBox_), 3, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-NoSoftwareLimit", limitToolBox_), 3, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-NoSoftwareLimit", limitToolBox_), 3, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-NoSoftwareLimit", limitToolBox_), 3, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-NoSoftwareLimit", limitToolBox_), 3, 4);

    layout->addWidget(new QLabel("Limit Switch On", limitToolBox_), 4, 0);
    layout->addWidget(new LStepExpressSettingsTripleCheckBox(settings_, "X-LimitSwitchOn", limitToolBox_), 4, 1);
    layout->addWidget(new LStepExpressSettingsTripleCheckBox(settings_, "Y-LimitSwitchOn", limitToolBox_), 4, 2);
    layout->addWidget(new LStepExpressSettingsTripleCheckBox(settings_, "Z-LimitSwitchOn", limitToolBox_), 4, 3);
    layout->addWidget(new LStepExpressSettingsTripleCheckBox(settings_, "A-LimitSwitchOn", limitToolBox_), 4, 4);  

    layout->addWidget(new QWidget(generalToolBox_), 5, 0);
    layout->setRowStretch(5, 100);
}

void LStepExpressSettingsWidget::fillClockPulseToolBox()
{
    QGridLayout *layout = new QGridLayout(clockPulseToolBox_);
    clockPulseToolBox_->setLayout(layout);

    layout->addWidget(new QLabel("X", clockPulseToolBox_), 0, 1);
    layout->addWidget(new QLabel("Y", clockPulseToolBox_), 0, 2);
    layout->addWidget(new QLabel("Z", clockPulseToolBox_), 0, 3);
    layout->addWidget(new QLabel("A", clockPulseToolBox_), 0, 4);

    layout->addWidget(new QLabel("Axis Assignment", clockPulseToolBox_), 1, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-ClockAxisAssignment", 0, 4, clockPulseToolBox_), 1, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-ClockAxisAssignment", 0, 4, clockPulseToolBox_), 1, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-ClockAxisAssignment", 0, 4, clockPulseToolBox_), 1, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-ClockAxisAssignment", 0, 4, clockPulseToolBox_), 1, 4);

    layout->addWidget(new QLabel("Activate", clockPulseToolBox_), 2, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-ClockPulseOn", clockPulseToolBox_), 2, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-ClockPulseOn", clockPulseToolBox_), 2, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-ClockPulseOn", clockPulseToolBox_), 2, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-ClockPulseOn", clockPulseToolBox_), 2, 4);

    layout->addWidget(new QLabel("Factor", clockPulseToolBox_), 3, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-ClockPulseFactor", -10000, 10000, clockPulseToolBox_), 3, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-ClockPulseFactor", -10000, 10000, clockPulseToolBox_), 3, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-ClockPulseFactor", -10000, 10000, clockPulseToolBox_), 3, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-ClockPulseFactor", -10000, 10000, clockPulseToolBox_), 3, 4);

    layout->addWidget(new QLabel("Mode", clockPulseToolBox_), 4, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-ClockMode", 0, 4, clockPulseToolBox_), 4, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-ClockMode", 0, 4, clockPulseToolBox_), 4, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-ClockMode", 0, 4, clockPulseToolBox_), 4, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-ClockMode", 0, 4, clockPulseToolBox_), 4, 4);

    layout->addWidget(new QWidget(generalToolBox_), 5, 0);
    layout->setRowStretch(5, 100);
}

void LStepExpressSettingsWidget::fillTriggerToolBox()
{
    QGridLayout *layout = new QGridLayout(triggerToolBox_);
    triggerToolBox_->setLayout(layout);

    layout->addWidget(new QLabel("X", triggerToolBox_), 0, 1);
    layout->addWidget(new QLabel("Y", triggerToolBox_), 0, 2);
    layout->addWidget(new QLabel("Z", triggerToolBox_), 0, 3);
    layout->addWidget(new QLabel("A", triggerToolBox_), 0, 4);

    layout->addWidget(new QLabel("Trigger On", triggerToolBox_), 1, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "TriggerOn", triggerToolBox_), 1, 1);

    //FIX ME!
    layout->addWidget(new QLabel("Axis selection", triggerToolBox_), 2, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "TriggerAxis", 0, 3, triggerToolBox_), 2, 1);

    layout->addWidget(new QLabel("Mode", triggerToolBox_), 3, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "TriggerMode", 0, 99, triggerToolBox_), 3, 1);

    layout->addWidget(new QLabel("Signal length", triggerToolBox_), 4, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "TriggerSignal", 3, 120, triggerToolBox_), 4, 1);

    layout->addWidget(new QLabel("Distance", triggerToolBox_), 5, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "TriggerDistance", 0, 100000, triggerToolBox_), 5, 1);

    layout->addWidget(new QLabel("Count", triggerToolBox_), 6, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "TriggerAxis", triggerToolBox_), 6, 1);

    layout->addWidget(new QWidget(generalToolBox_), 7, 0);
    layout->setRowStretch(7, 100);
}

void LStepExpressSettingsWidget::fillExtraToolBox()
{
    QGridLayout *layout = new QGridLayout(extraToolBox_);
    extraToolBox_->setLayout(layout);

    layout->addWidget(new QLabel("X", extraToolBox_), 0, 1);
    layout->addWidget(new QLabel("Y", extraToolBox_), 0, 2);
    layout->addWidget(new QLabel("Z", extraToolBox_), 0, 3);
    layout->addWidget(new QLabel("A", extraToolBox_), 0, 4);

    layout->addWidget(new QLabel("Jog Mode", extraToolBox_), 1, 0);
    //    layout->addWidget(new QLabel("Jog Mode On", triggerToolBox_), 7, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "JogModeOn", extraToolBox_), 1, 1);

    layout->addWidget(new QLabel("Velocity", extraToolBox_), 2, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-JogModeVelocity", 0.0, 70.0, extraToolBox_), 2, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-JogModeVelocity", 0.0, 70.0, extraToolBox_), 2, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-JogModeVelocity", 0.0, 70.0, extraToolBox_), 2, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-JogModeVelocity", 0.0, 70.0, extraToolBox_), 2, 4);    

    layout->addWidget(new QLabel("Filter time", extraToolBox_), 3, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-JogModeFilterTime", 0.0, 500000.0, extraToolBox_), 3, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-JogModeFilterTime", 0.0, 500000.0, extraToolBox_), 3, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-JogModeFilterTime", 0.0, 500000.0, extraToolBox_), 3, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-JogModeFilterTime", 0.0, 500000.0, extraToolBox_), 3, 4);    

    layout->addWidget(new QLabel("Direction", extraToolBox_), 4, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-JogModeDirection", extraToolBox_), 4, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-JogModeDirection", extraToolBox_), 4, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-JogModeDirection", extraToolBox_), 4, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-JogModeDirection", extraToolBox_), 4, 4);    

    layout->addWidget(new QLabel("Axis Enabled", extraToolBox_), 5, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-JogModeAxesEnabled", extraToolBox_), 5, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-JogModeAxesEnabled", extraToolBox_), 5, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-JogModeAxesEnabled", extraToolBox_), 5, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-JogModeAxesEnabled", extraToolBox_), 5, 4);    

    layout->addWidget(new QLabel("Current Reduction", extraToolBox_), 6, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-JogModeCurrentReduction", extraToolBox_), 6, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-JogModeCurrentReduction", extraToolBox_), 6, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-JogModeCurrentReduction", extraToolBox_), 6, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-JogModeCurrentReduction", extraToolBox_), 6, 4);    

    layout->addWidget(new QLabel("Trackball Mode", extraToolBox_), 7, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "TrackBallOn", extraToolBox_), 7, 1);

    layout->addWidget(new QLabel("Velocity", extraToolBox_), 8, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-TrackBallVelocity", 0.0, 70.0, extraToolBox_), 8, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-TrackBallVelocity", 0.0, 70.0, extraToolBox_), 8, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-TrackBallVelocity", 0.0, 70.0, extraToolBox_), 8, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-TrackBallVelocity", 0.0, 70.0, extraToolBox_), 8, 4);    

    layout->addWidget(new QLabel("Filter time", extraToolBox_), 9, 0);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "X-TrackBallFilterTime", 0.0, 500000.0, extraToolBox_), 9, 1);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Y-TrackBallFilterTime", 0.0, 500000.0, extraToolBox_), 9, 2);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "Z-TrackBallFilterTime", 0.0, 500000.0, extraToolBox_), 9, 3);
    layout->addWidget(new LStepExpressSettingsDoubleSpinBox(settings_, "A-TrackBallFilterTime", 0.0, 500000.0, extraToolBox_), 9, 4);    

    layout->addWidget(new QLabel("Direction", extraToolBox_), 10, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-TrackBallDirection", extraToolBox_), 10, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-TrackBallDirection", extraToolBox_), 10, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-TrackBallDirection", extraToolBox_), 10, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-TrackBallDirection", extraToolBox_), 10, 4);    

    layout->addWidget(new QLabel("Axis Enabled", extraToolBox_), 11, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-TrackBallAxesEnabled", extraToolBox_), 11, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-TrackBallAxesEnabled", extraToolBox_), 11, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-TrackBallAxesEnabled", extraToolBox_), 11, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-TrackBallAxesEnabled", extraToolBox_), 11, 4);    

    layout->addWidget(new QLabel("Current Reduction", extraToolBox_), 12, 0);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "X-TrackBallCurrentReduction", extraToolBox_), 12, 1);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Y-TrackBallCurrentReduction", extraToolBox_), 12, 2);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "Z-TrackBallCurrentReduction", extraToolBox_), 12, 3);
    layout->addWidget(new LStepExpressSettingsCheckBox(settings_, "A-TrackBallCurrentReduction", extraToolBox_), 12, 4);    

    layout->addWidget(new QLabel("Axis Assignment", extraToolBox_), 13, 0);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "X-TrackBallAxisAssignment", 0, 4, extraToolBox_), 13, 1);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Y-TrackBallAxisAssignment", 0, 4, extraToolBox_), 13, 2);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "Z-TrackBallAxisAssignment", 0, 4, extraToolBox_), 13, 3);
    layout->addWidget(new LStepExpressSettingsIntSpinBox(settings_, "A-TrackBallAxisAssignment", 0, 4, extraToolBox_), 13, 4);    

    layout->addWidget(new QWidget(generalToolBox_), 14, 0);
    layout->setRowStretch(14, 100);
}

void LStepExpressSettingsWidget::controlStateChanged(bool enabled)
{
  //    NQLog("LStepExpressSettingsWidget ", NQLog::Spam) << "controlStateChanged(bool enabled) " << enabled    ;

    if (enabled) {
        mainToolBox_->setEnabled(settings_->getDeviceState());
        readFromDeviceButton_->setEnabled(settings_->getDeviceState());
        //readFromFileButton_->setEnabled(settings_->getDeviceState());
        applyButton_->setEnabled(settings_->getDeviceState());
        resetButton_->setEnabled(settings_->getDeviceState());                                                                                                                                              
        writeToDeviceButton_->setEnabled(settings_->getDeviceState());                                                                                                                                       
        //writeToFileButton_->setEnabled(settings_->getDeviceState());
    } else {
        mainToolBox_->setEnabled(false);
        readFromDeviceButton_->setEnabled(false);
        applyButton_->setEnabled(false);
        resetButton_->setEnabled(false);
        writeToDeviceButton_->setEnabled(false);
    }
}

void LStepExpressSettingsWidget::readFromDeviceClicked()
{
  //    NQLog("LStepExpressSettingsWidget ", NQLog::Spam) << "readFromDeviceClicked()"    ;

    settings_->readSettingsFromDevice();
}

void LStepExpressSettingsWidget::readFromFileClicked()
{
  //    NQLog("LStepExpressSettingsWidget ", NQLog::Spam) << "readFromFileClicked()"    ;

    QString filename = QFileDialog::getOpenFileName(this, "Read Settings", "~/", "*.lstep");
    if (filename.isNull()) return;

    settings_->readSettingsFromFile(filename);
}

void LStepExpressSettingsWidget::applyClicked()
{
  //    NQLog("LStepExpressSettingsWidget ", NQLog::Spam) << "applyClicked()"    ;

    settings_->writeSettingsToDevice();

}

void LStepExpressSettingsWidget::resetClicked()
{
  //    NQLog("LStepExpressSettingsWidget ", NQLog::Spam) << "resetClicked()"    ;
    
    settings_->resetSettings();
}

void LStepExpressSettingsWidget::writeToDeviceClicked()
{
  //    NQLog("LStepExpressSettingsWidget ", NQLog::Spam) << "writeToDeviceClicked()"    ;

    settings_->writeSettingsToDevice();

    //    NQLog("LStepExpressSettingsWidget ", NQLog::Spam) << "ask to save to device"    ;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Test", "Are you sure you want to save the configuration on the controller? (this will make the configuration the default at startup)",
		          QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      //  NQLog("LStepExpressSettingsWidget ", NQLog::Spam) << "Yes was clicked"    ;
        settings_->saveSettingsOnDevice();
    } else {
      //NQLog("LStepExpressSettingsWidget ", NQLog::Spam) << "Yes was *not* clicked"    ;
    }
}

void LStepExpressSettingsWidget::writeToFileClicked()
{
  //    NQLog("LStepExpressSettingsWidget ", NQLog::Spam) << "writeToFileClicked()"    ;

    QString filename = QFileDialog::getSaveFileName(this, "Write Settings", "~/", "*.lstep");
    if (filename.isNull()) return;

    settings_->writeSettingsToFile(filename);
}
