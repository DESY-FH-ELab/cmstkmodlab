/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <QApplication>

#include <nqlogger.h>

#include "HuberUnistat525wModel.h"

/*
  HuberUnistat525wModel implementation
  */
HuberUnistat525wModel::HuberUnistat525wModel(const char* port,
                                             float updateInterval,
                                             QObject * /*parent*/)
 : QObject(),
   AbstractDeviceModel<HuberPilotOne_t>(),
   HuberUnistat525w_PORT(port),
   updateInterval_(updateInterval)
{
  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );

  setDeviceEnabled(true);
  setControlsEnabled(true);

  updateInformation();
}

double HuberUnistat525wModel::getTemperatureSetPoint() const
{
  return temperatureSetPoint_;
}

void HuberUnistat525wModel::setTemperatureSetPoint(double temperature)
{
  if (state_ == READY) {

    if (temperatureSetPoint_!=temperature) {

      if (controller_->SetTemperatureSetPoint(temperature)) {
        temperatureSetPoint_ = temperature;
        emit informationChanged();
      }
    }
  }
}

bool HuberUnistat525wModel::getTemperatureControlMode() const
{
  return temperatureControlMode_;
}

void HuberUnistat525wModel::setTemperatureControlMode(bool process)
{
  if (state_ == READY) {

    if (temperatureControlMode_!=process) {

      if (controller_->SetTemperatureControlMode(process)) {
        temperatureControlMode_ = process;
        emit informationChanged();
      }
    }
  }
}
bool HuberUnistat525wModel::getTemperatureControlEnabled() const
{
  return temperatureControlEnabled_;
}

void HuberUnistat525wModel::setTemperatureControlEnabled(bool enabled)
{
  if (state_ == READY) {

    if (temperatureControlEnabled_!=enabled) {

      if (controller_->SetTemperatureControlEnabled(enabled)) {
        temperatureControlEnabled_ = enabled;
        emit informationChanged();
      }
    }
  }
}

bool HuberUnistat525wModel::getCirculatorEnabled() const
{
  return circulatorEnabled_;
}

void HuberUnistat525wModel::setCirculatorEnabled(bool enabled)
{
  if (state_ == READY) {

    if (circulatorEnabled_!=enabled) {

      if (controller_->SetCirculatorEnabled(enabled)) {
        circulatorEnabled_ = enabled;
        emit informationChanged();
      }
    }
  }
}

double HuberUnistat525wModel::getInternalTemperature() const
{
  return internalTemperature_;
}

double HuberUnistat525wModel::getProcessTemperature() const
{
  return processTemperature_;
}

double HuberUnistat525wModel::getReturnTemperature() const
{
  return returnTemperature_;
}

double HuberUnistat525wModel::getPumpPressure() const
{
  return pumpPressure_;
}

int HuberUnistat525wModel::getPower() const
{
  return power_;
}

double HuberUnistat525wModel::getCoolingWaterInletTemperature() const
{
  return cwInletTemperature_;
}

double HuberUnistat525wModel::getCoolingWaterOutletTemperature() const
{
  return cwOutletTemperature_;
}

bool HuberUnistat525wModel::getAutoPID() const
{
  return autoPID_;
}

void HuberUnistat525wModel::setAutoPID(bool autoPID)
{
  if (state_ == READY) {

    if (autoPID_!=autoPID) {

      if (controller_->SetAutoPID(autoPID)) {
        autoPID_ = autoPID;
        emit informationChanged();
      }
    }
  }
}

int HuberUnistat525wModel::getKpInternal() const
{
  return KpInternal_;
}

void HuberUnistat525wModel::setKpInternal(int Kp)
{
  if (state_ == READY) {

    if (KpInternal_!=Kp) {

      if (controller_->SetKpInternal(Kp)) {
        KpInternal_ = Kp;
        emit informationChanged();
      }
    }
  }
}

double HuberUnistat525wModel::getTnInternal() const
{
  return TnInternal_;
}

void HuberUnistat525wModel::setTnInternal(double Tn)
{
  if (state_ == READY) {

    if (TnInternal_!=Tn) {

      if (controller_->SetTnInternal(Tn)) {
        TnInternal_ = Tn;
        emit informationChanged();
      }
    }
  }
}

double HuberUnistat525wModel::getTvInternal() const
{
  return TvInternal_;
}

void HuberUnistat525wModel::setTvInternal(double Tv)
{
  if (state_ == READY) {

    if (TvInternal_!=Tv) {

      if (controller_->SetTvInternal(Tv)) {
        TvInternal_ = Tv;
        emit informationChanged();
      }
    }
  }
}

int HuberUnistat525wModel::getKpJacket() const
{
  return KpJacket_;
}

