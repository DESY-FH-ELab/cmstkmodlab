/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <cassert>

#include <nqlogger.h>

#include <RelayCardManager.h>

RelayCardManager::RelayCardManager(VRelayCardModel* const model)
 : relayCardModel_(model)
 , channelNumber_(0)
{
	relayCardModel();

  liveTimer_ = new QTimer(this);
  liveTimer_->setSingleShot(true);

  connect(liveTimer_, SIGNAL(timeout()), this, SLOT(vacuumToggled()));

  NQLog("RelayCardManager", NQLog::Debug) << "constructed";
}

RelayCardManager::~RelayCardManager()
{
	delete liveTimer_;
}

VRelayCardModel* RelayCardManager::relayCardModel() const
{
  assert(relayCardModel_);

  return relayCardModel_;
}

/// toggleVacuum slot description
void RelayCardManager::toggleVacuum(const int chNumber)
{
  if(liveTimer_->isActive())
  {
    NQLog("RelayCardManager", NQLog::Critical) << "toggleVacuum(" << chNumber << ")"
        << ": ERROR! Another line is still in transition.";
    return;
  }

  State switchState;
  try{
    switchState = relayCardModel()->getSwitchState(chNumber);
  } catch (...){
    NQLog("RelayCardManager", NQLog::Critical) << "toggleVacuum(" << chNumber << ")"
         << ": ERROR! Toggling vacuum error : Requested channel is likely unassigned.";
    return;
  }

  if(switchState == 0)
  {
    NQLog("RelayCardManager", NQLog::Debug) << "toggleVacuum(" << chNumber << ")"
       << ": emitting signal \"disableVacuumButton\"";

    emit disableVacuumButton();

    //NQLog("RelayCardManager") << ": attempt to turn ON the vacuum on channel " << chNumber;
    relayCardModel()->setSwitchEnabled(chNumber, true);
    channelNumber_ = chNumber;

    emit vacuumChannelState(channelNumber_, SwitchState::CHANNEL_SWITCHING);

    // here will be a QtTimer for about 2 secs
    liveTimer_->start(togglingVacuumDelay);

    emit DBLogMessage("Turned vacuum ON");
  }
  else if(switchState == 1)
  {
    NQLog("RelayCardManager", NQLog::Debug) << "toggleVacuum(" << chNumber << ")"
       << ": emitting signal \"disableVacuumButton\"";

    emit disableVacuumButton();

    //NQLog("RelayCardManager") << ": attempt to turn OFF the vacuum on channel " << chNumber;
    relayCardModel()->setSwitchEnabled(chNumber, false);
    channelNumber_ = chNumber;

    emit vacuumChannelState(channelNumber_, SwitchState::CHANNEL_SWITCHING);

    // here will be a QtTimer for about 2 secs
    liveTimer_->start(togglingVacuumDelay);

    emit DBLogMessage("Turned vacuum OFF");
  }
  else
  {
    NQLog("RelayCardManager", NQLog::Critical) << "toggleVacuum(" << chNumber << ")"
       << ": ERROR! Toggling vacuum error : SwithState != 0|1";
  }

  // looped status checking with qt timer
}

void RelayCardManager::vacuumToggled()
{
  NQLog("RelayCardManager", NQLog::Debug) << "vacuumToggled"
     << ": emitting signal \"vacuumChannelState(" << channelNumber_ << ", " << relayCardModel()->getSwitchState(channelNumber_) << ")\"";

  SwitchState state;
  if(relayCardModel()->getDeviceState() == State::OFF) {
    state = SwitchState::DEVICE_OFF;
  } else {
    switch(relayCardModel()->getSwitchState(channelNumber_)) {
      case OFF:
        state = SwitchState::CHANNEL_OFF;
        break;
      case READY:
        state = SwitchState::CHANNEL_ON;
        break;
      case INITIALIZING:
        state = SwitchState::CHANNEL_SWITCHING;
        break;
      case CLOSING:
        state = SwitchState::CHANNEL_SWITCHING;
        break;
    }
  }

  emit vacuumChannelState(channelNumber_, state);

  NQLog("RelayCardManager", NQLog::Debug) << "vacuumToggled"
     << ": emitting signal \"enableVacuumButton\"";

  emit enableVacuumButton();

  NQLog("RelayCardManager", NQLog::Debug) << "vacuumToggled"
     << ": emitting signal \"vacuum_toggled\"";

  emit vacuum_toggled();
}

