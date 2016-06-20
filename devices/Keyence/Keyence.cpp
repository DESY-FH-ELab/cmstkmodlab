#include <unistd.h>

#include <cmath>
#include <sstream>
#include <iostream>

#include "Keyence.h"

#define LSTEPDEBUG 1

Keyence::Keyence( const ioport_t ioPort )
  :VKeyence(ioPort),
   isDeviceAvailable_(false)
{
    //    std::cout<<"[Keyence] begin constructor"<<std::endl;
    comHandler_ = new KeyenceComHandler( ioPort );
    DeviceInit();
    //    std::cout<<"[Keyence] end constructor"<<std::endl;
}

Keyence::~Keyence()
{
  delete comHandler_;
}

bool Keyence::DeviceAvailable() const
{
  return isDeviceAvailable_;
}

// low level debugging methods
void Keyence::SendCommand(const std::string & command)
{
#ifdef LSTEPDEBUG
  std::cout << "SendCommand: " << command << std::endl;
#endif
  comHandler_->SendCommand(command.c_str());
  std::string buffer;
  ReceiveString(buffer);
}

void Keyence::ReceiveString(std::string & buffer)
{
  usleep(1000);

  char buf[1000];
  comHandler_->ReceiveString(buf);
  StripBuffer(buf);
  buffer = buf;
  std::cout<< "[Keyence::ReceiveString] buffer = "<<buffer<<std::endl;
#ifdef LSTEPDEBUG
  std::cout << "ReceiveCommand: " << buffer << std::endl;
#endif
}

//ABLE -> automatic adjustement of light intensity to surface
//set on head 1 or head 2 -> out
//on = 0 = AUTO, on = 1 = MANUAL
//only in communication mode
void Keyence::SetABLE(bool on, int out)
{
    if(!comMode_){ChangeToCommunicationMode(true);}
    SetValue("SW,HA,M,",out,on);
    ChangeToCommunicationMode(false);
}

/*Set the measurement mode depending on the material of the target sample
  set on head 1 or head 2 -> out
 mode = 0 -> normal
 mode = 1 -> translucent object
 mode = 2 -> transparent object
 mode = 3 -> transparent object 2
 mode = 4 -> multi-reflective object
*/
void Keyence::SetMaterialMode(int out, int mode)
{
    if(!comMode_){ChangeToCommunicationMode(true);}
    SetValue("SW,HB,",out, mode);
    ChangeToCommunicationMode(false);
}

/*Change settings if target sample is a mirror
set on head 1 or head 2 -> out
mode = 0 -> diffuse = normal
mode = 1 -> mirror reflection mode
*/
void Keyence::SetDiffuseMode(int out, int mode)
{
    if(!comMode_){ChangeToCommunicationMode(true);}
    SetValue("SW,HE,",out,mode);
    ChangeToCommunicationMode(false);
}

/*
  Frequency of internal measurements
    mode = 0 -> 20 microsec
    mode = 1 -> 50 microsec
    mode = 2 -> 100 microsec
    mode = 3 -> 200 microsec
    mode = 4 -> 500 microsec
    mode = 5 -> 1000 microsec
only in communication mode
*/
void Keyence::SetSamplingRate(int mode)
{
    if(!comMode_){ChangeToCommunicationMode(true);}
    SetValue("SW,CA,",mode);
    ChangeToCommunicationMode(false);
}

/*
  Number of internal measurements to average to output measurement
    mode = 0 -> average over 1
    mode = 1 -> 4
    mode = 2 -> 16
    mode = 3 -> 64
    mode = 4 -> 256
    mode = 5 -> 1024
    mode = 6 -> 4096
    mode = 7 -> 16384
    mode = 8 -> 65536
    mode = 9 -> 262144
only in communication mode
*/
void Keyence::SetAveraging(int out, int mode)
{
    if(!comMode_){ChangeToCommunicationMode(true);}
    SetValue("SW,OC,",out,"0",mode);
    ChangeToCommunicationMode(false);
}


//normal mode (commOn = 0) -> measurements can be received, writing or reading system settings is not accepted
//communication mode (commOn = 1) -> no measurement possible, writing and reading system settings is possible 
void Keyence::ChangeToCommunicationMode(bool commOn)
{
    //    std::cout<<"[Keyence] begin ChangeToCommunicationMode"<<std::endl;
    if(commOn){
        SendCommand("Q0");
        comMode_ = true;
    }else{
        SendCommand("R0");
        comMode_ = false;
    }
    //    std::cout<<"[Keyence] end ChangeToCommunicationMode"<<std::endl;
}

void Keyence::MeasurementValueOutput(int out, double value)
{
    std::cout<<"measurement value test"<<std::endl;
    GetValue("M",out,value);
    //    std::cout<<"measurement value test 2 "<<std::endl;
    //SetValue("M",out);
}

//lock the panel on the controller to avoid accidental pushing of buttons
void Keyence::PanelLock(int status)
{
  SetValue("KL,", status);
}

void Keyence::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<sizeof(buffer);++c) {
    if(buffer[c]=='\r') {
      buffer[c] = '\0';
      break;
    }
  }
}

//No version checking for Keyence laser available
void Keyence::DeviceInit()
{
  std::cout<<"[Keyence] begin device init"<<std::endl;
    isDeviceAvailable_ = false;
    
    if (comHandler_->DeviceAvailable()) {
        
        isDeviceAvailable_ = true;

    }

    //    this->ChangeToCommunicationMode(false);
    std::cout<<"[Keyence] end device init"<<std::endl;
}

void Keyence::Reset(int out)
{
  SetValue("VR,", out);
}


void Keyence::ProgramChange(int prog_number)
{
  SetValue("PW,",prog_number);
}



void Keyence::ProgramCheck(int value)
{
  GetValue("PR", value);
}


/*
void Keyence::Timing(int out, int status)
{
  SetValue("T", status, out);
}
*/

/*
void Keyence::AutoZero(int out, bool status)
{
  if(status){
    SetValue("V", out);
  }else{
    SetValue("W", out);
  }
}
*/

/*
void Keyence::StatResultOutput(int out, std::string value)
{
  SetValue("DO,", out, value);
}
*/

/*
void Keyence::ClearStat(int out)
{
  SetValue("DQ,", out);
}
*/

/*
void Keyence::StartDataStorage()
{
  SendCommand("AS");
  //string response; ReceiveString(response);
  //if(input == response){return 0;}
  //else{return 1;}
}
*/

/*
void Keyence::StopDataStorage()
{
  SendCommand("AP");
  //string response; ReceiveString(response);
}
*/

/*
void Keyence::InitDataStorage()
{
  SendCommand("AQ");
  //string response; ReceiveString(response);
}
*/

/*
void Keyence::OutputDataStorage(int out, std::vector<double> values)
{
  SetValue("AO,",out,values);
}
*/

/*
void Keyence::DataStorageStatus(std::string value)
{
  SetValue("AN",value);
}
*/


