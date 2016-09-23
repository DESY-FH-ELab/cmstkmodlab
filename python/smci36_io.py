import PyTkModLab

# connect to device
smci36 = PyTkModLab.NanotecSMCI36("/dev/tty.SLAB_USBtoUART")

# set output pin 1 function to user defined
smci36.SetOutputPinFunction(1, 0)

# set output pin 2 function to user defined
smci36.SetOutputPinFunction(2, 0)

# set masking of outputs pins 1 and 2 to user defined
smci36.SetIOMask(0x0004003F)

# get the state of the IO pins
smci36.GetIO()

# set output pins 1 and 2 high
smci36.SetIO(0x00010000 + 0x00020000)

# get the state of the IO pins
smci36.GetIO()
