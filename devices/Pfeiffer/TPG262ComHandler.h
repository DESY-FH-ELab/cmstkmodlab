#ifndef _TPG262COMHANDLER_H_
#define _TPG262COMHANDLER_H_

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
#define COM5 "/dev/ttyS3"

#define ttyS0 "/dev/ttyS0"
#define ttyS1 "/dev/ttyS1"
#define ttyS2 "/dev/ttyS2"
#define ttyS3 "/dev/ttyS3"
#define ttys4 "/dev/ttyS3"

#define _COMHANDLER_DELAY 1000

typedef const char* ioport_t;
typedef struct termios termios_t;

class TPG262ComHandler {

 public:
  
  //! Constructor.
  TPG262ComHandler( ioport_t );

  //! Destructor.
  ~TPG262ComHandler();

  //! Default bitwise copy constructor.
  TPG262ComHandler( const TPG262ComHandler & );

  void SendCommand( const char* );
  void SendEnquiry();
  void ReceiveString( char* );
  void SendResetInterface ();


 private:

  void OpenIoPort( void );
  void InitializeIoPort( void );
  void RestoreIoPort( void );
  void CloseIoPort( void );
  void SendFeedString( void );

  int fIoPortFileDescriptor;

  ioport_t fIoPort;
  termios_t fCurrentTermios, fThisTermios;
};

#endif



