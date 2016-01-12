#include <QApplication>

#include <nqlogger.h>

#include "LStepExpressModel.h"

LStepExpressModel::LStepExpressModel(const char* port,
                                     int updateInterval,
                                     int motionUpdateInterval,
                                     QObject * /*parent*/)
    : QObject(),
      AbstractDeviceModel<LStepExpress_t>(),
      LStepExpress_PORT(port),
      updateInterval_(updateInterval),
      motionUpdateInterval_(motionUpdateInterval)
{
    std::vector<int> allZerosI{ 0, 0, 0, 0 };
    std::vector<double> allZerosD{ 0.0, 0.0, 0.0, 0.0 };

    axis_ = allZerosI;
    axisDirection_ = allZerosI;
    dim_ = allZerosI;
    pa_ = allZerosI;
    joystickEnabled_ = -1;
    joystickAxisEnabled_ = std::vector<int>{ -1, -1, -1, -1 };

    axisStatus_ = std::vector<int>{ -1, -1, -1, -1 };
    position_ = allZerosD;

    inMotion_ = false;

    timer_ = new QTimer(this);
    timer_->setInterval(updateInterval_);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));

    motionTimer_ = new QTimer(this);
    motionTimer_->setInterval(motionUpdateInterval);
    connect(motionTimer_, SIGNAL(timeout()), this, SLOT(updateMotionInformation()));
}

QString LStepExpressModel::getAxisName(unsigned int axis)
{
    QMutexLocker locker(&mutex_);

    QString temp(controller_->GetAxisName((VLStepExpress::Axis)axis));
    return temp;
}

QString LStepExpressModel::getAxisStatusText(unsigned int axis)
{
    QMutexLocker locker(&mutex_);

    QString temp(controller_->GetAxisStatusText((VLStepExpress::AxisStatus)axisStatus_[axis]));
    return temp;
}

bool LStepExpressModel::getAxisState(unsigned int axis)
{
    QMutexLocker locker(&mutex_);

    return (axisStatus_[axis]!=VLStepExpress::AXISDISABLED);
}

bool LStepExpressModel::getAxisEnabled(unsigned int axis)
{
    QMutexLocker locker(&mutex_);

    return axis_[axis];
}

double LStepExpressModel::getPosition(unsigned int axis)
{
    QMutexLocker locker(&mutex_);

    return position_[axis];
}

