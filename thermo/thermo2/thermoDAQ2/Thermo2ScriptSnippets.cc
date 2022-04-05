/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include "Thermo2ScriptSnippets.h"

Thermo2ScriptSnippets::Thermo2ScriptSnippets(QWidget *parent)
  : QComboBox(parent)
{
  addItem("", "");
  addItem("comment block", "/*\n\n*/\n");

  addItem("for loop", "for (var i = 0; i < 5; i++) {\n  // do something with i\n}\n");
  addItem("for loop array", "var array = [0, 1, 2];\nfor (var i=0;i<array.length;i++) {\n  // do something with array[i]\n}\n");
  
  insertSeparator(999);

  addItem("thermo - start measurement", "thermo.startMeasurement();\n");
  addItem("thermo - stop measurement", "thermo.stopMeasurement();\n");
  addItem("thermo - wait", "thermo.wait(seconds);\n");
  addItem("thermo - message", "thermo.message();\n");
  addItem("thermo - log", "thermo.log(\"\");\n");
  addItem("thermo - mattermost", "thermo.mattermost(\"\");\n");
  addItem("thermo - unix time", "utime = thermo.uTime();\n");
  addItem("thermo - execution time", "etime = thermo.uTime();\n");
  addItem("thermo - make unix time", "utime = thermo.mkUTime(year, month, day, hour, minute, second);\n");

  insertSeparator(999);

  addItem("leybold - get pressure", "pressure = leybold.getPressure();\n");
  addItem("leybold - wait for pressure below", "leybold.waitForPressureBelow(pressure, timeout [s]);\n");

  insertSeparator(999);

  addItem("huber - set temperature set point", "huber.setTemperatureSetPoint(temperature);\n");
  addItem("huber - get temperature set point", "temperature = huber.getTemperatureSetPoint();\n");
  addItem("huber - get auto PID", "autoPID = huber.getAutoPID();\n");
  addItem("huber - set auto PID", "huber.setAutoPID(autoPID);\n");
  addItem("huber - get control mode", "controlMode = huber.getControlMode();\n");
  addItem("huber - set control mode", "huber.setControlMode(controlMode);\n");
  addItem("huber - get Kp internal", "KpInternal = huber.getKpInternal();\n");
  addItem("huber - set Kp internal", "huber.setKpInternal(KpInternal);\n");
  addItem("huber - get Tn internal", "TnInternal = huber.getTnInternal();\n");
  addItem("huber - set Tn internal", "huber.setTnInternal(TnInternal);\n");
  addItem("huber - get Tv internal", "TvInternal = huber.getTvInternal();\n");
  addItem("huber - set Tv internal", "huber.setTvInternal(TvInternal);\n");
  addItem("huber - get Kp process", "KpProcess = huber.getKpProcess();\n");
  addItem("huber - set Kp process", "huber.setKpProcess(KpProcess);\n");
  addItem("huber - get Tn process", "TnProcess = huber.getTnProcess();\n");
  addItem("huber - set Tn process", "huber.setTnProcess(TnProcess);\n");
  addItem("huber - get Tv process", "TvProcess = huber.getTvProcess();\n");
  addItem("huber - set Tv process", "huber.setTvProcess(TvProcess);\n");
  addItem("huber - wait for internal temperature above", "huber.waitForInternalTemperatureAbove(temperature, timeout [s]);\n");
  addItem("huber - wait for internal temperature below", "huber.waitForInternalTemperatureBelow(temperature, timeout [s]);\n");
  addItem("huber - wait for stable internal temperature", "huber.waitForStableInternalTemperature(temperature, delay [s], timeout [s]);\n");
  addItem("huber - wait for process temperature above", "huber.waitForProcessTemperatureAbove(temperature, timeout [s]);\n");
  addItem("huber - wait for process temperature below", "huber.waitForProcessTemperatureBelow(temperature, timeout [s]);\n");
  addItem("huber - wait for stable process temperature", "huber.waitForStableProcessTemperature(temperature, delay [s], timeout [s]);\n");

  insertSeparator(999);

  addItem("marta - set temperature set point", "marta.setTemperatureSetPoint(temperature);\n");
  addItem("marta - get temperature set point", "temperature = marta.getTemperatureSetPoint();\n");
  addItem("marta - set speed set point", "marta.setSpeedSetPoint(speed);\n");
  addItem("marta - get speed set point", "speed = marta.getSpeedSetPoint();\n");
  addItem("marta - set flow set point", "marta.setFlowSetPoint(flow);\n");
  addItem("marta - get flow set point", "flow = marta.getFlowSetPoint();\n");

  insertSeparator(999);

  addItem("nge103b - get output state", "state = nge103b.getOutputState(channel);\n");
  addItem("nge103b - set output state", "nge103b.setOutputState(channel, state);\n");
  addItem("nge103b - get voltage", "nge103b.getVoltage(channel);\n");
  addItem("nge103b - set voltage", "nge103b.setVoltage(channel, voltage);\n");
  addItem("nge103b - get measured voltage", "nge103b.getMeasuredVoltage(channel);\n");
  addItem("nge103b - get current", "nge103b.getCurrent(channel);\n");
  addItem("nge103b - set current", "nge103b.setCurrent(channel, current);\n");
  addItem("nge103b - get measured current", "nge103b.getMeasuredCurrent(channel);\n");

  insertSeparator(999);

  addItem("keithley - get sensor state", "state = keithley.state(sensor);\n");
  addItem("keithley - get temperature", "temperature = keithley.temperature(sensor);\n");
  addItem("keithley - get temperature as string", "temperature = keithley.temperatureAsString(sensor);\n");
  addItem("keithley - wait for temperature above", "keithley.waitForTemperatureAbove(sensor, temperature, timeout [s]);\n");
  addItem("keithley - wait for temperature below", "keithley.waitForTemperatureBelow(sensor, temperature, timeout [s]);\n");
  addItem("keithley - wait for stable temperatures", "keithley.waitForStableTemperature(sensors, timeout [s]);\n");

  insertSeparator(999);

  addItem("t2tp - get sink temperature", "temperature = t2tp.getSinkTemperature();\n");
  addItem("t2tp - set sink temperature", "t2tp.setSinkTemperature(temperature);\n");
  addItem("t2tp - get source power", "power = t2tp.getSourcePower();\n");
  addItem("t2tp - set source power", "t2tp.setSourcePower(power);\n");
  addItem("t2tp - get number of sensor in top block", "sensor = t2tp.getTopSensor(position);\n");
  addItem("t2tp - get number of sensor in bottom block", "sensor = t2tp.getBottomSensor(position);\n");
  addItem("t2tp - get temperature in top block", "temperature = t2tp.getTopTemperature(position);\n");
  addItem("t2tp - get temperature in bottom block", "temperature = t2tp.getBottomTemperature(position);\n");
}
