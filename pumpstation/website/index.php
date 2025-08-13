<!DOCTYPE html>

<!--
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
-->

<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv="X-UA-Compatible" content="chrome=1,IE=edge" />
    <meta name="viewport" content="user-scalable=no, width=800" />
    <meta charset="utf-8" />
    <meta http-equiv="refresh" content="15">
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
        background-color:#FFFFFF;
        margin-top: 10px; /*set to a negative number 1/2 of your height*/
        margin-left: -400px; /*set to a negative number 1/2 of your width*/
      }
    </style>
  </head>
  <body>
   
    <div id="centered">
      <div style="margin:auto;width:800px;background-color:#FFFFFF;">

        <div style="width:800px;position:absolute;">
          <img src="data/pumpstation_schematic@2x.png" width="100%"/>
        </div>
        
<?php
$ini_array = parse_ini_file ( "pumpstation.ini" );

$command = $ini_array ['DocumentRoot'] . "/PumpStationControl --web getVacuumStatus";
exec ( $command, $valVS, $return );
if (is_array ( $valVS )) {
	$VS = $valVS [0];
} else {
	$VS = $valVS;
}
list ( $valSS1, $valP1, $valSS2, $valP2, $valSS3, $valP3 ) = explode ( ";", $VS );

$SSSYS = intval ( $valSS1 );
$SS1 = intval ( $valSS2 );
$SS2 = intval ( $valSS3 );

$PSYS = floatval ( $valP1 );
$P1 = floatval ( $valP2 );
$P2 = floatval ( $valP3 );

// $command = $ini_array['DocumentRoot']."/PumpStationControl --web getPressure 1";
// exec ($command, $valP1, $return);
// if (is_array($valP1)) {
// $P1 = floatval($valP1[0]);
// } else {
// $P1 = floatval($valP1);
// }
if ($P1 >= 200.0 || $P1 < 0.0) {
	echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:5px;background-color:#FF5555;">');
} else if ($P1 < 150.0) {
	echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:5px;background-color:#55FF55;">');
} else {
	echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:5px;background-color:#FFAA55;">');
}
echo ('<div style="font-size:16px;text-align:right;margin-top:5px;margin-right:10px;">');
if ($P1 >= 10.0) {
	printf ( "%.0f", $P1);
} else if ($P1 >= 0.1) {
	printf ( "%.2f", $P1);
} else {
	printf ( "%.1E", $P1);
}
echo (" mbar");
echo ('</div></div>');

// $command = $ini_array['DocumentRoot']."/PumpStationControl --web getPressure 2";
// exec ($command, $valP2, $return);
// if (is_array($valP2)) {
// $P2 = floatval($valP2[0]);
// } else {
// $P2 = floatval($valP2);
// }
if ($P2 >= 200.0 || $P2 < 0.0) {
	echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:45px;background-color:#FF5555;">');
} else if ($P2 < 150.0) {
	echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:45px;background-color:#55FF55;">');
} else {
	echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:45px;background-color:#FFAA55;">');
}
echo ('<div style="font-size:16px;text-align:right;margin-top:5px;margin-right:10px;">');
if ($P2 >= 10.0) {
	printf ( "%.0f", $P2);
} else if ($P2 >= 0.1) {
	printf ( "%.2f", $P2);
} else {
	printf ( "%.1E", $P2);
}
echo (" mbar");
echo ('</div></div>');

// $command = $ini_array['DocumentRoot']."/PumpStationControl --web getPressure 3";
// exec ($command, $valP3, $return);
// if (is_array($valP3)) {
// $P3 = floatval($valP3[0]);
// } else {
// $P3 = floatval($valP3);
// }
if ($PSYS >= 200.0 || $PSYS < 0.0) {
	echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:85px;background-color:#FF5555;">');
} else if ($P3 < 150.0) {
	echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:85px;background-color:#55FF55;">');
} else {
	echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:85px;background-color:#FFAA55;">');
}
echo ('<div style="font-size:16px;text-align:right;margin-top:5px;margin-right:10px;">');
if ($PSYS>= 10.0) {
	printf ( "%.0f", $PSYS);
} else if ($PSYS>= 0.1) {
	printf ( "%.2f", $PSYS);
} else {
	printf ( "%.1E", $PSYS);
}
echo (" mbar");
echo ('</div></div>');

$command = $ini_array ['DocumentRoot'] . "/PumpStationControl --web getSwitchStatus";
exec ( $command, $valSS, $return );
if (is_array ( $valSS )) {
	$SS = $valSS [0];
} else {
	$SS = $valSS;
}
list ( $valS0, $valS1, $valS2, $valS3, $valS4 ) = explode ( ";", $SS );

$Pump1State = intval ( $valS0 );
$Pump2State = intval ( $valS1 );
$Valve1State = intval ( $valS2 );
$Valve2State = intval ( $valS3 );
$Valve3State = intval ( $valS4 );

