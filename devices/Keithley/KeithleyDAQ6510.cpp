/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <sstream>

#include "KeithleyDAQ6510.h"

///
///
///
KeithleyDAQ6510::KeithleyDAQ6510(ioport_t port)
    : VKeithleyDAQ6510(port)
{
  comHandler_ = new KeithleyUSBTMCComHandler(port);

  /*
  isDebug_ = false;
  isScanOk_ = false;

  #ifdef __DEBUG
  isDebug_ = true;
  #endif
  */
  
  DeviceInit();
}

void KeithleyDAQ6510::ActivateChannel(unsigned int card, unsigned int channel,
                                      bool active)
{
  if (card<1 || card>2) return;
  if (channel<1 || channel>10) return;
  activeChannels_[card-1][channel-1] = active;

  DeviceSetChannels();
}

/*
///
/// enables the channels given by string
/// format: see ::ParseChannelString
///
void KeithleyDAQ6510::SetActiveChannels( std::string channelString )
{
  enabledChannels_.resize( 0 );
  enabledChannels_ = ParseChannelString( channelString );

  CalculateDelay();

  Device_SetChannels();
}

void KeithleyDAQ6510::SetActiveChannels( channels_t channels )
{
  enabledChannels_ = channels;

  CalculateDelay();

  Device_SetChannels();
}

///
/// adds the channels given by string to the list of enabled ones
/// format: see ParseChannelString
///
void KeithleyDAQ6510::AddActiveChannels( std::string channelString ) {

  // append new channels
  const channels_t& newChannels = ParseChannelString( channelString );
  enabledChannels_.insert( enabledChannels_.end(), newChannels.begin(), newChannels.end() );

  // re-sort and remove duplicates
  std::sort( enabledChannels_.begin(), enabledChannels_.end() );
  enabledChannels_.erase( std::unique( enabledChannels_.begin(), enabledChannels_.end() ), enabledChannels_.end() );

  CalculateDelay();

  Device_SetChannels();
}

///
/// removes the channels given by string from the list of enabled ones
/// format: see ParseChannelString
///
void KeithleyDAQ6510::DisableActiveChannels( std::string channelString ) {

  const channels_t& newChannels = ParseChannelString( channelString );

  for( channels_t::const_iterator it = newChannels.begin(); it < newChannels.end(); ++it ) {
    
    channels_t::iterator pos = find( enabledChannels_.begin(), enabledChannels_.end(), *it );
    if( enabledChannels_.end() == pos ) {
      std::cerr << " [KeithleyDAQ6510::DisableActiveChannels] ** WARNING:" << std::endl;
      std::cerr << "  Request to disable channel: " << *it
                << " which is currently inactive. Skipping." << std::endl;
    } else {
      enabledChannels_.erase( pos );
    }

  }
  
  CalculateDelay();

  Device_SetChannels();
}

///
///
///
const reading_t KeithleyDAQ6510::Scan( void ) {

  reading_t theReading(0);
  char buffer[1000];

  // presume that it will work..
  isScanOk_ = true;

  // clear buffer
  comHandler_->SendCommand( "TRAC:CLE" );

  // enable scan
  comHandler_->SendCommand( "ROUT:SCAN:LSEL INT" );
  
  // go!
  comHandler_->SendCommand( "READ?" );

  // wait until scanned
  usleep( uSecDelay_ );

  // get output
  comHandler_->ReceiveString( buffer );

  // disable scan
  comHandler_->SendCommand( "ROUT:SCAN:LSEL NONE" );

  // tokenize output
  std::vector<std::string> tokens(0);
  Tokenize( std::string( buffer ), tokens, "," );

  if( isDebug_ ) {
    std::cout << " [KeithleyDAQ6510::Scan] -- DEBUG: Received "
              <<  tokens.size() / 3 << " reading(s)" << std::endl;
    std::string bufferStr( buffer );
    bufferStr.erase( std::remove( bufferStr.begin(), bufferStr.end(), '\n' ), bufferStr.end() );
    std::cout << " [KeithleyDAQ6510::Scan] -- DEBUG: <RAWOUTPUT.BEGIN> "
              << bufferStr << " <RAWOUTPUT.END>" << std::endl;;
  }

  if( tokens.size() / 3 != enabledChannels_.size() ) {
    std::cerr << " [KeithleyDAQ6510::Scan] ** ERROR: expect "
              << enabledChannels_.size() << " reading(s) but received "
              << tokens.size() / 3 << "." << std::endl;
    std::cerr << "                         Probably a timing problem.." << std::endl;
    isScanOk_ = false;
    if( isDebug_ ) throw;
  }
  
  std::vector<std::string>::const_iterator tokensIt = tokens.begin();
  channels_t::const_iterator channelsIt = enabledChannels_.begin();
  for( ; tokensIt < tokens.end(); tokensIt += 3, ++channelsIt ) {
    const double convertedReading = atof( tokensIt->c_str() );
    theReading.push_back( std::pair<unsigned int, double>( *channelsIt, convertedReading ) );
  }

  return theReading;
}

///
///
///
void KeithleyDAQ6510::Dump( void ) const {

  std::cout << " [KeithleyDAQ6510::Dump] -- Channels enabled in scan: \n ";

  for( channels_t::const_iterator it = enabledChannels_.begin();
      it < enabledChannels_.end();
      ++it ) {
    std::cout << " " << *it;
  }
  std::cout << std::endl;
}
*/

bool KeithleyDAQ6510::GetScanStatus() const
{
  char buffer[1000];
  std::string buf;

  comHandler_->SendCommand("ROUT:SCAN:STAT?");
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);
  buf = buffer;
  std::cout << buf << std::endl;

  return true;
}

