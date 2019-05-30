/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <ConradManager.h>
#include <nqlogger.h>

#include <cassert>

ConradManager::ConradManager(ConradModel* const cnrd)
 : conradModel_(cnrd)
 , channelNumber_(0)
{
  conradModel();

  liveTimer_ = new QTimer(this);
  liveTimer_->setSingleShot(true);

  connect(liveTimer_, SIGNAL(timeout()), this, SLOT(vacuumToggled()));

  NQLog("ConradManager", NQLog::Debug) << "constructed";
}

ConradModel* ConradManager::conradModel() const
{
  assert(conradModel_);

  return conradModel_;
}

/// toggleVacuum slot description
void ConradManager::toggleVacuum(const int chNumber)
{
  if(conradModel()->getSwitchState(chNumber) == 0)
  {
    NQLog("ConradManager", NQLog::Debug) << "toggleVacuum(" << chNumber << ")"
       << ": emitting signal \"disableVacuumButton\"";

    emit disableVacuumButton();

    //NQLog("ConradManager") << ": attempt to turn ON the vacuum on channel " << chNumber;
    conradModel()->setSwitchEnabled(chNumber, true);
    channelNumber_ = chNumber;

    // here will be a QtTimer for about 2 secs
    liveTimer_->start(togglingVacuumDelay);
  }
  else if(conradModel()->getSwitchState(chNumber) == 1)
  {
    NQLog("ConradManager", NQLog::Debug) << "toggleVacuum(" << chNumber << ")"
       << ": emitting signal \"disableVacuumButton\"";

    emit disableVacuumButton();

    //NQLog("ConradManager") << ": attempt to turn OFF the vacuum on channel " << chNumber;
    conradModel()->setSwitchEnabled(chNumber, false);
    channelNumber_ = chNumber;

    // here will be a QtTimer for about 2 secs
    liveTimer_->start(togglingVacuumDelay);
  }
  else
  {
    NQLog("ConradManager", NQLog::Critical) << "toggleVacuum(" << chNumber << ")"
       << ": ERROR! Toggling vacuum error : SwithState != 0|1";
  }

  // looped status checking with qt timer
}

void ConradManager::vacuumToggled()
{
  NQLog("ConradManager", NQLog::Debug) << "vacuumToggled"
     << ": emitting signal \"vacuumChannelState(" << channelNumber_ << ", " << conradModel()->getSwitchState(channelNumber_) << ")\"";

  emit vacuumChannelState(channelNumber_, conradModel()->getSwitchState(channelNumber_));

  NQLog("ConradManager", NQLog::Debug) << "vacuumToggled"
     << ": emitting signal \"enableVacuumButton\"";

  emit enableVacuumButton();

  NQLog("ConradManager", NQLog::Debug) << "vacuumToggled"
     << ": emitting signal \"vacuum_toggled\"";

  emit vacuum_toggled();
}

void ConradManager::transmit_vacuumChannelState(const int chNumber)
{
  NQLog("ConradManager", NQLog::Debug) << "transmit_vacuumChannelState(" << chNumber << ")"
     << ": emitting signal \"vacuumChannelState("
     << chNumber << ", " << conradModel()->getSwitchState(chNumber) << ")\"";

  emit vacuumChannelState(chNumber, conradModel()->getSwitchState(chNumber));
}

// maybe need checkStatus SLOT

void ConradManager::enableVacuum(const int chNumber)
{
  const auto state = conradModel()->getSwitchState(chNumber);

  if(state == 0) // vacuum line is OFF
  {
    conradModel()->setSwitchEnabled(chNumber, true);
    channelNumber_ = chNumber;

    liveTimer_->start(togglingVacuumDelay);
  }
  else if(state == 1) // vacuum line is ON
  {
    channelNumber_ = chNumber;

    NQLog("ConradManager", NQLog::Debug) << "enableVacuum(" << chNumber << ")"
       << ": emitting signal \"vacuum_enabled\"";

    emit vacuum_enabled();
  }
  else
  {
    NQLog("ConradManager", NQLog::Critical) << "toggleVacuum(" << chNumber << ")"
       << ": ERROR! Toggling vacuum error : SwithState != 0|1";

    NQLog("ConradManager", NQLog::Debug) << "enableVacuum(" << chNumber << ")"
       << ": emitting signal \"vacuum_error\"";

    emit vacuum_error();
  }

  return;
}

void ConradManager::disableVacuum(const int chNumber)
{
  const auto state = conradModel()->getSwitchState(chNumber);

  if(state == 0) // vacuum line is OFF
  {
    channelNumber_ = chNumber;

    NQLog("ConradManager", NQLog::Debug) << "disableVacuum(" << chNumber << ")"
       << ": emitting signal \"vacuum_disabled\"";

    emit vacuum_disabled();
  }
  else if(state == 1) // vacuum line is ON
  {
    conradModel()->setSwitchEnabled(chNumber, false);
    channelNumber_ = chNumber;

    liveTimer_->start(togglingVacuumDelay);
  }
  else
  {
    NQLog("ConradManager", NQLog::Critical) << "toggleVacuum(" << chNumber << ")"
       << ": ERROR! Toggling vacuum error : SwitchState (" << state << ") != 0|1";

    NQLog("ConradManager", NQLog::Debug) << "disableVacuum(" << chNumber << ")"
       << ": emitting signal \"vacuum_error\"";

    emit vacuum_error();
  }

  return;
}
