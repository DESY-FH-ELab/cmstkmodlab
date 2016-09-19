import PyTkModLab

# connect to device
smci36 = PyTkModLab.NanotecSMCI36("/dev/tty.SLAB_USBtoUART")

# get the status of the controller (see VNanotecSMCI36.h)
smci36.GetStatus()

# set the ramp mode to jerk-free (see VNanotecSMCI36.h)
smci36.SetRampMode(2)

# get the position of the encoder 
smci36.GetEncoderPosition()

# get the position of the controller
smci36.GetPosition()

# set the rotation direction (only valid in relative positioning mode)
smci36.SetDirection(False)

# set the maximum step frequency
smci36.SetMaximumFrequency(2000)

# set positioning mode to external reference run (see VNanotecSMCI36.h)
smci36.SetPositioningMode(4)

# start the motor
smci36.Start()
