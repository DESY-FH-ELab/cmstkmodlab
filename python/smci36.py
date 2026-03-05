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
import PyTkModLabNanotec

# connect to device
smci36 = PyTkModLabNanotec.NanotecSMCI36("/dev/tty.SLAB_USBtoUART")

# set the ramp mode to trapezoidal (see VNanotecSMCI36.h)
smci36.SetRampMode(1)

# get the status of the controller (see VNanotecSMCI36.h)
print "status:              " + repr(smci36.GetStatus())

# get the position of the encoder 
print "encoder position:    " + repr(smci36.GetEncoderPosition())

# get the position of the controller
print "controller position: " + repr(smci36.GetPosition()) + "\n"

# set the maximum step frequency
smci36.SetMinimumFrequency(80)

# set the maximum step frequency
smci36.SetMaximumFrequency(800)

# set number of steps
smci36.SetTravelDistance(800)

# set positioning mode to absolute (see VNanotecSMCI36.h)
smci36.SetPositioningMode(2)

# start the motor
smci36.Start()

time.sleep(1)

while (smci36.GetStatus()!=161):
	# get the status of the controller (see VNanotecSMCI36.h)
	print "status:              " + repr(smci36.GetStatus())
	
	# get the position of the encoder 
	print "encoder position:    " + repr(smci36.GetEncoderPosition())
	
	# get the position of the controller
	print "controller position: " + repr(smci36.GetPosition()) + "\n"
	
	time.sleep(1)
	
# get the position of the encoder 
print "encoder position:    " + repr(smci36.GetEncoderPosition())

# get the position of the controller
print "controller position: " + repr(smci36.GetPosition())
