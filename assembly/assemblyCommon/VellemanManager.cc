/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//       Modeled on cmstkmodlab/assembly/assemblyCommon/ConradManager.cc       //
//                         Last Updated August 26, 2019                        //
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <VellemanManager.h>
#include <nqlogger.h>

#include <cassert>

VellemanManager::VellemanManager(VellemanModel* const vlmn)
  : vellemanModel_(vlmn)
  , relayNumber_(0)
{
  vellemanModel();

  liveTimer_ = new QTimer(this);
  liveTimer_->setSingleShot(true);

  connect(liveTimer_, SIGNAL(timeout()), this, SLOT(vacuumToggled()));

  NQLog("VellemanManager", NQLog::Debug) << "constructed";
}

VellemanModel* VellemanManager::vellemanModel() const
{
  // NOTE: change assert statement???
  assert(vellemanModel_);

  return vellemanModel_;
}

// Toggle vacuum (SLOT)
void VellemanManager::toggleVacuum(const int relNumber)
{
  if (vellemanModel()->getRelayState(relNumber) == 0)
    {
      NQLog("VellemanManager", NQLog::Debug) << "toggleVacuum(" << relNumber+1 << ")" // NOTE -> relNumber should be relay-1 right now?
					     << ": emitting signal \"disableVacuumButton\"";

      emit disableVacuumButton();

      // NQLog("VellemanManager") << ": attempt to turn ON the vacuum on relay " << relNumber + 1;
      vellemanModel()->setRelayEnabled(relNumber, true);
      relayNumber_ = relNumber;

      // Here is a QTimer for about 2 seconds
      liveTimer_->start(togglingVacuumDelay);
    }
  else if(vellemanModel()->getRelayState(relNumber) == 1)
    {
      NQLog("VellemanManager", NQLog::Debug) << "toggleVacuum(" << relNumber + 1 << ")" // NOTE -> relNumber should be relay-1 right now?
					     << ": emitting signal \"disalbeVacuumButton\"";

      emit disableVacuumButton();

      //NQLog("VellemanManager") << ": attempt to turn OFF the vacuum on relay " << relNumber + 1;
      vellemanModel()->setRelayEnabled(relNumber, false);
      relayNumber_ = relNumber;

      // Here is a QTimer for about 2 seconds
      liveTimer_->start(togglingVacuumDelay);
    }
  else
    {
      NQLog("VellemanManager", NQLog::Critical) << "toggleVacuum(" << relNumber + 1 << ")" // NOTE -> relNumber should be relay-1 right now?
						<< ": ERROR! Toggling vacuum error : RelayState != 0|1";
    }
  // looped status checking with qt timer --> NOTE: what does this mean??
}

void VellemanManager::vacuumToggled()
{
  NQLog("VellemanManager", NQLog::Debug) << "vacuumToggled"
					 << ": emitting signal \"vacuumRelayState(" << relayNumber_ + 1 << ", " << vellemanModel()->getRelayState(relayNumber_) << ")\"";

  emit vacuumRelayState(relayNumber_, vellemanModel()->getRelayState(relayNumber_));

  NQLog("VellemanManager", NQLog::Debug) << "vacuumToggled"
					 << ": emitting signal \"enableVacuumButton\"";

  emit enableVacuumButton();

  NQLog("VellemanManager", NQLog::Debug) << "vacuumToggled"
					 << ": emitting signal \"vacuum_toggled\"";

  emit vacuum_toggled();
}

void VellemanManager::transmit_vacuumRelayState(const int relNumber)
{
  NQLog("VellemanManager", NQLog::Debug) << "transmit_vacuumRelayState(" << relNumber << ")"
					 << ": emitting signal \"vacuumRelayState("
					 << relNumber << ", " << vellemanModel()->getRelayState(relNumber) << ")\"";

  emit vacuumRelayState(relNumber, vellemanModel()->getRelayState(relNumber));
}


// maybe need checkStatus SLOT (NOTE FROM CONRAD MANAGER)

void VellemanManager::enableVacuum(const int relNumber)
{
  const auto state = vellemanModel()->getRelayState(relNumber);

  if(state==0) // vacuum line is OFF
    {
      vellemanModel()->setRelayEnabled(relNumber, true);
      relayNumber_ = relNumber;

      liveTimer_->start(togglingVacuumDelay);
    }
  else if(state==1) // vacuum line is ON (READY)
    {
      relayNumber_ = relNumber;

      NQLog("VellemanManager", NQLog::Debug) << "enableVacuum(" << relNumber << ")"
					     << ": emitting signal \"vacuum_enabled\"";

      emit vacuum_enabled();
    }
  else
    {
      NQLog("VellemanManager", NQLog::Critical) << "toggleVacuum(" << relNumber << ")"
						<< ": ERROR! Toggling vacuum error : RelayState != 0|1";

      NQLog("VellemanManager", NQLog::Debug) << "enableVacuum(" << relNumber << ")"
					     << ": emitting signal \"vacuum_error\"";

      emit vacuum_error();
    }

  return;
}

void VellemanManager::disableVacuum(const int relNumber)
{
  const auto state = vellemanModel()->getRelayState(relNumber);

  if(state==0) // vacuum line is OFF
    {
      relayNumber_ = relNumber;

      NQLog("VellemanManager", NQLog::Debug) << "disableVacuum(" << relNumber << ")"
					     << ": emitting signal \"vacuum_disabled\"";

      emit vacuum_disabled();
    }
  else if(state == 1) // vacuum line is ON (READY)
    {
      vellemanModel()->setRelayEnabled(relNumber, false);
      relayNumber_ = relNumber;

      liveTimer_->start(togglingVacuumDelay);
    }
  else
    {
      NQLog("VellemanManager", NQLog::Critical) << "toggleVacuum(" << relNumber << ")"
						<< ": ERROR! Toggling vacuum error : RelayState (" << state << ") != 0|1";

      NQLog("VellemanManager", NQLog::Debug) << "disableVacuum(" << relNumber << ")"
					     << ": emitting signal \"vacuum_error\"";

      emit vacuum_error();
    }
  
  return;
}
