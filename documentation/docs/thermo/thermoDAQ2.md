# thermoDAQ2 Scripting

## thermo2

* <b>thermo2.startMeasurement();</b></br>
start the measurement

* <b>thermo2.stopMeasurement();</b></br>
stop the measurement

* <b>thermo2.wait(int delay);</b></br>
pause execution of script for delay seconds

* <b>thermo2.message(message);</b></br>
print a message to the message log. Message can be of type int, uint, double or string.

* <b>thermo2.log(message);</b></br>
write a message to the data stream.

* <b>uint thermo2.uTime();</b></br>
returns the current unix time.

* <b>uint thermo2.eTime();</b></br>
returns the execution time of the running script.

* <b>uint thermo2.mkUTime( int year, int month, int day, int hour, int minute, int second );</b></br>
constructs and returns a unix time from the provided parameters.

* <b>thermo2.slack(message);</b></br>
post a message to slack.
