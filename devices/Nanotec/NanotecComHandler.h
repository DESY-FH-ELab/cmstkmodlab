#ifndef _NANOTECCOMHANDLER_H_
#define _NANOTECCOMHANDLER_H_

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef const char* ioport_t;
typedef struct termios termios_t;

class NanotecComHandler {

 public:
  
  //! Constructor.
  NanotecComHandler( ioport_t );

  //! Destructor.
  ~NanotecComHandler();

  //! Default bitwise copy constructor.
  NanotecComHandler( const NanotecComHandler& );

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



