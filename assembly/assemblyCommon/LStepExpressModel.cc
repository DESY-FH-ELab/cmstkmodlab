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

    axisStatus_ = std::vector<int>{ 0x0b, 0x0b, 0x0b, 0x0b };
    position_ = allZerosD;

    inMotion_ = false;
    isUpdating_ = false;
    isPaused_ = false;
    finishedCalibrating_ = false;

    timer_ = new QTimer(this);
    timer_->setInterval(motionUpdateInterval_);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateMotionInformationFromTimer()));
    //    connect(this, SIGNAL(informationChanged()), this, SLOT(updateInformation()));
    
    spyTimer = new QSignalSpy(timer_, SIGNAL(timeout()));
    spyDeviceStateChanged = new QSignalSpy(this, SIGNAL(deviceStateChanged(State)));
    spyInformationChanged = new QSignalSpy(this, SIGNAL(informationChanged()));
    spyMotionInformationChanged = new QSignalSpy(this, SIGNAL(motionInformationChanged()));
    spyMessage = new QSignalSpy(this, SIGNAL(message(QString)));
    spyControlStateChanged = new QSignalSpy(this, SIGNAL(controlStateChanged(bool)));
    spyMotionStarted = new QSignalSpy(this, SIGNAL(motionStarted()));
    spyMotionFinished = new QSignalSpy(this, SIGNAL(motionFinished()));

    /*
    connect(timer_, SIGNAL(timeout()), this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(deviceStateChanged(State)), this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(informationChanged()), this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(motionInformationChanged()), this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(message(QString)), this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(controlStateChanged(bool)), this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(motionStarted()), this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(motionFinished()), this, SLOT(printSpyInformation()));
    */
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
    for(int i = 0; i < spyTimer->size(); i++){
        NQLog("SPY LStepExpressModel ", NQLog::Debug)<< "timer_, signal timeout() ";
    }
    spyTimer->clear();
    for(int i = 0; i < spyDeviceStateChanged->size(); i++){
        NQLog("SPY LStepExpressModel ", NQLog::Debug)<< "this_, signal deviceStateChanged( "<<(spyDeviceStateChanged->value(i))[0].toString().toStdString()<<") ";
    }
    spyDeviceStateChanged->clear();
    for(int i = 0; i < spyInformationChanged->size(); i++){
        NQLog("SPY LStepExpressModel ", NQLog::Debug)<< "this_, signal informationChanged() ";
    }
    spyInformationChanged->clear();
    for(int i = 0; i < spyMotionInformationChanged->size(); i++){
        NQLog("SPY LStepExpressModel ", NQLog::Debug)<< "this_, signal motionInformationChanged()"  ;
    }
    spyMotionInformationChanged->clear();
    for(int i = 0; i < spyMessage->size(); i++){
        NQLog("SPY LStepExpressModel ", NQLog::Debug)<< "this_, signal message()"  ;
    }
    spyControlStateChanged->clear();
    for(int i = 0; i < spyMessage->size(); i++){
        NQLog("SPY LStepExpressModel ", NQLog::Debug)<< "this_, signal controlStateChanged( "<<(spyControlStateChanged->value(i))[0].toBool()<<")"  ;
    }
    spyMessage->clear();
    for(int i = 0; i < spyMotionStarted->size(); i++){
        NQLog("SPY LStepExpressModel ", NQLog::Debug)<< "this_, signal motionStarted()"  ;
    }
    spyMotionStarted->clear();
    for(int i = 0; i < spyMotionFinished->size(); i++){
        NQLog("SPY LStepExpressModel ", NQLog::Debug)<< "this_, signal motionFinished()"  ;
    }
    spyMotionFinished->clear();
}

void LStepExpressModel::getStatus(bool& status)
{
  QMutexLocker locker(&mutex_);
    status = controller_->GetStatus();
}

void LStepExpressModel::getError(int& error)
{
  QMutexLocker locker(&mutex_);
    error = controller_->GetError();
}

void LStepExpressModel::getSystemStatus(std::string& value)
{
  QMutexLocker locker(&mutex_);
    controller_->GetSystemStatusText(value);
}

