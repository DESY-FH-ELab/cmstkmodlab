#include "ConradManager.h"


ConradManager::ConradManager(ConradModel* cnrd1)
{

  ConradModel_ = cnrd1;
  channelNumber = 0;
  liveTimer_ = new QTimer(this);
  liveTimer_ -> setSingleShot(true);

  //connect(liveTimer_, SIGNAL(timeout()), this, SIGNAL(updateVacuumChannelState(channelNumber, state)));
  connect(liveTimer_, SIGNAL(timeout()), this, SLOT(vacuumToggled()));
  
}

/// toggleVacuum slot description
void ConradManager::toggleVacuum(int chNumber)
{
  NQLog("ConradManager") << ": toggling vacuum with ConradManager";
  emit disableVacuumButton();

  if (ConradModel_ -> getSwitchState(chNumber) == 0){
    
    //NQLog("ConradManager") << ": attempt to turn ON the vacuum on channel " << chNumber;
    ConradModel_ -> setSwitchEnabled(chNumber, true);
    channelNumber = chNumber;

    // here will be a QtTimer for about 2 secs
    liveTimer_ -> start(togglingVacuumDelay);

    //emit updateVacuumChannelState(chNumber - 1, true);
                
  }else if (ConradModel_ -> getSwitchState(chNumber) == 1){
    
    //NQLog("ConradManager") << ": attempt to turn OFF the vacuum on channel " << chNumber;
    ConradModel_ -> setSwitchEnabled(chNumber, false);
    channelNumber = chNumber;

    // here will be a QtTimer for about 2 secs
    liveTimer_ -> start(togglingVacuumDelay);
    
    // here will be a QtTimer for about 2 secs

    //emit updateVacuumChannelState(chNumber - 1, false);
  }else{    
    NQLog("ConradManager") << ": ERROR! Toggling vacuum error : SwithState != 0|1";
  }
  
  //looped status checking with qt timer
  
}

void ConradManager::vacuumToggled()
{
  //NQLog("ConradManager") << ": updating channel " << channelNumber << " status to " << ConradModel_ -> getSwitchState(channelNumber);
  emit updateVacuumChannelState(channelNumber - 1, ConradModel_ -> getSwitchState(channelNumber));
  emit enableVacuumButton();
}

void ConradManager::updateVacuumChannelsStatus()
{  
  NQLog("ConradManager") << ": updating Vacuum Channels Status.";
  for (int i = 0; i < 3; ++i)
    {
       emit updateVacuumChannelState(i, ConradModel_ -> getSwitchState(i + 1));
    }
}

//maybe need checkStatus SLOT
