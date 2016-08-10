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
      motionUpdateInterval_(motionUpdateInterval),
      updateCount_(0)
{
    std::vector<int> allZerosI{ 0, 0, 0, 0 };
    std::vector<double> allZerosD{ 0.0, 0.0, 0.0, 0.0 };

    axis_ = allZerosI;
    axisDirection_ = allZerosI;
    dim_ = allZerosI;
    pa_ = allZerosI;
    joystickEnabled_ = 0;
    joystickAxisEnabled_ = std::vector<int>{ 0, 0, 0, 0 };

    //axisStatus_ = std::vector<int>{ -1, -1, -1, -1 };
    axisStatus_ = std::vector<int>{ 0x0b, 0x0b, 0x0b, 0x0b }; //Kelly
    position_ = allZerosD;

    inMotion_ = false;
    isUpdating_ = false;
    isPaused_ = false;
    finishedCalibrating_ = false;

    timer_ = new QTimer(this);
    //timer_->setInterval(motionUpdateInterval_);
    timer_->setInterval(1000);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateMotionInformationFromTimer()));
    //    connect(this, SIGNAL(informationChanged()), this, SLOT(updateInformation()));
    /*
      spyTimer_ = new QTimer(this);
      spyTimer_->setInterval(500);
      spyTimer_->start();
      connect(spyTimer_, SIGNAL(timeout()), this, SLOT(printSpyInformation()));
    */
    spyTimer = new QSignalSpy(timer_, SIGNAL(timeout()));
    spyDeviceStateChanged = new QSignalSpy(this, SIGNAL(deviceStateChanged(State)));
    spyInformationChanged = new QSignalSpy(this, SIGNAL(informationChanged()));
    spyMotionInformationChanged = new QSignalSpy(this, SIGNAL(motionInformationChanged()));
    spyMessage = new QSignalSpy(this, SIGNAL(message(QString)));
    spyControlStateChanged = new QSignalSpy(this, SIGNAL(controlStateChanged(bool)));
    spyMotionStarted = new QSignalSpy(this, SIGNAL(motionStarted()));
    spyMotionFinished = new QSignalSpy(this, SIGNAL(motionFinished()));
    connect(timer_, SIGNAL(timeout()), this, SLOT(printSpyInformation()));

    connect(this, SIGNAL(deviceStateChanged(State)), this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(informationChanged()), this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(motionInformationChanged()), this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(message(QString)), this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(controlStateChanged(bool)), this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(motionStarted()), this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(motionFinished()), this, SLOT(printSpyInformation()));
}

LStepExpressModel::~LStepExpressModel()
{
    if(timer_){delete timer_; timer_ = NULL;}
    if(spyTimer){delete spyTimer; spyTimer = NULL;}
    if(spyDeviceStateChanged){delete spyDeviceStateChanged; spyDeviceStateChanged = NULL;}
    if(spyInformationChanged){delete spyInformationChanged; spyInformationChanged = NULL;}
    if(spyMotionInformationChanged){delete spyMotionInformationChanged; spyMotionInformationChanged = NULL;}
    if(spyMessage){delete spyMessage; spyMessage = NULL;}
    if(spyControlStateChanged){delete spyControlStateChanged; spyControlStateChanged = NULL;}
    if(spyMotionStarted){delete spyMotionStarted; spyMotionStarted = NULL;}
    if(spyMotionFinished){delete spyMotionFinished; spyMotionFinished = NULL;}
}