void LStepExpressModel::pauseUpdate()
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "pauseUpdate()"  ;
    QMutexLocker locker(&mutex_);
    isPaused_ = true;
    setControlsEnabled(false);
}

void LStepExpressModel::continueUpdate()
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "continueUpdate()"  ;
    QMutexLocker locker(&mutex_);
    isPaused_ = false;
    setControlsEnabled(true);
}

QString LStepExpressModel::getAxisName(unsigned int axis)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "getAxisName(axis)"  ;
    QString temp(controller_->GetAxisName((VLStepExpress::Axis)axis));
    return temp;
}

QString LStepExpressModel::getAxisDimensionShortName(unsigned int axis)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "getAxisDimensionShortName(axis)"  ;
    QString temp(controller_->GetAxisDimensionShortName((VLStepExpress::Dimension)dim_[axis]));
    return temp;
}

QString LStepExpressModel::getAxisStatusText(unsigned int axis)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<<"getAxisStatusText(axis)"  ;
    QString temp(controller_->GetAxisStatusText((VLStepExpress::AxisStatus)axisStatus_[axis]));
    NQLog("LStepExpressModel ", NQLog::Debug)<<"getAxisStatusText(axis) =  "<<temp.toStdString();
    return temp;
}

bool LStepExpressModel::getAxisState(unsigned int axis)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "getAxisState(axis) 1"  ;
    QMutexLocker locker(&mutex_);
    std::vector<int> ivalues;
    controller_->GetAxisStatus(ivalues);
    axisStatus_[axis] = (ivalues)[axis];
    NQLog("LStepExpressModel ", NQLog::Debug)<< "getAxisState(axis) 2; axis =  "<<axisStatus_[axis]  ;
    return (axisStatus_[axis]!=VLStepExpress::AXISDISABLED);
}

bool LStepExpressModel::getAxisEnabled(unsigned int axis)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "getAxisEnabled(axis), axis nr =  "<<axis<< " enabled =  "<<axis_[axis]  ;

    return axis_[axis];
}

double LStepExpressModel::getPosition(unsigned int axis)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "getPosition(axis)"  ;

    return position_[axis];
}

void LStepExpressModel::moveRelative(std::vector<double> & values)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "moveRelative() v1 x = "<<(values)[0]<<" y = "<<(values)[1]<<" z = "<<(values)[2]<<" a = "<<(values)[3]  ;
  controller_->MoveRelative(LStepExpress_t::X, (values)[0]);
  controller_->MoveRelative(LStepExpress_t::Y, (values)[1]);
  controller_->MoveRelative(LStepExpress_t::Z, (values)[2]);
  controller_->MoveRelative(LStepExpress_t::A, (values)[3]);

    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveRelative(double x, double y, double z, double a)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "moveRelative() v2 x = "<<x<<" y = "<<y<<" z = "<<z<<" a = "<<a  ;
  controller_->MoveRelative(LStepExpress_t::X, x);
  controller_->MoveRelative(LStepExpress_t::Y, y);
  controller_->MoveRelative(LStepExpress_t::Z, z);
  controller_->MoveRelative(LStepExpress_t::A, a);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveRelative(unsigned int axis, double value)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "moveRelative v3"  ;
    controller_->MoveRelative((VLStepExpress::Axis)axis, value);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(std::vector<double> & values)
{
    NQLog("LStepExpresModel ", NQLog::Debug)<< "moveAbsolute v1"  ;
    controller_->MoveAbsolute(LStepExpress_t::X, (values)[0]);
    controller_->MoveAbsolute(LStepExpress_t::Y, (values)[1]);
    controller_->MoveAbsolute(LStepExpress_t::Z, (values)[2]);
    controller_->MoveAbsolute(LStepExpress_t::A, (values)[3]);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(double x, double y, double z, double a)
{
    NQLog("LStepExpresModel ", NQLog::Debug)<< "moveAbsolute v2, go to " << x << " x,  "<< y <<" y,  "<< z <<" z,  "<< a <<" a"  ;
    controller_->MoveAbsolute(LStepExpress_t::X, x);
    controller_->MoveAbsolute(LStepExpress_t::Y, y);
    controller_->MoveAbsolute(LStepExpress_t::Z, z);
    controller_->MoveAbsolute(LStepExpress_t::A, a);    
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(unsigned int axis, double value)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "moveAbsolute v3"  ;
    controller_->MoveAbsolute((VLStepExpress::Axis)axis, value);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::calibrate()
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "calibrate: go to calibrate"  ;
    controller_->Calibrate();
    inMotion_ = true;
    finishedCalibrating_ = true;
    NQLog("LStepExpressModel ", NQLog::Debug)<< "calibrate: emit motionStarted"  ;
    emit motionStarted();
}

void LStepExpressModel::emergencyStop()
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "emergencyStop"  ;
    controller_->EmergencyStop();
    inMotion_ = false;
    finishedCalibrating_ = false;
    emit motionFinished();
}

