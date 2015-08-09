#include <cmath>

#include "LStepExpressFake.h"

LStepExpressFake::LStepExpressFake( const ioport_t ioPort )
  :VLStepExpress(ioPort)
{

}

LStepExpressFake::~LStepExpressFake()
{

}

unsigned int LStepExpressFake::GetStatus() const
{
  return 0;
}
