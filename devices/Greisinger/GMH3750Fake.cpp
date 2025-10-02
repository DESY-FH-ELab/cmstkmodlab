/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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
