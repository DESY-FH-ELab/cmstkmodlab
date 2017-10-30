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

#include <LStepExpressModel.h>
#include <nqlogger.h>

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
}

LStepExpressModel::~LStepExpressModel()
{
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
//    NQLog("LStepExpressModel ", NQLog::Spam)<< "pauseUpdate()"  ;
    QMutexLocker locker(&mutex_);
    isPaused_ = true;
    setControlsEnabled(false);
}

void LStepExpressModel::continueUpdate()
{
  //    NQLog("LStepExpressModel ", NQLog::Spam)<< "continueUpdate()"  ;
    QMutexLocker locker(&mutex_);
    isPaused_ = false;
    setControlsEnabled(true);
}

QString LStepExpressModel::getAxisName(unsigned int axis)
{
  //    NQLog("LStepExpressModel ", NQLog::Spam)<< "getAxisName(axis)"  ;
    QString temp(controller_->GetAxisName((VLStepExpress::Axis)axis));
    return  temp;
}

QString LStepExpressModel::getAxisDimensionShortName(unsigned int axis)
{
  //    NQLog("LStepExpressModel ", NQLog::Spam)<< "getAxisDimensionShortName(axis)"  ;
    QString temp(controller_->GetAxisDimensionShortName((VLStepExpress::Dimension)dim_[axis]));
    return  temp;
}

QString LStepExpressModel::getAxisStatusText(unsigned int axis)
{
//    NQLog("LStepExpressModel ", NQLog::Spam)<<"getAxisStatusText(axis)"  ;
    QString temp(controller_->GetAxisStatusText((VLStepExpress::AxisStatus)axisStatus_[axis]));
//    NQLog("LStepExpressModel ", NQLog::Spam)<<"getAxisStatusText(axis) =  "<<temp.toStdString();
    return  temp;
}

bool LStepExpressModel::getAxisState(unsigned int axis)
{
//    NQLog("LStepExpressModel ", NQLog::Spam)<< "getAxisState(axis) 1"  ;
    QMutexLocker locker(&mutex_);
    std::vector<int> ivalues;
    controller_->GetAxisStatus(ivalues);
    axisStatus_[axis] = (ivalues)[axis];
//    NQLog("LStepExpressModel ", NQLog::Spam)<< "getAxisState(axis) 2; axis =  "<<axisStatus_[axis]  ;
    return (axisStatus_[axis]!=VLStepExpress::AXISDISABLED);
}

bool LStepExpressModel::getAxisEnabled(unsigned int axis)
{
//    NQLog("LStepExpressModel ", NQLog::Spam)<< "getAxisEnabled(axis), axis nr =  "<<axis<< " enabled =  "<<axis_[axis]  ;

    return axis_[axis];
}

double LStepExpressModel::getPosition(unsigned int axis)
{
//    NQLog("LStepExpressModel ", NQLog::Spam)<< "getPosition(axis)"  ;

    return position_[axis];
}

void LStepExpressModel::moveRelative(std::vector<double> & values)
{
    this->moveRelative(
      values.at(0),
      values.at(1),
      values.at(2),
      values.at(3)
    );
}

void LStepExpressModel::moveRelative(double x, double y, double z, double a)
{
    NQLog("LStepExpressModel::moveRelative", NQLog::Spam)
         << "("  << "x=" << x
         << ", " << "y=" << y
         << ", " << "z=" << z
         << ", " << "a=" << a
         << ")";

    if(!controller_)
    {
      NQLog("LStepExpressModel::moveRelative", NQLog::Critical)
           << "null pointer to controller, no action taken";

      emit motionFinished();
    }
    else
    {
      controller_->MoveRelative(LStepExpress_t::X, x);
      controller_->MoveRelative(LStepExpress_t::Y, y);
      controller_->MoveRelative(LStepExpress_t::Z, z);
      controller_->MoveRelative(LStepExpress_t::A, a);

      inMotion_ = true;

      NQLog("LStepExpressModel::moveRelative", NQLog::Spam) << "emitting signal \"motionStarted\"";

      emit motionStarted();
    }
}

