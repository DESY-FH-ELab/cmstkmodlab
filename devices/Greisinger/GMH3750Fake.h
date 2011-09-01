#ifndef _GMH3750Fake_H_
#define _GMH3750Fake_H_

#include "VGMH3750.h"

using namespace std;

class GMH3750Fake : public VGMH3750
{
 public:
  
  //! Constructor.
  GMH3750Fake( ioport_t );

  //! Destructor.
  ~GMH3750Fake();

  bool Read(double & temperature);
};

#endif
