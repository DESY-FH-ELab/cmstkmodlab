#ifndef _ARDUINOPRESCOMHANDLER_H_
#define _ARDUINOPRESCOMHANDLER_H_

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>

typedef const char* ioport_t;
typedef struct termios termios_t;

class ArduinoPresComHandler {

 public:
  
  //! Constructor.
  ArduinoPresComHandler( ioport_t );

  //! Destructor.
  ~ArduinoPresComHandler();

  //! Default bitwise copy constructor.
  ArduinoPresComHandler( const ArduinoPresComHandler& );

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
