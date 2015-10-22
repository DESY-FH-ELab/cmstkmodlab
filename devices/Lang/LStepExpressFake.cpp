#include <iostream>

#include "LStepExpressFake.h"

LStepExpressFake::LStepExpressFake( const ioport_t ioPort )
  :VLStepExpress(ioPort)
{

}

LStepExpressFake::~LStepExpressFake()
{

}

void LStepExpressFake::SendCommand(const std::string & command)
{
  std::cout << "SendCommand: " << command << std::endl;
}
