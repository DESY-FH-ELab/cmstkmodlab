#include <unistd.h>

#include "PfeifferTPG262Fake.h"

///
///
///
PfeifferTPG262Fake::PfeifferTPG262Fake( const ioport_t ioPort )
    :VPfeifferTPG262(ioPort)
{

}

stat_t PfeifferTPG262Fake::GetPressure(float & pressure1, float & pressure2)
{
  pressure1 = 5.e-2;
  pressure2 = 5.e-2;
  return stat_t(5, 5);
}
