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

 public:

  virtual bool DeviceAvailable() const = 0;

 protected:

};

#endif // _VLEYBOLDGRAPHIXTHREE_H_
