#include "labcontrolcommand.h"

LabControlCommand::LabControlCommand(unsigned int time,
                                     int type,
                                     float parameter)
    : time_(time),
      type_(type),
      parameter_(parameter)
{

}
