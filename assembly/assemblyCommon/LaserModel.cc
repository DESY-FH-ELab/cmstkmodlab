#include <QApplication>

#include <nqlogger.h>

#include "LaserModel.h"

LaserModel::LaserModel(const char* port,
	           QObject * /*parent*/)
    : QObject(),
      AbstractDeviceModel<Keyence_t>(),
      Laser_PORT(port)
{
    std::cout<<"LaserModel " << "[LaserModel::LaserModel]";
    laserHead_ = 2; //note: head A = 2

    timer_ = new QTimer(this);
    timer_->setInterval(100);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));
    value_ = 0;
    isInRange_ = true;
    //std::cout<<"[LaserModel::LaserModel] set device enabled"<< std::endl; AbstractDeviceModel<Keyence_t>::setDeviceEnabled(1);
    //    std::cout<<"address of controller = "<<&controller_<<std::endl;
}

LaserModel::~LaserModel()
{
  if(timer_) {delete timer_; timer_ = NULL;}
}

void LaserModel::setLaserHead(int out)
{
    std::cout<<"LaserModel " << "[LaserModel::setLaserHead]";
    laserHead_ = out;
}

void LaserModel::setSamplingRate(int mode)
{
    if(state_ == OFF) return;

    std::cout<<"LaserModel " << "[LaserModel::setSamplingRate]"    <<std::endl;
    controller_->SetSamplingRate(mode);
}

void LaserModel::setAveraging(int mode)
{
    if(state_ == OFF) return;

    std::cout<<"LaserModel " << "[setAveraging]"    <<std::endl;
    controller_->SetAveraging(laserHead_, mode);
}

void LaserModel::updateInformation()
{
    getMeasurement();
}

void LaserModel::getMeasurement()
{
    if(state_ == OFF) return;

    std::cout<<"LaserModel " << "[getMeasurement], isInRange_ = "<< isInRange_    <<std::endl;
    double ivalue = 0;
    try{
      std::cout<<"LaserModel " << "[getMeasurement] try to get measurement"    <<std::endl;
        controller_->MeasurementValueOutput(laserHead_, ivalue);
        std::cout<<"LaserModel " << "[getMeasurement] does the code reach this point?"    <<std::endl;
        if(!isInRange_){
	  std::cout<<"LaserModel " <<"[getMeasurement] emit goes back into range"    <<std::endl;
	isInRange_ = true;
	emit inRangeStateChanged(isInRange_);
        }
    }catch (std::string ){
      std::cout<<"LaserModel " << "[getMeasurement] exception caught"    <<std::endl;
        if(isInRange_){ 
	  std::cout<<"LaserModel " <<"[getMeasurement] emit goes out of range"    <<std::endl;
	isInRange_ = false;
	emit inRangeStateChanged(isInRange_);
        }
    }
    std::cout<<"LaserModel " << "[getMeasurement] value = " << ivalue    <<std::endl;
    if(ivalue != value_){
        value_ = ivalue;
        emit measurementChanged(value_);
    }
}

//dummy method for testing
void LaserModel::setMeasurement(double value)
{
    std::cout<<"LaserModel " << "[setMeasurement]"    <<std::endl;

    emit measurementChanged(value);
}

void LaserModel::initialize()
{
    std::cout<<"LaserModel " << "[initialize]"    <<std::endl;
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
    std::cout<<"LaserModel " << "[setDeviceState]"    <<std::endl;
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

void LaserModel::setDeviceEnabled(bool enabled)
{
    std::cout<<"LaserModel " << "setDeviceEnabled(bool enabled)"    <<std::endl;

    usleep(1000);

    AbstractDeviceModel<Keyence_t>::setDeviceEnabled(enabled);

}

