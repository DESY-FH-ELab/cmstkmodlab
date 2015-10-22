#include <sstream>

#include "VLStepExpress.h"

VLStepExpress::VLStepExpress( const ioport_t ioPort )
{

}

VLStepExpress::~VLStepExpress()
{

}

char VLStepExpress::GetAxisName(VLStepExpress::Axis axis)
{
  switch (axis) {
  case 0:
    return 'x';
  case 1:
    return 'y';
  case 2:
    return 'z';
  case 3:
    return 'a';
  }
  return 'x';
}
