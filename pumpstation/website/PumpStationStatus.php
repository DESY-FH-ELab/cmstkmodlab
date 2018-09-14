<?php

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2018 - The DESY CMS Group                  //
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
exec ( $command, $result, $return );
$SB = split(';', $result);

$command = $ini_array ['DocumentRoot'] . "/PumpStationControl --web getSwitchStatus";
exec ( $command, $result, $return );
$SS = split(';', $result);

json = array(
	'SB0' => $SB[0], 'SB1' => $SB[1], 'SB2' => $SB[2], 'SB3' => $SB[3], 'SB4' => $SB[4], 
	'SS0' => $SS[0], 'SS1' => $SS[1], 'SS2' => $SS[2], 'SS3' => $SS[3], 'SS4' => $SS[4], 
);

echo (json_encode($json));
?>