void LStepExpressModel::printSpyInformation()
{

    //std::cout<<"LStepExpressModel "<< "printSpyInformation";
    for(int i = 0; i < spyTimer->size(); i++){
        std::cout<<"SPY LStepExpressModel "<< "timer_, signal timeout() "<<std::endl;
    }
    spyTimer->clear();
    for(int i = 0; i < spyDeviceStateChanged->size(); i++){
        std::cout<<"SPY LStepExpressModel "<< "this_, signal deviceStateChanged( "<<(spyDeviceStateChanged->value(i))[0].toString().toStdString()<<") "<<std::endl;
    }
    spyDeviceStateChanged->clear();
    for(int i = 0; i < spyInformationChanged->size(); i++){
      std::cout<<"SPY LStepExpressModel "<< "this_, signal informationChanged() "<<std::endl;
    }
    spyInformationChanged->clear();
    for(int i = 0; i < spyMotionInformationChanged->size(); i++){
        std::cout<<"SPY LStepExpressModel "<< "this_, signal motionInformationChanged()"  <<std::endl;
    }
    spyMotionInformationChanged->clear();
    for(int i = 0; i < spyMessage->size(); i++){
        std::cout<<"SPY LStepExpressModel "<< "this_, signal message()"  <<std::endl;
    }
    spyControlStateChanged->clear();
    for(int i = 0; i < spyMessage->size(); i++){
        std::cout<<"SPY LStepExpressModel "<< "this_, signal controlStateChanged( "<<(spyControlStateChanged->value(i))[0].toBool()<<")"  <<std::endl;
    }
    spyMessage->clear();
    for(int i = 0; i < spyMotionStarted->size(); i++){
        std::cout<<"SPY LStepExpressModel "<< "this_, signal motionStarted()"  <<std::endl;
    }
    spyMotionStarted->clear();
    for(int i = 0; i < spyMotionFinished->size(); i++){
        std::cout<<"SPY LStepExpressModel "<< "this_, signal motionFinished()"  <<std::endl;
    }
    spyMotionFinished->clear();
}

void LStepExpressModel::pauseUpdate()
{
    std::cout<<"LStepExpressModel "<< "pauseUpdate()"  <<std::endl;
    QMutexLocker locker(&mutex_);
    isPaused_ = true;
    setControlsEnabled(false);
}

void LStepExpressModel::continueUpdate()
{
    std::cout<<"LStepExpressModel "<< "continueUpdate()"  <<std::endl;
    QMutexLocker locker(&mutex_);
    isPaused_ = false;
    setControlsEnabled(true);
}

QString LStepExpressModel::getAxisName(unsigned int axis)
{
    std::cout<<"LStepExpressModel "<< "getAxisName(axis)"  <<std::endl;
    //QMutexLocker locker(&mutex_);
    QString temp(controller_->GetAxisName((VLStepExpress::Axis)axis));
    return temp;
}

QString LStepExpressModel::getAxisDimensionShortName(unsigned int axis)
{
    std::cout<<"LStepExpressModel "<< "getAxisDimensionShortName(axis)"  <<std::endl;
    //QMutexLocker locker(&mutex_);
    QString temp(controller_->GetAxisDimensionShortName((VLStepExpress::Dimension)dim_[axis]));
    return temp;
}

QString LStepExpressModel::getAxisStatusText(unsigned int axis)
{
    std::cout<<"LStepExpressModel "<<"getAxisStatusText(axis)"  <<std::endl;
    //QMutexLocker locker(&mutex_);
    QString temp(controller_->GetAxisStatusText((VLStepExpress::AxisStatus)axisStatus_[axis]));
    std::cout<<"LStepExpressModel "<<"getAxisStatusText(axis) =  "<<temp.toStdString()<<std::endl;
    return temp;
}

bool LStepExpressModel::getAxisState(unsigned int axis)
{
    std::cout<<"LStepExpressModel "<< "getAxisState(axis) 1"  <<std::endl;
    QMutexLocker locker(&mutex_);
    std::vector<int> ivalues;
    controller_->GetAxisStatus(ivalues);
    axisStatus_[axis] = (ivalues)[axis];
    std::cout<<"LStepExpressModel "<< "getAxisState(axis) 2; axis =  "<<axisStatus_[axis]  <<std::endl;
    return (axisStatus_[axis]!=VLStepExpress::AXISDISABLED);
}

bool LStepExpressModel::getAxisEnabled(unsigned int axis)
{
    std::cout<<"LStepExpressModel "<< "getAxisEnabled(axis), axis nr =  "<<axis<< " enabled =  "<<axis_[axis]  <<std::endl;
    //QMutexLocker locker(&mutex_);
    //int temp;
    //controller_->GetAxisEnabled((VLStepExpress::Axis)axis, temp);
    //axis_[axis] = temp;

    return axis_[axis];
}

double LStepExpressModel::getPosition(unsigned int axis)
{
    std::cout<<"LStepExpressModel "<< "getPosition(axis)"  <<std::endl;
    //QMutexLocker locker(&mutex_);

    return position_[axis];
}

