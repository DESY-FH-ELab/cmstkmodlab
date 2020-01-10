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

void KeithleyDAQ6510Fake::ActivateChannel(unsigned int card, unsigned int channel,
                                          bool active)
{
  if (card<1 || card>2) return;
  if (channel<1 || channel>10) return;
  activeChannels_[card-1][channel-1] = active;
}

void KeithleyDAQ6510Fake::Scan()
{

}

float KeithleyDAQ6510Fake::GetScanDuration() const
{
  return 0.2 * GetActiveChannelCount();
}
