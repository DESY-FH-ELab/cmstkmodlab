#include <unistd.h>

//#####################
// TODO:
// query error codes
//#####################



#include "Iota300Fake.h"

///
///
///
Iota300Fake::Iota300Fake( const ioport_t ioPort )
    :VIota300(ioPort)
{
  circulatorStatus = false;
  parameterProp_ = 77.2;
  parameterIntl_ = 3456;
  parameterDiff_ = 128;
}

///
/// returns success flag
///

unsigned int Iota300Fake::GetPumpPressure( void ) const {
  
  std::cout << " [Iota300Fake::GetPumpPressure] -- FAKE: Returning: 2" << std::endl;
  usleep( 10000 );
  return 2;

}

//////////////////
//////////////////
//////////////////
void Iota300Fake::Device_Init( void ) {

  
}
