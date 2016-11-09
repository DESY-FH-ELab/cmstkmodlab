<!DOCTYPE html>

<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv="X-UA-Compatible" content="chrome=1,IE=edge" />
    <meta name="viewport" content="user-scalable=no, width=800" />
    <meta charset="utf-8" />
    <meta http-equiv="refresh" content="5">
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
        
        <?php
        
          $ini_array = parse_ini_file("pumpstation.ini");
          
          $command = $ini_array['DocumentRoot']."/PumpStationControl --web getPressure 0";
          exec ($command, $valP0, $return);
          if (is_array($valP0)) {
            $P0 = floatval($valP0[0]);
          } else {
            $P0 = floatval($valP0);
          }
          if ($P0 >= 200.0) {
            echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:5px;background-color:#FF5555;">');
          } else if ($P0 < 150.0 ) {
            echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:5px;background-color:#55FF55;">');
          } else {
            echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:5px;background-color:#FFAA55;">');
          }
          echo ('<div style="font-size:16px;text-align:right;margin-top:5px;margin-right:10px;">');
          echo (number_format($P0, 1, ".", ","));
          echo (" mbar");
          echo ('</div></div>');

          $command = $ini_array['DocumentRoot']."/PumpStationControl --web getPressure 1";
          exec ($command, $valP1, $return);
          if (is_array($valP1)) {
            $P1 = floatval($valP1[0]);
          } else {
            $P1 = floatval($valP1);
          }
          if ($P1 >= 200.0) {
            echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:45px;background-color:#FF5555;">');
          } else if ($P1 < 150.0 ) {
            echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:45px;background-color:#55FF55;">');
          } else {
            echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:45px;background-color:#FFAA55;">');
          }
          echo ('<div style="font-size:16px;text-align:right;margin-top:5px;margin-right:10px;">');
          echo (number_format($P1, 1, ".", ","));
          echo (" mbar");
          echo ('</div></div>');

          $command = $ini_array['DocumentRoot']."/PumpStationControl --web getPressure 2";
          exec ($command, $valP2, $return);
          if (is_array($valP2)) {
            $P2 = floatval($valP2[0]);
          } else {
            $P2 = floatval($valP2);
          }
          if ($P2 >= 200.0) {
            echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:85px;background-color:#FF5555;">');
          } else if ($P2 < 150.0 ) {
            echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:85px;background-color:#55FF55;">');
          } else {
            echo ('<div style="position:absolute;width:128px;height:30px;margin-left:341px;margin-top:85px;background-color:#FFAA55;">');
          }
          echo ('<div style="font-size:16px;text-align:right;margin-top:5px;margin-right:10px;">');
          echo (number_format($P2, 1, ".", ","));
          echo (" mbar");
          echo ('</div></div>');

          $command = $ini_array['DocumentRoot']."/PumpStationControl --web getSwitchState 0";
          exec ($command, $valS0, $return);
          if (is_array($valS0)) {
            $S0 = boolval($valS0[0]);
          } else {
            $S0 = boolval($valS0);
          }
          if ($S0 == 0 ) {
            echo ('<div style="position:absolute;width:68px;height:30px;margin-left:221px;margin-top:29px;background-color:#FF5555;">');
          } else if ($S0 == 1 ) {
            echo ('<div style="position:absolute;width:68px;height:30px;margin-left:221px;margin-top:29px;background-color:#55FF55;">');
          }
          echo ('<div style="font-size:16px;text-align:center;margin-top:5px;">');
          if ($S0 == 0 ) {
            echo ("closed");
          } else if ($S0 == 1 ) {
            echo ("open");
          } else {
            echo ("XXX");
          }
          echo ("</div></div>");

          $command = $ini_array['DocumentRoot']."/PumpStationControl --web getSwitchState 1";
          exec ($command, $valS1, $return);
          if (is_array($valS1)) {
            $S1 = boolval($valS1[0]);
          } else {
            $S1 = boolval($valS1);
          }
          if ($S1 == 0 ) {
            echo ('<div style="position:absolute;width:68px;height:30px;margin-left:131px;margin-top:29px;background-color:#FF5555;">');
          } else if ($S1 == 1 ) {
            echo ('<div style="position:absolute;width:68px;height:30px;margin-left:131px;margin-top:29px;background-color:#55FF55;">');
          }
          echo ('<div style="font-size:16px;text-align:center;margin-top:5px;">');
          if ($S1 == 0 ) {
            echo ("closed");
          } else if ($S1 == 1 ) {
            echo ("open");
          } else {
            echo ("XXX");
          }
          echo ("</div></div>");

          $command = $ini_array['DocumentRoot']."/PumpStationControl --web getSwitchState 2";
          exec ($command, $valS2, $return);
          if (is_array($valS2)) {
            $S2 = boolval($valS2[0]);
          } else {
            $S2 = boolval($valS2);
          }
          if ($S2 == 0 ) {
            echo ('<div style="position:absolute;width:68px;height:30px;margin-left:41px;margin-top:29px;background-color:#FF5555;">');
          } else if ($S2 == 1 ) {
            echo ('<div style="position:absolute;width:68px;height:30px;margin-left:41px;margin-top:29px;background-color:#55FF55;">');
          }
          echo ('<div style="font-size:16px;text-align:center;margin-top:5px;">');
          if ($S2 == 0 ) {
            echo ("closed");
          } else if ($S2 == 1 ) {
            echo ("open");
          } else {
            echo ("XXX");
          }
          echo ("</div></div>");

          $command = $ini_array['DocumentRoot']."/PumpStationControl --web getSwitchState 3";
          exec ($command, $valS3, $return);
          if (is_array($valS3)) {
            $S3 = boolval($valS3[0]);
          } else {
            $S3 = boolval($valS3);
          }
          if ($S3 == 0 ) {
            echo ('<div style="position:absolute;width:58px;height:30px;margin-left:601px;margin-top:5px;background-color:#FF5555;">');
          } else if ($S3 == 1 ) {
            echo ('<div style="position:absolute;width:58px;height:30px;margin-left:601px;margin-top:5px;background-color:#55FF55;">');
          }
          echo ('<div style="font-size:16px;text-align:center;margin-top:5px;">');
          if ($S3 == 0 ) {
            echo ("OFF");
          } else if ($S3 == 1 ) {
            echo ("ON");
          } else {
            echo ("XXX");
          }
          echo ("</div></div>");

          $command = $ini_array['DocumentRoot']."/PumpStationControl --web getSwitchState 4";
          exec ($command, $valS4, $return);
          if (is_array($valS4)) {
            $S4 = boolval($valS4[0]);
          } else {
            $S4 = boolval($valS4);
          }
          if ($S4 == 0 ) {
            echo ('<div style="position:absolute;width:58px;height:30px;margin-left:601px;margin-top:45px;background-color:#FF5555;">');
          } else if ($S4 == 1 ) {
            echo ('<div style="position:absolute;width:58px;height:30px;margin-left:601px;margin-top:45px;background-color:#55FF55">');
          }
          echo ('<div style="font-size:16px;text-align:center;margin-top:5px;">');
          if ($S4 == 0 ) {
            echo ("OFF");
          } else if ($S4 == 1 ) {
            echo ("ON");
          } else {
            echo ("XXX");
          }
          echo ("</div></div>");
        ?>

        <div style="position:absolute;width:800px;margin-top: 370px;">
        
        <div style="font-size:18px;position:absolute;width:100px;margin-left:0px;text-align:center;">Valve 1</div>
        <div style="font-size:18px;position:absolute;width:100px;margin-left:0px;margin-top:24px;text-align:center;">
        <?php
        if ($S0 == 0 ) {
		  echo ("<img id='switch_0' src='data/button_red.png' onclick='changeSwitch(0);'/>");
		} else {
		  echo ("<img id='switch_0' src='data/button_green.png' onclick='changeSwitch(0);'/>");
		}	
        ?>
        </div>
        
        <div style="font-size:18px;position:absolute;width:100px;margin-left:100px;text-align:center;">Valve 2</div>
        <div style="font-size:18px;position:absolute;width:100px;margin-left:100px;margin-top:24px;text-align:center;">
        <?php
        if ($S1 == 0 ) {
		  echo ("<img id='switch_1' src='data/button_red.png' onclick='changeSwitch(1);'/>");
		} else {
		  echo ("<img id='switch_1' src='data/button_green.png' onclick='changeSwitch(1);'/>");
		}	
        ?>
        </div>
        
        <div style="font-size:18px;position:absolute;width:100px;margin-left:200px;text-align:center;">Valve 3</div>
        <div style="font-size:18px;position:absolute;width:100px;margin-left:200px;margin-top:24px;text-align:center;">
        <?php
        if ($S2 == 0 ) {
		  echo ("<img id='switch_2' src='data/button_red.png' onclick='changeSwitch(2);'/>");
		} else {
		  echo ("<img id='switch_2' src='data/button_green.png' onclick='changeSwitch(2);'/>");
		}	
        ?>
        </div>

        <div style="font-size:18px;position:absolute;width:100px;margin-left:565px;text-align:center;">Pump 1</div>
        <div style="font-size:18px;position:absolute;width:100px;margin-left:565px;margin-top:24px;text-align:center;">
        <?php
        if ($S3 == 0 ) {
		  echo ("<img id='switch_3' src='data/button_red.png' onclick='changeSwitch(3);'/>");
		} else {
		  echo ("<img id='switch_3' src='data/button_green.png' onclick='changeSwitch(3);'/>");
		}	
        ?>
        </div>

        <div style="font-size:18px;position:absolute;width:100px;margin-left:665px;text-align:center;">Pump 2</div>
        <div style="font-size:18px;position:absolute;width:100px;margin-left:665px;margin-top:24px;text-align:center;">
        <?php
        if ($S4 == 0 ) {
		  echo ("<img id='switch_4' src='data/button_red.png' onclick='changeSwitch(4);'/>");
		} else {
		  echo ("<img id='switch_4' src='data/button_green.png' onclick='changeSwitch(4);'/>");
		}	
        ?>
        </div>

	    </div>
	   
        <div style="position:absolute;width:800px;margin-top: 460px;vertical-align:top;">
	    <hr>        
        <img style="width:32px;" src="data/DESY-Logo.png"/>
        <img style="width:32px;" src="data/CMS-Logo.png"/>
        <span style="vertical-align:top;">
        <?php
        $data = shell_exec('uptime');
	    echo (gethostname().' - '.$data);
        ?>
        </span>
	    </div>	    

      </div>
    </div>

	<!-- javascript -->
	<script src="script.js"></script>
</body>
</html>