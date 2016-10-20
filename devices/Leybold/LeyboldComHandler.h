#ifndef _LEYBOLDCOMHANDLER_H_
#define _LEYBOLDCOMHANDLER_H_

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

typedef const char* ioport_t;
typedef struct termios termios_t;

class LeyboldComHandler {

 public:
  
  //! Constructor.
  LeyboldComHandler( ioport_t );

  //! Destructor.
  ~LeyboldComHandler();

  //! Default bitwise copy constructor.
  LeyboldComHandler( const LeyboldComHandler& );

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

#endif // _LEYBOLDCOMHANDLER_H_