bool LStepExpressModel::getJoystickEnabled()
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "getJoystickEnabled:  "<<joystickEnabled_  ;

    return (joystickEnabled_==1);
}

bool LStepExpressModel::getJoystickAxisEnabled(unsigned int axis)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "getJoystickAxisEnabled:  "<<joystickAxisEnabled_[axis]  ;

    return (joystickAxisEnabled_[axis]==1);
}

void LStepExpressModel::setAxisEnabled(unsigned int axis, bool enabled)
{
    int temp = (int)enabled;
    NQLog("LStepExpressModel ", NQLog::Debug)<< "setAxisEnabled " << axis << " " << temp  ;

    int temp2;
    controller_->GetAxisEnabled((VLStepExpress::Axis)axis, temp2);
    axis_[axis] = temp2;

    if (axis_[axis]!=temp) {
        controller_->SetPowerAmplifierStatus((VLStepExpress::Axis)axis, temp);
        controller_->SetAxisEnabled((VLStepExpress::Axis)axis, temp);
        axis_[axis] = temp;
        updateInformation();
        emit informationChanged();
    }
}

void LStepExpressModel::setJoystickEnabled(bool enabled)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "setJoystickEnabled  "<<enabled  ;

    int temp = (int)enabled;

    if (joystickEnabled_!=temp) {
      std::vector<int> ivalues = {0,0,0,0};
      if(temp == 0){controller_->SetJoystickAxisEnabled(ivalues); joystickAxisEnabled_ = ivalues;}
      controller_->SetJoystickEnabled(temp);
      int temp2 = (int)controller_->GetJoystickEnabled();
      joystickEnabled_ = temp;
      if(temp2 == 1){controller_->SetJoystickAxisEnabled(ivalues); joystickAxisEnabled_ = ivalues;}
      updateInformation();
      emit informationChanged();
    }
}

void LStepExpressModel::setJoystickAxisEnabled(unsigned int axis, bool enabled)
{
    int temp = (int)enabled;
    NQLog("LStepExpressModel ", NQLog::Debug)<< "setJoystickAxisEnabled " << axis << " " << temp << " joystick enabled? =  "<<joystickEnabled_  ;

    if (joystickEnabled_ && joystickAxisEnabled_[axis]!=temp) {
      controller_->SetJoystickAxisEnabled((VLStepExpress::Axis)axis, temp);
      joystickAxisEnabled_[axis] = temp;
    }
}

void LStepExpressModel::setValue(const QString & command, const QString & value)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "setValue"  ;
    std::string temp;
    controller_->SetValue(command.toStdString(), value.toStdString());
}

void LStepExpressModel::getValue(const QString & command, QString & value)
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "getValue"  ;
    std::string temp;
    controller_->GetValue(command.toStdString(), temp);
    value = temp.c_str();
}

void LStepExpressModel::validConfig()
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "validConfig"  ;
    controller_->ValidConfig();
}

void LStepExpressModel::validParameter()
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "validParameter"  ;
    controller_->ValidParameter();
}

void LStepExpressModel::saveConfig()
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "save config"  ;
    controller_->SaveConfig();
}

