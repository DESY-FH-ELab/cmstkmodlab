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
#include <utility>
#include <cstdlib>
#include <algorithm>
#include <sstream>

#include "VKeithleyDAQ6510.h"

VKeithleyDAQ6510::VKeithleyDAQ6510( ioport_t port )
{

}

VKeithleyDAQ6510::~VKeithleyDAQ6510()
{

}

bool VKeithleyDAQ6510::IsCardAvailable(unsigned int card) const
{
  if (card<1 || card>2) return false;
  return availableCards_[card-1];
}

bool VKeithleyDAQ6510::IsChannelAvailable(unsigned int card, unsigned int channel) const
{
  if (card<1 || card>2) return false;
  if (channel<1 || channel>10) return false;
  return availableChannels_[card-1][channel-1];
}

bool VKeithleyDAQ6510::IsChannelAvailable(unsigned int channel) const
{
  unsigned int card = channel / 100;
  unsigned int ch = channel % 100;
  return IsChannelAvailable(card, ch);
}

/*
///
/// string:
/// comma separated list of channels or channel ranges
/// e.g. "0,3,6-9,12"
///
const channels_t VKeithleyDAQ6510::ParseChannelString( const std::string& channelString ) const
{
  channels_t channels;
  channels.resize( 0 );

  if( 0 == channelString.size() ) {
    std::cerr << " [VKeithleyDAQ6510::ParseChannelString] ** DEBUG: Received empty string."
              << std::endl;
    return channels;
  }

  // tokenize comma-separated channels/channel-ranges
  std::vector<std::string> tokens;
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
void VKeithleyDAQ6510::Tokenize(const std::string& string,
                             std::vector<std::string>& tokens,
                             const std::string& delimiters ) const
{
  // Skip delimiters at beginning.
  std::string::size_type lastPos = string.find_first_not_of( delimiters, 0 );

  // Find first "non-delimiter".
  std::string::size_type pos = string.find_first_of( delimiters, lastPos );

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
unsigned int VKeithleyDAQ6510::EvaluateChannelToken( const std::string& string) const
{
  int channel = atoi( string.c_str() );
  if( channel < RangeMin || channel > RangeMax ) {
    std::cerr << " [VKeithleyDAQ6510::EvaluateChannelToken] ** ERROR: Limit violation or malformed string: \""
              << string << "\"." << std::endl;
    throw;
  }

  return (unsigned int) channel ;
}

///
///
///
const range_t VKeithleyDAQ6510::EvaluateRangeToken( const std::string& string ) const
{
  std::vector<std::string> tokens;
  Tokenize( string, tokens, "-" );
  range_t range;

  if( tokens.size() == 2 ) {
    range = range_t( atoi( tokens.at( 0 ).c_str() ), atoi( tokens.at( 1 ).c_str() ) );

    if( ! (range.first > range.second || range.second > RangeMax ) ) {
      return range;
    }
  }

  std::cerr << " [VKeithleyDAQ6510::EvaluateRangeToken] ** ERROR: Limit violation or malformed range: \""
            << string << "\"." << std::endl;
  throw;
}
*/


std::string VKeithleyDAQ6510::CreateChannelString(unsigned int card, channels_t& channels)
{
  std::stringstream ss;
  
  bool inRange = false;
  unsigned int rangeStart = 0;
  unsigned int rangeEnd = 0;
  bool hasPrevious = false;
  for (int i=0;i<10;++i) {
    
    // std::cout << card*100+i+1 << ": " << channels[i] << std::endl;
    
    if (channels[i]) {
      if (!inRange) {
        rangeStart = i;
      }
      rangeEnd = i;
      
      if (!inRange) {
        if (hasPrevious) ss << ",";
        ss << card * 100 + i+1;
      }
      
      if (inRange && i==9) {
        if (rangeEnd>rangeStart) ss << ":" << card * 100 + rangeEnd+1;
      }
      
      inRange = true;
      hasPrevious = true;
    } else {
      if (inRange) {
        if (rangeEnd>rangeStart) ss << ":" << card * 100 + rangeEnd+1;
        inRange = false;
      }
    }
  }

  return ss.str();
}

unsigned int VKeithleyDAQ6510::GetActiveChannelCount() const
{
  unsigned int count = 0;
  for (unsigned int card = 1;card<=2;++card) {
    for (unsigned int channel = 1;channel<=10;++channel) {
      if (activeChannels_[card-1][channel-1]) count++;
    }
  }
  return count;
}
