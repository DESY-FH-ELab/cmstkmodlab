#ifndef _PETITEFLEURCOMHANDLER_H_
#define _PETITEFLEURCOMHANDLER_H_

#include <termios.h>
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

#define ttyACM0 "/dev/ttyACM0"

typedef const char* ioport_t;
typedef struct termios termios_t;

class PetiteFleurComHandler {

 public:
  
  //! Constructor.
  PetiteFleurComHandler( ioport_t );

  //! Destructor.
  ~PetiteFleurComHandler();

  //! Default bitwise copy constructor.
  PetiteFleurComHandler( const PetiteFleurComHandler& );

  void SendCommand( const char* );
  void ReceiveString( char* );

  static constexpr int ComHandlerDelay = 1000;

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
