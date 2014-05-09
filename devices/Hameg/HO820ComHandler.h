#ifndef _HO820COMHANDLER_H_
#define _HO820COMHANDLER_H_

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef const char* ioport_t;
typedef struct termios termios_t;

class HO820ComHandler {

 public:
  
  //! Constructor.
  HO820ComHandler( ioport_t );

  //! Destructor.
  ~HO820ComHandler();

  //! Default bitwise copy constructor.
  HO820ComHandler( const HO820ComHandler& );

  void SendCommand( const char* );
  void ReceiveString( char* );

  bool DeviceAvailable();

 private:

  void OpenIoPort( void );
  void InitializeIoPort( void );
  void RestoreIoPort( void );
  void CloseIoPort( void );
  void SendFeedString( void );

  bool fDeviceAvailable;
  int fIoPortFileDescriptor;

  ioport_t fIoPort;
  termios_t fCurrentTermios, fThisTermios;
};

#endif