void RelayCardManager::transmit_vacuumChannelState(const int chNumber)
{
  NQLog("RelayCardManager", NQLog::Debug) << "transmit_vacuumChannelState(" << chNumber << ")"
     << ": emitting signal \"vacuumChannelState("
     << chNumber << ", " << relayCardModel()->getSwitchState(chNumber) << ")\"";

  SwitchState state;
  if(relayCardModel()->getDeviceState() == State::OFF) {
    state = SwitchState::DEVICE_OFF;
  } else {
    switch(relayCardModel()->getSwitchState(chNumber)) {
      case OFF:
        state = SwitchState::CHANNEL_OFF;
        break;
      case READY:
        state = SwitchState::CHANNEL_ON;
        break;
      case INITIALIZING:
        state = SwitchState::CHANNEL_SWITCHING;
        break;
      case CLOSING:
        state = SwitchState::CHANNEL_SWITCHING;
        break;
    }
  }

  emit vacuumChannelState(chNumber, state);
}

// maybe need checkStatus SLOT

void RelayCardManager::enableVacuum(const int chNumber)
{
  if(liveTimer_->isActive())
  {
    NQLog("RelayCardManager", NQLog::Critical) << "enableVacuum(" << chNumber << ")"
        << ": ERROR! Another line is still in transition.";
    return;
  }

  State state;
  try{
    state = relayCardModel()->getSwitchState(chNumber);
  } catch (...){
    NQLog("RelayCardManager", NQLog::Critical) << "enableVacuum(" << chNumber << ")"
         << ": ERROR! Toggling vacuum error : Requested channel is likely unassigned.";
    return;
  }

  if(state == 0) // vacuum line is OFF
  {
  	relayCardModel()->setSwitchEnabled(chNumber, true);
    channelNumber_ = chNumber;

    emit vacuumChannelState(channelNumber_, SwitchState::CHANNEL_SWITCHING);

    liveTimer_->start(togglingVacuumDelay);

    emit DBLogMessage("Turned vacuum ON");
  }
  else if(state == 1) // vacuum line is ON
  {
    channelNumber_ = chNumber;

    NQLog("RelayCardManager", NQLog::Debug) << "enableVacuum(" << chNumber << ")"
       << ": emitting signal \"vacuum_enabled\"";

    emit vacuum_enabled();
  }
  else
  {
    NQLog("RelayCardManager", NQLog::Critical) << "enableVacuum(" << chNumber << ")"
       << ": ERROR! Toggling vacuum error : SwithState != 0|1";

    NQLog("RelayCardManager", NQLog::Debug) << "enableVacuum(" << chNumber << ")"
       << ": emitting signal \"vacuum_error\"";

    emit vacuum_error();
  }

  return;
}

void RelayCardManager::disableVacuum(const int chNumber)
{
  if(liveTimer_->isActive())
  {
    NQLog("RelayCardManager", NQLog::Critical) << "disableVacuum(" << chNumber << ")"
        << ": ERROR! Another line is still in transition.";
    return;
  }

  State state;
  try{
    state = relayCardModel()->getSwitchState(chNumber);
  } catch (...){
    NQLog("RelayCardManager", NQLog::Critical) << "disableVacuum(" << chNumber << ")"
         << ": ERROR! Toggling vacuum error : Requested channel is likely unassigned.";
    return;
  }

  if(state == 0) // vacuum line is OFF
  {
    channelNumber_ = chNumber;

    NQLog("RelayCardManager", NQLog::Debug) << "disableVacuum(" << chNumber << ")"
       << ": emitting signal \"vacuum_disabled\"";

    emit vacuum_disabled();
  }
  else if(state == 1) // vacuum line is ON
  {
  	relayCardModel()->setSwitchEnabled(chNumber, false);
    channelNumber_ = chNumber;

    emit vacuumChannelState(channelNumber_, SwitchState::CHANNEL_SWITCHING);

    liveTimer_->start(togglingVacuumDelay);

    emit DBLogMessage("Turned vacuum OFF");
  }
  else
  {
    NQLog("RelayCardManager", NQLog::Critical) << "toggleVacuum(" << chNumber << ")"
       << ": ERROR! Toggling vacuum error : SwitchState (" << state << ") != 0|1";

    NQLog("RelayCardManager", NQLog::Debug) << "disableVacuum(" << chNumber << ")"
       << ": emitting signal \"vacuum_error\"";

    emit vacuum_error();
  }

  return;
}
