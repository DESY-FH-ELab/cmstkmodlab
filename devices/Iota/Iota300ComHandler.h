#ifndef _IOTA300COMHANDLER_H_
#define _IOTA300COMHANDLER_H_

#include <termios.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>

#define COM1 "/dev/ttyS0"
#define COM2 "/dev/ttyS1"
#define COM3 "/dev/ttyS2"
#define COM4 "/dev/ttyS3"

#define ttyS0 "/dev/ttyS0"
#define ttyS1 "/dev/ttyS1"
#define ttyS2 "/dev/ttyS2"
#define ttyS3 "/dev/ttyS3"
#define ttyS4 "/dev/ttyS4"
#define ttyS5 "/dev/ttyS5"

#define ttyACM0 "/dev/ttyACM0"

#define _COMHANDLER_DELAY 1000

typedef const char* ioport_t;
typedef struct termios termios_t;

class Iota300ComHandler {

 public:
  
  //! Constructor.
  Iota300ComHandler( ioport_t );

  //! Destructor.
  ~Iota300ComHandler();

  //! Default bitwise copy constructor.
  Iota300ComHandler( const Iota300ComHandler& );

  void SendCommand( const char* );
  void ReceiveString( char* );

 private:

  void OpenIoPort( void ) throw (int);
  void InitializeIoPort( void );
  void RestoreIoPort( void );
  void CloseIoPort( void );
  void SendFeedString( void );

  int fIoPortFileDescriptor;

  ioport_t fIoPort;
  termios_t fCurrentTermios, fThisTermios;
};

#endif
