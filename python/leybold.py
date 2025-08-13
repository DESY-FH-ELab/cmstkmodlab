#/////////////////////////////////////////////////////////////////////////////////
#//                                                                             //
#//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
#//                           All rights reserved                               //
#//                                                                             //
#//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
#//      You have the right to modify and/or redistribute this source code      //      
#//      under the terms specified in the license, which may be found online    //
#//      at http://www.gnu.org/licenses or at License.txt.                      //
#//                                                                             //
#/////////////////////////////////////////////////////////////////////////////////

import time
import PyTkModLabLeybold

leybold = PyTkModLabLeybold.LeyboldGraphixThree("/dev/ttyLeybold")

print "version:   " + leybold.GetVersion()

print "date:      " + leybold.GetDate()
print "time:      " + leybold.GetTime()

