/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
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

void HuberUnistat525wModel::setTemperatureControlMode(bool external)
{
  if (state_ == READY) {

    if (temperatureControlMode_!=external) {

      if (controller_->SetTemperatureControlMode(external)) {
        temperatureControlMode_ = external;
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

double HuberUnistat525wModel::getBathTemperature() const
{
  return bathTemperature_;
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
    double newBathTemperature = controller_->GetBathTemperature();
    double newReturnTemperature = controller_->GetReturnTemperature();
    double newCWInletTemperature = controller_->GetCoolingWaterInletTemperature();
    double newCWOutletTemperature = controller_->GetCoolingWaterOutletTemperature();
    double newPumpPressure = controller_->GetPumpPressure();
    int newPower = controller_->GetPower();

    if (newTemperatureSetPoint != temperatureSetPoint_ ||
        newTemperatureControlMode != temperatureControlMode_ ||
        newTemperatureControlEnabled != temperatureControlEnabled_ ||
        newCirculatorEnabled != circulatorEnabled_ ||
        newBathTemperature != bathTemperature_ ||
        newReturnTemperature != returnTemperature_ ||
        newCWInletTemperature != cwInletTemperature_ ||
        newCWOutletTemperature != cwOutletTemperature_ ||
        newPumpPressure != pumpPressure_ ||
        newPower != power_) {

      temperatureSetPoint_ = newTemperatureSetPoint;
      temperatureControlMode_ = newTemperatureControlMode;
      temperatureControlEnabled_ = newTemperatureControlEnabled;
      circulatorEnabled_ = newCirculatorEnabled;
      bathTemperature_ = newBathTemperature;
      returnTemperature_ = newReturnTemperature;
      pumpPressure_ = newPumpPressure;
      power_ = newPower;
      cwInletTemperature_ = newCWInletTemperature;
      cwOutletTemperature_ = newCWOutletTemperature;

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
