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

ConradManager::ConradManager(ConradModel* cnrd1)
{
  ConradModel_ = cnrd1;
  channelNumber = 0;

  liveTimer_ = new QTimer(this);
  liveTimer_ -> setSingleShot(true);

//  connect(liveTimer_, SIGNAL(timeout()), this, SIGNAL(updateVacuumChannelState(channelNumber, state)));
  connect(liveTimer_, SIGNAL(timeout()), this, SLOT(vacuumToggled()));

  NQLog("ConradManager", NQLog::Debug) << "constructed";
}

/// toggleVacuum slot description
void ConradManager::toggleVacuum(int chNumber)
{
  NQLog("ConradManager", NQLog::Debug) << "toggleVacuum(" << chNumber << ")"
     << ": emitting signal \"disableVacuumButton\"";

  emit disableVacuumButton();

  if(ConradModel_->getSwitchState(chNumber) == 0)
  {
    //NQLog("ConradManager") << ": attempt to turn ON the vacuum on channel " << chNumber;
    ConradModel_ -> setSwitchEnabled(chNumber, true);
    channelNumber = chNumber;

    // here will be a QtTimer for about 2 secs
    liveTimer_ -> start(togglingVacuumDelay);

    //emit updateVacuumChannelState(chNumber - 1, true);
  }
  else if(ConradModel_->getSwitchState(chNumber) == 1)
  {
    //NQLog("ConradManager") << ": attempt to turn OFF the vacuum on channel " << chNumber;
    ConradModel_ -> setSwitchEnabled(chNumber, false);
    channelNumber = chNumber;

    // here will be a QtTimer for about 2 secs
    liveTimer_ -> start(togglingVacuumDelay);

    //emit updateVacuumChannelState(chNumber - 1, false);
  }
  else
  {
    NQLog("ConradManager", NQLog::Critical) << "toggleVacuum(" << chNumber << ")"
       << ": ERROR! Toggling vacuum error : SwithState != 0|1";
  }

  //looped status checking with qt timer
}

void ConradManager::vacuumToggled()
{
  NQLog("ConradManager", NQLog::Debug) << "vacuumToggled"
     << ": emitting signal \"updateVacuumChannelState("
     << (channelNumber-1) << ", " << ConradModel_->getSwitchState(channelNumber) << ")\"";

  emit updateVacuumChannelState(channelNumber-1, ConradModel_->getSwitchState(channelNumber));

  NQLog("ConradManager", NQLog::Debug) << "vacuumToggled"
     << ": emitting signal \"enableVacuumButton\"";

  emit enableVacuumButton();
}

void ConradManager::updateVacuumChannelsStatus()
{  
  for(int i=0; i<3; ++i)
  {
    NQLog("ConradManager", NQLog::Debug) << "vacuumToggled"
       << ": emitting signal \"updateVacuumChannelState("
       << i << ", " << ConradModel_->getSwitchState(i+1) << ")\"";

    emit updateVacuumChannelState(i, ConradModel_->getSwitchState(i+1));
  }
}

// maybe need checkStatus SLOT
