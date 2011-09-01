#include <unistd.h>

#include <iostream>

#include "GMH3750Fake.h"

GMH3750Fake::GMH3750Fake( ioport_t ioPort )
    :VGMH3750(ioPort)
{

}

///
///
///
GMH3750Fake::~GMH3750Fake( void )
{

}

bool GMH3750Fake::Read(double & temperature)
{
    temperature = 16.0;

    usleep(500);

    return true;
}
