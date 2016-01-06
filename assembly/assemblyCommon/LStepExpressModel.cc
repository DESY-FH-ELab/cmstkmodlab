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

    axisStatus_ = allZerosI;
    position_ = allZerosD;

    timer_ = new QTimer(this);
    timer_->setInterval(updateInterval_);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));

    motionTimer_ = new QTimer(this);
    motionTimer_->setInterval(motionUpdateInterval);
    connect(motionTimer_, SIGNAL(timeout()), this, SLOT(updateMotionInformation()));
}

QString LStepExpressModel::getAxisName(unsigned int axis)
{
    QString temp(controller_->GetAxisName((VLStepExpress::Axis)axis));
    return temp;
}

bool LStepExpressModel::getAxisState(unsigned int axis)
{
    return (axisStatus_[axis]!=VLStepExpress::AXISDISABLED);
}

bool LStepExpressModel::getAxisEnabled(unsigned int axis)
{
    return axis_[axis];
}

double LStepExpressModel::getPosition(unsigned int axis)
{
    return position_[axis];
}

void LStepExpressModel::moveRelative(std::vector<double> & values)
{
    controller_->MoveRelative(values);
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

        // NQLog("LStepExpressModel", NQLog::Debug) << "updateInformation()";

        /*
    if (thread()==QApplication::instance()->thread()) {
      NQLog("LStepExpressModel", NQLog::Debug) << " running in main application thread";
    } else {
      NQLog("LStepExpressModel", NQLog::Debug) << " running in dedicated thread";
    }
*/

        bool changed = false;

        std::vector<int> ivalues;

        controller_->GetAxisEnabled(ivalues);
        if (ivalues!=axis_) {
            axis_ = ivalues;
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

        // NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation()";

        /*
    if (thread()==QApplication::instance()->thread()) {
      NQLog("LStepExpressModel", NQLog::Debug) << " running in main application thread";
    } else {
      NQLog("LStepExpressModel", NQLog::Debug) << " running in dedicated thread";
    }
*/

        bool changed = false;

        std::vector<int> ivalues;
        std::vector<double> dvalues;

        controller_->GetAxisStatus(ivalues);
        if (ivalues!=axisStatus_) {
            axisStatus_ = ivalues;
            changed = true;
        }

        controller_->GetPosition(dvalues);
        if (dvalues!=position_) {
            position_ = dvalues;
            changed = true;
        }

        if (changed) {
            // NQLog("LStepExpressModel", NQLog::Spam) << "motion information changed";
            emit motionInformationChanged();
        }
    }
}

void LStepExpressModel::setDeviceEnabled(bool enabled)
{
    // NQLog("LStepExpressModel", NQLog::Debug) << "setDeviceEnabled(bool enabled)";

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