void LStepExpressModel::moveRelative(std::vector<double> & values)
{
    std::cout<<"LStepExpressModel "<< "moveRelative() v1"  <<std::endl;
    controller_->MoveRelative(values);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveRelative(double x, double y, double z, double a)
{
    std::cout<<"LStepExpressModel "<< "moveRelative() v2"  <<std::endl;
    controller_->MoveRelative(x, y, z, a);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveRelative(unsigned int axis, double value)
{
    std::cout<<"LStepExpressModel "<< "moveRelative v3"  <<std::endl;
    controller_->MoveRelative((VLStepExpress::Axis)axis, value);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(std::vector<double> & values)
{
    std::cout<<"LStepExpresModel "<< "moveAbsolute v1"  <<std::endl;
    controller_->MoveAbsolute(values);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(double x, double y, double z, double a)
{
    std::cout<<"LStepExpresModel "<< "moveAbsolute v2, go to " << x << " x,  "<< y <<" y,  "<< z <<" z,  "<< a <<" a"  <<std::endl;
    controller_->MoveAbsolute(x, y, z, a);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(unsigned int axis, double value)
{
    std::cout<<"LStepExpressModel "<< "moveAbsolute v3"  <<std::endl;
    controller_->MoveAbsolute((VLStepExpress::Axis)axis, value);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::calibrate()
{
    std::cout<<"LStepExpressModel "<< "calibrate: go to calibrate"  <<std::endl;
    controller_->Calibrate();
    inMotion_ = true;
    finishedCalibrating_ = true;
    std::cout<<"LStepExpressModel "<< "calibrate: emit motionStarted"  <<std::endl;
    emit motionStarted();

    /*
    std::cout<<"LStepExpressModel "<< "calibrate: set position to zero";
    //set positions to all zeros
    bool changed = false;
    std::vector<double> dvalues{0.0, 0.0, 0.0, 0.0};
    controller_->SetPosition(dvalues);
    if (dvalues!=position_) {
        position_ = dvalues;
        changed = true;
    }
    if (changed) {
        // std::cout<<"LStepExpressModel " << "information changed";
        emit informationChanged();
    }
    */
}

void LStepExpressModel::emergencyStop()
{
    std::cout<<"LStepExpressModel "<< "emergencyStop"  <<std::endl;
    controller_->EmergencyStop();
    inMotion_ = false;
    finishedCalibrating_ = false;
    emit motionFinished();
}

bool LStepExpressModel::getJoystickEnabled()
{
    std::cout<<"LStepExpressModel "<< "getJoystickEnabled:  "<<joystickEnabled_  <<std::endl;
    //QMutexLocker locker(&mutex_);

    return (joystickEnabled_==1);
}

bool LStepExpressModel::getJoystickAxisEnabled(unsigned int axis)
{
    std::cout<<"LStepExpressModel "<< "getJoystickAxisEnabled:  "<<joystickAxisEnabled_[axis]  <<std::endl;
  //QMutexLocker locker(&mutex_);

    return (joystickAxisEnabled_[axis]==1);
}

void LStepExpressModel::setAxisEnabled(unsigned int axis, bool enabled)
{
    int temp = (int)enabled;
    std::cout<<"LStepExpressModel "<< "setAxisEnabled " << axis << " " << temp  <<std::endl;

    int temp2;
    //QMutexLocker locker(&mutex_);
    controller_->GetAxisEnabled((VLStepExpress::Axis)axis, temp2);
    axis_[axis] = temp2;

    if (axis_[axis]!=temp) {
        controller_->SetPowerAmplifierStatus((VLStepExpress::Axis)axis, temp);
        controller_->SetAxisEnabled((VLStepExpress::Axis)axis, temp);
        axis_[axis] = temp;
        //emit deviceStateChanged(state_);
        updateInformation();
        emit informationChanged();
    }
}

void LStepExpressModel::setJoystickEnabled(bool enabled)
{
    std::cout<<"LStepExpressModel "<< "setJoystickEnabled  "<<enabled  <<std::endl;

    int temp = (int)enabled;

    if (joystickEnabled_!=temp) {
      std::vector<int> ivalues = {0,0,0,0};
      if(temp == 0){controller_->SetJoystickAxisEnabled(ivalues); joystickAxisEnabled_ = ivalues;}
      controller_->SetJoystickEnabled(temp);
      int temp2 = (int)controller_->GetJoystickEnabled();
      if(temp2 != temp){std::cout<<"ERROR!!!! problem seting joystick "<<std::endl;}
      joystickEnabled_ = temp;
      if(temp2 == 1){controller_->SetJoystickAxisEnabled(ivalues); joystickAxisEnabled_ = ivalues;}
      //emit deviceStateChanged(state_);
      updateInformation();
      emit informationChanged();
    }
}

void LStepExpressModel::setJoystickAxisEnabled(unsigned int axis, bool enabled)
{
    //int temp = (enabled == true)?1:0;
    int temp = (int)enabled;
    std::cout<<"LStepExpressModel "<< "setJoystickAxisEnabled " << axis << " " << temp << " joystick enabled? =  "<<joystickEnabled_  <<std::endl;

    if (joystickEnabled_ && joystickAxisEnabled_[axis]!=temp) {
      std::cout<<"LStepExpressModel "<< "setJoystickAxisEnabled do something"  <<std::endl;
      controller_->SetJoystickAxisEnabled((VLStepExpress::Axis)axis, temp);
      joystickAxisEnabled_[axis] = temp;
    }
}

void LStepExpressModel::setValue(const QString & command, const QString & value)
{
    std::cout<<"LStepExpressModel "<< "saveValue"  <<std::endl;
    std::string temp;
    controller_->SetValue(command.toStdString(), value.toStdString());
}

void LStepExpressModel::getValue(const QString & command, QString & value)
{
    std::cout<<"LStepExpressModel "<< "getValue"  <<std::endl;
    std::string temp;
    controller_->GetValue(command.toStdString(), temp);
    value = temp.c_str();
}

void LStepExpressModel::validConfig()
{
    std::cout<<"LStepExpressModel "<< "validConfig"  <<std::endl;
    controller_->ValidConfig();
}

void LStepExpressModel::validParameter()
{
    std::cout<<"LStepExpressModel "<< "validParameter"  <<std::endl;
    controller_->ValidParameter();
}

void LStepExpressModel::saveConfig()
{
    std::cout<<"LStepExpressModel "<< "save config"  <<std::endl;
    controller_->SaveConfig();
}

void LStepExpressModel::reset()
{
    std::cout<<"LStepExpressModel "<< "reset to start-up conditions"  <<std::endl;
    controller_->Reset();
}

void LStepExpressModel::initialize()
{
    std::cout<<"LStepExpressModel in initialize "<<std::endl;

    setDeviceState(INITIALIZING);

    renewController(LStepExpress_PORT);

    bool enabled = (controller_ != NULL) && (controller_->DeviceAvailable());

    if ( enabled ) {
      std::cout<<"enabling "<<std::endl;
      std::vector<int> allZerosI{ 0, 0, 0, 0 };
      controller_->SetPowerAmplifierStatus(allZerosI);
      //std::vector<int> ivalues;
      //controller_->GetPowerAmplifierStatus(ivalues);
      //std::cout<<"LStepExpressModel "<< "initialize, pa status =  "<<(ivalues)[0]<<"  "<<(ivalues)[1]<<"  "<<(ivalues)[2]<<"  "<<(ivalues)[3];
      controller_->SetAxisEnabled(allZerosI);

      //std::cout<<"LStepExpressModel "<< "initialize, 1 model_ axes enabled =  "<<this->getAxisEnabled(0)<<"  "<<this->getAxisEnabled(1)<<"  "<<getAxisEnabled(2)<<"  "<<getAxisEnabled(3);
      //this->setAxisEnabled(0,0); this->setAxisEnabled(1,0); this->setAxisEnabled(2,0); this->setAxisEnabled(3,0);
      //controller_->GetAxisEnabled(ivalues);
      //std::cout<<"LStepExpressModel "<< "initialize, controller_ axes enabled =  "<<(ivalues)[0]<<"  "<<(ivalues)[1]<<"  "<<(ivalues)[2]<<"  "<<(ivalues)[3];
      //std::cout<<"LStepExpressModel "<< "initialize, 2 model_ axes enabled =  "<<this->getAxisEnabled(0)<<"  "<<this->getAxisEnabled(1)<<"  "<<getAxisEnabled(2)<<"  "<<getAxisEnabled(3);

      //controller_->GetAxisStatus(ivalues);
      //std::cout<<"LStepExpressModel "<< "initialize, controller_ axes status =  "<<(ivalues)[0]<<"  "<<(ivalues)[1]<<"  "<<(ivalues)[2]<<"  "<<(ivalues)[3];
      controller_->SetJoystickEnabled(false);
      std::cout<<"in init, after disabling joystick, status on controller is  "<<controller_->GetJoystickEnabled()<<std::endl;
      //this->setJoystickEnabled(false);
      //std::cout<<"LStepExpressModel "<< "initialize, joystick enabled =  "<<controller_->GetJoystickEnabled();
      //controller_->SetJoystickAxisEnabled(allZerosI);
      //this->setJoystickAxisEnabled(0,0);       this->setJoystickAxisEnabled(1,0);       this->setJoystickAxisEnabled(2,0);       this->setJoystickAxisEnabled(3,0);
      //controller_->SetJoystickAxisEnabled(allZerosI);
      //controller_->GetJoystickAxisEnabled(ivalues);
      //std::cout<<"LStepExpressModel "<< "initialize, joystick axes enabled =  "<<(ivalues)[0]<<"  "<<(ivalues)[1]<<"  "<<(ivalues)[2]<<"  "<<(ivalues)[3];
      QMutexLocker locker(&mutex_);
      axis_ = allZerosI;
      joystickEnabled_ = false;
      joystickAxisEnabled_ = allZerosI;
      std::cout<<"init set device state ready "<<std::endl;
      setDeviceState(READY);
      //updateInformation();
      //updateMotionInformation();
    }
    else {
        setDeviceState( OFF );
        delete controller_;
        controller_ = NULL;
    }

    std::cout<<"finished initializing "<<std::endl;
}

void LStepExpressModel::setDeviceState( State state )
{
    std::cout<<"set device state =  "<<state<<" present state =  "<<state_<<std::endl;
    if ( state_ != state ) {
        state_ = state;

        if ( state_ == READY ) {
            timer_->start();
        } else {
            timer_->stop();
        }

        std::cout<<"emit devicestateChanged "<<std::endl;
        emit deviceStateChanged(state);
    }
}

void LStepExpressModel::updateInformation()
{
    std::cout<<"LStepExpressModel "<< "updateInformation()"  <<std::endl;

    /*


        if (thread()==QApplication::instance()->thread()) {
            std::cout<<"LStepExpressModel "<< " running in main application thread";
        } else {
            std::cout<<"LStepExpressModel "<< " running in dedicated thread";
        }
        */

    bool changed = false;

    std::vector<int> ivalues;
    std::vector<double> dvalues;

    std::cout<<"LStepExpressModel "<< "updateInformation() get axis enabled"  <<std::endl;
    controller_->GetAxisEnabled(ivalues);
    if (ivalues!=axis_) {
      std::cout<<"LStepExpressModel "<< "updateInformation() axis enabled changed"  <<std::endl;
        axis_ = ivalues;
        changed = true;
    }

    std::cout<<"LStepExpressModel "<< "updateInformation() get dimension"  <<std::endl;
    controller_->GetDimension(ivalues);
    if (ivalues!=dim_) {
              std::cout<<"LStepExpressModel "<< "updateInformation() dimension changed"  <<std::endl;
        dim_ = ivalues;
        changed = true;
    }

    std::cout<<"LStepExpressModel "<< "updateInformation() get position "  <<std::endl;
    controller_->GetPosition(dvalues);
    if (dvalues!=position_) {
      std::cout<<"LStepExpressModel "<< "updateInformation() position changed"  <<std::endl;
      position_ = dvalues;
      changed = true;
    }

    int joystick = controller_->GetJoystickEnabled();
    int joystick_model = this->getJoystickEnabled();
    std::cout<<"LStepExpressModel "<< "updateInformation() joystick =  "<<joystick<<" model joystick =  "<<joystick_model  <<std::endl;
    if (joystick!=joystickEnabled_) {
        std::cout<<"LStepExpressModel "<< "updateInformation() joystick changed"<<std::endl;
        joystickEnabled_ = joystick;
        changed = true;
    }

    if(joystickEnabled_){
      controller_->GetJoystickAxisEnabled(ivalues);
      std::cout<<"LStepExpressModel "<< "updateInformation() axis joystick =  "<<(ivalues)[0]<<"  "<<(ivalues)[1]<<"  "<<(ivalues)[2]<<"  "<<(ivalues)[3]  <<std::endl;
      if (ivalues!=joystickAxisEnabled_) {
        std::cout<<"LStepExpressModel "<< "updateInformation() axis joystick changed"<<std::endl;
        joystickAxisEnabled_ = ivalues;
        changed = true;
      }
    }
    if (changed) {
        // std::cout<<"LStepExpressModel " << "information changed";
        emit informationChanged();
    }
}

void LStepExpressModel::updateMotionInformation()
{
    std::cout<<"LStepExpressModel "<< "updateMotionInformation(), state =  "<<state_<<" isPaused_ =  "<<isPaused_  <<std::endl;

    static const int nUpdates = updateInterval_/motionUpdateInterval_;

    if ( state_ == READY && !isPaused_) {

      std::cout<<"LStepExpressModel "<< "updateMotionInformation(), in if"  <<std::endl;

      //QMutexLocker locker(&mutex_);

      isUpdating_ = true;

      updateCount_++;
      if (updateCount_==nUpdates) {
	std::cout<<"LStepExpressModel "<< "updateMotionInformation(), going to updateInformation()"  <<std::endl;
	updateInformation();
	updateCount_ = 0;
      }

      /*
        if (thread()==QApplication::instance()->thread()) {
        std::cout<<"LStepExpressModel "<< " running in main application thread";
        } else {
        std::cout<<"LStepExpressModel "<< " running in dedicated thread";
        }
      */

      bool changed = false;

      std::vector<int> ivalues;
      std::vector<int> pavalues;
      std::vector<int> evalues;
      std::vector<double> dvalues;

      controller_->GetAxisStatus(ivalues);
      controller_->GetPowerAmplifierStatus(pavalues);
      controller_->GetAxisEnabled(evalues);

      std::cout<<"LStepExpressModel "<< "updateMotionInformation() received axis status"  <<std::endl;
      std::cout<<"LStepExpressModel "<< " x-axis: new status =  "<<(ivalues)[0]<<" old status =  "<<(axisStatus_)[0]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " y-axis: new status =  "<<(ivalues)[1]<<" old status =  "<<(axisStatus_)[1]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " z-axis: new status =  "<<(ivalues)[2]<<" old status =  "<<(axisStatus_)[2]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " a-axis: new status =  "<<(ivalues)[3]<<" old status =  "<<(axisStatus_)[3]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " x-axis: old enabled =  "<<(axis_)[0]<<" new enabled =  "<<(evalues)[0]<<" pa =  "<<(pavalues)[0]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " y-axis: old enabled =  "<<(axis_)[1]<<" new enabled =  "<<(evalues)[1]<<" pa =  "<<(pavalues)[1]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " z-axis: old enabled =  "<<(axis_)[2]<<" new enabled =  "<<(evalues)[2]<<" pa =  "<<(pavalues)[2]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " a-axis: old enabled =  "<<(axis_)[3]<<" new enabled =  "<<(evalues)[3]<<" pa =  "<<(pavalues)[3]  <<std::endl;

      if (ivalues!=axisStatus_) {
	std::cout<<"LStepExpressModel "<< "updateMotionInformation() axis status changed"  <<std::endl;
	axisStatus_ = ivalues;
	changed = true;
      }

      if (inMotion_) {
          std::cout<<"LStepExpressModel "<< "updateMotionInformation() inMotion_ = true"  <<std::endl;
          
          bool temp = true;
          for(int i = 0; i < 4; i++){
	    bool ifaxisenabled = ( (ivalues)[i] == LStepExpress_t::AXISSTANDSANDREADY || (ivalues)[i] == LStepExpress_t::AXISACKAFTERCALIBRATION) && (axis_)[i] == 1;
	    bool ifaxisnotenabled = (axis_)[i] == 0;
	    temp = ifaxisenabled || ifaxisnotenabled;
	    std::cout<<"LStepExpressModel axis status =  "<<(ivalues)[i]<<" axis enabled = "<<(axis_)[i]<<" temp = "<<temp<<std::endl;
          }
	  //          if(temp){inMotion_ = false; std::cout<<"about to emit motionfinished()"<<std::endl; emit motionFinished();}
          
          /*
	if (std::all_of(ivalues.begin(), ivalues.end(),
	[](int i){	  std::cout<<"LStepExpressModel "<< "updateMotionInformation() axis status =  "<<i; bool temp = (i==LStepExpress_t::AXISSTANDSANDREADY || i==LStepExpress_t::AXISACKAFTERCALIBRATION) && (axis_)[i]==1; return temp;})) {
	std::cout<<"LStepExpressModel "<< "updateMotionInformation() finished moving inMotion_ = false";
	inMotion_ = false;
	emit motionFinished();
	}*/
      }
      
      if( (axis_)[0] || (axis_)[1] || (axis_)[2] || (axis_)[3]){
          controller_->GetPosition(dvalues);
          if (dvalues!=position_) {
	  std::cout<<"LStepExpressModel "<< "updateMotionInformation() new position values"  <<std::endl;
	  position_ = dvalues;
	  changed = true;
          }
      }
      
      std::cout<<"LStepExpressModel "<< "updateMotionInformation() finishedCalibrating_ =  "<<finishedCalibrating_<<" in motion =  "<<inMotion_  <<std::endl;
      if(!inMotion_ && finishedCalibrating_){
          std::cout<<"LStepExpressModel "<< "updateMotionInformation() after calibration, setting position values to zero"  <<std::endl;
          std::vector<double> posvalues{0.0, 0.0, 0.0, 0.0};
          controller_->SetPosition(posvalues);
          position_ = posvalues;
          changed = true;
          finishedCalibrating_ = false;
      }
      
      if (changed) {
          std::cout<<"LStepExpressModel "<< "updateMotionInformation() motion information changed"  <<std::endl;
          emit motionInformationChanged();
      }
      
      isUpdating_ = false;
    }
}

void LStepExpressModel::updateMotionInformationFromTimer()
{
    std::cout<<"LStepExpressModel "<< "updateMotionInformationFromTimer, state =  "<<state_<<" isPaused_ =  "<<isPaused_  <<std::endl;
    
    static const int nUpdates = updateInterval_/motionUpdateInterval_;
    
    if ( state_ == READY && !isPaused_) {
      
        std::cout<<"LStepExpressModel "<< "updateMotionInformationFromTimer, in if"  <<std::endl;
        
      //QMutexLocker locker(&mutex_);
        
      isUpdating_ = true;
      
      updateCount_++;
      if (updateCount_==nUpdates) {
          std::cout<<"LStepExpressModel "<< "updateMotionInformationFromTimer, going to updateInformation"  <<std::endl;
          updateInformation();
          updateCount_ = 0;
      }
      
      /*
        if (thread()==QApplication::instance()->thread()) {
        std::cout<<"LStepExpressModel "<< " running in main application thread";
        } else {
        std::cout<<"LStepExpressModel "<< " running in dedicated thread";
        }
      */
      
      bool changed = false;
      
      std::vector<int> ivalues;
      std::vector<int> pavalues;
      std::vector<int> evalues;
      std::vector<double> dvalues;
      
      controller_->GetAxisStatus(ivalues);
      controller_->GetPowerAmplifierStatus(pavalues);
      controller_->GetAxisEnabled(evalues);
      
      std::cout<<"LStepExpressModel "<< "updateMotionInformationFromTimer received axis status"  <<std::endl;
      std::cout<<"LStepExpressModel "<< " x-axis: new status =  "<<(ivalues)[0]<<" old status =  "<<(axisStatus_)[0]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " y-axis: new status =  "<<(ivalues)[1]<<" old status =  "<<(axisStatus_)[1]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " z-axis: new status =  "<<(ivalues)[2]<<" old status =  "<<(axisStatus_)[2]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " a-axis: new status =  "<<(ivalues)[3]<<" old status =  "<<(axisStatus_)[3]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " x-axis: old enabled =  "<<(axis_)[0]<<" new enabled =  "<<(evalues)[0]<<" pa =  "<<(pavalues)[0]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " y-axis: old enabled =  "<<(axis_)[1]<<" new enabled =  "<<(evalues)[1]<<" pa =  "<<(pavalues)[1]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " z-axis: old enabled =  "<<(axis_)[2]<<" new enabled =  "<<(evalues)[2]<<" pa =  "<<(pavalues)[2]  <<std::endl;
      std::cout<<"LStepExpressModel "<< " a-axis: old enabled =  "<<(axis_)[3]<<" new enabled =  "<<(evalues)[3]<<" pa =  "<<(pavalues)[3]  <<std::endl;
      
      if (ivalues!=axisStatus_) {
          std::cout<<"LStepExpressModel "<< "updateMotionInformationFromTimer axis status changed"  <<std::endl;
          axisStatus_ = ivalues;
          changed = true;
      }
      
      if (inMotion_) {
	std::cout<<"LStepExpressModel "<< "updateMotionInformationFromTimer inMotion_ = true, axisstandsandready = "<<LStepExpress_t::AXISSTANDSANDREADY<<" axisackaftercalibration = "<<LStepExpress_t::AXISACKAFTERCALIBRATION  <<std::endl;
          bool temp = true;
          for(int i = 0; i < 4; i++){
	    std::cout<<"LStepExpressModel "<< "updateMotionInformationFromTimer inMotion_ = true, axis nr = "<<i<<std::endl;
	    bool ifaxisenabled = ( (ivalues)[i] == LStepExpress_t::AXISSTANDSANDREADY || (ivalues)[i] == LStepExpress_t::AXISACKAFTERCALIBRATION) && (axis_)[i] == 1;
	    bool ifaxisnotenabled = (axis_)[i] == 0;
	    temp *= (ifaxisenabled || ifaxisnotenabled);
	    std::cout<<"LStepExpressModel updateMotionInformationFromTimer inMotion_ = true, axis status =  "<<(ivalues)[i]<<" axis enabled = "<<(axis_)[i]<<" ifaxisenabled = "<<ifaxisenabled<<" ifaxisnotenabled = "<<ifaxisnotenabled<<" temp = "<<temp<<std::endl;
          }
	  if(temp){inMotion_ = false; std::cout<<"about to emit motionfinished()"<<std::endl; emit motionFinished();}

          /*
	if (std::all_of(ivalues.begin(), ivalues.end(),
	[](int i){	  std::cout<<"LStepExpressModel "<< "updateMotionInformationFromTimer axis status =  "<<i; bool temp = (i==LStepExpress_t::AXISSTANDSANDREADY || i==LStepExpress_t::AXISACKAFTERCALIBRATION) && (axis_)[i]==1; return temp;})) {
	std::cout<<"LStepExpressModel "<< "updateMotionInformationFromTimer finished moving inMotion_ = false";
	inMotion_ = false;
	emit motionFinished();
	}*/
      }
      
      if( (axis_)[0] || (axis_)[1] || (axis_)[2] || (axis_)[3]){
          controller_->GetPosition(dvalues);
          if (dvalues!=position_) {
	  std::cout<<"LStepExpressModel "<< "updateMotionInformationFromTimer new position values"  <<std::endl;
	  position_ = dvalues;
	  changed = true;
          }
      }
      
      std::cout<<"LStepExpressModel "<< "updateMotionInformationFromTimer finishedCalibrating_ =  "<<finishedCalibrating_<<" in motion =  "<<inMotion_  <<std::endl;
      if(!inMotion_ && finishedCalibrating_){
          std::cout<<"LStepExpressModel "<< "updateMotionInformationFromTimer after calibration, setting position values to zero"  <<std::endl;
          std::vector<double> posvalues{0.0, 0.0, 0.0, 0.0};
          controller_->SetPosition(posvalues);
          position_ = posvalues;
          changed = true;
          finishedCalibrating_ = false;
      }
      
      if (changed) {
          std::cout<<"LStepExpressModel "<< "updateMotionInformationFromTimer motion information changed"  <<std::endl;
          emit motionInformationChanged();
      }
      
      isUpdating_ = false;
    }
}

void LStepExpressModel::setDeviceEnabled(bool enabled)
{
    std::cout<<"LStepExpressModel "<< "setDeviceEnabled(bool enabled), enabled =  "<<enabled  <<std::endl;
    
    if (state_ == READY && !enabled) {
        std::cout<<"LStepExpressModel "<< "setDeviceEnabled(bool enabled), turn off power amplifiers and axes"  <<std::endl;
        std::vector<int> allZeros{ 0, 0, 0, 0 };
        //std::vector<int> allOn{ 1, 1, 1, 1 };
        controller_->SetPowerAmplifierStatus(allZeros);
        controller_->SetAxisEnabled(allZeros);
    }

    AbstractDeviceModel<LStepExpress_t>::setDeviceEnabled(enabled);
}

void LStepExpressModel::setControlsEnabled(bool enabled)
{
    std::cout<<"LStepExpressModel "<< "setControlsEnabled(bool enabled), enabled =  "<<enabled  <<std::endl;
    emit controlStateChanged(enabled);
}
