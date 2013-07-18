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

  //setting bit-rate to 9600
  SetBRate();
  
  //setting pressure unit to mbar
  SetPressureUnit();	 
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
	  std::cout << "PfeifferTPG262: negative acknowledgement of a recieved message" << std::endl;
	  return -1;
  }

    usleep( 100000);
    comHandler_->SendEnquiry();

    usleep( 10000 );
    comHandler_->ReceiveString( buffer );
    StripBuffer( buffer );

    buffer[4]='\0'; //In order for the strcmp function to compare buffer to a char array of 4 elements

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
		std::cout << "PfeifferTPG262: negative acknowledgement of a recieved message" << std::endl;
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
		std::cout << "PfeifferTPG262: negative acknowledgement of a recieved message" << std::endl;
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
		std::cout << "PfeifferTPG262: negative acknowledgement of a recieved message" << std::endl;
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
			  << std::scientific << reading1.second << " mbar (" << (int)reading1.first << ")" << std::endl;

	reading2.first = static_cast<GaugeStatus>(std::atoi(v[2].c_str()));
	reading2.second = std::atof(v[3].c_str());
	std::cout <<"PfeifferTPG262: the measured pressure of guage2 is: "
			  << std::scientific << reading2.second << " mbar (" << (int)reading2.first << ")" << std::endl;

	usleep( 500000 );

	comHandler_->ReceiveString( buffer );
	StripBuffer( buffer );

	return true;
}

