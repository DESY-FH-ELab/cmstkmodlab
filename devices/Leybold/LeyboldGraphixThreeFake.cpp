#include <cmath>
#include <iostream>

#include "LeyboldGraphixThreeFake.h"

LeyboldGraphixThreeFake::LeyboldGraphixThreeFake( const ioport_t ioPort )
  :VLeyboldGraphixThree(ioPort)
{

}

LeyboldGraphixThreeFake::~LeyboldGraphixThreeFake()
{

}

std::string LeyboldGraphixThreeFake::GetVersion() const
{
  return std::string("HW:X.XX SW:X.XX");
}
