
#ifndef _FP50COMHANDLER_H_
#define _FP50COMHANDLER_H_

#include <termios.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FALSE 0
#define TRUE 1

#define COM1 "/dev/ttyS0"
#define COM2 "/dev/ttyS1"
#define COM3 "/dev/ttyS2"
#define COM4 "/dev/ttyS3"

#define ttyS0 "/dev/ttyS0"
#define ttyS1 "/dev/ttyS1"
#define ttyS2 "/dev/ttyS2"
#define ttyS3 "/dev/ttyS3"


typedef const char* ioport_t;
typedef struct termios termios_t;

using namespace std;



class FP50ComHandler {


 public:
  
  //! Constructor.
  FP50ComHandler( ioport_t );

  //! Destructor.
  ~FP50ComHandler();

  //! Default bitwise copy constructor.
  FP50ComHandler( const FP50ComHandler& );

  void SendCommand( const char* );
  void ReceiveString( char* );


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



