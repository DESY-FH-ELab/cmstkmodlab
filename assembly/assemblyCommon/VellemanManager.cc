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
  , channelNumber_(0)
{
  vellemanModel();

  liveTimer_ = new QTimer(this);
  liveTimer_->setSingleShot(true);

  connect(liveTimer_, SIGNAL(timeout()), this, SLOT(vacuumToggled()));

  NQLog("VellemanManager", NQLog::Debug) << "constructed";
}

VellemanModel* VellemanManager::vellemanModel() const
{
  // NOTE: changed from assert statement to guard against issues caused by program quitting from
  //       failed assert (even if possibility is unlikely)
  if(!vellemanModel_)
    {
      NQLog("VellemanManager", NQLog::Critical) << "vellemanModel()"
						<< ": ERROR! Pointer to VellemanModel instance is NULL";
    }

  return vellemanModel_;
}

// Toggle vacuum (SLOT)
void VellemanManager::toggleVacuum(const int chNumber)
{
  if (vellemanModel()->getChannelState(chNumber) == 0)
    {
      NQLog("VellemanManager", NQLog::Debug) << "toggleVacuum(" << chNumber << ")" // NOTE -> chNumber should be same as relay number right now
					     << ": emitting signal \"disableVacuumButton\"";

      emit disableVacuumButton();

      // NQLog("VellemanManager") << ": attempt to turn ON the vacuum on channel " << chNumber;
      vellemanModel()->setChannelEnabled(chNumber, true);
      channelNumber_ = chNumber;

      // Here is a QTimer for about 2 seconds
      liveTimer_->start(togglingVacuumDelay);
    }
  else if(vellemanModel()->getChannelState(chNumber) == 1)
    {
      NQLog("VellemanManager", NQLog::Debug) << "toggleVacuum(" << chNumber  << ")" // NOTE -> chNumber should be same as relay number right now
					     << ": emitting signal \"disableVacuumButton\"";

      emit disableVacuumButton();

      //NQLog("VellemanManager") << ": attempt to turn OFF the vacuum on channel " << chNumber;
      vellemanModel()->setChannelEnabled(chNumber, false);
      channelNumber_ = chNumber;

      // Here is a QTimer for about 2 seconds
      liveTimer_->start(togglingVacuumDelay);
    }
  else
    {
      NQLog("VellemanManager", NQLog::Critical) << "toggleVacuum(" << chNumber << ")" // NOTE -> chNumber should be same as relay number right now
						<< ": ERROR! Toggling vacuum error : ChannelState != 0|1";
    }
  // looped status checking with qt timer --> NOTE: what does this mean??
}

void VellemanManager::vacuumToggled()
{
  NQLog("VellemanManager", NQLog::Debug) << "vacuumToggled"
					 << ": emitting signal \"vacuumChannelState(" << channelNumber_ << ", " << vellemanModel()->getChannelState(channelNumber_) << ")\"";

  emit vacuumChannelState(channelNumber_, vellemanModel()->getChannelState(channelNumber_));

  NQLog("VellemanManager", NQLog::Debug) << "vacuumToggled"
					 << ": emitting signal \"enableVacuumButton\"";

  emit enableVacuumButton();

  NQLog("VellemanManager", NQLog::Debug) << "vacuumToggled"
					 << ": emitting signal \"vacuum_toggled\"";

  emit vacuum_toggled();
}

void VellemanManager::transmit_vacuumChannelState(const int chNumber)
{
  NQLog("VellemanManager", NQLog::Debug) << "transmit_vacuumChannelState(" << chNumber << ")"
					 << ": emitting signal \"vacuumChannelState("
					 << chNumber << ", " << vellemanModel()->getChannelState(chNumber) << ")\"";

  emit vacuumChannelState(chNumber, vellemanModel()->getChannelState(chNumber));
}


// maybe need checkStatus SLOT (NOTE FROM CONRAD MANAGER)

void VellemanManager::enableVacuum(const int chNumber)
{
  const auto state = vellemanModel()->getChannelState(chNumber);

  if(state==0) // vacuum line is OFF
    {
      vellemanModel()->setChannelEnabled(chNumber, true);
      channelNumber_ = chNumber;

      liveTimer_->start(togglingVacuumDelay);
    }
  else if(state==1) // vacuum line is ON (READY)
    {
      channelNumber_ = chNumber;

      NQLog("VellemanManager", NQLog::Debug) << "enableVacuum(" << chNumber << ")"
					     << ": emitting signal \"vacuum_enabled\"";

      emit vacuum_enabled();
    }
  else
    {
      NQLog("VellemanManager", NQLog::Critical) << "toggleVacuum(" << chNumber << ")"
						<< ": ERROR! Toggling vacuum error : ChannelState != 0|1";

      NQLog("VellemanManager", NQLog::Debug) << "enableVacuum(" << chNumber << ")"
					     << ": emitting signal \"vacuum_error\"";

      emit vacuum_error();
    }

  return;
}

void VellemanManager::disableVacuum(const int chNumber)
{
  const auto state = vellemanModel()->getChannelState(chNumber);

  if(state==0) // vacuum line is OFF
    {
      channelNumber_ = chNumber;

      NQLog("VellemanManager", NQLog::Debug) << "disableVacuum(" << chNumber << ")"
					     << ": emitting signal \"vacuum_disabled\"";

      emit vacuum_disabled();
    }
  else if(state == 1) // vacuum line is ON (READY)
    {
      vellemanModel()->setChannelEnabled(chNumber, false);
      channelNumber_ = chNumber;

      liveTimer_->start(togglingVacuumDelay);
    }
  else
    {
      NQLog("VellemanManager", NQLog::Critical) << "toggleVacuum(" << chNumber << ")"
						<< ": ERROR! Toggling vacuum error : ChannelState (" << state << ") != 0|1";

      NQLog("VellemanManager", NQLog::Debug) << "disableVacuum(" << chNumber << ")"
					     << ": emitting signal \"vacuum_error\"";

      emit vacuum_error();
    }
  
  return;
}
