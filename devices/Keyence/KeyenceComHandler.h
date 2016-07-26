#ifndef _KEYENCECOMHANDLER_H_
#define _KEYENCECOMHANDLER_H_

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef const char* ioport_t;
typedef struct termios termios_t;

/*
Class for establishing communication with the Keyence LK-G3000 laser 
*/

class KeyenceComHandler {

 public:
  
  //! Constructor.
  KeyenceComHandler( ioport_t );

  //! Destructor.
  ~KeyenceComHandler();

  //! Default bitwise copy constructor.
  KeyenceComHandler( const KeyenceComHandler & );

  void SendCommand( const char* );
  //  std::string ReceiveString(int samplingRate, int averagingRate );
  void ReceiveString(std::string & receiveString, char *temp_output, int samplingRate, int averagingRate );

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



