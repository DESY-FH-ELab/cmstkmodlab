#include "VLeyboldGraphixThree.h"

VLeyboldGraphixThree::VLeyboldGraphixThree( const ioport_t ioPort )
{

}

VLeyboldGraphixThree::~VLeyboldGraphixThree()
{

}

char VLeyboldGraphixThree::GetChecksum(const char * buffer)
{
  int sum = 0;

  for (int i=0;i<std::strlen(buffer);++i) {
    sum += buffer[i];
  }

  char CRC = 255 - (sum%256);

  return CRC;
}