void LStepExpressModel::moveRelative(unsigned int axis, double value)
{
//    NQLog("LStepExpressModel ", NQLog::Spam)<< "moveRelative v3"  ;
    controller_->MoveRelative((VLStepExpress::Axis)axis, value);

    inMotion_ = true;

    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(std::vector<double> & values)
{
//    NQLog("LStepExpresModel ", NQLog::Spam)<< "moveAbsolute v1"  ;
    controller_->MoveAbsolute(LStepExpress_t::X, (values)[0]);
    controller_->MoveAbsolute(LStepExpress_t::Y, (values)[1]);
    controller_->MoveAbsolute(LStepExpress_t::Z, (values)[2]);
    controller_->MoveAbsolute(LStepExpress_t::A, (values)[3]);

    inMotion_ = true;

    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(double x, double y, double z, double a)
{
//    NQLog("LStepExpresModel ", NQLog::Spam)<< "moveAbsolute v2, go to " << x << " x,  "<< y <<" y,  "<< z <<" z,  "<< a <<" a"  ;
    controller_->MoveAbsolute(LStepExpress_t::X, x);
    controller_->MoveAbsolute(LStepExpress_t::Y, y);
    controller_->MoveAbsolute(LStepExpress_t::Z, z);
    controller_->MoveAbsolute(LStepExpress_t::A, a);    

    inMotion_ = true;

    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(unsigned int axis, double value)
{
//    NQLog("LStepExpressModel ", NQLog::Spam)<< "moveAbsolute v3"  ;

    controller_->MoveAbsolute((VLStepExpress::Axis)axis, value);

    inMotion_ = true;

    emit motionStarted();
}

void LStepExpressModel::calibrate()
{
//    NQLog("LStepExpressModel ", NQLog::Spam)<< "calibrate: go to calibrate"  ;

    controller_->Calibrate();

    inMotion_ = true;

    finishedCalibrating_ = true;

//    NQLog("LStepExpressModel ", NQLog::Spam)<< "calibrate: emit motionStarted"  ;

    emit motionStarted();
}

void LStepExpressModel::emergencyStop()
{
//    NQLog("LStepExpressModel ", NQLog::Spam)<< "emergencyStop"  ;
    controller_->EmergencyStop();
    inMotion_ = false;
    finishedCalibrating_ = false;
    emit emergencyStopSignal();
    emit motionFinished();
}

bool LStepExpressModel::getJoystickEnabled()
{
  //    NQLog("LStepExpressModel ", NQLog::Spam)<< "getJoystickEnabled:  "<<joystickEnabled_  ;

    return (joystickEnabled_==1);
}

bool LStepExpressModel::getJoystickAxisEnabled(unsigned int axis)
{
  //    NQLog("LStepExpressModel ", NQLog::Spam)<< "getJoystickAxisEnabled:  "<<joystickAxisEnabled_[axis]  ;

    return (joystickAxisEnabled_[axis]==1);
}

void LStepExpressModel::setAxisEnabled(unsigned int axis, bool enabled)
{
    int temp = (int)enabled;
    // NQLog("LStepExpressModel ", NQLog::Spam)<< "setAxisEnabled " << axis << " " << temp  ;

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
  //    NQLog("LStepExpressModel ", NQLog::Spam)<< "setJoystickEnabled  "<<enabled  ;

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
    //NQLog("LStepExpressModel ", NQLog::Spam)<< "setJoystickAxisEnabled " << axis << " " << temp << " joystick enabled? =  "<<joystickEnabled_  ;

    if (joystickEnabled_ && joystickAxisEnabled_[axis]!=temp) {
      controller_->SetJoystickAxisEnabled((VLStepExpress::Axis)axis, temp);
      joystickAxisEnabled_[axis] = temp;
    }
}

void LStepExpressModel::setValue(const QString & command, const QString & value)
{
  //    NQLog("LStepExpressModel ", NQLog::Spam)<< "setValue"  ;
    std::string temp;
    controller_->SetValue(command.toStdString(), value.toStdString());
}

void LStepExpressModel::getValue(const QString & command, QString & value)
{
  // NQLog("LStepExpressModel ", NQLog::Spam)<< "getValue"  ;
    std::string temp;
    controller_->GetValue(command.toStdString(), temp);
    value = temp.c_str();
}

void LStepExpressModel::validConfig()
{
  //NQLog("LStepExpressModel ", NQLog::Spam)<< "validConfig"  ;
    controller_->ValidConfig();
}

void LStepExpressModel::validParameter()
{
  //    NQLog("LStepExpressModel ", NQLog::Spam)<< "validParameter"  ;
    controller_->ValidParameter();
}

void LStepExpressModel::saveConfig()
{
  //    NQLog("LStepExpressModel ", NQLog::Spam)<< "save config"  ;
    controller_->SaveConfig();
}

void LStepExpressModel::reset()
{
  //    NQLog("LStepExpressModel ", NQLog::Spam)<< "reset to start-up conditions"  ;
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
    // int joystick_model = this->getJoystickEnabled();
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
/*
    NQLog("LStepExpressModel ")<< "updateMotionInformation motion information changed"  ;
*/
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
	    //	    NQLog("LStepExpressModel", NQLog::Spam) <<" axis status =  "<<(ivalues)[i]<<" axis enabled = "<<(axis_)[i]<<" temp = "<<temp;
          }
          if(temp)
          {
            inMotion_ = false;

            emit motionFinished();
          }
          
          /*
	if (std::all_of(ivalues.begin(), ivalues.end(),
	[](int i){	  NQLog("LStepExpressModel ", NQLog::Spam)<< "updateMotionInformation() axis status =  "<<i; bool temp = (i==LStepExpress_t::AXISSTANDSANDREADY || i==LStepExpress_t::AXISACKAFTERCALIBRATION) && (axis_)[i]==1; return temp;})) {
	NQLog("LStepExpressModel ", NQLog::Spam)<< "updateMotionInformation() finished moving inMotion_ = false";
	inMotion_ = false;
	emit motionFinished();
	}*/
      }
      
      if( (axis_)[0] || (axis_)[1] || (axis_)[2] || (axis_)[3]){
          controller_->GetPosition(dvalues);
          if (dvalues!=position_) {
	    //NQLog("LStepExpressModel ", NQLog::Spam)<< "updateMotionInformation() new position values"  ;
	  position_ = dvalues;
	  changed = true;
          }
      }
      
      //      NQLog("LStepExpressModel ", NQLog::Spam)<< "updateMotionInformation() finishedCalibrating_ =  "<<finishedCalibrating_<<" in motion =  "<<inMotion_  ;
      if(!inMotion_ && finishedCalibrating_){
        //  NQLog("LStepExpressModel ", NQLog::Spam)<< "updateMotionInformation() after calibration, setting position values to zero"  ;
          std::vector<double> posvalues{0.0, 0.0, 0.0, 0.0};
          controller_->SetPosition(posvalues);
          position_ = posvalues;
          changed = true;
          finishedCalibrating_ = false;
      }
      
      if (changed) {
	//          NQLog("LStepExpressModel ", NQLog::Spam)<< "updateMotionInformation() motion information changed"  ;
          emit motionInformationChanged();
      }
      
      isUpdating_ = false;
    }
}

void LStepExpressModel::updateMotionInformationFromTimer()
{
/*
    NQLog("LStepExpressModel") << "update from timer";
*/
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
	  if(temp)
          {
            inMotion_ = false;

            emit motionFinished();
          }
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
	//NQLog("LStepExpressModel ", NQLog::Spam)<< "updateMotionInformationFromTimer motion information changed"  ;
          emit motionInformationChanged();
      }
      
      isUpdating_ = false;
    }
}

void LStepExpressModel::setDeviceEnabled(bool enabled)
{
    if(state_ == READY && !enabled)
    {
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
