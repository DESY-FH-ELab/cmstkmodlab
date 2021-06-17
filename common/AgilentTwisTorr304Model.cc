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

#include <chrono>
#include <thread>

#include <QApplication>

#include <nqlogger.h>

#include "AgilentTwisTorr304Model.h"

AgilentTwisTorr304Model::AgilentTwisTorr304Model(const char* port,
                                                 int updateInterval,
                                                 QObject * /*parent*/)
 : QObject(),
   AbstractDeviceModel<AgilentTwisTorr304_t>(),
   port_(port),
   updateInterval_(updateInterval)
{
  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));

  pumpState_ = false;
  pumpStatus_ = 0;
  errorCode_ = 0;

  setDeviceEnabled(true);
  setControlsEnabled(true);
}

/* DEVICE CONTROL */

/// Sets up the communication with the multimeter.
void AgilentTwisTorr304Model::initialize()
{
  setDeviceState(INITIALIZING);

  try {

    renewController(port_);

    // Give the device some time to process before spamming it with upcoming
    // commands
    std::this_thread::sleep_for(std::chrono::seconds(1));

    setDeviceState(READY);
  }
  catch (int e) {
    // TODO log failure
    setDeviceState(OFF);
  }
}

const std::string AgilentTwisTorr304Model::getPumpStatusText() const
{
  return controller_->GetPumpStatusText(static_cast<VAgilentTwisTorr304::StatusCode>(pumpStatus_));
}

void AgilentTwisTorr304Model::switchPumpOn()
{
  controller_->SwitchPumpOn();
}

void AgilentTwisTorr304Model::switchPumpOff()
{
  controller_->SwitchPumpOff();
}

void AgilentTwisTorr304Model::setDeviceEnabled(bool enabled)
{
  // Trivial reimplementation as slot.
  AbstractDeviceModel<AgilentTwisTorr304_t>::setDeviceEnabled(enabled);

  // scanTemperatures();
}

void AgilentTwisTorr304Model::setDeviceState(State state)
{
  if (state_ != state) {
    state_ = state;

    if ( state == READY )
      timer_->start();
    else
      timer_->stop();

    emit deviceStateChanged(state);
  }
}

void AgilentTwisTorr304Model::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}

/**
  Calls the Keithley controller to get the current reading on the temperatures.
  These are then cached and signals are emitted upon changes.
  */
void AgilentTwisTorr304Model::updateInformation()
{
  NQLog("AgilentTwisTorr304Model", NQLog::Debug) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    NQLog("AgilentTwisTorr304Model", NQLog::Debug) << " running in main application thread";
  } else {
    NQLog("AgilentTwisTorr304Model", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  if ( state_ == READY ) {

    bool newPumpState = controller_->GetPumpState();
    unsigned int newPumpStatus = controller_->GetPumpStatus();
    unsigned int newErrorCode = controller_->GetErrorCode();

    if (newPumpState != pumpState_ ||
        newPumpStatus != pumpStatus_ ||
        newErrorCode != errorCode_) {

      pumpState_ = newPumpState;
      pumpStatus_ = newPumpStatus;
      errorCode_ = newErrorCode;

      NQLog("AgilentTwisTorr304Model", NQLog::Spam) << "information changed";

      emit informationChanged();
    }
  }
}