$command = $ini_array ['DocumentRoot'] . "/PumpStationControl --web getSwitchBlockedStatus";
exec ( $command, $valSB, $return );
if (is_array ( $valSB )) {
	$SB = $valSB [0];
} else {
	$SB = $valSB;
}
list ( $valB0, $valB1, $valB2, $valB3, $valB4 ) = explode ( ";", $SB );

$Pump1Blocked = intval ( $valB0 );
$Pump2Blocked = intval ( $valB1 );
$Valve1Blocked = intval ( $valB2 );
$Valve2Blocked = intval ( $valB3 );
$Valve3Blocked = intval ( $valB4 );

$command = $ini_array ['DocumentRoot'] . "/PumpStationControl --web getPumpOperatingHours";
exec ( $command, $valOH, $return );
if (is_array ( $valOH )) {
	$OH = $valOH [0];
} else {
	$OH = $valOH;
}
list ( $valOH1, $valOH2 ) = explode ( ";", $OH );

// $command = $ini_array['DocumentRoot']."/PumpStationControl --web getSwitchState " . $ini_array['Valve1Switch'];
// exec ($command, $valV1, $return);
// if (is_array($valV1)) {
// $Valve1State= boolval($valV1[0]);
// } else {
// $Valve1State= boolval($valV1);
// }
if ($Valve1State== 0) {
	echo ('<div style="position:absolute;width:68px;height:30px;margin-left:221px;margin-top:29px;background-color:#FF5555;">');
} else if ($Valve1State== 1) {
	echo ('<div style="position:absolute;width:68px;height:30px;margin-left:221px;margin-top:29px;background-color:#55FF55;">');
}
echo ('<div style="font-size:16px;text-align:center;margin-top:5px;">');
if ($Valve1State== 0) {
	echo ("closed");
} else if ($Valve1State== 1) {
	echo ("open");
} else {
	echo ("XXX");
}
echo ("</div></div>");

// $command = $ini_array['DocumentRoot']."/PumpStationControl --web getSwitchState " . $ini_array['Valve2Switch'];
// exec ($command, $valV2, $return);
// if (is_array($valV2)) {
// $Valve2State= boolval($valV2[0]);
// } else {
// $Valve2State= boolval($valV2);
// }
if ($Valve2State== 0) {
	echo ('<div style="position:absolute;width:68px;height:30px;margin-left:131px;margin-top:29px;background-color:#FF5555;">');
} else if ($Valve2State== 1) {
	echo ('<div style="position:absolute;width:68px;height:30px;margin-left:131px;margin-top:29px;background-color:#55FF55;">');
}
echo ('<div style="font-size:16px;text-align:center;margin-top:5px;">');
if ($Valve2State== 0) {
	echo ("closed");
} else if ($Valve2State== 1) {
	echo ("open");
} else {
	echo ("XXX");
}
echo ("</div></div>");

// $command = $ini_array['DocumentRoot']."/PumpStationControl --web getSwitchState " . $ini_array['Valve3Switch'];
// exec ($command, $valV3, $return);
// if (is_array($valV3)) {
// $Valve3State= boolval($valV3[0]);
// } else {
// $Valve3State= boolval($valV3);
// }
if ($Valve3State== 0) {
	echo ('<div style="position:absolute;width:68px;height:30px;margin-left:41px;margin-top:29px;background-color:#FF5555;">');
} else if ($Valve3State== 1) {
	echo ('<div style="position:absolute;width:68px;height:30px;margin-left:41px;margin-top:29px;background-color:#55FF55;">');
}
echo ('<div style="font-size:16px;text-align:center;margin-top:5px;">');
if ($Valve3State== 0) {
	echo ("closed");
} else if ($Valve3State== 1) {
	echo ("open");
} else {
	echo ("XXX");
}
echo ("</div></div>");

// $command = $ini_array['DocumentRoot']."/PumpStationControl --web getSwitchState " . $ini_array['Pump2Switch'];
// exec ($command, $valP2, $return);
// if (is_array($valP2)) {
// $Pump2State= boolval($valP2[0]);
// } else {
// $Pump2State= boolval($valP2);
// }
if ($Pump2State== 0) {
	echo ('<div style="position:absolute;width:108px;height:30px;margin-left:551px;margin-top:5px;background-color:#FF5555;">');
} else if ($Pump2State== 1) {
	echo ('<div style="position:absolute;width:108px;height:30px;margin-left:551px;margin-top:5px;background-color:#55FF55;">');
}
echo ('<div style="font-size:16px;text-align:right;margin-top:5px;margin-right:5px;">');
printf ( "%.1f h", $valOH2 );
echo ("</div></div>");

