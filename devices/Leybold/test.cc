#include <iostream>

#ifdef USE_FAKEIO
#include "LeyboldGraphixThreeFake.h"
typedef LeyboldGraphixThreeFake LeyboldGraphixThree_t;
#else
#include "LeyboldGraphixThree.h"
typedef LeyboldGraphixThree LeyboldGraphixThree_t;
#endif

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  LeyboldGraphixThree_t gt("/dev/tty.SLAB_USBtoUART");

  return 0;
}
