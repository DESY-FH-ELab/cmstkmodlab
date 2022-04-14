/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <unistd.h>

#include "KeithleyDAQ6510Fake.h"

using namespace std;

///
///
///
KeithleyDAQ6510Fake::KeithleyDAQ6510Fake( ioport_t port )
    : VKeithleyDAQ6510(port)
{
  for (unsigned int card = 1;card<=2;++card) {
    availableCards_[card-1] = true;
    for (unsigned int channel = 1;channel<=10;++channel) {
      availableChannels_[card-1][channel-1] = true;
      activeChannels_[card-1][channel-1] = false;
    }
  }
}

void KeithleyDAQ6510Fake::SetChannelMode(unsigned int card, unsigned int channel,
		ChannelMode_t mode)
{
	if (card<1 || card>2) return;
	if (channel<1 || channel>10) return;
	channelModes_[card-1][channel-1] = mode;
}

void KeithleyDAQ6510Fake::ActivateChannel(unsigned int card, unsigned int channel,
                                          bool active)
{
  if (card<1 || card>2) return;
  if (channel<1 || channel>10) return;
  activeChannels_[card-1][channel-1] = active;
}

void KeithleyDAQ6510Fake::Scan()
{
  data_.clear();

  unsigned int i = 0;
  for (unsigned int card = 1;card<=2;++card) {
    for (unsigned int channel = 1;channel<=10;++channel) {
      if (activeChannels_[card-1][channel-1]) {
        double temperature = channel;
        temperature /= 10.;
        temperature += card * 10;
        temperature += normalDistribution_(randomGenerator_);
        unsigned int sensor = card * 100 + channel;
        data_.push_back(std::tuple<unsigned int,double,double>(sensor,temperature,i*0.1));
        i++;
      }
    }
  }
}

void KeithleyDAQ6510Fake::GetScanData(reading_t & data)
{
  data = data_;
}

float KeithleyDAQ6510Fake::GetScanDuration() const
{
  return 0.2 * GetActiveChannelCount();
}
