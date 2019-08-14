/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//            Modeled on cmstkmodlab/devices/Conrad/utils/conradTest.cpp       //
//                           Last Updated August 20, 2019                      //                
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////


#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "VellemanController.h"
#include "VellemanCommunication_cpp.h"
#include "IVellemanController_cpp.h"
#include "VellemanController_cpp.h"
#include "VellemanControllerFake_cpp.h"

// NOTE: These two #ifndef statements are important (issue with --off if left out)
#ifndef CMD_SWITCH_RELAY_ON
#define CMD_SWITCH_RELAY_ON 0x11
#endif

#ifndef CMD_SWITCH_RELAY_OFF
#define CMD_SWITCH_RELAY_OFF 0x12
#endif

#include <iostream>

int main(int argc, char** argv)
{
  if (argc <= 2) {
    fprintf(stderr, "Usage: vellemanTest <device name> <parameter>\n\n");
    fprintf(stderr, "Parameter:\n");
    fprintf(stderr, "   --status : Display status of all relays\n");
    fprintf(stderr, "   --off <x>: Disable relay <x> (1..8)\n");
    fprintf(stderr, "   --on  <x>: Enable  relay <x> (1..8)\n");
    fprintf(stderr, "   --multiON  <a b..z>: Enable relays <a b..z> (1..8) without affecting other relays\n");
    fprintf(stderr, "   --multiOFF  <a b..z>: Disable relays <a b..z> (1..8) without affecting other relays\n");
    return -1;
  }

  char* deviceName = argv[1];
  fprintf(stderr, "Using device: \"%s\"\n", deviceName);

  VellemanController controller(deviceName);
  if (!controller.initialize()) {
    fprintf(stderr, "[Velleman] ERROR (port %s): Failure to initialize Velleman relay card, ERRNO %i\n", controller.comPort().c_str(), errno);
    return -1;
  }

  char* command = argv[2];

  // Execute on or off command for SINGLE relay
  bool isOn = !strcmp(command, "--on");
  bool isOff = !strcmp(command, "--off");

  if (isOn || isOff) {
    if (argc <= 3) {
      fprintf(stderr, "[Velleman] ERROR (port %s): Please pass a <relay> as a third argument!, ERRNO %i\n", controller.comPort().c_str(), errno);
      return -1;
    }

    int relay = atoi(argv[3]);

    fprintf(stderr, "[Velleman] Calling %i, %s\n", relay, (isOn) ? "ON" : "OFF");
    controller.setRelay(relay, (isOn) ? CMD_SWITCH_RELAY_ON : CMD_SWITCH_RELAY_OFF);
    return 0;
  }

  // Execute on or off command for MULTIPLE relays (all inputs on or all inputs off)
  bool multiOn = !strcmp(command, "--multiON");
  bool multiOff = !strcmp(command, "--multiOFF");
  
  if (multiOn || multiOff) {
    if (argc <= 3) {
      fprintf(stderr, "[Velleman] ERROR (port %s): Please pass at least one <relay> as an additional argument!, ERRNO %i\n", controller.comPort().c_str(), errno);
      return -1;
    }

    std::vector<unsigned> relays;
    
    for (int i = 3; i < argc; ++i) {
      unsigned relay = atoi(argv[i]);
      relays.push_back(relay);
      fprintf(stderr, "[Velleman] Calling %i, %s\n", relay, (multiOn) ? "ON" : "OFF");
    }

    controller.setMultiRelays(relays, (multiOn) ? CMD_SWITCH_RELAY_ON : CMD_SWITCH_RELAY_OFF);
    return 0;
  }

  // Execute command to check relay status
  if (!strcmp(command, "--status")) {
    std::vector<bool> result = controller.queryRelayStatus();

    for (int i=0; i<8 ; ++i) 
      fprintf(stderr, "[Velleman] Channel %i: %s\n", i + 1, (result[i]) ? "ON" : "OFF");

    return 0;
  }

  // K.I.T.T. demo effect from Conrad driver -- just for fun :)
  if (!strcmp(command, "--kitt")) {

    int times = 0;

    while (times < 2) {
      for (unsigned i = 1; i <= 8; ++i) {
	controller.setSingleRelay(i, CMD_SWITCH_RELAY_ON);
	usleep(5e5);
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
