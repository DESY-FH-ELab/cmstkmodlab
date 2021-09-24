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

#ifndef __VKEITHLEYDAQ6510_H
#define __VKEITHLEYDAQ6510_H

#include <string>
#include <array>
#include <vector>
#include <tuple>

typedef const char* ioport_t;

class VKeithleyDAQ6510
{
 public:
  
  typedef std::array<bool, 10> channels_t;
  typedef std::array<channels_t,2> cards_t;
  typedef std::vector<std::tuple<unsigned int,double,double>> reading_t;
  
  VKeithleyDAQ6510( ioport_t );
  virtual ~VKeithleyDAQ6510();
    
  virtual bool DeviceAvailable() const = 0;

  const std::array<bool,2> & GetAvailableCards() const { return availableCards_; }
  bool IsCardAvailable(unsigned int card) const;

  virtual void SetTime(int year, int month, int day,
                       int hour, int minute, int second) = 0;
  
  const cards_t & GetAvailableChannels() const { return availableChannels_; }
  bool IsChannelAvailable(unsigned int card, unsigned int channel) const;
  bool IsChannelAvailable(unsigned int channel) const;
  
  virtual void ActivateChannel(unsigned int card, unsigned int channel,
                               bool active) = 0;
  unsigned int GetActiveChannelCount() const;

  virtual bool GetScanStatus() const = 0;
  virtual void Scan() = 0;
  virtual void GetScanData(reading_t & data) = 0;

  /*
  virtual void SetActiveChannels( std::string ) = 0;
  virtual void SetActiveChannels( channels_t ) = 0;
  virtual void AddActiveChannels( std::string ) = 0;
  virtual void DisableActiveChannels( std::string ) = 0;
  virtual const reading_t Scan( void ) = 0;
  virtual void Dump( void ) const = 0;
  virtual bool IsScanOk( void ) = 0;
  virtual void Reset() = 0;

  const channels_t GetActiveChannels() { return enabledChannels_; }

  // the number of channels available to the device,
  // for range checking purposes
  static constexpr unsigned int RangeMin = 0;
  static constexpr unsigned int RangeMax = 9;
  */
  
  virtual float GetScanDuration() const = 0;
  std::string CreateChannelString(unsigned int card, channels_t& channels);

 protected:

  std::array<bool,2> availableCards_;
  cards_t availableChannels_;
  cards_t activeChannels_;

  void Tokenize( const std::string&, std::vector<std::string>&,
                 const std::string& ) const;
};

#endif
