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

#include <cmath>

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
    updateInterval_(updateInterval),
    autoUpdateDelay_(500)
{
  PT03_ = 0.0;

  initializeAlarmTexts();
  
  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );
  
  setDeviceEnabled(true);
  setControlsEnabled(true);

  updateInformation();
}

MartaModel::~MartaModel()
{

}

uint16_t MartaModel::getAlarms(int idx) const
{
  if (idx<0 || idx>3) return 0x0000;
  return Alarms_[idx];
}

const QStringList& MartaModel::getCurrentAlarms() const
{
  return CurrentAlarmTexts_;
}

bool MartaModel::getChillerOn() const
{
  return Status_&0x0001;
}

bool MartaModel::getCO2On() const
{
  return Status_&0x0002;
}

bool MartaModel::getPumpFixedFlow() const
{
  return Status_&0x0004;
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

    // No need to run the timer if the chiller is not ready
    if ( state_ == READY )
      timer_->start();
    else
      timer_->stop();
    
    emit deviceStateChanged(state);
  }
}

void MartaModel::updateInformation()
{
  NQLog("MartaModel", NQLog::Debug) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    NQLog("MartaModel", NQLog::Debug) << " running in main application thread";
  } else {
    NQLog("MartaModel", NQLog::Debug) << " running in dedicated DAQ thread";
  }
  
  if ( state_ == READY ) {

    bool changed = false;
    bool alarmChanged = false;
    uint16_t tab_reg[74];
    
    controller_->ReadRegisters(0, 72, tab_reg);
    
    changed |= valueChanged(PT03_, controller_->ToFloatBADC(&tab_reg[0]), 3);
    changed |= valueChanged(PT05_, controller_->ToFloatBADC(&tab_reg[2]), 3);
    changed |= valueChanged(PT01CO2_, controller_->ToFloatBADC(&tab_reg[4]), 3);
    changed |= valueChanged(PT02CO2_, controller_->ToFloatBADC(&tab_reg[6]), 3);
    changed |= valueChanged(PT03CO2_, controller_->ToFloatBADC(&tab_reg[8]), 3);
    changed |= valueChanged(PT04CO2_, controller_->ToFloatBADC(&tab_reg[10]), 3);
    changed |= valueChanged(PT05CO2_, controller_->ToFloatBADC(&tab_reg[12]), 3);
    changed |= valueChanged(PT06CO2_, controller_->ToFloatBADC(&tab_reg[14]), 3);
    changed |= valueChanged(TT02_, controller_->ToFloatBADC(&tab_reg[16]), 3);
    changed |= valueChanged(TT01CO2_, controller_->ToFloatBADC(&tab_reg[18]), 3);
    changed |= valueChanged(TT02CO2_, controller_->ToFloatBADC(&tab_reg[20]), 3);
    changed |= valueChanged(TT03CO2_, controller_->ToFloatBADC(&tab_reg[22]), 3);
    changed |= valueChanged(TT04CO2_, controller_->ToFloatBADC(&tab_reg[24]), 3);
    changed |= valueChanged(TT05CO2_, controller_->ToFloatBADC(&tab_reg[26]), 3);
    changed |= valueChanged(TT06CO2_, controller_->ToFloatBADC(&tab_reg[28]), 3);
    changed |= valueChanged(TT07CO2_, controller_->ToFloatBADC(&tab_reg[30]), 3);
    changed |= valueChanged(SH05_, controller_->ToFloatBADC(&tab_reg[32]), 3);
    changed |= valueChanged(SC01CO2_, controller_->ToFloatBADC(&tab_reg[34]), 3);
    changed |= valueChanged(SC02CO2_, controller_->ToFloatBADC(&tab_reg[36]), 3);
    changed |= valueChanged(SC03CO2_, controller_->ToFloatBADC(&tab_reg[38]), 3);
    changed |= valueChanged(SC05CO2_, controller_->ToFloatBADC(&tab_reg[40]), 3);
    changed |= valueChanged(SC06CO2_, controller_->ToFloatBADC(&tab_reg[42]), 3);
    changed |= valueChanged(dP01CO2_, controller_->ToFloatBADC(&tab_reg[44]), 3);
    changed |= valueChanged(dP02CO2_, controller_->ToFloatBADC(&tab_reg[46]), 3);
    changed |= valueChanged(dP03CO2_, controller_->ToFloatBADC(&tab_reg[48]), 3);
    changed |= valueChanged(dP04CO2_, controller_->ToFloatBADC(&tab_reg[50]), 3);
    changed |= valueChanged(dT02CO2_, controller_->ToFloatBADC(&tab_reg[52]), 3);
    changed |= valueChanged(dT03CO2_, controller_->ToFloatBADC(&tab_reg[54]), 3);
    changed |= valueChanged(ST01CO2_, controller_->ToFloatBADC(&tab_reg[56]), 3);
    changed |= valueChanged(ST02CO2_, controller_->ToFloatBADC(&tab_reg[58]), 3);
    changed |= valueChanged(ST03CO2_, controller_->ToFloatBADC(&tab_reg[60]), 3);
    changed |= valueChanged(ST04CO2_, controller_->ToFloatBADC(&tab_reg[62]), 3);
    changed |= valueChanged(FT01CO2_, controller_->ToFloatBADC(&tab_reg[64]), 6);
    changed |= valueChanged(SpeedSetpoint_, controller_->ToFloatBADC(&tab_reg[66]), 3);
    changed |= valueChanged(FlowSetpoint_, controller_->ToFloatBADC(&tab_reg[68]), 3);
    changed |= valueChanged(TemperatureSetpoint_, controller_->ToFloatBADC(&tab_reg[70]), 3);

    /*
    printf("PT03:            %f\n", PT03_);
    printf("PT05:            %f\n", PT05_);
    printf("PT01CO2:         %f\n", PT01CO2_);
    printf("PT02CO2:         %f\n", PT02CO2_);
    printf("PT03CO2:         %f\n", PT03CO2_);
    printf("PT04CO2:         %f\n", PT04CO2_);
    printf("PT05CO2:         %f\n", PT05CO2_);
    printf("PT06CO2:         %f\n", PT06CO2_);
    printf("TT02:            %f\n", TT02_);
    printf("TT01CO2:         %f\n", TT01CO2_);
    printf("TT02CO2:         %f\n", TT02CO2_);
    printf("TT03CO2:         %f\n", TT03CO2_);
    printf("TT04CO2:         %f\n", TT04CO2_);
    printf("TT05CO2:         %f\n", TT05CO2_);
    printf("TT06CO2:         %f\n", TT06CO2_);
    printf("TT07CO2:         %f\n", TT07CO2_);
    printf("SH05:            %f\n", SH05_);
    printf("SC01CO2:         %f\n", SC01CO2_);
    printf("SC02CO2:         %f\n", SC02CO2_);
    printf("SC03CO2:         %f\n", SC03CO2_);
    printf("SC05CO2:         %f\n", SC05CO2_);
    printf("SC06CO2:         %f\n", SC06CO2_);
    printf("dP01CO2:         %f\n", dP01CO2_);
    printf("dP02CO2:         %f\n", dP02CO2_);
    printf("dP03CO2:         %f\n", dP03CO2_);
    printf("dP04CO2:         %f\n", dP04CO2_);
    printf("dT02CO2:         %f\n", dT02CO2_);
    printf("dT03CO2:         %f\n", dT03CO2_);
    printf("ST01CO2:         %f\n", ST01CO2_);
    printf("ST02CO2:         %f\n", ST02CO2_);
    printf("ST03CO2:         %f\n", ST03CO2_);
    printf("ST04CO2:         %f\n", ST04CO2_);
    printf("FT01CO2:         %f\n", FT01CO2_);
    printf("SpeedSetpoint:   %f\n", SpeedSetpoint_);
    printf("FlowSetpoint:    %f\n", FlowSetpoint_);
    printf("TempSetpoint:    %f\n", TemperatureSetpoint_);
    */
    
    controller_->ReadRegisters(80, 5, tab_reg);
    
    alarmChanged |= valueChanged(Alarms_[0], tab_reg[0]);
    alarmChanged |= valueChanged(Alarms_[1], tab_reg[1]);
    alarmChanged |= valueChanged(Alarms_[2], tab_reg[2]);
    alarmChanged |= valueChanged(Alarms_[3], tab_reg[3]);
    alarmChanged |= valueChanged(AlarmStatus_, tab_reg[4]);

    /*
    printf("Alarm 1:     0x%04x\n", Alarms_[0]);
    printf("Alarm 2:     0x%04x\n", Alarms_[1]);
    printf("Alarm 3:     0x%04x\n", Alarms_[2]);
    printf("Alarm 4:     0x%04x\n", Alarms_[3]);
    printf("AlarmStatus:   %d\n", AlarmStatus_);
    */
    
    controller_->ReadRegisters(100, 7, tab_reg);
    
    changed |= valueChanged(Status_, tab_reg[0]);
    changed |= valueChanged(TemperatureSetpoint2_, controller_->ToFloatBADC(&tab_reg[1]), 3);
    changed |= valueChanged(SpeedSetpoint2_, controller_->ToFloatBADC(&tab_reg[3]), 3);
    changed |= valueChanged(FlowSetpoint2_, controller_->ToFloatBADC(&tab_reg[5]), 3);

    /*
    printf("Status:        0x%04x\n", Status_);
    printf("TempSetpoint2:   %f\n", TemperatureSetpoint2_);
    printf("SpeedSetpoint2:  %f\n", SpeedSetpoint2_);
    printf("FlowSetpoint2:   %f\n", FlowSetpoint2_);
    */
    
    if (alarmChanged) {

      CurrentAlarmTexts_.clear();

      for (int idx=0;idx<4;++idx) {
        uint16_t bit = 1;
        for (int b=0;b<16;++b) {

          if (Alarms_[idx] & bit) {
            std::map<uint16_t,Alarm_t>::iterator itfind = AllAlarmTexts_[idx].find(bit);
            if (itfind!=AllAlarmTexts_[idx].end()) {
              CurrentAlarmTexts_.append(QString(std::get<1>(itfind->second).c_str()) + " (" + std::get<0>(itfind->second).c_str());
            }
          }

          bit <<= 1;
        }
      }

      emit alarmsChanged();
    }

    if (changed || alarmChanged) {
      NQLog("MartaModel", NQLog::Spam) << "information changed";
      emit informationChanged();
    }
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

void MartaModel::statusMessage(const QString & text)
{
  emit message(text);
}

void MartaModel::setStartChiller(bool value)
{
  if (state_!=READY) return;

  bool stateChiller = getChillerOn();
  
  if (value==stateChiller) return;

  uint16_t temp = Status_;
  if (value) {
    temp |= 0x0001;
  } else {
    temp &= ~0x0003;
  }

  controller_->WriteRegisters(100, 1, &temp);

  QTimer::singleShot(autoUpdateDelay_, this, SLOT(updateInformation()));
}

void MartaModel::setStartCO2(bool value)
{
  if (state_!=READY) return;

  bool stateChiller = getChillerOn();
  bool stateCO2 = getCO2On();
  
  if (value==stateCO2) return;

  if (value && !stateChiller) return;

  uint16_t temp = Status_;
  if (value) {
    temp |= 0x0002;
  } else {
    temp &= ~0x0002;
  }

  controller_->WriteRegisters(100, 1, &temp);

  QTimer::singleShot(autoUpdateDelay_, this, SLOT(updateInformation()));
}

void MartaModel::setPumpFixedFlow(bool value)
{
  if (state_!=READY) return;

  bool state = getPumpFixedFlow();
  if (value==state) return;
  
  uint16_t temp = Status_;
  if (value) {
    temp |= 0x0004;
  } else {
    temp &= ~0x0004;
  }

  controller_->WriteRegisters(100, 1, &temp);

  QTimer::singleShot(autoUpdateDelay_, this, SLOT(updateInformation()));
}

void MartaModel::setTemperatureSetpoint(double value)
{
  if (state_!=READY) return;

  if (value<-35.0 || value>25.0) return;

  uint16_t temp[2];
  controller_->FromFloatBADC(value, &temp[0]);
  controller_->WriteRegisters(101, 2, &temp[0]);

  QTimer::singleShot(autoUpdateDelay_, this, SLOT(updateInformation()));
}

void MartaModel::setSpeedSetpoint(double value)
{
  if (state_!=READY) return;

  if (value<500.0 || value>6000.0) return;

  uint16_t temp[2];
  controller_->FromFloatBADC(value, &temp[0]);
  controller_->WriteRegisters(103, 2, &temp[0]);

  QTimer::singleShot(autoUpdateDelay_, this, SLOT(updateInformation()));
}

void MartaModel::setFlowSetpoint(double value)
{
  if (state_!=READY) return;

  if (value<0.1 || value>6.0) return;

  uint16_t temp[2];
  controller_->FromFloatBADC(value, &temp[0]);
  controller_->WriteRegisters(105, 2, &temp[0]);

  QTimer::singleShot(autoUpdateDelay_, this, SLOT(updateInformation()));
}

bool MartaModel::valueChanged(double &storage, double value, unsigned int precision)
{
  double power = std::pow(10, precision);
  double ftemp = std::round(value * power);
  bool ret = (storage!=ftemp);
  storage = ftemp/power;
  return ret;
}

bool MartaModel::valueChanged(uint16_t &storage, uint16_t value)
{
  bool ret = (storage!=value);
  storage = value;
  return ret;
}

void MartaModel::initializeAlarmTexts()
{
  AllAlarmTexts_[0][0x0001] = std::make_tuple("b_dP01_CO2_AL", "Pressure drop over FL1 high", false);
  AllAlarmTexts_[0][0x0002] = std::make_tuple("b_dP01_CO2_FS", "Pressure drop over FL1 too high", true);
  AllAlarmTexts_[0][0x0004] = std::make_tuple("b_dP02_CO2_AL", "Pressure drop over FL2 high", false);
  AllAlarmTexts_[0][0x0008] = std::make_tuple("b_dP02_CO2_FS", "Pressure drop over FL2 too high", true);
  AllAlarmTexts_[0][0x0010] = std::make_tuple("b_dP03_CO2_LP_AL", "Pump LP1 delta pressure low", false);
  AllAlarmTexts_[0][0x0020] = std::make_tuple("b_dP03_CO2_LP_FS", "Pump LP1 delta pressure too low", true);
  AllAlarmTexts_[0][0x0040] = std::make_tuple("b_dP03_CO2_HP_AL", "Pump LP1 delta pressure high", false);
  AllAlarmTexts_[0][0x0080] = std::make_tuple("b_dP03_CO2_HP_FS", "Pump LP1 delta pressure too high", true);
  AllAlarmTexts_[0][0x0100] = std::make_tuple("b_dP04_CO2_HP_FS", "b_dP04_CO2_HP_FS", true);
  AllAlarmTexts_[0][0x0200] = std::make_tuple("b_dT03_CO2_HT_AL", "Pump LP1 delta temperature high", false);
  AllAlarmTexts_[0][0x0400] = std::make_tuple("b_PT03_CO2_HP_FS", "Pump outlet pressure too high", true);
  AllAlarmTexts_[0][0x0800] = std::make_tuple("b_SC01_CO2_AL", "Pump suction subcooling low", false);
  AllAlarmTexts_[0][0x1000] = std::make_tuple("b_SC01_CO2_FS", "Pump suction subcooling too low", true);
  AllAlarmTexts_[0][0x2000] = std::make_tuple("b_SC01_CO2_FS", "Too low CO2 temperature", true);
  AllAlarmTexts_[0][0x4000] = std::make_tuple("b_PT04_CO2_HP_TS", "Accumulator pressure high", false);
  AllAlarmTexts_[0][0x8000] = std::make_tuple("b_PT04_CO2_HP_FS", "Accumulator pressure too high", true);

  AllAlarmTexts_[1][0x0001] = std::make_tuple("b_TT04_CO2_HT_AL", "Accumulator heater temperature high", false);
  AllAlarmTexts_[1][0x0002] = std::make_tuple("b_TT04_CO2_HT_TS", "Accumulator heater temperature very high", false);
  AllAlarmTexts_[1][0x0004] = std::make_tuple("b_TT04_CO2_HT_FS", "Accumulator heater temperature too high", true);
  AllAlarmTexts_[1][0x0008] = std::make_tuple("b_Compressor_FS", "Compressor circuit breaker", true);
  AllAlarmTexts_[1][0x0010] = std::make_tuple("b_Vent_FS", "Fans circuit breaker", true);
  AllAlarmTexts_[1][0x0020] = std::make_tuple("b_Heater_FS", "Heater circuit breaker", true);
  AllAlarmTexts_[1][0x0040] = std::make_tuple("b_Pump_FS", "CO2 pump circuit breaker of driver error", true);
  AllAlarmTexts_[1][0x0080] = std::make_tuple("b_Pressure_Switch_LP_FS", "Low pressure - Pressure switch", true);
  AllAlarmTexts_[1][0x0100] = std::make_tuple("b_Pressure_Switch_HP_FS", "High pressure - Pressure switch", true);
  AllAlarmTexts_[1][0x0200] = std::make_tuple("b_Switching_Off_Pressure", "Pressure in compressor too high", true);
  AllAlarmTexts_[1][0x0400] = std::make_tuple("b_Supply_Error_FS", "Power supply error", true);
  AllAlarmTexts_[1][0x0800] = std::make_tuple("b_Emergency_Stop", "Emergency stop", true);
  AllAlarmTexts_[1][0x1000] = std::make_tuple("bInterlock_IN_HMI", "Interlock IN trigger", true);
  AllAlarmTexts_[1][0x2000] = std::make_tuple("bKill_Switch_HMI", "Kill switch trigger", true);
  AllAlarmTexts_[1][0x4000] = std::make_tuple("b_PT03_R507A_IOError_FS", "b_PT03_R507A_IOError_FS", true);
  AllAlarmTexts_[1][0x8000] = std::make_tuple("b_PT05_R507A_IOError_FS", "b_PT05_R507A_IOError_FS", true);

  AllAlarmTexts_[2][0x0001] = std::make_tuple("b_PT01_CO2_IOError_FS", "b_PT01_CO2_IOError_FS", true);
  AllAlarmTexts_[2][0x0002] = std::make_tuple("b_PT02_CO2_IOError_FS", "b_PT02_CO2_IOError_FS", true);
  AllAlarmTexts_[2][0x0004] = std::make_tuple("b_PT03_CO2_IOError_FS", "b_PT03_CO2_IOError_FS", true);
  AllAlarmTexts_[2][0x0008] = std::make_tuple("b_PT04_CO2_IOError_FS", "b_PT04_CO2_IOError_FS", true);
  AllAlarmTexts_[2][0x0010] = std::make_tuple("b_PT05_CO2_IOError_FS", "b_PT05_CO2_IOError_FS", true);
  AllAlarmTexts_[2][0x0020] = std::make_tuple("b_PT06_CO2_IOError_FS", "b_PT06_CO2_IOError_FS", true);
  AllAlarmTexts_[2][0x0040] = std::make_tuple("b_Flowmeter_IOError_FS", "b_Flowmeter_IOError_FS", true);
  AllAlarmTexts_[2][0x0080] = std::make_tuple("b_TT02_R507A_IOError_FS", "b_TT02_R507A_IOError_FS", true);
  AllAlarmTexts_[2][0x0100] = std::make_tuple("b_TT01_CO2_IOError_FS", "b_TT01_CO2_IOError_FS", true);
  AllAlarmTexts_[2][0x0200] = std::make_tuple("b_TT02_CO2_IOError_FS", "b_TT02_CO2_IOError_FS", true);
  AllAlarmTexts_[2][0x0400] = std::make_tuple("b_TT03_CO2_IOError_FS", "b_TT03_CO2_IOError_FS", true);
  AllAlarmTexts_[2][0x0800] = std::make_tuple("b_TT04_CO2_IOError_FS", "b_TT04_CO2_IOError_FS", true);
  AllAlarmTexts_[2][0x1000] = std::make_tuple("b_TT05_CO2_IOError_FS", "b_TT05_CO2_IOError_FS", true);
  AllAlarmTexts_[2][0x2000] = std::make_tuple("b_TT06_CO2_IOError_FS", "b_TT06_CO2_IOError_FS", true);
  AllAlarmTexts_[2][0x4000] = std::make_tuple("b_Compressor_LP_AL", "b_Compressor_LP_AL", false);
  AllAlarmTexts_[2][0x8000] = std::make_tuple("b_Compressor_LP_FS", "b_Compressor_LP_FS", true);

  AllAlarmTexts_[3][0x0001] = std::make_tuple("b_Compressor_HP_AL", "b_Compressor_HP_AL", false);
  AllAlarmTexts_[3][0x0002] = std::make_tuple("b_Compressor_HP_FS", "b_Compressor_HP_FS", true);
  AllAlarmTexts_[3][0x0004] = std::make_tuple("b_Compressor_Cnt_Alarm", "b_Compressor_Cnt_Alarm", false);
  AllAlarmTexts_[3][0x0008] = std::make_tuple("bEV1_Error_FS", "bEV1_Error_FS", true);
  AllAlarmTexts_[3][0x0010] = std::make_tuple("bEV2_Error_FS", "bEV2_Error_FS", true);
  AllAlarmTexts_[3][0x0020] = std::make_tuple("bEV3_Error_FS", "bEV3_Error_FS", true);
  AllAlarmTexts_[3][0x0040] = std::make_tuple("b_dT03_CO2_HT_FS", "b_dT03_CO2_HT_FS", true);
  AllAlarmTexts_[3][0x0080] = std::make_tuple("b_Chiller_SH05_TS", "b_Chiller_SH05_TS", false);
  AllAlarmTexts_[3][0x0100] = std::make_tuple("Reserve", "Reserve", false);
  AllAlarmTexts_[3][0x0200] = std::make_tuple("Reserve", "Reserve", false);
  AllAlarmTexts_[3][0x0400] = std::make_tuple("Reserve", "Reserve", false);
  AllAlarmTexts_[3][0x0800] = std::make_tuple("Reserve", "Reserve", false);
  AllAlarmTexts_[3][0x1000] = std::make_tuple("Reserve", "Reserve", false);
  AllAlarmTexts_[3][0x2000] = std::make_tuple("Reserve", "Reserve", false);
  AllAlarmTexts_[3][0x4000] = std::make_tuple("Reserve", "Reserve", false);
  AllAlarmTexts_[3][0x8000] = std::make_tuple("Reserve", "Reserve", false);
}
