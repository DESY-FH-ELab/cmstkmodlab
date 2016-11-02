import time
import PyTkModLabNanotec

# connect to device
smci36 = PyTkModLabNanotec.NanotecSMCI36("/dev/tty.SLAB_USBtoUART")

# set the ramp mode to trapezoidal (see VNanotecSMCI36.h)
smci36.SetRampMode(1)

# set the step mode to quarter steps (4 usteps per full step) (see VNanotecSMCI36.h)
smci36.SetStepMode(4)

# set the error correction mode to correction after travel (see VNanotecSMCI36.h)
smci36.SetErrorCorrectionMode(1)

# get the position of the encoder 
print "encoder position:    " + repr(smci36.GetEncoderPosition())

# get the position of the controller
print "controller position: " + repr(smci36.GetPosition()) + "\n"

# set the rotation direction (only valid in relative positioning mode)
smci36.SetDirection(True)

# set the maximum step frequency
smci36.SetMinimumFrequency(80)

# set the maximum step frequency
smci36.SetMaximumFrequency(800)

# set the maximum step frequency
smci36.SetMaximumFrequency2(1000)

# set positioning mode to external reference run (see VNanotecSMCI36.h)
smci36.SetPositioningMode(4)

# start the motor
smci36.Start()

time.sleep(1)

while (smci36.GetStatus()!=163):
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
