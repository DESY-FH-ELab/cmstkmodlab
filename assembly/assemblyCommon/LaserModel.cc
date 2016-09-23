#include <QApplication>

#include <nqlogger.h>

#include "LaserModel.h"

LaserModel::LaserModel(const char* port,
	           QObject * /*parent*/)
    : QObject(),
      AbstractDeviceModel<Keyence_t>(),
      Laser_PORT(port)
{
    laserHead_ = 2; //note: head A = 2

    timer_ = new QTimer(this);
    timer_->setInterval(100);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));
    value_ = 0;
    isInRange_ = true;
}

LaserModel::~LaserModel()
{
  if(timer_) {delete timer_; timer_ = NULL;}
}

void LaserModel::setLaserHead(int out)
{
  //    NQLog("LaserModel ", NQLog::Debug) << "[LaserModel::setLaserHead]";
    laserHead_ = out;
}

void LaserModel::setSamplingRate(int mode)
{
    if(state_ == OFF) return;

    //    NQLog("LaserModel ", NQLog::Debug) << "[LaserModel::setSamplingRate]"    ;
    controller_->SetSamplingRate(mode);
}

void LaserModel::setAveraging(int mode)
{
    if(state_ == OFF) return;

    //    NQLog("LaserModel ", NQLog::Debug) << "[setAveraging]"    ;
    controller_->SetAveraging(laserHead_, mode);
}

void LaserModel::setMaterialMode(int mode)
{
    if(state_ == OFF) return;
    controller_->SetMaterialMode(laserHead_, mode);
}

void LaserModel::setDiffuseMode(int mode)
{
    if(state_ == OFF) return;
    controller_->SetDiffuseMode(laserHead_, mode);
}

void LaserModel::updateInformation()
{
    double value = 0;
    getMeasurement(value);
}

void LaserModel::getMeasurement(double& value)
{
    if(state_ == OFF) return;

    //    NQLog("LaserModel ", NQLog::Debug) << "[getMeasurement], isInRange_ = "<< isInRange_    ;
    QMutexLocker locker(&mutex_);
    controller_->MeasurementValueOutput(laserHead_, value);
    bool inRange = (value != 9999 && value != -9999);
    if(!isInRange_ && inRange){
      isInRange_ = true;
      emit inRangeStateChanged(isInRange_);
    }
    if(isInRange_ && !inRange){
      isInRange_ = false;
      emit inRangeStateChanged(isInRange_);
    }

    if(value != value_){
      value_ = value;
      emit measurementChanged(value_);
    }
}

//dummy method for testing
void LaserModel::setMeasurement(double value)
{
  //    NQLog("LaserModel ", NQLog::Debug) << "[setMeasurement]"    ;

    emit measurementChanged(value);
}

void LaserModel::initialize()
{
  //    NQLog("LaserModel ", NQLog::Debug) << "[initialize]"    ;
    setDeviceState(INITIALIZING);

    renewController(Laser_PORT);

    bool enabled = (controller_ != NULL) && (controller_->DeviceAvailable());

    if ( enabled ) {
        setLaserHead(2);
        setAveraging(0);
        setSamplingRate(0);
        setDeviceState(READY);
    }
    else {
        setDeviceState( OFF );
        delete controller_;
        controller_ = NULL;
    }
}

void LaserModel::setDeviceState( State state )
{
  //    NQLog("LaserModel ", NQLog::Debug) << "[setDeviceState]"    ;
    if ( state_ != state ) {
        state_ = state;

        if ( state_ == READY ) {
            timer_->start();
        } else {
            timer_->stop();
        }

        emit deviceStateChanged(state);
    }
}

void LaserModel::setDeviceEnabled(bool enabled)
{
  //    NQLog("LaserModel ", NQLog::Debug) << "setDeviceEnabled(bool enabled)"    ;

    usleep(1000);

    AbstractDeviceModel<Keyence_t>::setDeviceEnabled(enabled);

}

