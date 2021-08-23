/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
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
  addItem("thermo2 - start measurement", "thermo2.startMeasurement()\n");
  addItem("thermo2 - stop measurement", "thermo2.stopMeasurement()\n");
  addItem("thermo2 - wait", "thermo2.wait(seconds)\n");
  addItem("thermo2 - message", "thermo2.message()\n");
  addItem("thermo2 - log", "thermo2.log(\"\")\n");
  addItem("thermo2 - mattermost", "thermo2.mattermost(\"\")\n");
  addItem("thermo2 - unix time", "utime = thermo2.uTime()\n");
  addItem("thermo2 - execution time", "etime = thermo2.uTime()\n");
  addItem("thermo2 - make unix time", "utime = thermo2.mkUTime(year, month, day, hour, minute, second)\n");

  // addItem("huber - set temperature set point", "huber.setTemperatureSetPoint(temperature)\n");
  // addItem("huber - get temperature set point", "temperature = huber.getTemperatureSetPoint()\n");

  addItem("marta - set temperature set point", "marta.setTemperatureSetPoint(temperature)\n");
  addItem("marta - get temperature set point", "temperature = marta.getTemperatureSetPoint()\n");
  addItem("marta - set speed set point", "marta.setSpeedSetPoint(speed)\n");
  addItem("marta - get speed set point", "speed = marta.getSpeedSetPoint()\n");
  addItem("marta - set flow set point", "marta.setFlowSetPoint(flow)\n");
  addItem("marta - get flow set point", "flow = marta.getFlowSetPoint()\n");

  addItem("nge103b - get output state", "state = nge103b.getOutputState(channel)\n");
  addItem("nge103b - set output state", "nge103b.setOutputState(channel, state)\n");
  addItem("nge103b - get voltage", "nge103b.getVoltage(channel)\n");
  addItem("nge103b - set voltage", "nge103b.setVoltage(channel, voltage)\n");
  addItem("nge103b - get measured voltage", "nge103b.getMeasuredVoltage(channel)\n");
  addItem("nge103b - get current", "nge103b.getCurrent(channel)\n");
  addItem("nge103b - set current", "nge103b.setCurrent(channel, current)\n");
  addItem("nge103b - get measured current", "nge103b.getMeasuredCurrent(channel)\n");

  addItem("daq6510 - get sensor state", "state = daq6510.state(sensor)\n");
  addItem("daq6510 - get temperature", "temperature = daq6510.temperature(sensor)\n");
  addItem("daq6510 - get temperature as string", "temperature = daq6510.temperatureAsString(sensor)\n");
  addItem("daq6510 - wait for stable temperatures", "daq6510.waitForStableTemperature(sensors, timeout)\n");
  addItem("daq6510 - wait for temperature above", "daq6510.waitForTemperatureAbove(sensor, temperature, timeout)\n");
  addItem("daq6510 - wait for temperature below", "daq6510.waitForTemperatureBelow(sensor, temperature, timeout)\n");
}
