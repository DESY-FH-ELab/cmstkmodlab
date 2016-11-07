import PyTkModLabNanotec

# connect to device
smci36 = PyTkModLabNanotec.NanotecSMCI36("/dev/tty.SLAB_USBtoUART")

# set output pin 2 function to user defined
smci36.SetOutputPinFunction(2, 0)

# set output pin 3 function to user defined
smci36.SetOutputPinFunction(3, 0)

# get the state of the IO pins
smci36.GetIO()

# set output pins 2 and 3 high
smci36.SetIO(0x00020000 + 0x00040000)

# get the state of the IO pins
smci36.GetIO()
