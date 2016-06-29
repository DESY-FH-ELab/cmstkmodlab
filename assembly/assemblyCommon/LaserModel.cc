#include <QApplication>

#include <nqlogger.h>

#include "LaserModel.h"

LaserModel::LaserModel(const char* port,
	           QObject * /*parent*/)
    : QObject(),
      AbstractDeviceModel<Keyence_t>(),
      Laser_PORT(port)
{
    NQLog("LaserModel") << "[LaserModel::LaserModel]";
    laserHead_ = 2; //note: head A = 2
}

void LaserModel::setLaserHead(int out)
{
    NQLog("LaserModel") << "[LaserModel::setLaserHead]";
    laserHead_ = out;
}

void LaserModel::setSamplingRate(int mode)
{
    NQLog("LaserModel") << "[LaserModel::setSamplingRate]";
    controller_->SetSamplingRate(mode);
}

void LaserModel::setAveraging(int mode)
{
    NQLog("LaserModel") << "[setAveraging]";
    controller_->SetAveraging(laserHead_, mode);
}

void LaserModel::getMeasurement()
{
    NQLog("LaserModel") << "[getMeasurement]";
    double value = 0;
    controller_->MeasurementValueOutput(laserHead_, value);
    NQLog("LaserModel") << "[getMeasurement] value = " << value;
    emit measurementChanged(value);
}

//dummy method for testing
void LaserModel::setMeasurement(double value)
{
    NQLog("LaserModel") << "[setMeasurement]";

    emit measurementChanged(value);
}

void LaserModel::initialize()
{
    NQLog("LaserModel") << "[initialize]";
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
    NQLog("LaserModel") << "[setDeviceState]";
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

