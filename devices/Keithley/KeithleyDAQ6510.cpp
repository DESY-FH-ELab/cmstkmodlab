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
#include <cstdlib>
#include <algorithm>
#include <sstream>

#include "KeithleyDAQ6510.h"

///
///
///
KeithleyDAQ6510::KeithleyDAQ6510( ioport_t port )
    : VKeithleyDAQ6510(port)
{
  comHandler_ = new KeithleyUSBTMCComHandler( port );

  /*
  isDebug_ = false;
  isScanOk_ = false;

  #ifdef __DEBUG
  isDebug_ = true;
  #endif
  */
  
  DeviceInit();
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

///
/// communicate the currently active channels
/// to the device
///
void KeithleyDAQ6510::Device_SetChannels( void ) const {

  std::stringstream theCommand;

  // build rout:scan command
  theCommand << "ROUT:SCAN (@";
  
  for( unsigned int channel = 0; channel < enabledChannels_.size(); ++channel ) {
    unsigned int theChannel = enabledChannels_.at( channel );
    if (theChannel<10) {
      theCommand << enabledChannels_.at( channel ) + 101;
    } else {
      theCommand << enabledChannels_.at( channel ) + 201 - 10;
    }
    if( channel < enabledChannels_.size() - 1 ) theCommand << ",";
  }

  theCommand << ")";

  // send
  if( isDebug_ ) {
    std::cout << " [KeithleyDAQ6510::Device_SetChannels] -- DEBUG: Sent: \""
              << theCommand.str() << "\"" << std::endl;
  }
  comHandler_->SendCommand( theCommand.str().c_str() );

  // cleanup
  theCommand.str("");

  // build samp:coun command
  
  theCommand << "SAMP:COUN ";
  theCommand << enabledChannels_.size();
  
  // send
  if( isDebug_ ) {
    std::cout << " [KeithleyDAQ6510::Device_SetChannels] -- DEBUG: Sent: \""
              << theCommand.str() << "\"" << std::endl;
  }
  comHandler_->SendCommand( theCommand.str().c_str() );
}
  
void KeithleyDAQ6510::Reset()
{
  comHandler_->SendCommand( "ROUT:SCAN:LSEL NONE" );
  comHandler_->SendCommand( "TRAC:CLE" );
  comHandler_->SendCommand( "ROUT:OPEN:ALL" );

  Device_Init();
}
*/

///
/// all initialization of device
///
void KeithleyDAQ6510::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {
    
    char buffer[1000];
    std::string buf;
    
    comHandler_->SendCommand("*IDN?");
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;
    std::cout << buf << std::endl;
    if (buf.find("KEITHLEY INSTRUMENTS,MODEL DAQ6510", 0)!=0) return;
    
    comHandler_->SendCommand("syst:card1:idn?");
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;
    std::cout << buf << std::endl;
    if (buf.find("7700,Pseudo 20Ch Mux w/CJC", 0)!=0) return;

    comHandler_->SendCommand("syst:card2:idn?");
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;
    std::cout << buf << std::endl;
    if (buf.find("7700,Pseudo 20Ch Mux w/CJC", 0)!=0) return;
    
    isDeviceAvailable_ = true;
  }

  /*
  // enable buffer auto clear
  comHandler_->SendCommand( "TRAC:CLE:AUTO ON" );

  // disable scan
  // in case still enabled from previous (aborted) scan call
  // otherwise we get a settings conflict (ERR -221)
  comHandler_->SendCommand( "ROUT:SCAN:LSEL NONE" );

  // we want to measure temperature on channels 101 - 110
  comHandler_->SendCommand( "SENS:FUNC 'TEMP',(@101:110)" );

  // number of digits
  comHandler_->SendCommand( "SENSE:TEMP:DIG 4" );

  // and we use 4-wire sensors there
  comHandler_->SendCommand( "SENS:TEMP:TRAN FRTD,(@101:110)" );

  // pt100, exactly speaking
  comHandler_->SendCommand( "SENS:TEMP:FRTD:TYPE PT100,(@101:110)" );

  // disable continuous initiation
  comHandler_->SendCommand( "INIT:CONT OFF" );

  // trigger control = immediate
  comHandler_->SendCommand( "TRIG:SOUR IMM" );

  // one reading per scan
  comHandler_->SendCommand( "TRIG:COUN 1" );

  // scan path,
  // init with full channel list,
  // will be overwritten
  comHandler_->SendCommand( "ROUT:SCAN (@101:110)" );
  comHandler_->SendCommand( "SAMP:COUN 10" );

  // scanning trigger source = immediate
  comHandler_->SendCommand( "ROUT:SCAN:TSO IMM" );

  // open all channels
  //  comHandler_->SendCommand( "ROUT:OPEN:ALL" );
  */
}

void KeithleyDAQ6510::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<strlen(buffer);++c) {
    if(buffer[c]=='\n') {
      buffer[c] = 0;
      break;
    }
  }
}

/*
///
///
///
void KeithleyDAQ6510::CalculateDelay( void ) {

  uSecDelay_ = DelayMin + ( DelayMax - DelayMin) / 10 * enabledChannels_.size();

  if( isDebug_ ) {
    std::cout << " [KeithleyDAQ6510::CalculateDelay] -- DEBUG: Delay is now: "
              << uSecDelay_ << " usec." << std::endl;
  }
}
*/