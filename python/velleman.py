#/////////////////////////////////////////////////////////////////////////////////#
#//                                                                             //#
#//                                                                             //#
#//                                                                             //#
#//                   Written by Elise Hinkle (Brown CMS Group)                 //#
#//                    Modeled on cmstkmodlab/python/conrad.py                  //#
#//                          Last Updated August 21, 2019                       //#
#//                                                                             //#
#//                                                                             //#
#//                                                                             //#
#/////////////////////////////////////////////////////////////////////////////////#

import PyTkModLabVelleman

velleman = PyTkModLabVelleman.Velleman("/dev/ttyACM0")
status = velleman.queryRelayStatus()

for s in status:
    print s


