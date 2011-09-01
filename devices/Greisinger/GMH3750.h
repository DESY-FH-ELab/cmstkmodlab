#ifndef _GMH3750_H_
#define _GMH3750_H_

#include "GMH3750ComHandler.h"

#include "VGMH3750.h"

using namespace std;

class GMH3750 : public VGMH3750
{
 public:
  
  //! Constructor.
  GMH3750( ioport_t );

  //! Destructor.
  ~GMH3750();

  bool Read(double & temperature);

 private:

  GMH3750ComHandler* comHandler_;
};

#endif
