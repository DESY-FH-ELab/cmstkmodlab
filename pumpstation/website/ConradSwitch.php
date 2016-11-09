<?php
if (isset($_GET["channel"])) {
	$channel = strip_tags ($_GET["channel"]);
	
	if ((is_numeric($channel)) && ($channel <= 7) && ($channel >= 0) ) {
		
		$command = "/var/www/html/PumpStationControl --web getSwitchState ".$channel;
		exec ($command, $status, $return);
		
		if ($status[0] == "0" ) {
			$status[0] = "1";
		} else if ($status[0] == "1" ) {
			$status[0] = "0";
		}
		
		$command = "/var/www/html/PumpStationControl --web setSwitchState ".$channel." ".$status[0];
		exec ($command, $status, $return);
		
		echo($status[0]);
		
	} else { 
		echo("fail");
	}
} else {
	echo("fail");
}
?>
