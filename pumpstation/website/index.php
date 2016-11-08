<!DOCTYPE html>

<html>
    <head>
        <meta charset="utf-8" />
        <title>.: CMS Pump Station Status & Control :.</title>
    </head>
 
    <body style="background-color: white;">
    <!-- On/Off button's picture -->
	<?php
	$val_array = array(0,0,0,0,0,0,0,0);
	//this php script generate the first page in function of the file
	for ( $i= 0; $i<8; $i++) {
		$command = "/Users/mussgill/Sites/PumpStationControl --web getSwitchState ".$i;
		exec ($command, $val_array[$i], $return);
	}
	//for loop to read the value
	$i =0;
	for ($i = 0; $i < 8; $i++) {
		//if off
		if ($val_array[$i][0] == 0 ) {
			echo ("<img id='button_".$i."' src='data/img/ValveClosed.png' onclick='changeSwitch(".$i.");'/>");
		}
		//if on
		if ($val_array[$i][0] == 1 ) {
			echo ("<img id='button_".$i."' src='data/img/ValveOpen.png' onclick='changeSwitch(".$i.");'/>");
		}
	}
	?>
	 
	<!-- javascript -->
	<script src="script.js"></script>
    </body>
</html>