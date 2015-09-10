# defoDAQ Scripting

## defo

* <b>defo.wait(int delay);</b></br>
pause execution of script for delay seconds

* <b>defo.message(message);</b></br>
print a message to the message log. Message can be of type int, uint, double
or string.

* <b>defo.newMeasurement();</b></br>
prepare DAQ for a new measurement

* <b>uint defo.uTime();</b></br>
returns the current unix time

* <b>uint defo.mkUTime( int year, int month, int day, int hour, int minute, int second );</b></br>
constructs and returns a unix time from the provided parameters.

## keithley

* <b>int keithley.state( uint channel = [0,9] );</b></br>
returns the state of a sensor.

* <b>double keithley.temperature( uint channel = [0,9] );</b></br>
returns the temperature reading of a sensor.

* <b>string keithley.temperatureAsString( uint channel = [0,9] );</b></br>
returns the temperature reading of a sensor as a string.

* <b>keithley.waitForStableTemperature( string channels, int timeout );</b></br>
wait for stable temperatures on the selected channels. wait at most
timeout seconds. Channels are to be provided as 'space' separated list
of integers.

* <b>keithley.waitForTemperatureBelow( uint channel= [0,9], float temperature, int timeout )</b></br>
wait for the temperature reading of a channel to be above a certain
temperature. Wait at most timeout seconds.

* <b>keithley.waitForTemperatureAbove( uint channel= [0,9] , float temperature, int timeout )</b></br>
wait for the temperature reading of a channel to be above a certain
temperature. Wait at most timeout seconds.

## conrad

* <b>conrad.enablePanel( uint panel = [1,5] )</b></br>
switches on a panel.

* <b>conrad.disablePanel( uint panel = [1,5] )</b></br>
switches off a panel.

* <b>int conrad.panelState( uint panel = [1,5] )</b></br>
returns the status of a panel.

* <b>conrad.enableCalibrationLEDs()</b></br>
switches on the calibration LEDs

* <b>conrad.disableCalibrationLEDs()</b></br>
switches off the calibration LEDs

* <b>int conrad.calibrationLEDsState()</b></br>
returns the status of calibration LEDs

## camera

* <b>camera.comment( string text )</b></br>
sets the comment text for the picture acquisition. the comment text will not be
cleared after an picture has been taken

* <b>camera.takePicture()</b></br>
take a new picture

* <b>camera.setNumberOfPictures(int count = [1,5] )</b></br>
changes the number of pictures taken per measurement

* <b>int camera.numberOfPictures()</b></br>
returns the number of pictures taken per measurement

## julabo

* <b>julabo.enableCirculator()</b></br>
switches circulator on

* <b>julabo.disableCirculator()</b></br>
switches circulator off

* <b>int julabo.circulatorState()</b></br>
returns the state of the circulator

* <b>julabo.setWorkingTemperature( double temperature = [-50,30] )</b></br>
set the working temperature

* <b>julabo.workingTemperature()</b></br>
returns the working temperature

* <b>julabo.bath()</b></br>
returns the bath temperature

* <b>julabo.safety()</b></br>
returns the safety temperature

* <b>julabo.setP( double xp = [0.1,99.9] )</b></br>
set new proportional PID controller parameter

* <b>julabo.setI( int tn = [3,9999] )</b></br>
set new integral PID controller parameter

* <b>julabo.setD( int tv = [0,999] )</b></br>
set new differential PID controller parameter

* <b>julabo.setPID( double xp = [0.1,99.9], int tn = [3,9999], int tv = [0,999] )</b></br>
set new PID controller parameters

* <b>double julabo.getP()</b></br>
returns proportional PID controller parameter

* <b>int julabo.getI()</b></br>
returns integral PID controller parameter

* <b>int julabo.getD()</b></br>
returns differential PID controller parameter

* <b>julabo.setPumpPressure( uint pressure = [1,4] )</b></br>
changes the pump pressure

* <b>uint julabo.pumpPressure()</b></br>
returns the pump pressure
