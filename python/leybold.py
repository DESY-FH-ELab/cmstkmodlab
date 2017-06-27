import time
import PyTkModLabLeybold

leybold = PyTkModLabLeybold.LeyboldGraphixThree("/dev/ttyUSB0")

print "version:   " + leybold.GetVersion()

