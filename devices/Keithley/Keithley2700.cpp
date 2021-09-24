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

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <sstream>

#include "Keithley2700.h"

///
///
///
Keithley2700::Keithley2700( ioport_t port )
    : VKeithley2700(port)
{
  comHandler_ = new KMMComHandler( port );

  isDebug_ = false;
  isScanOk_ = false;

  #ifdef __DEBUG
  isDebug_ = true;
  #endif
 
  Device_Init();
}

///
/// enables the channels given by string
/// format: see ::ParseChannelString
///
void Keithley2700::SetActiveChannels( std::string channelString )
{
  enabledChannels_.resize( 0 );
  enabledChannels_ = ParseChannelString( channelString );

  CalculateDelay();

  Device_SetChannels();
}

void Keithley2700::SetActiveChannels( channels_t channels )
{
  enabledChannels_ = channels;

  CalculateDelay();

  Device_SetChannels();
}

///
/// adds the channels given by string to the list of enabled ones
/// format: see ParseChannelString
///
void Keithley2700::AddActiveChannels( std::string channelString ) {

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
void Keithley2700::DisableActiveChannels( std::string channelString ) {

  const channels_t& newChannels = ParseChannelString( channelString );

  for( channels_t::const_iterator it = newChannels.begin(); it < newChannels.end(); ++it ) {
    
    channels_t::iterator pos = find( enabledChannels_.begin(), enabledChannels_.end(), *it );
    if( enabledChannels_.end() == pos ) {
      std::cerr << " [Keithley2700::DisableActiveChannels] ** WARNING:" << std::endl;
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
const VKeithley2700::reading_t Keithley2700::Scan( void ) {

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
    std::cout << " [Keithley2700::Scan] -- DEBUG: Received "
              <<  tokens.size() / 3 << " reading(s)" << std::endl;
    std::string bufferStr( buffer );
    bufferStr.erase( std::remove( bufferStr.begin(), bufferStr.end(), '\n' ), bufferStr.end() );
    std::cout << " [Keithley2700::Scan] -- DEBUG: <RAWOUTPUT.BEGIN> "
              << bufferStr << " <RAWOUTPUT.END>" << std::endl;;
  }

  if( tokens.size() / 3 != enabledChannels_.size() ) {
    std::cerr << " [Keithley2700::Scan] ** ERROR: expect "
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
void Keithley2700::Dump( void ) const {

  std::cout << " [Keithley2700::Dump] -- Channels enabled in scan: \n ";

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
void Keithley2700::Device_SetChannels( void ) const {

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
    std::cout << " [Keithley2700::Device_SetChannels] -- DEBUG: Sent: \""
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
    std::cout << " [Keithley2700::Device_SetChannels] -- DEBUG: Sent: \""
              << theCommand.str() << "\"" << std::endl;
  }
  comHandler_->SendCommand( theCommand.str().c_str() );
}
  
void Keithley2700::Reset()
{
  comHandler_->SendCommand( "ROUT:SCAN:LSEL NONE" );
  comHandler_->SendCommand( "TRAC:CLE" );
  comHandler_->SendCommand( "ROUT:OPEN:ALL" );

  Device_Init();
}

///
/// all initialization of device
///
void Keithley2700::Device_Init( void ) const {

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
}

///
///
///
void Keithley2700::CalculateDelay( void ) {

  uSecDelay_ = DelayMin + ( DelayMax - DelayMin) / 10 * enabledChannels_.size();

  if( isDebug_ ) {
    std::cout << " [Keithley2700::CalculateDelay] -- DEBUG: Delay is now: "
              << uSecDelay_ << " usec." << std::endl;
  }
}
