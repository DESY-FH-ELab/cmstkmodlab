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

#include <unistd.h>

#include "Keithley2700Fake.h"

using namespace std;

///
///
///
Keithley2700Fake::Keithley2700Fake( ioport_t port )
    : VKeithley2700(port)
{

}

///
/// enables the channels given by string
/// format: see ::ParseChannelString
///
void Keithley2700Fake::SetActiveChannels( string channelString )
{
  enabledChannels_.resize( 0 );
  enabledChannels_ = ParseChannelString( channelString );
}

void Keithley2700Fake::SetActiveChannels( channels_t channels )
{
  enabledChannels_ = channels;
}

///
/// adds the channels given by string to the list of enabled ones
/// format: see ParseChannelString
///
void Keithley2700Fake::AddActiveChannels( string channelString )
{
  // append new channels
  const channels_t& newChannels = ParseChannelString( channelString );
  enabledChannels_.insert( enabledChannels_.end(), newChannels.begin(), newChannels.end() );

  // re-sort and remove duplicates
  std::sort( enabledChannels_.begin(), enabledChannels_.end() );
  enabledChannels_.erase( std::unique( enabledChannels_.begin(), enabledChannels_.end() ), enabledChannels_.end() );
}

///
/// removes the channels given by string from the list of enabled ones
/// format: see ParseChannelString
///
void Keithley2700Fake::DisableActiveChannels( string channelString ) {

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
}

///
///
///
const VKeithley2700::reading_t Keithley2700Fake::Scan( void )
{
  reading_t theReading;

  for (channels_t::const_iterator channelsIt = enabledChannels_.begin();
       channelsIt != enabledChannels_.end();
       ++channelsIt) {
    theReading.push_back( std::pair<unsigned int, double>( *channelsIt, 10.0 + *channelsIt + (std::rand() % 100)/100. ) );
    usleep(500);
  }

  return theReading;
}

///
///
///
bool Keithley2700Fake::IsScanOk( void )
{
  return true;
}

///
///
///
void Keithley2700Fake::Dump( void ) const
{
  std::cout << " [Keithley2700Fake::Dump] -- Channels enabled in scan: \n ";

  for( channels_t::const_iterator it = enabledChannels_.begin(); it < enabledChannels_.end(); ++it ) {
    std::cout << " " << *it;
  }
  std::cout << std::endl;
}
