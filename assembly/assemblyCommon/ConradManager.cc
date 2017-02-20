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
  NQLog("AssemblyManager") << ": toggling vacuum with ConradManager";

  if (ConradModel_ -> getSwitchState(channelNumber) == 0){
    ConradModel_ -> setSwitchEnabled(channelNumber, true);
                
  }else if (ConradModel_ -> getSwitchState(channelNumber) == 1){
    ConradModel_ -> setSwitchEnabled(channelNumber, false);
  }

  //looped status checking with qt timer
  
}


//maybe need checkStatus SLOT
