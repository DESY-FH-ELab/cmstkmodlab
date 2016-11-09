<!DOCTYPE html>

<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv="X-UA-Compatible" content="chrome=1,IE=edge" />
    <meta name="viewport" content="user-scalable=no, width=800" />
    <meta charset="utf-8" />
    <title>.: CMS Pump Station Status & Control :.</title>
    <style type="text/css">
      body {
        background-color: #FFFFFF;
        margin: 0px;
        font-family: Verdana;
        font-size: 12px;
      }
      #centered {
        position:absolute;
        top: 0%;
        left: 50%;
        width:30em;
        height:18em;
        background-color:#aaFFFF;
        margin-top: 10px; /*set to a negative number 1/2 of your height*/
        margin-left: -400px; /*set to a negative number 1/2 of your width*/
      }
    </style>
  </head>
<html>
    <head>
        <meta charset="utf-8" />
        <title>.: CMS Pump Station Status & Control :.</title>
    </head>
 
    <div id="centered">
      <div style="margin:auto;width:800px;background-color:#FFFFFF;">
        <div style="width:800px;position:absolute;">
          <img src="data/pumpstation_schematic.png" width="100%"/>
        </div>
        <div style="font-size:16px;width:800px;position:absolute;margin-left:348px;margin-top:9px;">
           <?php
             $command = "/Users/mussgill/Sites/PumpStationControl --web getPressure 0";
             exec ($command, $valP0, $return);
             echo ($valP0[0]);
           ?>
        </div>
        <div style="font-size:16px;width:800px;position:absolute;margin-left:348px;margin-top:49px;">
           <?php
             $command = "/Users/mussgill/Sites/PumpStationControl --web getPressure 1";
             exec ($command, $valP1, $return);
             echo ($valP1[0]);
           ?>
        </div>
        <div style="font-size:16px;width:800px;position:absolute;margin-left:348px;margin-top:89px;">
           <?php
             $command = "/Users/mussgill/Sites/PumpStationControl --web getPressure 2";
             exec ($command, $valP2, $return);
             echo ($valP2[0]);
           ?>
        </div>
        <div style="font-size:16px;width:800px;position:absolute;margin-left:230px;margin-top:33px;">
           <?php
             $command = "/Users/mussgill/Sites/PumpStationControl --web getSwitchState 0";
             exec ($command, $valS0, $return);
             if ($valS0[0] == 0 ) {
               echo ("closed");
             } else if ($valS0[0] == 1 ) {
               echo ("open");
             } else {
               echo ("XXX");
             }
           ?>
        </div>
        <div style="font-size:16px;width:800px;position:absolute;margin-left:140px;margin-top:33px;">
           <?php
             $command = "/Users/mussgill/Sites/PumpStationControl --web getSwitchState 1";
             exec ($command, $valS1, $return);
             if ($valS1[0] == 0 ) {
               echo ("closed");
             } else if ($valS1[0] == 1 ) {
               echo ("open");
             } else {
               echo ("XXX");
             }
           ?>
        </div>
        <div style="font-size:16px;width:800px;position:absolute;margin-left:50px;margin-top:33px;">
           <?php
             $command = "/Users/mussgill/Sites/PumpStationControl --web getSwitchState 2";
             exec ($command, $valS2, $return);
             if ($valS2[0] == 0 ) {
               echo ("closed");
             } else if ($valS2[0] == 1 ) {
               echo ("open");
             } else {
               echo ("XXX");
             }
           ?>
        </div>
        <div style="font-size:16px;width:800px;position:absolute;margin-left:615px;margin-top:9px;">
           <?php
             $command = "/Users/mussgill/Sites/PumpStationControl --web getSwitchState 3";
             exec ($command, $valS3, $return);
             if ($valS3[0] == 0 ) {
               echo ("OFF");
             } else if ($valS3[0] == 1 ) {
               echo ("ON");
             } else {
               echo ("XXX");
             }
           ?>
        </div>
        <div style="font-size:16px;width:800px;position:absolute;margin-left:615px;margin-top:49px;">
           <?php
             $command = "/Users/mussgill/Sites/PumpStationControl --web getSwitchState 4";
             exec ($command, $valS4, $return);
             if ($valS4[0] == 0 ) {
               echo ("OFF");
             } else if ($valS4[0] == 1 ) {
               echo ("ON");
             } else {
               echo ("XXX");
             }
           ?>
        </div>
        <div style="position:absolute;width:800px;margin-top: 370px;">

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
	        </div>
      </div>
    </div>

	<!-- javascript -->
	<script src="script.js"></script>
</body>
</html>