void LStepExpressModel::reset()
{
    NQLog("LStepExpressModel ", NQLog::Debug)<< "reset to start-up conditions"  ;
    controller_->Reset();
}

void LStepExpressModel::initialize()
{
    setDeviceState(INITIALIZING);

    renewController(LStepExpress_PORT);

    bool enabled = (controller_ != NULL) && (controller_->DeviceAvailable());

    if ( enabled ) {
        controller_->SetAutoStatus(2);
      std::vector<int> allZerosI{ 0, 0, 0, 0 };
      std::vector<int> OnI{1,1,1,1};
      controller_->SetPowerAmplifierStatus(allZerosI);
      controller_->SetAxisEnabled(allZerosI);
      controller_->SetJoystickEnabled(false);
      QMutexLocker locker(&mutex_);
      axis_ = allZerosI;
      joystickEnabled_ = false;
      joystickAxisEnabled_ = allZerosI;
      setDeviceState(READY);
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
        } else {
            timer_->stop();
        }

        emit deviceStateChanged(state);
    }
}

void LStepExpressModel::updateInformation()
{
    /*
      if (thread()==QApplication::instance()->thread()) {
      NQLog("LStepExpressModel "<< " running in main application thread";
      } else {
      NQLog("LStepExpressModel "<< " running in dedicated thread";
      }
    */

    bool changed = false;

    std::vector<int> ivalues;
    std::vector<double> dvalues;

    controller_->GetAxisEnabled(ivalues);
    if (ivalues!=axis_) {
        axis_ = ivalues;
        changed = true;
    }

    controller_->GetDimension(ivalues);
    if (ivalues!=dim_) {
        dim_ = ivalues;
        changed = true;
    }

    controller_->GetPosition(dvalues);
    if (dvalues!=position_) {
      position_ = dvalues;
      changed = true;
    }

    int joystick = controller_->GetJoystickEnabled();
    int joystick_model = this->getJoystickEnabled();
    if (joystick!=joystickEnabled_) {
        joystickEnabled_ = joystick;
        changed = true;
    }

    if(joystickEnabled_){
      controller_->GetJoystickAxisEnabled(ivalues);
      if (ivalues!=joystickAxisEnabled_) {
        joystickAxisEnabled_ = ivalues;
        changed = true;
      }
    }
    if (changed) {
        emit informationChanged();
    }
}

void LStepExpressModel::updateMotionInformation()
{
    static const int nUpdates = updateInterval_/motionUpdateInterval_;

    if ( state_ == READY && !isPaused_) {

      isUpdating_ = true;

      updateCount_++;
      if (updateCount_==nUpdates) {
	updateInformation();
	updateCount_ = 0;
      }

      /*
        if (thread()==QApplication::instance()->thread()) {
        NQLog("LStepExpressModel "<< " running in main application thread";
        } else {
        NQLog("LStepExpressModel "<< " running in dedicated thread";
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

      if (ivalues!=axisStatus_) {
	axisStatus_ = ivalues;
	changed = true;
      }

      if (inMotion_) {
          bool temp = true;
          for(int i = 0; i < 4; i++){
	    bool ifaxisenabled = ( (ivalues)[i] == LStepExpress_t::AXISSTANDSANDREADY || (ivalues)[i] == LStepExpress_t::AXISACKAFTERCALIBRATION) && (axis_)[i] == 1;
	    bool ifaxisnotenabled = (axis_)[i] == 0;
	    temp = ifaxisenabled || ifaxisnotenabled;
	    NQLog("LStepExpressModel", NQLog::Debug) <<" axis status =  "<<(ivalues)[i]<<" axis enabled = "<<(axis_)[i]<<" temp = "<<temp;
          }
          if(temp){inMotion_ = false; emit motionFinished();}
          
          /*
	if (std::all_of(ivalues.begin(), ivalues.end(),
	[](int i){	  NQLog("LStepExpressModel ", NQLog::Debug)<< "updateMotionInformation() axis status =  "<<i; bool temp = (i==LStepExpress_t::AXISSTANDSANDREADY || i==LStepExpress_t::AXISACKAFTERCALIBRATION) && (axis_)[i]==1; return temp;})) {
	NQLog("LStepExpressModel ", NQLog::Debug)<< "updateMotionInformation() finished moving inMotion_ = false";
	inMotion_ = false;
	emit motionFinished();
	}*/
      }
      
      if( (axis_)[0] || (axis_)[1] || (axis_)[2] || (axis_)[3]){
          controller_->GetPosition(dvalues);
          if (dvalues!=position_) {
	  NQLog("LStepExpressModel ", NQLog::Debug)<< "updateMotionInformation() new position values"  ;
	  position_ = dvalues;
	  changed = true;
          }
      }
      
      NQLog("LStepExpressModel ", NQLog::Debug)<< "updateMotionInformation() finishedCalibrating_ =  "<<finishedCalibrating_<<" in motion =  "<<inMotion_  ;
      if(!inMotion_ && finishedCalibrating_){
          NQLog("LStepExpressModel ", NQLog::Debug)<< "updateMotionInformation() after calibration, setting position values to zero"  ;
          std::vector<double> posvalues{0.0, 0.0, 0.0, 0.0};
          controller_->SetPosition(posvalues);
          position_ = posvalues;
          changed = true;
          finishedCalibrating_ = false;
      }
      
      if (changed) {
          NQLog("LStepExpressModel ", NQLog::Debug)<< "updateMotionInformation() motion information changed"  ;
          emit motionInformationChanged();
      }
      
      isUpdating_ = false;
    }
}

