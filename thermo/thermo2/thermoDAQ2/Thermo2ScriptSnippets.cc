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
}
