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
    NQLog("LaserModel") << "[LaserModel::setLaserHead]";
    laserHead_ = out;
}

void LaserModel::setSamplingRate(int mode)
{
    if(state_ == OFF) return;

    NQLog("LaserModel") << "[LaserModel::setSamplingRate]";
    controller_->SetSamplingRate(mode);
}

void LaserModel::setAveraging(int mode)
{
    if(state_ == OFF) return;

    NQLog("LaserModel") << "[setAveraging]";
    controller_->SetAveraging(laserHead_, mode);
}

void LaserModel::updateInformation()
{
    getMeasurement();
}

void LaserModel::getMeasurement()
{
    if(state_ == OFF) return;

    NQLog("LaserModel") << "[getMeasurement], isInRange_ = "<< isInRange_;
    double ivalue = 0;
    try{
      NQLog("LaserModel") << "[getMeasurement] try to get measurement";
        controller_->MeasurementValueOutput(laserHead_, ivalue);
        NQLog("LaserModel") << "[getMeasurement] does the code reach this point?";
        if(!isInRange_){
	  NQLog("LaserModel") <<"[getMeasurement] emit goes back into range";
	isInRange_ = true;
	emit inRangeStateChanged(isInRange_);
        }
    }catch (std::string ){
      NQLog("LaserModel") << "[getMeasurement] exception caught";
        if(isInRange_){ 
	  NQLog("LaserModel") <<"[getMeasurement] emit goes out of range";
	isInRange_ = false;
	emit inRangeStateChanged(isInRange_);
        }
    }
    NQLog("LaserModel") << "[getMeasurement] value = " << ivalue;
    if(ivalue != value_){
        value_ = ivalue;
        emit measurementChanged(value_);
    }
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

void LaserModel::setDeviceEnabled(bool enabled)
{
    NQLog("LaserModel", NQLog::Debug) << "setDeviceEnabled(bool enabled)";

    usleep(1000);

    AbstractDeviceModel<Keyence_t>::setDeviceEnabled(enabled);

    std::cout<<"after"<<std::endl;
}

