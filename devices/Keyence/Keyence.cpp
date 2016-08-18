#include <unistd.h>

#include <cmath>
#include <sstream>
#include <iostream>

#include "Keyence.h"

#define KEYENCEDEBUG 1

Keyence::Keyence( const ioport_t ioPort )
  :VKeyence(ioPort),
   isDeviceAvailable_(false)
{
    samplingRate_ = 20;
    averagingRate_ = 1;
    comHandler_ = new KeyenceComHandler( ioPort );
    DeviceInit();
}

Keyence::~Keyence()
{
  if(comHandler_) {delete comHandler_; comHandler_ = NULL;}
}

bool Keyence::DeviceAvailable() const
{
  return isDeviceAvailable_;
}

// low level debugging methods
void Keyence::SendCommand(const std::string & command)
{
#ifdef KEYENCEDEBUG
  std::cout << "SendCommand: " << command << std::endl;
#endif
  comHandler_->SendCommand(command.c_str());
}

void Keyence::ReceiveString(std::string & buffer)
{
  usleep(1000);

  char temp[1000];
  comHandler_->ReceiveString(buffer, temp, samplingRate_, averagingRate_);
  StripBuffer(buffer);
#ifdef KEYENCEDEBUG
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
    std::string response = SetValue("SW,HA,M,",out,on);
    if(response != "SW,HA"){
        std::cerr << "[Keyence::SetABLE] ** ERROR: could not be executed, response : "
	      << response 
	      << std::endl;
        return;
    }
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
    std::string response = SetValue("SW,HB,",out, mode);
    if(response != "SW,HB"){
        std::cerr << "[Keyence::SetMaterialMode] ** ERROR: could not be executed, response : "
	      << response 
                  << std::endl;
        return;
    }
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
    std::string response = SetValue("SW,HE,",out,mode);
    if(response != "SW,HE"){
        std::cerr << "[Keyence::SetDiffuseMode] ** ERROR: could not be executed, response : "
	      << response
                  << std::endl;
        return;
    }
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
    std::string response = SetValue("SW,CA,",mode);
    if(response != "SW,CA"){
        std::cerr << "[Keyence::SetSamplingRate] ** ERROR: could not be executed, response : "
	      << response
                  << std::endl;
        return;
    }else{
        switch(mode){  
        case 0 : samplingRate_ = 20; break;
        case 1 : samplingRate_ = 50; break;
        case 2 : samplingRate_ = 100; break;
        case 3 : samplingRate_ = 200; break;
        case 4 : samplingRate_ = 500; break;
        case 5 : samplingRate_ = 1000; break;
        default : samplingRate_ = 20; 
        }
    }

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
    std::string response = SetValue("SW,OC,",out,"0",mode);
    if(response != "SW,OC"){
        std::cerr << "[Keyence::SetAveraging] ** ERROR: could not be executed, response : "
	      << response
                  << std::endl;
        return;
    }else{
        switch(mode){  
        case 0 : averagingRate_ = 1;
        case 1 : averagingRate_ = 4;
        case 2 : averagingRate_ = 16;
        case 3 : averagingRate_ = 64;
        case 4 : averagingRate_ = 256;
        case 5 : averagingRate_ = 1024;
        case 6 : averagingRate_ = 4096;
        case 7 : averagingRate_ = 16384;
        case 8 : averagingRate_ = 65536;
        case 9 : averagingRate_ = 262144;
        default : averagingRate_ = 1;
        }
    }

    ChangeToCommunicationMode(false);
}


//normal mode (commOn = 0) -> measurements can be received, writing or reading system settings is not accepted
//communication mode (commOn = 1) -> no measurement possible, writing and reading system settings is possible 
void Keyence::ChangeToCommunicationMode(bool commOn)
{
    std::string response;
    if(commOn){
        response = SetValue("Q0");
        if(response != "Q0"){
	std::cerr << "[Keyence::ChangeToCommunicationMode] ** ERROR: could not be executed, response : "
	          << response
	          << std::endl;
	return;
        }
        comMode_ = true;
    }else{
        response = SetValue("R0");
        if(response != "R0"){
	std::cerr << "[Keyence::ChangeToCommunicationMode] ** ERROR: could not be executed, response : "
	          << response
	          << std::endl;
	return;
        }
        comMode_ = false;
    }
}

void Keyence::MeasurementValueOutput(int out, double & value)
{
    std::string response = SetValue("M",out);
    if(response.find("ER") != std::string::npos || response.find("M") == std::string::npos ){
	std::cerr << "[Keyence::MeasurementValueOutput] ** ERROR: could not be executed, response : "
	          << response
	          << std::endl;
	return;
    } 

    if(response.substr(3,8).find("F") != std::string::npos){
      std::cerr << "[Keyence::MeasurementValueOutput] ** ERROR: laser out of range, please adjust position "
	    << std::endl;
      if(response.substr(3,8).find("-") != std::string::npos){value = -9999;}else{value = 9999;}
      return;
      //throw response.substr(3,8);
    }else{
    std::istringstream is(response.substr(3,8));

    double temp;
    is >> temp;
    value = temp;
    }
}

