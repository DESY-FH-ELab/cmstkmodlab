import time
import PyTkModLabLeybold

leybold = PyTkModLabLeybold.LeyboldGraphixThree("/dev/ttyUSB0")

print "version:   " + leybold.GetVersion()

print "date:      " + leybold.GetDate()
print "time:      " + leybold.GetTime()

