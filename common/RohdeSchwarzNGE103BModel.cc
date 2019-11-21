/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
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

#include "RohdeSchwarzNGE103BModel.h"

/*
  RohdeSchwarzNGE103BModel implementation
 */
RohdeSchwarzNGE103BModel::RohdeSchwarzNGE103BModel(const char* port,
                                                   float updateInterval,
                                                   QObject * /*parent*/)
 :QObject(),
  AbstractDeviceModel<RohdeSchwarzNGE103B_t>(),
  RohdeSchwarzNGE103B_PORT(port),
  updateInterval_(updateInterval)
{
  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()) );

  setDeviceEnabled(true);
}

/**
  Sets up the communication with the RohdeSchwarzNGE103B power supply and retrieves the
  settings and read-outs.
  */
void RohdeSchwarzNGE103BModel::initialize()
{
  setDeviceState(INITIALIZING);

  renewController(RohdeSchwarzNGE103B_PORT);

  bool enabled = ( controller_ != NULL ) && ( controller_->DeviceAvailable() );

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

/// \see AbstractDeviceModel::setDeviceState
void RohdeSchwarzNGE103BModel::setDeviceState( State state )
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

/**
  Updates the cached information about the RohdeSchwarzNGE103B power supply and signals any
  changes.
  */
void RohdeSchwarzNGE103BModel::updateInformation()
{
  NQLog("RohdeSchwarzNGE103BModel", NQLog::Debug) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
      NQLog("RohdeSchwarzNGE103BModel", NQLog::Debug) << " running in main application thread";
  } else {
      NQLog("RohdeSchwarzNGE103BModel", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  if ( state_ == READY ) {

  }
}

/// Attempts to enable/disable the (communication with) the RohdeSchwarzNGE103B power supply.
void RohdeSchwarzNGE103BModel::setDeviceEnabled(bool enabled)
{
  AbstractDeviceModel<RohdeSchwarzNGE103B_t>::setDeviceEnabled(enabled);
}

void RohdeSchwarzNGE103BModel::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}