void LStepExpressModel::moveRelative(std::vector<double> & values)
{
    controller_->MoveRelative(values);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveRelative(double x, double y, double z, double a)
{
    controller_->MoveRelative(x, y, z, a);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveRelative(unsigned int axis, double value)
{
    controller_->MoveRelative((VLStepExpress::Axis)axis, value);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(std::vector<double> & values)
{
    controller_->MoveAbsolute(values);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(double x, double y, double z, double a)
{
    controller_->MoveAbsolute(x, y, z, a);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(unsigned int axis, double value)
{
    controller_->MoveAbsolute((VLStepExpress::Axis)axis, value);
    inMotion_ = true;
    emit motionStarted();
}

bool LStepExpressModel::getJoystickEnabled()
{
    QMutexLocker locker(&mutex_);

    return (joystickEnabled_==1);
}

bool LStepExpressModel::getJoystickAxisEnabled(unsigned int axis)
{
    QMutexLocker locker(&mutex_);

    return (joystickAxisEnabled_[axis]==1);
}

void LStepExpressModel::setAxisEnabled(unsigned int axis, bool enabled)
{
    int temp = (int)enabled;

    NQLog("LStepExpressModel", NQLog::Debug) << "setAxisEnabled " << axis << " " << temp;

    if (axis_[axis]!=temp) {
        controller_->SetAxisEnabled((VLStepExpress::Axis)axis, temp);
        axis_[axis] = temp;
        emit deviceStateChanged(state_);
    }
}

void LStepExpressModel::setJoystickEnabled(bool enabled)
{
    int temp = (int)enabled;

    if (joystickEnabled_!=temp) {
        controller_->SetJoystickEnabled(enabled);
        joystickEnabled_ = temp;
        emit deviceStateChanged(state_);
    }
}

void LStepExpressModel::setJoystickAxisEnabled(unsigned int axis, bool enabled)
{
    int temp = (enabled == true)?1:0;

    if (joystickEnabled_ && joystickAxisEnabled_[axis]!=temp) {
        joystickAxisEnabled_[axis] = temp;
        NQLog("LStepExpressModel", NQLog::Debug) << "setJoystickAxisEnabled " << axis << " " << temp;
        controller_->SetJoystickAxisEnabled((VLStepExpress::Axis)axis, temp);   
    }
}

void LStepExpressModel::initialize()
{
    setDeviceState(INITIALIZING);

    renewController(LStepExpress_PORT);

    bool enabled = (controller_ != NULL) && (controller_->DeviceAvailable());

    if ( enabled ) {
        setDeviceState(READY);
        updateInformation();
        updateMotionInformation();
    }
    else {
        setDeviceState( OFF );
        delete controller_;
        controller_ = NULL;
    }
}

void LStepExpressModel::setDeviceState( State state )
{
    if ( state_ != state ) {
        state_ = state;

        if ( state_ == READY ) {
            timer_->start();
            motionTimer_->start();
        } else {
            timer_->stop();
            motionTimer_->stop();
        }

        emit deviceStateChanged(state);
    }
}

void LStepExpressModel::updateInformation()
{
    if ( state_ == READY ) {

        QMutexLocker locker(&mutex_);

        NQLog("LStepExpressModel", NQLog::Debug) << "updateInformation()";

        if (thread()==QApplication::instance()->thread()) {
            NQLog("LStepExpressModel", NQLog::Debug) << " running in main application thread";
        } else {
            NQLog("LStepExpressModel", NQLog::Debug) << " running in dedicated thread";
        }

        bool changed = false;

        std::vector<int> ivalues;

        controller_->GetAxisEnabled(ivalues);
        if (ivalues!=axis_) {
            axis_ = ivalues;
            changed = true;
        }

        int joystick = controller_->GetJoystickEnabled();
        if (joystick!=joystickEnabled_) {
            joystickEnabled_ = joystick;
            changed = true;
        }

        controller_->GetJoystickAxisEnabled(ivalues);
        if (ivalues!=joystickAxisEnabled_) {
            joystickAxisEnabled_ = ivalues;
            changed = true;
        }

        if (changed) {
            NQLog("LStepExpressModel", NQLog::Spam) << "information changed";
            emit informationChanged();
        }
    }
}

void LStepExpressModel::updateMotionInformation()
{
    if ( state_ == READY ) {

        QMutexLocker locker(&mutex_);

        NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation()";

        if (thread()==QApplication::instance()->thread()) {
            NQLog("LStepExpressModel", NQLog::Debug) << " running in main application thread";
        } else {
            NQLog("LStepExpressModel", NQLog::Debug) << " running in dedicated thread";
        }

        bool changed = false;

        std::vector<int> ivalues;
        std::vector<double> dvalues;

        controller_->GetAxisStatus(ivalues);
        if (ivalues!=axisStatus_) {
            axisStatus_ = ivalues;
            changed = true;
        }
        if (inMotion_) {
            if (std::all_of(ivalues.begin(), ivalues.end(),
                            [](int i){return i==LStepExpress_t::AXISSTANDSANDREADY;})) {
                inMotion_ = false;
                emit motionFinished();
            }
        }

        controller_->GetPosition(dvalues);
        if (dvalues!=position_) {
            position_ = dvalues;
            changed = true;
        }

        if (changed) {
            NQLog("LStepExpressModel", NQLog::Spam) << "motion information changed";
            emit motionInformationChanged();
        }
    }
}

void LStepExpressModel::setDeviceEnabled(bool enabled)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "setDeviceEnabled(bool enabled)";

    if (state_ == READY && !enabled) {
        //std::vector<int> allZeros{ 0, 0, 0, 0 };
        //controller_->SetPowerAmplifierStatus(allZeros);
        //controller_->SetAxisEnabled(allZeros);
    }

    AbstractDeviceModel<LStepExpress_t>::setDeviceEnabled(enabled);
}

void LStepExpressModel::setControlsEnabled(bool enabled)
{
    emit controlStateChanged(enabled);
}
