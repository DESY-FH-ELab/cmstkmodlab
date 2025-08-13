<?php

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

$ini_array = parse_ini_file ( "pumpstation.ini" );

$command = $ini_array ['DocumentRoot'] . "/PumpStationControl --web getSwitchBlockedStatus";
exec ( $command, $switchblocked, $return );
list ($SwitchBlocked0, $SwitchBlocked1, $SwitchBlocked2, $SwitchBlocked3, $SwitchBlocked4) = split(';', $switchblocked[0]);

$command = $ini_array ['DocumentRoot'] . "/PumpStationControl --web getSwitchStatus";
exec ( $command, $switchstate, $return );
list ($SwitchState0, $SwitchState1, $SwitchState2, $SwitchState3, $SwitchState4) = split(';', $switchstate[0]);

$command = $ini_array ['DocumentRoot'] . "/PumpStationControl --web getVacuumStatus";
exec ( $command, $vacuumstate, $return );
list ($SensorState0, $Pressure0, $SensorState1, $Pressure1, $SensorState2, $Pressure2) = split(';', $vacuumstate[0]);

$command = $ini_array ['DocumentRoot'] . "/PumpStationControl --web getPumpOperatingHours";
exec ( $command, $pumphours, $return );
list ($Pump0Hours, $Pump1Hours) = split(';', $pumphours[0]);

$Timestamp = date('c');

$json = array(
  'SwitchBlocked0' => $SwitchBlocked0,
  'SwitchBlocked1' => $SwitchBlocked1,
  'SwitchBlocked2' => $SwitchBlocked2,
  'SwitchBlocked3' => $SwitchBlocked3,
  'SwitchBlocked4' => $SwitchBlocked4,
  'SwitchState0' => $SwitchState0,
  'SwitchState1' => $SwitchState1,
  'SwitchState2' => $SwitchState2,
  'SwitchState3' => $SwitchState3,
  'SwitchState4' => $SwitchState4,
  'SensorState0' => $SensorState0,
  'Pressure0' => $Pressure0,
  'SensorState1' => $SensorState1,
  'Pressure1' => $Pressure1,
  'SensorState2' => $SensorState2,
  'Pressure2' => $Pressure2,
  'Pump0Hours' => $Pump0Hours,
  'Pump1Hours' => $Pump1Hours,
  'Timestamp' => $Timestamp
);

echo (json_encode($json));
?>