/*
///
/// Continuous output of measurement values (RS232)
///
///
void PfeifferTPG262::SetContinuous( void ) const
{
  #ifdef __PfeifferTPG262_DEBUG
  std::cout << "[PfeifferTPG262::GetContinuous] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
   usleep( 100000 );
  comHandler_->SendCommand( "COM,1" );

  ***********************************************************************************************************************
  std::cout<<"***************************************************************************************************************"<<endl;
  std::cout<<"PfeifferTPG262::Continuous output of measurement values in Mode 1 (1 second intervals)\n";
  std::cout<<"PfeifferTPG262::After setting the device to continuous transmit of measurements call the Readout function "<<endl;
  std::cout<<"As soon as receiving another command the automatic transmision of measurements stops\n";
  ************************************************************************************************************************
  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );
  //buffer[0] should be 0x06 for a positive acknowledgement
  std::cout <<"PfeifferTPG262::Length of received string is: "<<strlen(buffer) <<" Acknowledgment is: " << (int)buffer[0] << "--->";

        if(buffer[0]==0x06)
                std::cout<<"Positive acknowledgement of a received message"<<std::endl;
        else if (buffer[0]==0x15)
                std::cout<<"Negative acknowledgement of a recieved message"<<std::endl;
        else
                std::cout<<"Invalid response"<<std::endl;

     std::cout<<endl<<endl<<endl;

}

///
///The continuous readout of measurments
///
///



stat_t PfeifferTPG262::Readout( float & pressure1, float & pressure2 ) const
{
  char buffer[1000];
  stat_t status = stat_t (10,10);

 usleep( 1000000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );
  std::cout <<"PfeifferTPG262::Length of received string is: "<<strlen(buffer) <<" The output buffer for continuous measurement of guage 1 and 2 is: " << buffer<<endl;

// Output string processed to extract the status and measurement from it
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  std::string s(buffer, buffer+(strlen(buffer)));
//cout<<"s    "<<s;
 string::size_type first= s.find( ",", 0);
string::size_type second=s.find(",", first+1);
string::size_type length1=(second-first)-1;
 string::size_type start=first+1;

 string temp1= s.substr (start,length1);

 //cout<<"temp1 is "<<temp1<<endl;
 const char* temp2=temp1.c_str();
 pressure1=atof(temp2);


 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



  std::cout<<"PfeifferTPG262::The measured pressure of guage1 is: "<<pressure1<<endl;
  std::cout<<"PfeifferTPG262::The status of gauge 1 is: "<<s[first-1]<<" which indicates that: ";
        switch (s[first-1])
        {
                 case '0':{
                    std::cout<<"Measurement data okay"<<endl;
                    status.first=0;
                    }
                break;
                case '1': {
                std::cout<<"Underrange"<<endl;
                status.first=1;
                }
                break;
                 case '2': {
                std::cout<<"Overrange"<<endl;
                status.first=2;
                }
                break;
                 case '3': {
                std::cout<<"Sensor error"<<endl;
                status.first=3;
                   }
                break;
                case '4': {std::cout<<"Sensor off"<<endl;
                        status.first=4;}
                break;
                case '5': {std::cout<<"No sensore (output:5,2.0000E-2 [mbar])"<<endl;
                        status.first=5;}
                break;
                case '6': {std::cout<<"Identification error"<<endl;
                        status.first=6;}
                break;
                default: {std::cout<<"Meaningless status!"<<endl;
                        status.first=7;}
                break;
        }
// Output string processed to extract the status and measurement from it
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 string::size_type third= s.find( ",", second+1);
string::size_type end=s.find("\0", third);
string::size_type length2=(end-third)-1;
 string::size_type start2=third+1;

 string temp3= s.substr (start2,length2);

// cout<<"temp3 is "<<temp3<<endl;
 const char* temp4=temp3.c_str();
 pressure2=atof(temp4);

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 std::cout<<"PfeifferTPG262::The pressure measurement of guage 2 is: "<<pressure2<<endl;
 std::cout<<"PfeifferTPG262::The status of guage 2 is: "<<buffer[third-1]<<" which indicates that: ";
        switch (buffer[third-1])
        {
                case '0': {std::cout<<"Measurement data okay"<<endl;
                    status.second = 0;}
                break;
                case '1': {std::cout<<"Underrange"<<endl;
                        status.second = 1;}
                break;
                case '2': {std::cout<<"Overrange"<<endl;
                        status.second = 2;}
                break;
                case '3': {std::cout<<"Sensor error"<<endl;
                        status.second = 3;}
                break;
                case '4': {std::cout<<"Sensor off"<<endl;
                        status.second = 4;}
                break;
                case '5': {std::cout<<"No sensore (output:5,2.0000E-2 [mbar])"<<endl;
                        status.second = 5;}
                break;
                case '6': {std::cout<<"Identification error"<<endl;
                        status.second = 6;}
                break;
                default: {std::cout<<"Meaningless status!"<<endl;
                        status.second = 7;}
                break;
        }


  // usleep( 100000 );


//usleep(500000);

//this->SendETX();

comHandler_->ReceiveString( buffer );
StripBuffer( buffer );
std::cout << "PfiefferTPG262::Recieved:" << strlen(buffer) << " " << (int)buffer[0]<< "--->End of message" << endl << endl << endl;

  return status;
}

///
/// Guage Identification
///
///

int PfeifferTPG262::GuageIdentification( void ) const
{
  #ifdef __PfeifferTPG262_DEBUG
  std::cout << "[PfeifferTPG262::GuageIdentification] -- DEBUG: Called." << std::endl;
  #endif

  char buffer[1000];
   usleep( 100000 );

  comHandler_->SendCommand( "TID" );
  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  //buffer[0] should be 0x06 for the positive acknowledgement
  std::cout <<"PfeifferTPG262::Length of received string is: "<<strlen(buffer) <<" Acknowledgment is: " << (int)buffer[0] << "--->";

        if(buffer[0]==0x06)
                std::cout<<"Positive acknowledgement of a received message"<<std::endl;
        else if (buffer[0]==0x15)
                std::cout<<"Negative acknowledgement of a recieved message"<<std::endl;
        else
                std::cout<<"Invalid response"<<std::endl;

  usleep( 100000 );
  comHandler_->SendEnquiry();

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  std::cout <<"PfeifferTPG262::Length of received string is: "<<strlen(buffer) <<" The identification result for Guage1,Guage2 is: "<<buffer;

   usleep( 500000 );

   comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );
  std::cout << "PfeifferTPG262::Recieved:" <<(int)buffer[0]<< "--->End of message" << endl << endl << endl;

  return 0;
}

*/


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
void PfeifferTPG262::Device_Init( void ) {

  #ifdef __PfeifferTPG262_DEBUG
  std::cout << "[PfeifferTPG262::Device_Init] -- DEBUG: Called." << std::endl;
  #endif

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
	  std::cout << "PfeifferTPG262: negative acknowledgement of a recieved message" << std::endl;
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
	  std::cout << "PfeifferTPG262: negative acknowledgement of a recieved message" << std::endl;
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

/*


////
//// This functions sends an end of text signal

void PfeifferTPG262::SendETX (void) const
{
  #ifdef __PfeifferTPG262_DEBUG
  std::cout << "[PfeifferTPG262::SendETX] -- DEBUG: Called." << std::endl;
  #endif

  usleep(100000);
  comHandler_->SendResetInterface();
  usleep(10000);
}
///
//// This fuunction cancels currenctly active errors and returns to measurement mode
///
void PfeifferTPG262::Reset(void) const
{
  #ifdef  __PfeifferTPG262_DEBUG
  std::cout << "[PfeifferTPG262::Reset] -- DEBUG: Called." << std::endl;
  #endif


  char buffer[1000];


   usleep( 100000 );
  comHandler_->SendCommand( "RES,1" );

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  //buffer[0] should be 0x06 for positive acknowledgement
  std::cout <<"PfeifferTPG262::Length of received string is: "<<strlen(buffer) <<"  Acknowledgment is: " << (int)buffer[0] << " ---> ";

        if(buffer[0]==0x06)
                std::cout<<"Positive acknowledgement of a received message"<<std::endl;
        else if (buffer[0]==0x15)
                std::cout<<"Negative acknowledgement of a recieved message"<<std::endl;
        else
                std::cout<<"Invalid response"<<std::endl;
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  usleep( 100000);
  comHandler_->SendEnquiry();

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  std::cout <<"PfeifferTPG262::Length of received string is: "<<strlen(buffer)<<"  ";
  std::cout<<"List of all present error messages is: "<<buffer<<" which indicates that: ";
  if (strlen( buffer ) == 1)
  {
        switch (buffer[0])
        {
        case '0': std::cout<<"No Error"<<endl;
                break;
        case '1': std::cout<<"Watchdog is responded"<<endl;
                break;
        case '2': std::cout<<"Task fail error"<<endl;
                break;
        case '3': std::cout<<"EPROM error"<<endl;
                break;
        case '4': std::cout<<"RAM error"<<endl;
                break;
        case '5': std::cout<<"EEPROM error"<<endl;
                break;
        case '6': std::cout<<"Display error"<<endl;
                break;
        case '7': std::cout<<"A/D converter error"<<endl;
                break;
        case '9': std::cout<<"Guage 1 error"<<endl;
                break;
         default: std::cout<<"Meaningless status!"<<endl;
                break;
            }
    }
  else if (strlen( buffer ) == 2)
  {

      if ( buffer[0]==1 && buffer[1]==0) {std::cout<<"Guage 1 identification error"<<endl;}

      else if (buffer[0]==1 && buffer[1]==0) {std::cout<<"Guage 2 error"<<endl;}

      else if (buffer[0]==1 && buffer[1]==2) {std::cout<<"Guage 2 identification error"<<endl;}
      else {std::cout<<"Meaningless status!"<<endl;}

  }
  else {std::cout<<"Meaningless status!"<<endl;}

   usleep( 100000 );

comHandler_->SendResetInterface();
usleep(500000);

 std::cout << "Recieved:" <<(int)buffer[0]<< "--->End of setting" << endl << endl << endl;

}

/////
///// KeyLock Function
/////
/////
int PfeifferTPG262::KeyLock( void ) const
{

  #ifdef  __PfeifferTPG262_DEBUG
  std::cout << "[PfeifferTPG262::KeyLock] -- DEBUG: Called." << std::endl;
  #endif

  int key=10;
  char buffer[1000];


  usleep( 100000 );
  comHandler_->SendCommand( "LOC,1" );

  usleep( 100000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  //buffer[0] should be 0x06 for positive acknowledgement
  std::cout <<"PfeifferTPG262::Length of received string is: "<<strlen(buffer) <<"  Acknowledgment is: " << (int)buffer[0] << " ---> ";

        if(buffer[0]==0x06)
                std::cout<<"Positive acknowledgement of a received message"<<std::endl;
        else if (buffer[0]==0x15)
                std::cout<<"Negative acknowledgement of a recieved message"<<std::endl;
        else
                std::cout<<"Invalid response"<<std::endl;
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  usleep( 100000);
  comHandler_->SendEnquiry();

  usleep( 10000 );
  comHandler_->ReceiveString( buffer );
  StripBuffer( buffer );

  std::cout <<"PfeifferTPG262::The received enquiry is: " << buffer;

        if ((strlen(buffer)==0) || (strlen(buffer)>1))
  {std::cout<<" --->  Error occured in KeyLoc function: the received enquiry is the error word\n"<<endl; key=7;}
        else if (strlen(buffer)==1)
        {
                if (buffer[0]=='0')
            { std::cout<<" --->  The keylock is off"<<endl; key=0;}
                else if(buffer[0]=='1')
                { std::cout<<" --->  The KeyLoc is on"<<endl; key=1;}
                else
                {std::cout<<" --->  Error occured in setting KeyLock function: the received enquiry is the error word"<<endl;
                    key=7;}
        }
   usleep( 500000 );


  comHandler_->SendResetInterface();
  usleep(500000);

 std::cout << "PfeifferTPG262::Recieved:" <<(int)buffer[0]<< "--->End of setting" << endl << endl << endl;

  return key;
}
*/
