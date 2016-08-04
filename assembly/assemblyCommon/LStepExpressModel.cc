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

    axisStatus_ = std::vector<int>{ -1, -1, -1, -1 };
    position_ = allZerosD;

    inMotion_ = false;
    isUpdating_ = false;
    isPaused_ = false;
    finishedCalibrating_ = false;
    
    timer_ = new QTimer(this);
    timer_->setInterval(motionUpdateInterval_);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateMotionInformation()));

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

void LStepExpressModel::printSpyInformation()
{
    //    NQLog("LStepExpressModel", NQLog::Debug) << "printSpyInformation";                                                                 
    for(int i = 0; i < spyTimer->size(); i++){
        NQLog("SPY LStepExpressModel", NQLog::Debug) << "timer_, signal timeout()";                                                                 
    } 
    spyTimer->clear();
    for(int i = 0; i < spyDeviceStateChanged->size(); i++){
        NQLog("SPY LStepExpressModel", NQLog::Debug) << "this_, signal deviceStateChanged("<<(spyDeviceStateChanged->value(i))[0].toString().toStdString()<<")";                                                
    } 
    spyDeviceStateChanged->clear();
    for(int i = 0; i < spyInformationChanged->size(); i++){
        NQLog("SPY LStepExpressModel", NQLog::Debug) << "this_, signal informationChanged()";                                                
    } 
    spyInformationChanged->clear();
    for(int i = 0; i < spyMotionInformationChanged->size(); i++){
        NQLog("SPY LStepExpressModel", NQLog::Debug) << "this_, signal motionInformationChanged()";                                                
    } 
    spyMotionInformationChanged->clear();
    for(int i = 0; i < spyMessage->size(); i++){
        NQLog("SPY LStepExpressModel", NQLog::Debug) << "this_, signal message()";                                                
    } 
    spyControlStateChanged->clear();
    for(int i = 0; i < spyMessage->size(); i++){
        NQLog("SPY LStepExpressModel", NQLog::Debug) << "this_, signal controlStateChanged("<<(spyControlStateChanged->value(i))[0].toBool()<<")";                                                
    } 
    spyMessage->clear();
    for(int i = 0; i < spyMotionStarted->size(); i++){
        NQLog("SPY LStepExpressModel", NQLog::Debug) << "this_, signal motionStarted()";                                                
    } 
    spyMotionStarted->clear();
    for(int i = 0; i < spyMotionFinished->size(); i++){
        NQLog("SPY LStepExpressModel", NQLog::Debug) << "this_, signal motionFinished()";                                                
    } 
    spyMotionFinished->clear();
}

void LStepExpressModel::pauseUpdate()
{
  NQLog("LStepExpressModel", NQLog::Debug) << "pauseUpdate()";                                                                 
    isPaused_ = true;
    setControlsEnabled(false);
}

void LStepExpressModel::continueUpdate()
{
  NQLog("LStepExpressModel", NQLog::Debug) << "continueUpdate()";                                                                 
    isPaused_ = false;
    setControlsEnabled(true);
}

QString LStepExpressModel::getAxisName(unsigned int axis)
{
      NQLog("LStepExpressModel", NQLog::Debug) << "getAxisName(axis)";                                                                 
    QMutexLocker locker(&mutex_);

    QString temp(controller_->GetAxisName((VLStepExpress::Axis)axis));
    return temp;
}

QString LStepExpressModel::getAxisDimensionShortName(unsigned int axis)
{
  NQLog("LStepExpressModel", NQLog::Debug) << "getAxisDimensionShortName(axis)";                                                                 
    QMutexLocker locker(&mutex_);


    QString temp(controller_->GetAxisDimensionShortName((VLStepExpress::Dimension)dim_[axis]));
    return temp;
}

QString LStepExpressModel::getAxisStatusText(unsigned int axis)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "getAxisStatusText(axis)";                                                                 
    QMutexLocker locker(&mutex_);

    QString temp(controller_->GetAxisStatusText((VLStepExpress::AxisStatus)axisStatus_[axis]));
    return temp;
}

