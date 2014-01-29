#include <iostream>
#include <vector>

#include "TPG262ComHandler.h"

#include "PfeifferTPG262.h"

//#define __PfeifferTPG262_DEBUG 1

///
///
/// Constructor
PfeifferTPG262::PfeifferTPG262( const ioport_t ioPort )
  : VPfeifferTPG262(ioPort)
{
  comHandler_ = new TPG262ComHandler( ioPort );
  isCommunication_ = false;
  Device_Init();

  if (isCommunication_) {
    //setting bit-rate to 9600
    SetBRate();
    
    //setting pressure unit to mbar
    SetPressureUnit();
  }
}

///
///
/// Destructor
PfeifferTPG262::~PfeifferTPG262()
{
  delete comHandler_;
}

///
///
/// Cheking the error flags
int PfeifferTPG262::GetErrorStatus( void ) const
{
#ifdef  __PfeifferTPG262_DEBUG
  std::cout << "[PfeifferTPG262::GetErrorStatus] -- DEBUG: Called." << std::endl;
#endif

  int error=10;
  char buffer[1000];

  usleep( 100000 );
  comHandler_->SendCommand( "ERR" );

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if (buffer[0]!=0x06) {
    std::cout << "PfeifferTPG262: negative acknowledgement of a recieved message"
         << std::endl;
    return -1;
  }

  usleep( 100000);
  comHandler_->SendEnquiry();

  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  //In order for the strcmp function to compare buffer to a char array of 4 elements
  buffer[4]='\0';
  int ec = std::atoi(buffer);

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return ec;
}

///
///
/// Getting the pressure measurement results for gauge 1
bool PfeifferTPG262::GetPressure1(reading_t & reading)
{
  char buffer[1000];

  usleep( 100000 );
  comHandler_->SendCommand( "PR1" );

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if (buffer[0]!=0x06) {
    std::cout << "PfeifferTPG262: negative acknowledgement of a recieved message"
        << std::endl;
    return false;
  }

  usleep( 100000);
  comHandler_->SendEnquiry();

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  std::string s(buffer);
  GaugeStatus status = static_cast<GaugeStatus>(std::atoi(s.substr(0, s.find(",", 0)).c_str()));
  float pressure = std::atof(s.substr(s.find(",", 0)+1).c_str());

  reading.first = status;
  reading.second = pressure;

  std::cout <<"PfeifferTPG262: the measured pressure of guage1 is: "
	    << std::scientific << pressure << " mbar (" << status << ")" << std::endl;

  usleep( 500000 );

  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return true;
}

///
///
/// Getting the pressure measurement results for guage 2
bool PfeifferTPG262::GetPressure2(reading_t &reading)
{
  char buffer[1000];

  usleep( 100000 );
  comHandler_->SendCommand( "PR2" );

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if (buffer[0]!=0x06) {
    std::cout << "PfeifferTPG262: negative acknowledgement of a recieved message"
        << std::endl;
    return false;
  }

  usleep( 100000);
  comHandler_->SendEnquiry();

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  std::string s(buffer);
  GaugeStatus status = static_cast<GaugeStatus>(std::atoi(s.substr(0, s.find(",", 0)).c_str()));
  float pressure = std::atof(s.substr(s.find(",", 0)+1).c_str());

  reading.first = status;
  reading.second = pressure;

  std::cout <<"PfeifferTPG262: the measured pressure of guage2 is: "
	    << std::scientific << pressure << " mbar (" << status << ")" << std::endl;

  usleep( 500000 );

  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return true;
}