// $command = $ini_array['DocumentRoot']."/PumpStationControl --web getSwitchState " . $ini_array['Pump1Switch'];
// exec ($command, $valP1, $return);
// if (is_array($valP1)) {
// $Pump1State= boolval($valP1[0]);
// } else {
// $Pump1State= boolval($valP1);
// }
if ($Pump1State== 0) {
	echo ('<div style="position:absolute;width:108px;height:30px;margin-left:551px;margin-top:45px;background-color:#FF5555;">');
} else if ($Pump1State== 1) {
	echo ('<div style="position:absolute;width:108px;height:30px;margin-left:551px;margin-top:45px;background-color:#55FF55">');
}
echo ('<div style="font-size:16px;text-align:right;margin-top:5px;margin-right:5px;">');
printf ( "%.1f h", $valOH1 );
echo ("</div></div>");
?>

        <div style="position:absolute;width:800px;margin-top: 370px;">
        
        <div style="font-size:18px;position:absolute;width:100px;margin-left:0px;text-align:center;">Valve 1</div>
        <div style="font-size:18px;position:absolute;width:100px;margin-left:0px;margin-top:24px;text-align:center;">
        <?php
        if ($Valve1Blocked== 0 ) {
        	if ($Valve1State== 0 ) {
        		echo ("<img id='switch_V1' src='data/button_red.png' onclick='changeSwitch(" . $ini_array['Valve1Switch']. ");'/>");
			} else {
		  		echo ("<img id='switch_V1' src='data/button_green.png' onclick='changeSwitch(" . $ini_array['Valve1Switch']. ");'/>");
			}
        } else {
        	echo ("<img id='switch_V1' src='data/button_grey.png'/>");
        }
        ?>
        </div>
        
        <div style="font-size:18px;position:absolute;width:100px;margin-left:100px;text-align:center;">Valve 2</div>
        <div style="font-size:18px;position:absolute;width:100px;margin-left:100px;margin-top:24px;text-align:center;">
        <?php
        if ($Valve2Blocked== 0 ) {
        	if ($Valve2State== 0 ) {
		  		echo ("<img id='switch_V2' src='data/button_red.png' onclick='changeSwitch(" . $ini_array['Valve2Switch']. ");'/>");
			} else {
		  		echo ("<img id='switch_V2' src='data/button_green.png' onclick='changeSwitch(" . $ini_array['Valve2Switch']. ");'/>");
			}
        } else {
        	echo ("<img id='switch_V2' src='data/button_grey.png'/>");
        }
        ?>
        </div>
        
        <div style="font-size:18px;position:absolute;width:100px;margin-left:200px;text-align:center;">Valve 3</div>
        <div style="font-size:18px;position:absolute;width:100px;margin-left:200px;margin-top:24px;text-align:center;">
        <?php
        if ($Valve3Blocked== 0 ) {
        	if ($Valve3State== 0 ) {
				echo ("<img id='switch_V3' src='data/button_red.png' onclick='changeSwitch(" . $ini_array['Valve3Switch']. ");'/>");
			} else {
			  echo ("<img id='switch_V3' src='data/button_green.png' onclick='changeSwitch(" . $ini_array['Valve3Switch']. ");'/>");
			}
        } else {
        	echo ("<img id='switch_V3' src='data/button_grey.png'/>");
        }
        ?>
        </div>

        <div style="font-size:18px;position:absolute;width:100px;margin-left:565px;text-align:center;">Pump 1</div>
        <div style="font-size:18px;position:absolute;width:100px;margin-left:565px;margin-top:24px;text-align:center;">
        <?php
        if ($Pump1Blocked== 0 ) {
        	if ($Pump1State== 0 ) {
		  		echo ("<img id='switch_P1' src='data/button_red.png' onclick='changeSwitch(" . $ini_array['Pump1Switch']. ");'/>");
			} else {
				echo ("<img id='switch_P1' src='data/button_green.png' onclick='changeSwitch(" . $ini_array['Pump1Switch']. ");'/>");
			}
        } else {
        	echo ("<img id='switch_P1' src='data/button_grey.png'/>");
        }
        ?>
        </div>

        <div style="font-size:18px;position:absolute;width:100px;margin-left:665px;text-align:center;">Pump 2</div>
        <div style="font-size:18px;position:absolute;width:100px;margin-left:665px;margin-top:24px;text-align:center;">
        <?php
        if ($Pump2Blocked== 0 ) {
        	if ($Pump2State== 0 ) {
				echo ("<img id='switch_P2' src='data/button_red.png' onclick='changeSwitch(" . $ini_array['Pump2Switch']. ");'/>");
			} else {
		  		echo ("<img id='switch_P2' src='data/button_green.png' onclick='changeSwitch(" . $ini_array['Pump2Switch']. ");'/>");
			}
		} else {
			echo ("<img id='switch_P2' src='data/button_grey.png'/>");
		}
        ?>
        </div>

	    </div>
	   
        <div style="position:absolute;width:800px;margin-top: 460px;vertical-align:top;">
	    <hr>        
        <img style="width:32px;" src="data/DESY-Logo.png"/>
        <img style="width:32px;" src="data/DAF_Logo_color2.png"/>
        <img style="width:32px;" src="data/CMS-Logo.png"/>
        <span style="vertical-align:top;">
<?php
date_default_timezone_set ( $ini_array ['TimeZone'] );
$date = date ( 'Y-m-d H:i:s' );
echo (gethostname () . ' - ' . $date);
?>
        </span>
	    </div>

      </div>
    </div>

	<!-- javascript -->
	<script src="script.js"></script>
  </body>
</html>