

#include "Keithley2700.h"


///
///
///
Keithley2700::Keithley2700( ioport_t port ) {

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
void Keithley2700::SetActiveChannels( string channelString ) {
  
  enabledChannels_.resize( 0 );
  enabledChannels_ = ParseChannelString( channelString );

  CalculateDelay();

  Device_SetChannels();

}





///
/// adds the channels given by string to the list of enabled ones
/// format: see ParseChannelString
///
void Keithley2700::AddActiveChannels( string channelString ) {

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
void Keithley2700::DisableActiveChannels( string channelString ) {

  const channels_t& newChannels = ParseChannelString( channelString );

  for( channels_t::const_iterator it = newChannels.begin(); it < newChannels.end(); ++it ) {
    
    channels_t::iterator pos = find( enabledChannels_.begin(), enabledChannels_.end(), *it );
    if( enabledChannels_.end() == pos ) {
      std::cerr << " [Keithley2700::DisableActiveChannels] ** WARNING:" << std::endl;
      std::cerr << "  Request to disable channel: " << *it << " which is currently inactive. Skipping." << std::endl;
    }
    else {
      enabledChannels_.erase( pos );
    }

  }
  
  CalculateDelay();

  Device_SetChannels();

}





///
///
///
const reading_t Keithley2700::Scan( void ) {

  reading_t theReading;
  char buffer[1000];

  // presume that it will work..
  isScanOk_ = true;

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
  std::vector<std::string> tokens;
  Tokenize( string( buffer ), tokens, "," );

  if( isDebug_ ) {
    std::cout << " [Keithley2700::Scan] -- DEBUG: Received " <<  tokens.size() / 3 << " reading(s)" << std::endl;
    std::string bufferStr( buffer );
    bufferStr.erase( std::remove( bufferStr.begin(), bufferStr.end(), '\n' ), bufferStr.end() );
    std::cout << " [Keithley2700::Scan] -- DEBUG: <RAWOUTPUT.BEGIN>" << bufferStr << "<RAWOUTPUT.END>" << std::endl;;
  }

  if( tokens.size() / 3 != enabledChannels_.size() ) {
    std::cerr << " [Keithley2700::Scan] ** ERROR: expect " << enabledChannels_.size() << " reading(s) but received " << tokens.size() / 3 << "." << std::endl;
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
bool Keithley2700::IsScanOk( void ) {
  return isScanOk_;
}





///
/// string:
/// comma separated list of channels or channel ranges
/// e.g. "0,3,6-9,12"
///
const channels_t Keithley2700::ParseChannelString( const string& channelString ) const {

  channels_t channels;
  channels.resize( 0 );
  
  if( 0 == channelString.size() ) {
    if( isDebug_ ) {
      std::cerr << " [Keithley2700::ParseChannelString] ** DEBUG: Received empty string." << std::endl;
    }
    return channels;
  }

  // tokenize comma-separated channels/channel-ranges
  std::vector<string> tokens;
  Tokenize( channelString, tokens, "," );
  
  for( std::vector<std::string>::const_iterator it = tokens.begin(); it < tokens.end(); ++it ) {
    
    // this token gives a single channel (no "-")
    if( std::string::npos == it->find( "-" ) ) {
      unsigned int aChannel = EvaluateChannelToken( *it );
      channels.push_back( aChannel );
    }
    
    // this is supposed to be a range
    else {
      range_t range = EvaluateRangeToken( *it );
      for( unsigned int aChannel = range.first; aChannel <= range.second; ++aChannel ) {
	channels.push_back( aChannel );
      }
    }
    
  }

  // sort & remove duplicates
  std::sort( channels.begin(), channels.end() );
  channels.erase( std::unique( channels.begin(), channels.end() ), channels.end() );

  return channels;
  
}





///
/// itemize a string into tokens
/// delimited by "delimiters"
///
void Keithley2700::Tokenize( const std::string& string, std::vector<std::string>& tokens, const std::string& delimiters ) const {

  // Skip delimiters at beginning.
  string::size_type lastPos = string.find_first_not_of( delimiters, 0 );

  // Find first "non-delimiter".
  string::size_type pos = string.find_first_of( delimiters, lastPos );
  
  while ( std::string::npos != pos || std::string::npos != lastPos ) {

    // Found a token, add it to the vector.
    tokens.push_back( string.substr( lastPos, pos - lastPos ) );

    // Skip delimiters. Note the "not_of"
    lastPos = string.find_first_not_of( delimiters, pos );

    // Find next "non-delimiter"
    pos = string.find_first_of( delimiters, lastPos );

  }

}





///
///
///
const unsigned int Keithley2700::EvaluateChannelToken( const std::string& string) const {

  int channel = atoi( string.c_str() );
  if( channel < __RANGE_MIN || channel > __RANGE_MAX ) {
    std::cerr << " [Keithley2700::EvaluateChannelToken] ** ERROR: Limit violation or malformed string: \"" 
	      << string << "\"." << std::endl;
    throw;
  }

  return (unsigned int) channel ;
}





///
///
///
const range_t Keithley2700::EvaluateRangeToken( const std::string& string ) const {

  std::vector<std::string> tokens;
  Tokenize( string, tokens, "-" );
  range_t range;

  if( tokens.size() == 2 ) {
    range = range_t( atoi( tokens.at( 0 ).c_str() ), atoi( tokens.at( 1 ).c_str() ) );

    if( ! (range.first > range.second || range.first < __RANGE_MIN || range.second > __RANGE_MAX ) ) {
      return range;
    }

  }

  std::cerr << " [Keithley2700::EvaluateRangeToken] ** ERROR: Limit violation or malformed range: \""
	    << string << "\"." << std::endl;
  throw;

}





///
///
///
void Keithley2700::Dump( void ) const {

  std::cout << " [Keithley2700::Dump] -- Channels enabled in scan: \n ";

  for( channels_t::const_iterator it = enabledChannels_.begin(); it < enabledChannels_.end(); ++it ) {
    std::cout << " " << *it;
  }
  std::cout << std::endl;

}





///
/// communicate the currently active channels
/// to the device
///
void Keithley2700::Device_SetChannels( void ) const {

  stringstream theCommand;

  // build rout:scan command
  theCommand << "ROUT:SCAN (@";
  
  for( unsigned int channel = 0; channel < enabledChannels_.size(); ++channel ) {
    theCommand << enabledChannels_.at( channel ) + 101;
    if( channel < enabledChannels_.size() - 1 ) theCommand << ",";
  }

  theCommand << ")";

  // send
  if( isDebug_ ) {
    std::cout << " [Keithley2700::Device_SetChannels] -- DEBUG: Sent: \"" << theCommand.str() << "\"" << std::endl;
  }
  comHandler_->SendCommand( theCommand.str().c_str() );

  // cleanup
  theCommand.str("");


  // build samp:coun command
  
  theCommand << "SAMP:COUN ";
  theCommand << enabledChannels_.size();
  
  // send
  if( isDebug_ ) {
    std::cout << " [Keithley2700::Device_SetChannels] -- DEBUG: Sent: \"" << theCommand.str() << "\"" << std::endl;
  }
  comHandler_->SendCommand( theCommand.str().c_str() );
  
}





///
/// all initialization of device
///
void Keithley2700::Device_Init( void ) const {

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
const void Keithley2700::CalculateDelay( void ) {

  uSecDelay_ = __DELAY_MIN + ( __DELAY_MAX - __DELAY_MIN ) / 10 * enabledChannels_.size();

  if( isDebug_ ) {
    std::cout << " [Keithley2700::CalculateDelay] -- DEBUG: Delay is now: " << uSecDelay_ << " usec." << std::endl;
  }

}
