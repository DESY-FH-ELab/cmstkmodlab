#include <QApplication>

#include <nqlogger.h>

#include "LaserModel.h"

LaserModel::LaserModel(const char* port,
	           QObject * /*parent*/)
    : QObject(),
      AbstractDeviceModel<Keyence_t>(),
      Laser_PORT(port)
{
    laserHead_ = 1;
}

void LaserModel::setLaserHead(int out)
{
    laserHead_ = out;
}

void LaserModel::setSamplingRate(int mode)
{
    controller_->SetSamplingRate(mode);
}

void LaserModel::setAveraging(int mode)
{
    controller_->SetAveraging(laserHead_, mode);
}

void LaserModel::getMeasurement(double value)
{
    controller_->MeasurementValueOutput(laserHead_, value);
    emit measurementChanged(value);
}

//dummy method for testing
void LaserModel::setMeasurement(double value)
{
    NQLog("LaserModel") << "in set measurement";

    emit measurementChanged(value);
}

void LaserModel::initialize()
{
    setDeviceState(INITIALIZING);

    renewController(Laser_PORT);

    bool enabled = (controller_ != NULL) && (controller_->DeviceAvailable());

    if ( enabled ) {
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
    if ( state_ != state ) {
        state_ = state;

        //        if ( state_ == READY ) {
        //    timer_->start();
        //} else {
        //    timer_->stop();
        //}

        emit deviceStateChanged(state);
    }
}