bool LStepExpressModel::getAxisState(unsigned int axis)
{
  NQLog("LStepExpressModel", NQLog::Debug) << "getAxisState(axis)";                                                                 
    QMutexLocker locker(&mutex_);

    return (axisStatus_[axis]!=VLStepExpress::AXISDISABLED);
}

bool LStepExpressModel::getAxisEnabled(unsigned int axis)
{
  NQLog("LStepExpressModel", NQLog::Debug) << "getAxisEnabled(axis)";                                                                 
  QMutexLocker locker(&mutex_);
  
  return axis_[axis];
}

double LStepExpressModel::getPosition(unsigned int axis)
{
  NQLog("LStepExpressModel", NQLog::Debug) << "getPosition(axis)";                                                                 
    QMutexLocker locker(&mutex_);

    return position_[axis];
}

void LStepExpressModel::moveRelative(std::vector<double> & values)
{
  NQLog("LStepExpressModel", NQLog::Debug) << "moveRelative() v1";                                                                 
    controller_->MoveRelative(values);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveRelative(double x, double y, double z, double a)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "moveRelative() v2";                                                                 
    controller_->MoveRelative(x, y, z, a);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveRelative(unsigned int axis, double value)
{
  NQLog("LStepExpressModel", NQLog::Debug) << "moveRelative v3";                                                                 
    controller_->MoveRelative((VLStepExpress::Axis)axis, value);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(std::vector<double> & values)
{
    NQLog("LStepExpresModel", NQLog::Debug) << "moveAbsolute v1";
    controller_->MoveAbsolute(values);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(double x, double y, double z, double a)
{
    NQLog("LStepExpresModel", NQLog::Debug) << "moveAbsolute v2, go to " << x << " x, "<< y <<" y, "<< z <<" z, "<< a <<" a";
    controller_->MoveAbsolute(x, y, z, a);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::moveAbsolute(unsigned int axis, double value)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "moveAbsolute v3";                                                                 
    controller_->MoveAbsolute((VLStepExpress::Axis)axis, value);
    inMotion_ = true;
    emit motionStarted();
}

void LStepExpressModel::calibrate()
{
    NQLog("LStepExpressModel", NQLog::Debug) << "calibrate: go to calibrate";                                                                 
    controller_->Calibrate();
    inMotion_ = true;
    finishedCalibrating_ = true;
    NQLog("LStepExpressModel", NQLog::Debug) << "calibrate: emit motionStarted";                                                                 
    emit motionStarted();

    /*
    NQLog("LStepExpressModel", NQLog::Debug) << "calibrate: set position to zero";                                                                 
    //set positions to all zeros
    bool changed = false;
    std::vector<double> dvalues{0.0, 0.0, 0.0, 0.0};
    controller_->SetPosition(dvalues);
    if (dvalues!=position_) {
        position_ = dvalues;
        changed = true;
    }
    if (changed) {
        // NQLog("LStepExpressModel", NQLog::Spam) << "information changed";                                                                 
        emit informationChanged();
    }
    */
}

void LStepExpressModel::emergencyStop()
{
    NQLog("LStepExpressModel", NQLog::Debug) << "emergencyStop";                                                                 
    controller_->EmergencyStop();
    inMotion_ = false;
    emit motionFinished();
}

bool LStepExpressModel::getJoystickEnabled()
{
  NQLog("LStepExpressModel", NQLog::Debug) << "getJoystickEnabled: "<<joystickEnabled_;                                                                 
    QMutexLocker locker(&mutex_);

    return (joystickEnabled_==1);
}

bool LStepExpressModel::getJoystickAxisEnabled(unsigned int axis)
{
  NQLog("LStepExpressModel", NQLog::Debug) << "getJoystickAxisEnabled: "<<joystickAxisEnabled_[axis];                                                                 
    QMutexLocker locker(&mutex_);

    return (joystickAxisEnabled_[axis]==1);
}

void LStepExpressModel::setAxisEnabled(unsigned int axis, bool enabled)
{
    int temp = (int)enabled;
    NQLog("LStepExpressModel", NQLog::Debug) << "setAxisEnabled " << axis << " " << temp;

    if (axis_[axis]!=temp) {
        controller_->SetPowerAmplifierStatus((VLStepExpress::Axis)axis, temp);
        controller_->SetAxisEnabled((VLStepExpress::Axis)axis, temp);
        axis_[axis] = temp;
        emit deviceStateChanged(state_);
    }
}

void LStepExpressModel::setJoystickEnabled(bool enabled)
{
  NQLog("LStepExpressModel", NQLog::Debug) << "setJoystickEnabled "<<enabled;

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
    NQLog("LStepExpressModel", NQLog::Debug) << "setJoystickAxisEnabled " << axis << " " << temp << " joystick enabled? = "<<joystickEnabled_;

    if (joystickEnabled_ && joystickAxisEnabled_[axis]!=temp) {
      NQLog("LStepExpressModel", NQLog::Debug) << "setJoystickAxisEnabled do something";
        joystickAxisEnabled_[axis] = temp;

        controller_->SetJoystickAxisEnabled((VLStepExpress::Axis)axis, temp);   
    }
}

void LStepExpressModel::setValue(const QString & command, const QString & value)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "saveValue";
    std::string temp;
    controller_->SetValue(command.toStdString(), value.toStdString());
}

void LStepExpressModel::getValue(const QString & command, QString & value)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "getValue";
    std::string temp;
    controller_->GetValue(command.toStdString(), temp);
    value = temp.c_str();
}

void LStepExpressModel::validConfig()
{
    NQLog("LStepExpressModel", NQLog::Debug) << "validConfig";
    controller_->ValidConfig();
}

void LStepExpressModel::validParameter()
{
    NQLog("LStepExpressModel", NQLog::Debug) << "validParameter";
    controller_->ValidParameter();
}

void LStepExpressModel::saveConfig()
{
    NQLog("LStepExpressModel", NQLog::Debug) << "save config";
    controller_->SaveConfig();
}

void LStepExpressModel::reset()
{
    NQLog("LStepExpressModel", NQLog::Debug) << "reset to start-up conditions";
    controller_->Reset();
}

void LStepExpressModel::initialize()
{
    NQLog("LStepExpressModel", NQLog::Debug) << "initialize";
    setDeviceState(INITIALIZING);

    renewController(LStepExpress_PORT);

    bool enabled = (controller_ != NULL) && (controller_->DeviceAvailable());

    if ( enabled ) {
      std::vector<int> allZerosI{ 0, 0, 0, 0 };
      controller_->SetPowerAmplifierStatus(allZerosI);
      controller_->SetAxisEnabled(allZerosI);
      controller_->SetJoystickEnabled(false);
      controller_->SetJoystickAxisEnabled(allZerosI);
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
    NQLog("LStepExpressModel", NQLog::Debug) << "setDeviceState";
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
  NQLog("LStepExpressModel", NQLog::Debug) << "updateInformation()";

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

    controller_->GetDimension(ivalues);
    if (ivalues!=dim_) {
        dim_ = ivalues;
        changed = true;
    }

    int joystick = controller_->GetJoystickEnabled();
    NQLog("LStepExpressModel", NQLog::Debug) << "updateInformation() joystick = "<<joystick;
    if (joystick!=joystickEnabled_) {
        joystickEnabled_ = joystick;
        changed = true;
    }

    controller_->GetJoystickAxisEnabled(ivalues);
    NQLog("LStepExpressModel", NQLog::Debug) << "updateInformation() axis joystick = "<<(ivalues)[0]<<" "<<(ivalues)[1]<<" "<<(ivalues)[2]<<" "<<(ivalues)[3];
    if (ivalues!=joystickAxisEnabled_) {
        joystickAxisEnabled_ = ivalues;
        changed = true;
    }

    if (changed) {
        // NQLog("LStepExpressModel", NQLog::Spam) << "information changed";
        emit informationChanged();
    }
}

void LStepExpressModel::updateMotionInformation()
{
    NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation(), state = "<<state_<<" isPaused_ = "<<isPaused_;

    static const int nUpdates = updateInterval_/motionUpdateInterval_;

    if ( state_ == READY && !isPaused_) {

        QMutexLocker locker(&mutex_);

        isUpdating_ = true;

        updateCount_++;
        if (updateCount_==nUpdates) {
	  NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation(), going to updateInformation()";
            updateInformation();
            updateCount_ = 0;
        }

        /*
        if (thread()==QApplication::instance()->thread()) {
            NQLog("LStepExpressModel", NQLog::Debug) << " running in main application thread";
        } else {
            NQLog("LStepExpressModel", NQLog::Debug) << " running in dedicated thread";
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

        NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation() received axis status";
        NQLog("LStepExpressModel", NQLog::Debug) << " x-axis: new status = "<<(ivalues)[0]<<" old status = "<<(axisStatus_)[0];
        NQLog("LStepExpressModel", NQLog::Debug) << " y-axis: new status = "<<(ivalues)[1]<<" old status = "<<(axisStatus_)[1];
        NQLog("LStepExpressModel", NQLog::Debug) << " z-axis: new status = "<<(ivalues)[2]<<" old status = "<<(axisStatus_)[2];
        NQLog("LStepExpressModel", NQLog::Debug) << " a-axis: new status = "<<(ivalues)[3]<<" old status = "<<(axisStatus_)[3];
        NQLog("LStepExpressModel", NQLog::Debug) << " x-axis: old enabled = "<<(axis_)[0]<<" new enabled = "<<(evalues)[0]<<" pa = "<<(pavalues)[0];
        NQLog("LStepExpressModel", NQLog::Debug) << " y-axis: old enabled = "<<(axis_)[1]<<" new enabled = "<<(evalues)[1]<<" pa = "<<(pavalues)[1];
        NQLog("LStepExpressModel", NQLog::Debug) << " z-axis: old enabled = "<<(axis_)[2]<<" new enabled = "<<(evalues)[2]<<" pa = "<<(pavalues)[2];
        NQLog("LStepExpressModel", NQLog::Debug) << " a-axis: old enabled = "<<(axis_)[3]<<" new enabled = "<<(evalues)[3]<<" pa = "<<(pavalues)[3];

        if (ivalues!=axisStatus_) {
            axisStatus_ = ivalues;
            changed = true;
        }
        if (inMotion_) {
	NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation() inMotion_ = true";
	
	if (std::all_of(ivalues.begin(), ivalues.end(),
                            [](int i){	  NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation() axis status = "<<i; return (i==LStepExpress_t::AXISSTANDSANDREADY || i==LStepExpress_t::AXISACKAFTERCALIBRATION);})) {
	    NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation() finished moving inMotion_ = false";
                inMotion_ = false;
                emit motionFinished();
            }
        }

        controller_->GetPosition(dvalues);
        if (dvalues!=position_) {
	NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation() new position values";
            position_ = dvalues;
            changed = true;
        }

        NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation() finishedCalibrating_ = "<<finishedCalibrating_<<" in motion = "<<inMotion_;
        if(!inMotion_ && finishedCalibrating_){
	NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation() after calibration, setting position values to zero";
	std::vector<double> posvalues{0.0, 0.0, 0.0, 0.0};
	controller_->SetPosition(posvalues);
	position_ = posvalues;
	changed = true;
	finishedCalibrating_ = false;
        }
        
        if (changed) {
	NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation() motion information changed";
            emit motionInformationChanged();
        }

        isUpdating_ = false;
    }
}

void LStepExpressModel::setDeviceEnabled(bool enabled)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "setDeviceEnabled(bool enabled), enabled = "<<enabled;

    if (state_ == READY && !enabled) {
        NQLog("LStepExpressModel", NQLog::Debug) << "setDeviceEnabled(bool enabled), turn off power amplifiers and axes";
        std::vector<int> allZeros{ 0, 0, 0, 0 };
        //std::vector<int> allOn{ 1, 1, 1, 1 };
        controller_->SetPowerAmplifierStatus(allZeros);
        controller_->SetAxisEnabled(allZeros);
    }

    AbstractDeviceModel<LStepExpress_t>::setDeviceEnabled(enabled);
}

void LStepExpressModel::setControlsEnabled(bool enabled)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "setControlsEnabled(bool enabled), enabled = "<<enabled;
    emit controlStateChanged(enabled);
}
