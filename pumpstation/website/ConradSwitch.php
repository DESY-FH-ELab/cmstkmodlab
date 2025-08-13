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

if (isset ( $_GET ["channel"] )) {
	$channel = strip_tags ( $_GET ["channel"] );
	
	if ((is_numeric ( $channel )) && ($channel <= 4) && ($channel >= 0)) {
		
		$ini_array = parse_ini_file ( "pumpstation.ini" );
		
		$command = $ini_array ['DocumentRoot'] . "/PumpStationControl --web getSwitchBlocked " . $channel;
		exec ( $command, $blocked, $return );
		
		$command = $ini_array ['DocumentRoot'] . "/PumpStationControl --web getSwitchState " . $channel;
		exec ( $command, $status, $return );
		
		if ($blocked [0] == "1") {
			echo ($status [0]);
		} else {
			if ($status [0] == "0") {
				$status [0] = "1";
			} else if ($status [0] == "1") {
				$status [0] = "0";
			}
		
			$command = $ini_array ['DocumentRoot'] . "/PumpStationControl --web setSwitchState " . $channel . " " . $status [0];
			exec ( $command, $status, $return );
		
			$command = $ini_array ['DocumentRoot'] . "/PumpStationControl --web getSwitchState " . $channel;
			exec ( $command, $status, $return );
		
			echo ($status [0]);
		}
	} else {
		echo ("fail");
	}
} else {
	echo ("fail");
}
?>