void HuberUnistat525wModel::setKpJacket(int Kp)
{
  if (state_ == READY) {

    if (KpJacket_!=Kp) {

      if (controller_->SetKpJacket(Kp)) {
        KpJacket_ = Kp;
        emit informationChanged();
      }
    }
  }
}

double HuberUnistat525wModel::getTnJacket() const
{
  return TnJacket_;
}

void HuberUnistat525wModel::setTnJacket(double Tn)
{
  if (state_ == READY) {

    if (TnJacket_!=Tn) {

      if (controller_->SetTnJacket(Tn)) {
        TnJacket_ = Tn;
        emit informationChanged();
      }
    }
  }
}

double HuberUnistat525wModel::getTvJacket() const
{
  return TvJacket_;
}

void HuberUnistat525wModel::setTvJacket(double Tv)
{
  if (state_ == READY) {

    if (TvJacket_!=Tv) {

      if (controller_->SetTvJacket(Tv)) {
        TvJacket_ = Tv;
        emit informationChanged();
      }
    }
  }
}

int HuberUnistat525wModel::getKpProcess() const
{
  return KpProcess_;
}

void HuberUnistat525wModel::setKpProcess(int Kp)
{
  if (state_ == READY) {

    if (KpProcess_!=Kp) {

      if (controller_->SetKpProcess(Kp)) {
        KpProcess_ = Kp;
        emit informationChanged();
      }
    }
  }
}

double HuberUnistat525wModel::getTnProcess() const
{
  return TnProcess_;
}

void HuberUnistat525wModel::setTnProcess(double Tn)
{
  if (state_ == READY) {

    if (TnProcess_!=Tn) {

      if (controller_->SetTnProcess(Tn)) {
        TnProcess_ = Tn;
        emit informationChanged();
      }
    }
  }
}

double HuberUnistat525wModel::getTvProcess() const
{
  return TvProcess_;
}

void HuberUnistat525wModel::setTvProcess(double Tv)
{
  if (state_ == READY) {

    if (TvProcess_!=Tv) {

      if (controller_->SetTvProcess(Tv)) {
        TvProcess_ = Tv;
        emit informationChanged();
      }
    }
  }
}

int HuberUnistat525wModel::getKp() const
{
  if (autoPID_) return 0;

  if (!temperatureControlMode_) {
    return getKpInternal();
  } else {
    return getKpProcess();
  }

  return 0;
}

void HuberUnistat525wModel::setKp(int Kp)
{
  if (autoPID_) return;

  if (!temperatureControlMode_) {
    setKpInternal(Kp);
  } else {
    setKpProcess(Kp);
  }
}

double HuberUnistat525wModel::getTn() const
{
  if (autoPID_) return 0;

  if (!temperatureControlMode_) {
    return getTnInternal();
  } else {
    return getTnProcess();
  }

  return 0;
}

void HuberUnistat525wModel::setTn(double Tn)
{
  if (autoPID_) return;

  if (!temperatureControlMode_) {
    setTnInternal(Tn);
  } else {
    setTnProcess(Tn);
  }
}

double HuberUnistat525wModel::getTv() const
{
  if (autoPID_) return 0;

  if (!temperatureControlMode_) {
    return getTvInternal();
  } else {
    return getTvProcess();
  }

  return 0;
}

void HuberUnistat525wModel::setTv(double Tv)
{
  if (autoPID_) return;

  if (!temperatureControlMode_) {
    setTvInternal(Tv);
  } else {
    setTvProcess(Tv);
  }
}

void HuberUnistat525wModel::setPID(int Kp, double Tn, double Tv)
{
  if (autoPID_) return;

  if (!temperatureControlMode_) {
    setKpInternal(Kp);
    setTnInternal(Tn);
    setTvInternal(Tv);
  } else {
    setKpProcess(Kp);
    setTnProcess(Tn);
    setTvProcess(Tv);
  }
}

void HuberUnistat525wModel::initialize()
{
  NQLog("HuberUnistat525wModel", NQLog::Message) << "initialize()";

  setDeviceState(INITIALIZING);

  renewController(HuberUnistat525w_PORT);

  bool enabled = ( controller_ != NULL ) && ( controller_->IsCommunication() );

  if ( enabled ) {
    setDeviceState(READY);
    updateInformation();
  }
  else {
    setDeviceState( OFF );
    delete controller_;
    controller_ = NULL;
  }
}

void HuberUnistat525wModel::setDeviceState( State state )
{
  if ( state_ != state ) {
    state_ = state;

    // No need to run the timer if the chiller is not ready
    if ( state_ == READY )
      timer_->start();
    else
      timer_->stop();

    emit deviceStateChanged(state);
  }
}

