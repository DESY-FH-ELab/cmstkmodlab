import PyTkModLabConrad

conrad = PyTkModLabConrad.Conrad("/dev/ttyUSB0")

status = conrad.queryStatus()
for s in status: print s
