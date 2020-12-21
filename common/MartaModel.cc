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
    
    controller_->ReadRegisters(0, 72, tab_reg);
    
    // printf("0x%04x 0x%04x\n", tab_reg[0], tab_reg[1]);
    
    printf("PT03:     %f\n", controller_->ToFloatBADC(&tab_reg[0]));
    printf("PT05:     %f\n", controller_->ToFloatBADC(&tab_reg[2]));
    printf("PT01CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[4]));
    printf("PT02CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[6]));
    printf("PT03CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[8]));
    printf("PT04CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[10]));
    printf("PT05CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[12]));
    printf("PT06CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[14]));
    printf("TT02:     %f\n", controller_->ToFloatBADC(&tab_reg[16]));
    printf("TT01CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[18]));
    printf("TT02CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[20]));
    printf("TT03CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[22]));
    printf("TT04CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[24]));
    printf("TT05CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[26]));
    printf("TT06CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[28]));
    printf("TT07CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[30]));
    printf("SH05:     %f\n", controller_->ToFloatBADC(&tab_reg[32]));
    printf("SC01CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[34]));
    printf("SC02CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[36]));
    printf("SC03CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[38]));
    printf("SC05CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[40]));
    printf("SC06CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[42]));
    printf("dP01CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[44]));
    printf("dP02CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[46]));
    printf("dP03CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[48]));
    printf("dP04CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[50]));
    printf("dT02CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[52]));
    printf("dT03CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[54]));
    printf("ST01CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[56]));
    printf("ST02CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[58]));
    printf("ST03CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[60]));
    printf("ST04CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[62]));

    printf("FT01CO2:  %f\n", controller_->ToFloatBADC(&tab_reg[64]));
    printf("SpeedSetpoint: %f\n", controller_->ToFloatBADC(&tab_reg[66]));
    printf("FlowSetpoint:  %f\n", controller_->ToFloatBADC(&tab_reg[68]));
    printf("TempSetpoint:  %f\n", controller_->ToFloatBADC(&tab_reg[70]));

    controller_->ReadRegisters(80, 5, tab_reg);
    printf("Status:   0x%04x\n", (int16_t)tab_reg[0]);
    printf("Status:   0x%04x\n", (int16_t)tab_reg[1]);
    printf("Status:   0x%04x\n", (int16_t)tab_reg[2]);
    printf("Status:   0x%04x\n", (int16_t)tab_reg[3]);
    printf("Status:   %d\n", (int16_t)tab_reg[4]);
    //printf("Status:   0x%04d\n", tab_reg[2]);
    //printf("Status:   0x%04d\n", tab_reg[3]);
    
    controller_->ReadRegisters(100, 7, tab_reg);
    printf("Status:   0x%02x\n", tab_reg[0]);
    printf("TempSetpoint:  %f\n", controller_->ToFloatBADC(&tab_reg[1]));    
    printf("SpeedSetpoint: %f\n", controller_->ToFloatBADC(&tab_reg[3]));    
    printf("FlowSetpoint: %f\n", controller_->ToFloatBADC(&tab_reg[5]));    
    
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
