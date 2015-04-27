#include <unistd.h>

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

//#####################
// TODO:
// query error codes
//#####################

#include "CoriFlowFake.h"

///
///
///
CoriFlowFake::CoriFlowFake( const ioport_t ioPort )
    :VCoriFlow(ioPort)
{
     test_ = 0;
}

float CoriFlowFake::test( void ) const
{
  std::cout << " [CoriFlowFake::test] -- FAKE: Returning test = "
        << test_ << std::endl;
  usleep( 10000 );
  return test_;
}



void CoriFlowFake::Device_Init( void ) {

  
}
