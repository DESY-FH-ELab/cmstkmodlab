#ifndef _VLEYBOLDGRAPHIXTHREE_H_
#define _VLEYBOLDGRAPHIXTHREE_H_

#include <string>
#include <vector>

typedef const char* ioport_t;

class VLeyboldGraphixThree
{
 public:

  VLeyboldGraphixThree( const ioport_t );
  virtual ~VLeyboldGraphixThree();

  virtual bool DeviceAvailable() const = 0;

  virtual std::string GetVersion() const = 0;

  virtual double GetPressure(int channel) const = 0;

  char GetChecksum(const std::string& buffer) const;
};

#endif // _VLEYBOLDGRAPHIXTHREE_H_
