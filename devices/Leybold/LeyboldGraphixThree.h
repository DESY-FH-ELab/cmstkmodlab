#ifndef _LEYBOLDGRAPHIXTHREE_H_
#define _LEYBOLDGRAPHIXTHREE_H_

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>

#include "VLeyboldGraphixThree.h"
#include "LeyboldComHandler.h"

class LeyboldGraphixThree : public VLeyboldGraphixThree
{
 public:

  LeyboldGraphixThree( const ioport_t );
  ~LeyboldGraphixThree();

 public:

  bool DeviceAvailable() const;

 private:

  void StripBuffer( char* ) const;
  void DeviceInit();

  LeyboldComHandler* comHandler_;
  bool isDeviceAvailable_;
};

#endif // _LEYBOLDGRAPHIXTHREE_H_
