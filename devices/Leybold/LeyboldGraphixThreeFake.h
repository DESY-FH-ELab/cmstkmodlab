#ifndef _LEYBOLDGRAPHIXTHREEFAKE_H_
#define _LEYBOLDGRAPHIXTHREEFAKE_H_

#include "VLeyboldGraphixThree.h"

class LeyboldGraphixThreeFake : public VLeyboldGraphixThree
{
 public:

  LeyboldGraphixThreeFake( const ioport_t );
  ~LeyboldGraphixThreeFake();

  bool DeviceAvailable() const { return true; }

  std::string GetVersion() const;

  double GetPressure(int channel) const;

 protected:

  double pressure_[3];
};

#endif // _LEYBOLDGRAPHIXTHREEFAKE_H_
