#include <cmath>
#include <iostream>

#include "LeyboldGraphixThreeFake.h"

LeyboldGraphixThreeFake::LeyboldGraphixThreeFake( const ioport_t ioPort )
  :VLeyboldGraphixThree(ioPort)
{
  pressure_[0] = 100.0;
  pressure_[1] = 100.1;
  pressure_[2] = 100.2;
}

LeyboldGraphixThreeFake::~LeyboldGraphixThreeFake()
{

}

std::string LeyboldGraphixThreeFake::GetVersion() const
{
  return std::string("HW:X.XX SW:X.XX");
}

double LeyboldGraphixThreeFake::GetPressure(int channel) const
{
  if (channel<0 || channel>2) return -1;
  return pressure_[channel];
}
