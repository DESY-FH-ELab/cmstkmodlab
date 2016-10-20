#ifndef _LEYBOLDGRAPHIXTHREEFAKE_H_
#define _LEYBOLDGRAPHIXTHREEFAKE_H_

#include "VLeyboldGraphixThree.h"

class LeyboldGraphixThreeFake : public VLeyboldGraphixThree
{
 public:

  LeyboldGraphixThreeFake( const ioport_t );
  ~LeyboldGraphixThreeFake();

 public:

  bool DeviceAvailable() const { return true; }

 private:

};

#endif // _LEYBOLDGRAPHIXTHREEFAKE_H_