void HuberUnistat525wModel::updateInformation()
{
  NQLog("HuberUnistat525wModel", NQLog::Debug) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    NQLog("HuberUnistat525wModel", NQLog::Debug) << " running in main application thread";
  } else {
    NQLog("HuberUnistat525wModel", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  if ( state_ == READY ) {

    double newTemperatureSetPoint = controller_->GetTemperatureSetPoint();
    bool newTemperatureControlMode = controller_->GetTemperatureControlMode();
    bool newTemperatureControlEnabled = controller_->GetTemperatureControlEnabled();
    bool newCirculatorEnabled = controller_->GetCirculatorEnabled();
    double newInternalTemperature = controller_->GetInternalTemperature();
    double newProcessTemperature = controller_->GetProcessTemperature();

    // double newReturnTemperature = controller_->GetReturnTemperature();
    double newReturnTemperature = 0.0;

    // double newCWInletTemperature = controller_->GetCoolingWaterInletTemperature();
    double newCWInletTemperature = 0.0;

    // double newCWOutletTemperature = controller_->GetCoolingWaterOutletTemperature();
    double newCWOutletTemperature = 0.0;

    double newPumpPressure = controller_->GetPumpPressure();

    // int newPower = controller_->GetPower();
    int newPower = 0.0;

    bool newAutoPID = controller_->GetAutoPID();

    int newKpInternal = 0;
    double newTnInternal = 0.0;
    double newTvInternal = 0.0;
    int newKpJacket = 0;
    double newTnJacket = 0.0;
    double newTvJacket = 0.0;
    int newKpProcess = 0;
    double newTnProcess = 0.0;
    double newTvProcess = 0.0;

    if (!newAutoPID) {
      if (!newTemperatureControlMode) {
        newKpInternal = controller_->GetKpInternal();
        newTnInternal = controller_->GetTnInternal();
        newTvInternal = controller_->GetTvInternal();
      } else {
        newKpProcess = controller_->GetKpProcess();
        newTnProcess = controller_->GetTnProcess();
        newTvProcess = controller_->GetTvProcess();
      }
    }

    // int newKpJacket = controller_->GetKpJacket();
    // double newTnJacket = controller_->GetTnJacket();
    // double newTvJacket = controller_->GetTvJacket();

    if (newTemperatureSetPoint != temperatureSetPoint_ ||
        newTemperatureControlMode != temperatureControlMode_ ||
        newTemperatureControlEnabled != temperatureControlEnabled_ ||
        newCirculatorEnabled != circulatorEnabled_ ||
        newInternalTemperature != internalTemperature_ ||
        newProcessTemperature != processTemperature_ ||
        newReturnTemperature != returnTemperature_ ||
        newCWInletTemperature != cwInletTemperature_ ||
        newCWOutletTemperature != cwOutletTemperature_ ||
        newPumpPressure != pumpPressure_ ||
        newPower != power_ ||
        newAutoPID != autoPID_ ||
        newKpInternal != KpInternal_ ||
        newTnInternal != TnInternal_ ||
        newTvInternal != TvInternal_ ||
        newKpJacket != KpJacket_ ||
        newTnJacket != TnJacket_ ||
        newTvJacket != TvJacket_||
        newKpProcess != KpProcess_ ||
        newTnProcess != TnProcess_ ||
        newTvProcess != TvProcess_) {

      temperatureSetPoint_ = newTemperatureSetPoint;
      temperatureControlMode_ = newTemperatureControlMode;
      temperatureControlEnabled_ = newTemperatureControlEnabled;
      circulatorEnabled_ = newCirculatorEnabled;
      internalTemperature_ = newInternalTemperature;
      processTemperature_ = newProcessTemperature;
      returnTemperature_ = newReturnTemperature;
      pumpPressure_ = newPumpPressure;
      power_ = newPower;
      cwInletTemperature_ = newCWInletTemperature;
      cwOutletTemperature_ = newCWOutletTemperature;

      autoPID_ = newAutoPID;
      KpInternal_ = newKpInternal ;
      TnInternal_ = newTnInternal ;
      TvInternal_ = newTvInternal ;
      KpJacket_ = newKpJacket ;
      TnJacket_ = newTnJacket ;
      TvJacket_ = newTvJacket ;
      KpProcess_ = newKpProcess ;
      TnProcess_ = newTnProcess ;
      TvProcess_ = newTvProcess ;

      NQLog("HuberUnistat525wModel", NQLog::Spam) << "information changed";

      emit informationChanged();
    }
  }
}

void HuberUnistat525wModel::setDeviceEnabled(bool enabled)
{
  AbstractDeviceModel<HuberPilotOne_t>::setDeviceEnabled(enabled);
}

void HuberUnistat525wModel::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}

void HuberUnistat525wModel::statusMessage(const QString & text)
{
  emit message(text);
}
