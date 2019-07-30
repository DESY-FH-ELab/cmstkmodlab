/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//            Modeled on cmstkmodlab/devices/Conrad/utils/conradTest.cpp       //
//                           Last Updated August 9, 2019                       //                
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////


#include <uistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "VellemanController.h"

#include <iostream>

int main(int argc, char** argv)
{
  if (argc <= 2) {
    fprintf(stderr, "Usage: vellemanTest <device name> <parameter>\n\n");
    fprintf(stderr, "Parameter:\n");
    fprintf(stderr, "   --status : Display status of all relays\n");
    fprintf(stderr, "   --off <x>: Disable relay <x> (1..8)\n");
    fprintf(stderr, "   --on  <x>: Enable  relay <x> (1..8)\n");
    return -1;
  }

  char* deviceName = argv[1];
  fprintf(stderr, "Using device: \"%s\"\n", deviceName);

  VellemanController controller(deviceName);
  if (!controller.initialize()) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to initialize Velleman relay card, ERRNO %i\n", controller->comPort().c_str(), errno);
    return -1;
  }

  char* command = argv[2];
  bool isOn = !strcmp(command, "--on");
  bool isOff = !strcmp(command, "--off");

  if (isOn || isOff) {
    if (argc <= 3) {
      fprintf(stderr, "[Velleman] ERROR (port %s): Please pass a <relay> as a third argument!, ERRNO %i\n", controller->comPort().c_str(), errno);
      return -1;
    }

    int relay = atoi(argv[3]);

    fprintf(stderr, "[Velleman] Calling %i, %i\n", relay, (isOn) ? isOn : isOff);
    controller.setRelay(relay, (isOn) ? CMD_SWITCH_RELAY_ON : CMD_SWITCH_RELAY_OFF);
    return 0;
  }
    
  if (!strcmp(command, "--status")) {
    std::vector<bool> result = controller.queryRelayStatus();

    // NOTE: changed from using std::vector<bool> iterator because std::vector<bool> is
    //       not an effective container for use with iteration
    for (int i=0; i<8 ; ++i) 
      fprintf(stderr, "[Velleman] Channel %i: %s\n", i + 1, (result[i]) ? "on" : "off");

    return 0;
  }


  if (!strcmp(command, "--kitt")) {
    // K.I.T.T. demo effect (for fun!)
    int times = 0;
    while (times < 10) {
      for (unsigned i = 1; i <= 8; ++i) {
	controller.setSingleRelay(i, CMD_SWITCH_RELAY_ON);
	usleep(1e5);
      }

      for (unsigned i = 7; i >= 2; --i) {
	controller.setSingleRelay(i, CMD_SWITCH_RELAY_ON);
	usleep(1e5);
      }

      ++times;
    }

    return 0;
  }
  
  return 0;
}
