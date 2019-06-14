/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef _KMMCOMHANDLER_H_
#define _KMMCOMHANDLER_H_

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

/*!
  \mainpage

  <br><br><br>

  \b class \link KMMComHandler \endlink
  - <b>A class for handling raw communication with the prema KMM 5017 digital multimeter</b><br>
  Sends and receives commands over the serial line. No parameter or limit checking. Opens and 
  initializes serial port, saves and restores serial port settings.

  \b class \link KMM5017 \endlink
  - <b>A class for steering prema KMM 5017 digital multimeter</b><br>
  Offers access to full device functionality with low level (representing SHQ 222M built-in commands) 
  and higher level functions. Does parameter and limit checking. Has several execution options and
  auto initializing. Uses instance of class \link SHQComHandler \endlink.

  <br><br><br><br>

  <CENTER>

  \b CVS \b location:
  \c $(CVSROOT)/dmm5017

  <br><br><br>

*/

//!  A class for handling raw communication with the prema KMM 5017 digital multimeter
/*!
  This class handles basic serial communication with KMM 5017 digital multimeter devices
  such as string sending and receiving. No command or limit checking is implemented.
  
  \par Example program "dmm"
  <br>
  \code

  #include <iostream.h>
  #include <KMMComHandler.h>

  int main( int argc, char **argv )  {
  
    // the received string container
    char string[41];

    // open serial communication to a device on COM2
    KMMComHandler *dmmComHandler = new KMMComHandler( COM2 );

    if ( argc != 2 ) {
      cout << "Command line arguments wrong." << endl;
      return ( -1 );
    }

    // send command line argument string
    dmmComHandler->SendCommand( argv[1] );

    // immediately receive answer string and stream it to stdout
    dmmComHandler->ReceiveString( string );
    cout << string;

    // quit
    delete dmmComHandler;

  }

  \endcode

  Usage: \c ./dmm "&lt;command&gt;"


  \attention <br>
  Make sure the I/O port device files are human readable.<br>
  Device must be set to RTS/CTS handshake mode manually.<br><br>
*/

class KMMComHandler {

 public:
  
  //! Constructor.
  KMMComHandler( ioport_t ioPort );

  //! Destructor.
  ~KMMComHandler();

  //! Default bitwise copy constructor.
  KMMComHandler( const KMMComHandler &dmmComHandler );

  void SendCommand( const char *commandString );
  void ReceiveString( char *receiveString );

 private:

  void OpenIoPort( void ) noexcept(false);
  void InitializeIoPort( void );
  void RestoreIoPort( void );
  void CloseIoPort( void );
  void SendFeedString( void );

  int fIoPortFileDescriptor;

  ioport_t fIoPort;
  termios_t fCurrentTermios, fThisTermios;
};

#endif



