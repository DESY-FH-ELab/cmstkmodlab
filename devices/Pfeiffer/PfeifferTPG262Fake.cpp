#include <unistd.h>

#include "PfeifferTPG262Fake.h"

///
///
///
PfeifferTPG262Fake::PfeifferTPG262Fake( const ioport_t ioPort )
    :VPfeifferTPG262(ioPort)
{

}

int PfeifferTPG262Fake::GetErrorStatus( void ) const
{
	return tgp262NoError;
}

bool PfeifferTPG262Fake::GetPressure1(reading_t & reading)
{
	reading.first = tpg262GaugeOkay;
	reading.second = 5.e-3;
	return true;
}

bool PfeifferTPG262Fake::GetPressure2(reading_t & reading)
{
	reading.first = tpg262GaugeOkay;
	reading.second = 5.e-3;
	return true;
}

bool PfeifferTPG262Fake::GetPressures(reading_t & reading1, reading_t & reading2)
{
	reading1.first = tpg262GaugeOkay;
	reading1.second = 5.e-3;
	reading2.first = tpg262GaugeOkay;
	reading2.second = 5.e-3;
	return true;
}
