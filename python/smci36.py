import PyTkModLab

smci36 = PyTkModLab.NanotecSMCI36("/dev/tty.SLAB_USBtoUART")

smci36.GetStatus()

smci36.SetRampMode(2)

smci36.SetTravelDistance(5000)

smci36.GetEncoderPosition()
smci36.GetPosition()

smci36.SetDirection(false)
smci36.SetMaximumFrequency(4000)

smci36.Start()
