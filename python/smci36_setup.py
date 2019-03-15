#/////////////////////////////////////////////////////////////////////////////////
#//                                                                             //
#//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
#//                           All rights reserved                               //
#//                                                                             //
#//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
#//      You have the right to modify and/or redistribute this source code      //      
#//      under the terms specified in the license, which may be found online    //
#//      at http://www.gnu.org/licenses or at License.txt.                      //
#//                                                                             //
#/////////////////////////////////////////////////////////////////////////////////

import PyTkModLabNanotec

# connect to device
smci36 = PyTkModLabNanotec.NanotecSMCI36("/dev/tty.SLAB_USBtoUART")

smci36.SetMotorID(0)

smci36.SetMotorType(0)
smci36.SetPhaseCurrent(50)
smci36.SetStandStillPhaseCurrent(10)
smci36.SetStepMode(4)
smci36.SetRampMode(1)
smci36.SetErrorCorrectionMode(1)
smci36.SetMaxEncoderDeviation(4)
smci36.SetDirection(False)
smci36.SetEncoderDirection(False)
smci36.SetQuickstopRampHzPerSecond(3000000)
smci36.SetAccelerationRampHzPerSecond(50000)
smci36.SetDecelerationRampHzPerSecond(0)
smci36.SetInputPinFunction(1, 1)
smci36.SetInputPinFunction(2, 2)
smci36.SetInputPinFunction(3, 3)
smci36.SetInputPinFunction(4, 4)
smci36.SetInputPinFunction(5, 5)
smci36.SetInputPinFunction(6, 7)
smci36.SetOutputPinFunction(1, 1)
smci36.SetOutputPinFunction(2, 2)
smci36.SetOutputPinFunction(3, 3)
smci36.SetIOMask(0x107003F)
smci36.SetReversePolarityMask(0x107003F)

print "drive address:                  " + repr(smci36.GetDriveAddress())
print "firmware version:               " + smci36.GetFirmwareVersion()
print "status:                         " + repr(smci36.GetStatus())
print "motor ID:                       " + repr(smci36.GetMotorID())
print "motor type:                     " + repr(smci36.GetMotorType())
print "phase current [%]:              " + repr(smci36.GetPhaseCurrent())
print "stand still phase current [%]:  " + repr(smci36.GetStandStillPhaseCurrent())
print "step mode:                      " + repr(smci36.GetStepMode())
print "ramp mode:                      " + repr(smci36.GetRampMode())
print "error correction mode:          " + repr(smci36.GetErrorCorrectionMode())
print "max. encoder deviation [steps]: " + repr(smci36.GetMaxEncoderDeviation())
print "positioning mode:               " + repr(smci36.GetPositioningMode())
print "encoder position [steps]:       " + repr(smci36.GetEncoderPosition())
print "controller position [steps]:    " + repr(smci36.GetPosition())
print "travel distance [steps]:        " + repr(smci36.GetTravelDistance())
print "direction:                      " + repr(smci36.GetDirection())
print "encoder direction:              " + repr(smci36.GetEncoderDirection())
print "min. frequency [Hz]:            " + repr(smci36.GetMinimumFrequency())
print "max. frequency [Hz]:            " + repr(smci36.GetMaximumFrequency())
print "max. frequency 2 [Hz]:          " + repr(smci36.GetMaximumFrequency2())
print "quick stop ramp [Hz/s]:         " + repr(smci36.GetQuickstopRampHzPerSecond())
print "acceleration ramp [Hz/s]:       " + repr(smci36.GetAccelerationRampHzPerSecond())
print "deceleration ramp [Hz/s]:       " + repr(smci36.GetDecelerationRampHzPerSecond())
print "function input pin 1:           " + repr(smci36.GetInputPinFunction(1))
print "function input pin 2:           " + repr(smci36.GetInputPinFunction(2))
print "function input pin 3:           " + repr(smci36.GetInputPinFunction(3))
print "function input pin 4:           " + repr(smci36.GetInputPinFunction(4))
print "function input pin 5:           " + repr(smci36.GetInputPinFunction(5))
print "function input pin 6:           " + repr(smci36.GetInputPinFunction(6))
print "function output pin 1:          " + repr(smci36.GetOutputPinFunction(1))
print "function output pin 2:          " + repr(smci36.GetOutputPinFunction(2))
print "function output pin 3:          " + repr(smci36.GetOutputPinFunction(3))
print "io mask:                        " + repr(smci36.GetIOMask())
print "reverse polarity mask:          " + repr(smci36.GetReversePolarityMask())
print "io:                             " + repr(smci36.GetIO())
