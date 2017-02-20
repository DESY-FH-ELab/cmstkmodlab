#include "nqlogger.h"

#include "ConradManager.h"
#include "ConradModel.h"


ConradManager::ConradManager(ConradModel* cnrd1)
{
  ConradModel_ = cnrd1;
  
}

/// toggleVacuum slot description
void ConradManager::toggleVacuum(int channelNumber)
{
  NQLog("ConradManager") << ": toggling vacuum with ConradManager";

  if (ConradModel_ -> getSwitchState(channelNumber) == 0){
    
    NQLog("ConradManager") << ": attempt to turn ON the vacuum";
    ConradModel_ -> setSwitchEnabled(channelNumber, true);
    
    // here will be a QtTimer for about 2 secs

    emit updateVacuumChannelState(channelNumber - 1, true);
                
  }else if (ConradModel_ -> getSwitchState(channelNumber) == 1){
    
    NQLog("ConradManager") << ": attempt to turn OFF the vacuum";
    ConradModel_ -> setSwitchEnabled(channelNumber, false);
    
    // here will be a QtTimer for about 2 secs

    emit updateVacuumChannelState(channelNumber - 1, false);
  }

  //looped status checking with qt timer
  
}


//maybe need checkStatus SLOT
