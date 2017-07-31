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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ConradController.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc <= 2) {
        fprintf(stderr, "Usage: conradTest <device name> <parameter>\n\n");
        fprintf(stderr, "Parameter:\n");
        fprintf(stderr, "   --status : Display status of all channels\n");
        fprintf(stderr, "   --off <x>: Disable channel <x> (1..8)\n");
        fprintf(stderr, "   --on  <x>: Enable  channel <x> (1..8)\n");
        return -1;
    }

    char* deviceName = argv[1];
    fprintf(stderr, "Using device: \"%s\"\n", deviceName);

    ConradController controller(deviceName);
    if (!controller.initialize())
        return -1;

    char* command = argv[2];
    bool isOn = !strcmp(command, "--on");
    bool isOff = !strcmp(command, "--off");

    if (isOn || isOff) {
        if (argc <= 3) {
            fprintf(stderr, "Please pass a <channel> as third argument!\n");
            return -1;
        }

        int channel = atoi(argv[3]);
		fprintf(stderr, "calling: %i, %i\n", channel, isOn);
        controller.setChannel(channel, isOn); 
        return 0;
    }

    if (!strcmp(command, "--status")) {
        std::vector<bool> result = controller.queryStatus();
        std::vector<bool>::iterator it = result.begin();
        std::vector<bool>::iterator end = result.end();

        for (; it != end; ++it)
            fprintf(stderr, "Channel %i: %s\n", int(it - result.begin()) + 1, (*it) ? "on" : "off");

        return 0;
    }


    if (!strcmp(command, "--kitt")) {
        // K.I.T.T. Demo effect - hidden as it's just fun :-)
        int times = 0;
        while (times < 10) {
            for (unsigned i = 1; i <= 8; ++i) {
                controller.setSingleChannel(i, true);
                usleep(1e5);
            }

            for (unsigned i = 7; i >= 2; --i) {
                controller.setSingleChannel(i, true);
                usleep(1e5);
            }

            ++times;
        }

        return 0;
    }

    return 0;
}
