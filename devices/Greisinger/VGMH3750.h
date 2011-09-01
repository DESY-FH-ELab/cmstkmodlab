#ifndef _VGMH3750_H_
#define _VGMH3750_H_

using namespace std;

typedef const char* ioport_t;

class VGMH3750
{
 public:

  //! Constructor.
  VGMH3750( ioport_t );

  //! Destructor.
  ~VGMH3750();

  virtual bool Read(double & temperature) = 0;
};

#endif
