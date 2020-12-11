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

#include "MartaModel.h"

/*
   MartaModel implementation
  */
MartaModel::MartaModel(const char* ipaddress,
		       float updateInterval, QObject* /*parent*/)
  : QObject(),
    AbstractDeviceModel<Marta_t>(),
    ipaddress_(ipaddress),
    updateInterval_(updateInterval)
{
  /*
  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );
  */
  
  setDeviceEnabled(true);
  setControlsEnabled(true);

  //updateInformation();
}

MartaModel::~MartaModel()
{

}

void MartaModel::initialize()
{
  NQLog("MartaModel", NQLog::Message) << "initialize()";

  setDeviceState(INITIALIZING);

  renewController(ipaddress_);
  
  bool enabled = ( controller_ != NULL ) && ( controller_->DeviceAvailable() );
  
  if ( enabled ) {
    setDeviceState(READY);
    updateInformation();
  } else {
    setDeviceState((State)OFF);
    delete controller_;
    controller_ = NULL;
  }
}

void MartaModel::setDeviceState( State state )
{
  if ( state_ != state ) {
    state_ = state;

    /*
    // No need to run the timer if the chiller is not ready
    if ( state_ == READY )
      timer_->start();
    else
      timer_->stop();
    */
    
    emit deviceStateChanged(state);
  }
}

void MartaModel::updateInformation()
{
  /*
  NQLog("MartaModel", NQLog::Debug) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    NQLog("MartaModel", NQLog::Debug) << " running in main application thread";
  } else {
    NQLog("MartaModel", NQLog::Debug) << " running in dedicated DAQ thread";
  }
  */

  std::cout << "void MartaModel::updateInformation()" << std::endl;
  
  if ( state_ == READY ) {

    uint16_t tab_reg[74];
    
    controller_->ReadRegisters(1, 16, tab_reg);
    
    // printf("0x%04x 0x%04x\n", tab_reg[0], tab_reg[1]);
    
    printf("PT03:     %f\n", controller_->ToFloatDCBA(&tab_reg[0]));
    printf("PT05:     %f\n", controller_->ToFloatDCBA(&tab_reg[2]));
    printf("PT01CO2:  %f\n", controller_->ToFloatDCBA(&tab_reg[4]));
    printf("PT02CO2:  %f\n", controller_->ToFloatDCBA(&tab_reg[6]));
    printf("PT03CO2:  %f\n", controller_->ToFloatDCBA(&tab_reg[8]));
    printf("PT04CO2:  %f\n", controller_->ToFloatDCBA(&tab_reg[10]));
    printf("PT05CO2:  %f\n", controller_->ToFloatDCBA(&tab_reg[12]));
    printf("PT06CO2:  %f\n", controller_->ToFloatDCBA(&tab_reg[14]));
    
    /*
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

      NQLog("MartaModel", NQLog::Spam) << "information changed";

      emit informationChanged();
    }
    */
  }
}

void MartaModel::setDeviceEnabled(bool enabled)
{
  AbstractDeviceModel<Marta_t>::setDeviceEnabled(enabled);
}

void MartaModel::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}

void MartaModel::ReadRegisters(int addr, int nb, uint16_t *dest)
{
  controller_->ReadRegisters(addr, nb, dest);
}

float MartaModel::ToFloatABCD(uint16_t *reg)
{
  return controller_->ToFloatABCD(reg);
}

float MartaModel::ToFloatBADC(uint16_t *reg)
{
  return controller_->ToFloatBADC(reg);
}

float MartaModel::ToFloatCDAB(uint16_t *reg)
{
  return controller_->ToFloatCDAB(reg);
}

float MartaModel::ToFloatDCBA(uint16_t *reg)
{
  return controller_->ToFloatDCBA(reg);
}