void KeithleyDAQ6510::Scan()
{
  char buffer[1000];
  std::string buf;

  comHandler_->SendCommand("TRAC:CLE");

  comHandler_->SendCommand("INIT");
}

void KeithleyDAQ6510::GetScanData(reading_t & data)
{
  char buffer[1000];
  std::string buf;

  std::stringstream ss;
  ss << "TRAC:DATA? 1, ";
  ss << GetActiveChannelCount();
  ss << ", 'defbuffer1', CHAN, READ, REL";

  comHandler_->SendCommand(ss.str().c_str());
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);
  buf = buffer;
  std::cout << buf << std::endl;

  std::vector<std::string> tokens(0);
  Tokenize(buf, tokens, ",");

  for (std::vector<std::string>::iterator it=tokens.begin();
       it!=tokens.end();
       ++it) {
    unsigned int sensor = std::atoi(it->c_str()); ++it;
    double temperature = std::atof(it->c_str()); ++it;
    double relTime = std::atof(it->c_str());

    data.push_back(std::tuple<unsigned int,double,double>(sensor,temperature,relTime));
  }
}

float KeithleyDAQ6510::GetScanDuration() const
{
  unsigned int channelCount = GetActiveChannelCount();

  return channelCount * (0.1 + 0.1 + 0.1);
}

void KeithleyDAQ6510::DeviceSetChannels()
{
  std::stringstream ss;
  unsigned int count;

  // build rout:scan command
  ss << "ROUT:SCAN (@";
  
  count = 0;
  for (unsigned int channel = 1;channel<=10;++channel) {
    if (activeChannels_[0][channel-1]) count++;
  }
  if (count) {
    ss << CreateChannelString(1, activeChannels_[0]);
  }

  count = 0;
  for (unsigned int channel = 1;channel<=10;++channel) {
    if (activeChannels_[0][channel-1]) count++;
  }
  if (count) {
    ss << ",";
    ss << CreateChannelString(2, activeChannels_[1]);
  }

  ss << ")";

  std::cout << ss.str() << std::endl;

  comHandler_->SendCommand(ss.str().c_str());
}

/*
void KeithleyDAQ6510::Reset()
{
  comHandler_->SendCommand( "ROUT:SCAN:LSEL NONE" );
  comHandler_->SendCommand( "TRAC:CLE" );
  comHandler_->SendCommand( "ROUT:OPEN:ALL" );

  Device_Init();
}
*/

void KeithleyDAQ6510::DeviceInit()
{
  isDeviceAvailable_ = false;

  std::cout << "void KeithleyDAQ6510::DeviceInit()" << std::endl;
  
  if (comHandler_->DeviceAvailable()) {
 
  std::cout << "void KeithleyDAQ6510::DeviceInit()" << std::endl;
    
    char buffer[1000];
    std::string buf;

    // get device identifyer
    comHandler_->SendCommand("*IDN?");
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;
    std::cout << buf << std::endl;
    if (buf.find("KEITHLEY INSTRUMENTS,MODEL DAQ6510", 0)!=0) return;

    // reset the device
    comHandler_->SendCommand("*RST");

    // abort a possibly running scan
    //comHandler_->SendCommand("TRAC:ABOR"); //check

    // open all routings
    comHandler_->SendCommand("ROUT:OPEN:ALL");

    // set ascii format precision
    comHandler_->SendCommand("FORM:ASC:PREC 9");

    comHandler_->SendCommand("SYST:CARD1:IDN?");
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;
    std::cout << buf << std::endl;
    if (buf.find("7700,20Ch Mux w/CJC", 0)==0) {
      availableCards_[0] = true;
      for (unsigned int channel = 1;channel<=10;++channel) {
        availableChannels_[0][channel-1] = true;
        activeChannels_[0][channel-1] = false;
      }

      comHandler_->SendCommand("SENS:FUNC 'VOLT', (@101:120)");
      //comHandler_->SendCommand("SENS:FUNC 'TEMP', (@101:110)");
      //comHandler_->SendCommand("SENS:TEMP:TRAN FRTD, (@101:110)");
      //comHandler_->SendCommand("SENS:TEMP:RTD:FOUR PT100, (@101:110)");
      //comHandler_->SendCommand("SENS:TEMP:UNIT CELS, (@101:110)");

      comHandler_->SendCommand("ROUT:DEL 0.1, (@101:110)");
    }

    comHandler_->SendCommand("SYST:CARD2:IDN?");
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;
    std::cout << buf << std::endl;
    if (buf.find("7700,20Ch Mux w/CJC", 0)==0) {
      availableCards_[1] = true;
      for (unsigned int channel = 1;channel<=10;++channel) {
        availableChannels_[1][channel-1] = true;
        activeChannels_[1][channel-1] = false;
      }

      comHandler_->SendCommand("SENS:FUNC 'VOLT', (@201:220)");
      //comHandler_->SendCommand("SENS:FUNC 'TEMP', (@201:210)");
      //comHandler_->SendCommand("SENS:TEMP:TRAN FRTD, (@201:210)");
      //comHandler_->SendCommand("SENS:TEMP:RTD:FOUR PT100, (@201:210)");
      //comHandler_->SendCommand("SENS:TEMP:UNIT CELS, (@201:210)");

      comHandler_->SendCommand("ROUT:DEL 0.1, (@201:210)");
    }
    
    isDeviceAvailable_ = true;
  }
}

void KeithleyDAQ6510::StripBuffer(char* buffer) const
{
  for (unsigned int c=0;c<std::strlen(buffer);++c) {
    if(buffer[c]=='\n') {
      buffer[c] = 0;
      break;
    }
  }
}
