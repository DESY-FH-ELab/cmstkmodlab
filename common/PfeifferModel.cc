/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
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

#include "PfeifferModel.h"

/*
  PfeifferModel implementation
  */
PfeifferModel::PfeifferModel(const char* port,
			     float updateInterval,
			     QObject * /*parent*/) :
    QObject(),
    AbstractDeviceModel<PfeifferTPG262_t>(),
    Pfeiffer_PORT(port),
    status1_(VPfeifferTPG262::tpg262GaugeInvalidStatus),
    pressure1_(1013),
    status2_(VPfeifferTPG262::tpg262GaugeInvalidStatus),
    pressure2_(1013),
    updateInterval_(updateInterval)
{
    timer_ = new QTimer(this);
    timer_->setInterval(updateInterval_ * 1000);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));

    setDeviceEnabled(true);
}

VPfeifferTPG262::GaugeStatus PfeifferModel::getStatus1() const
{
    return status1_;
}

double PfeifferModel::getPressure1() const
{
    return pressure1_;
}

VPfeifferTPG262::GaugeStatus PfeifferModel::getStatus2() const
{
    return status2_;
}

double PfeifferModel::getPressure2() const
{
    return pressure2_;
}

/**
  Sets up the communication with the Pfeiffer TPG262 gauge and retrieves the
  settings and read-outs.
  */
void PfeifferModel::initialize() {

  setDeviceState(INITIALIZING);

  renewController(Pfeiffer_PORT);

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

/// \see AbstractDeviceModel::setDeviceState
void PfeifferModel::setDeviceState( State state ) {

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
  Updates the cached information about the Pfeiffer chiller and signals any
  changes.
  */
void PfeifferModel::updateInformation() {

    NQLog("PfeifferModel", NQLog::Debug) << "updateInformation()";

    if (thread()==QApplication::instance()->thread()) {
        NQLog("PfeifferModel", NQLog::Debug) << " running in main application thread";
    } else {
        NQLog("PfeifferModel", NQLog::Debug) << " running in dedicated DAQ thread";
    }

    if ( state_ == READY ) {

        VPfeifferTPG262::reading_t reading1, reading2;
        bool ok = controller_->GetPressures(reading1, reading2);

        if (ok) {
            if ( status1_ != reading1.first ||
                 pressure1_ != reading1.second ||
                 status2_ != reading2.first ||
                 pressure2_!= reading2.second ) {

                status1_ = reading1.first;
                pressure1_ = reading1.second;
                status2_ = reading2.first;
                pressure2_ = reading2.second;

                NQLog("PfeifferModel", NQLog::Spam) << "information changed";

                emit informationChanged();
            }
        }
    }
}

/// Attempts to enable/disable the (communication with) the Pfeiffer TPG262 gauge.
void PfeifferModel::setDeviceEnabled(bool enabled) {
  AbstractDeviceModel<PfeifferTPG262_t>::setDeviceEnabled(enabled);
}

void PfeifferModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}