void LStepExpressModel::updateMotionInformationFromTimer()
{
    static const int nUpdates = updateInterval_/motionUpdateInterval_;
    
    if ( state_ == READY && !isPaused_) {
      
      isUpdating_ = true;
      
      updateCount_++;
      if (updateCount_==nUpdates) {
          updateInformation();
          updateCount_ = 0;
      }
      
      /*
        if (thread()==QApplication::instance()->thread()) {
        NQLog("LStepExpressModel "<< " running in main application thread";
        } else {
        NQLog("LStepExpressModel "<< " running in dedicated thread";
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
      
      if (ivalues!=axisStatus_) {
          axisStatus_ = ivalues;
          changed = true;
      }
      
      if (inMotion_) {
          bool temp = true;
          for(int i = 0; i < 4; i++){
	    bool ifaxisenabled = ( (ivalues)[i] == LStepExpress_t::AXISSTANDSANDREADY || (ivalues)[i] == LStepExpress_t::AXISACKAFTERCALIBRATION) && (axis_)[i] == 1;
	    bool ifaxisnotenabled = (axis_)[i] == 0;
	    temp *= (ifaxisenabled || ifaxisnotenabled);
          }
	  if(temp){inMotion_ = false; emit motionFinished();}
      }
      
      if( (axis_)[0] || (axis_)[1] || (axis_)[2] || (axis_)[3]){
          controller_->GetPosition(dvalues);
          if (dvalues!=position_) {
	  position_ = dvalues;
	  changed = true;
          }
      }
      
      if(!inMotion_ && finishedCalibrating_){
          std::vector<double> posvalues{0.0, 0.0, 0.0, 0.0};
          controller_->SetPosition(posvalues);
          position_ = posvalues;
          changed = true;
          finishedCalibrating_ = false;
      }
      
      if (changed) {
          NQLog("LStepExpressModel ", NQLog::Debug)<< "updateMotionInformationFromTimer motion information changed"  ;
          emit motionInformationChanged();
      }
      
      isUpdating_ = false;
    }
}

void LStepExpressModel::setDeviceEnabled(bool enabled)
{
    if (state_ == READY && !enabled) {
        std::vector<int> allZeros{ 0, 0, 0, 0 };
        controller_->SetPowerAmplifierStatus(allZeros);
        controller_->SetAxisEnabled(allZeros);
    }

    AbstractDeviceModel<LStepExpress_t>::setDeviceEnabled(enabled);
}

void LStepExpressModel::setControlsEnabled(bool enabled)
{
    emit controlStateChanged(enabled);
}