///
/// Measurement data guage1 and 2
///
///
bool PfeifferTPG262::GetPressures(reading_t & reading1, reading_t & reading2)
{
  char buffer[1000];

  usleep( 100000 );
  comHandler_->SendCommand( "PRX" );

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if (buffer[0]!=0x06) {
    std::cout << "PfeifferTPG262: negative acknowledgement of a recieved message"
        << std::endl;
    return false;
  }

  usleep( 100000);
  comHandler_->SendEnquiry();

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  std::string s(buffer);

  std::vector<std::string> v;
  size_t start = 0, end = 0;
  while (end != string::npos) {
    end = s.find(",", start);
    v.push_back(s.substr(start, (end==string::npos) ? string::npos : end - start));
    start = ( ( end > (string::npos - 1) ) ? string::npos : end + 1);
  }

  if (v.size()!=4) return false;

  reading1.first = static_cast<GaugeStatus>(std::atoi(v[0].c_str()));
  reading1.second = std::atof(v[1].c_str());
  std::cout <<"PfeifferTPG262: the measured pressure of guage1 is: "
	    << std::scientific << reading1.second << " mbar (" << (int)reading1.first << ")"
	    << std::endl;

  reading2.first = static_cast<GaugeStatus>(std::atoi(v[2].c_str()));
  reading2.second = std::atof(v[3].c_str());
  std::cout <<"PfeifferTPG262: the measured pressure of guage2 is: "
	    << std::scientific << reading2.second << " mbar (" << (int)reading2.first << ")"
	    << std::endl;

  usleep( 500000 );

  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  return true;
}

///
/// strip trailing newlines & stuff
/// from machine answer
///
void PfeifferTPG262::StripBuffer( char* buffer ) const {

#ifdef __PfeifferTPG262_DEBUG
  std::cout << "[PfeifferTPG262::StripBuffer] -- DEBUG: Called." << std::endl;
  std::cout << "[PfeifferTPG262::StripBuffer] -- DEBUG: Buffer is:" << std::endl;
  std::cout << " > " << buffer << std::endl;
#endif

  for( unsigned int c = 0; c < strlen( buffer ); ++c ) {
    if( '\n' == buffer[c] || '\r' == buffer[c] ) {
      buffer[c] = 0;
      break;
    }
  }
  
}
///
/// read back software version
/// to check communication with device
///
void PfeifferTPG262::Device_Init( void )
{
  char buffer[1000];

  usleep( 100000 );
  comHandler_->SendCommand( "PNR" );

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if (buffer[0]!=0x06) {
    std::cout << "PfeifferTPG262: negative acknowledgement of a recieved message"
        << std::endl;
    isCommunication_ = false;
    return;
  }

  usleep( 100000);
  comHandler_->SendEnquiry();

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  std::string s(buffer);

  if (s.find("302-510", 0)!=0) {
    isCommunication_ = false;
    return;
  }
  
  usleep( 500000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  isCommunication_ = true;
}

///
///The mbar is the assummed unit for every displayed measurement,
///Setting the pressure unit to mbar
///
int PfeifferTPG262::SetPressureUnit(void) const
{
  std::cout << "PfeifferTPG262: set pressure unit to mbar"<<endl;

  char buffer[1000];

  usleep( 100000 );
  comHandler_->SendCommand( "UNI,0" );
   
  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if (buffer[0]!=0x06) {
    std::cout << "PfeifferTPG262: negative acknowledgement of a recieved message"
        << std::endl;
    return -1;
  }

  usleep( 100000 );
  comHandler_->SendEnquiry();

  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  int returnCode = std::atoi(buffer);


  usleep( 100000 );
  comHandler_->SendResetInterface();

  return returnCode;
}

///
/// The object comHandler_ is initialized to buad rate of 9600
/// To set the baudrate to 9600
///
bool PfeifferTPG262::SetBRate(void)
{
  int rate = 10;
  std::cout << "PfeifferTPG262: set baud rate to 9600"<<endl;

  char buffer[1000];

  usleep( 100000 );
  comHandler_->SendCommand( "BAU,0" );
   
  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  if (buffer[0]!=0x06) {
    std::cout << "PfeifferTPG262: negative acknowledgement of a recieved message"
        << std::endl;
    return -1;
  }

  usleep( 100000 );
  comHandler_->SendEnquiry();

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  int returnCode = std::atoi(buffer);

  usleep( 100000 );
  comHandler_->SendResetInterface();

  return returnCode;
}