//lock the panel on the controller to avoid accidental pushing of buttons
void Keyence::PanelLock(int status)
{
    std::string response = SetValue("KL,", status);
    if(response != "KL"){
        std::cerr << "[Keyence::PanelLock] ** ERROR: could not be executed, response : "
	      << response
                  << std::endl;
        return;
    }
}

void Keyence::StripBuffer(std::string &buffer) const
{
    if(buffer.size() == 0){return;}
    if(buffer.at(buffer.size() - 1) == '\r'){buffer.erase(buffer.size() - 1);}
}

//No version checking for Keyence laser available
void Keyence::DeviceInit()
{
    isDeviceAvailable_ = false;
    
    if (comHandler_->DeviceAvailable()) {
        
        isDeviceAvailable_ = true;
    }

    this->ChangeToCommunicationMode(false);
}

void Keyence::Reset(int out)
{
    std::string response = SetValue("VR,", out);
    if(response != "VR"){
        std::cerr << "[Keyence::Reset] ** ERROR: could not be executed, response : "
	      << response
                  << std::endl;
        return;
    }
}


void Keyence::ProgramChange(int prog_number)
{
    std::string response = SetValue("PW,",prog_number);
    if(response != "PW"){
        std::cerr << "[Keyence::ProgramChange] ** ERROR: could not be executed, response : "
	      << response
                  << std::endl;
        return;
    }
}



void Keyence::ProgramCheck(int & value)
{
    std::string response = SetValue("PR");
    if(response.find("ER") != std::string::npos || response.find("PR") == std::string::npos ){
        std::cerr << "[Keyence::ProgramCheck] ** ERROR: could not be executed, response : "
	      << response
	      << std::endl;
        return;
    }
    std::istringstream is(response.substr(3,1));
    int temp;
    is >> temp;
    value = temp;
}


/*
void Keyence::Timing(int out, int status)
{
  std::string response = SetValue("T", status, out);
  std::ostringstream os;
  os << "T" << status;
    if(response != os.str()){
    std::cerr << "[Keyence::Timing] ** ERROR: could not be executed, response : "
    << response
    << std::endl;
        return;
    }
}
*/

/*
void Keyence::AutoZero(int out, bool status)
{
std::string response;
  if(status){
    response = SetValue("V", out);
    std::ostringstream os;
    os << "V" << out;
    if(response != os.str()){
    std::cerr << "[Keyence::AutoZero] ** ERROR: could not be executed, response : "
    << response
    << std::endl;
        return;
    }
  }else{
    response = SetValue("W", out);
    std::ostringstream os;
    os << "W" << out;
    if(response != os.str()){
    std::cerr << "[Keyence::AutoZero] ** ERROR: could not be executed, response : "
    << response
    << std::endl;
        return;
    }
  }
}
*/

/*
//FIX ME!!!
void Keyence::StatResultOutput(int out, std::string value)
{
  std::string response = SetValue("DO,", out, value);
    if(response != "PW"){
    std::cerr << "[Keyence::StatResultOutput] ** ERROR: could not be executed, response : "
    << response
    << std::endl;
        return;
    }
}
*/

/*
void Keyence::ClearStat(int out)
{
  std::string response = SetValue("DQ,", out);
  std::stringstream os;
  os << "DQ," << out;
  if(response != os.str()){
    std::cerr << "[Keyence::ClearStat] ** ERROR: could not be executed, response : "
    << response
    << std::endl;
        return;
    }
}
*/

/*
void Keyence::StartDataStorage()
{
  std::string response = SetValue("AS");
  if(response != "AS"){
  std::cerr << "[Keyence::StartDataStorage] ** ERROR: could not be executed, response : "
  << response
  << std::endl;
  return;
  }
  }
*/

/*
void Keyence::StopDataStorage()
{
  std::string response = SetValue("AP");
  if(response != "AP"){
  std::cerr << "[Keyence::StopDataStorage] ** ERROR: could not be executed, response : "
  << response
  << std::endl;
  return;
  }
  }
*/

/*
void Keyence::InitDataStorage()
{
  std::string response = SetValue("AQ");
  if(response != "AQ"){
  std::cerr << "[Keyence::InitDataStorage] ** ERROR: could not be executed, response : "
  << response
  << std::endl;
  return;
  }
  }
*/

/*
//FIX ME
void Keyence::OutputDataStorage(int out, std::vector<double> values)
{
  std::string response = SetValue("AO,",out,values);
    if(response != "PW"){
    std::cerr << "[Keyence::OutputDataStorage] ** ERROR: could not be executed, response : "
    << response
    << std::endl;
        return;
    }
}
*/

/*
//FIX ME
void Keyence::DataStorageStatus(std::string value)
{
  std::string response = SetValue("AN",value);
    if(response != "PW"){
    std::cerr << "[Keyence::DataStorageStatus] ** ERROR: could not be executed, response : "
    << response
    << std::endl;
        return;
    }
}
*/


