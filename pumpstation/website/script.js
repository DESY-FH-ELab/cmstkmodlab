var switch_V1 = document.getElementById("switch_V1");
var switch_V2 = document.getElementById("switch_V2");
var switch_V3 = document.getElementById("switch_V3");
var switch_P1 = document.getElementById("switch_P1");
var switch_P2 = document.getElementById("switch_P2");

//Create an array for easy access later
var Switches = [ switch_V1, switch_V2, switch_V3, switch_P1, switch_P2];

function changeSwitch(channel) {
	var data = 0;
	var request = new XMLHttpRequest();
	request.open("GET", "ConradSwitch.php?channel=" + channel, true);
	request.send(null);
	request.onreadystatechange = function () {
		if (request.readyState == 4 && request.status == 200) {
			data = request.responseText;
			//update the index pic
			if (!(data.localeCompare("0"))) {
				Switches[channel].src = "data/button_red.png";
				window.location.reload(true);
			} else if (!(data.localeCompare("1"))) {
				Switches[channel].src = "data/button_green.png";
				window.location.reload(true);
			} else if (!(data.localeCompare("fail"))) {
				alert ("Something went wrong!" );
				return ("fail");
			} else {
				alert ("Something went wrong!");
				return ("fail"); 
			}

		} else if (request.readyState == 4 && request.status == 500) {
			alert ("server error");
			return ("fail");
		} else if (request.readyState == 4 && request.status != 200 && request.status != 500 ) { 
			alert ("Something went wrong!");
			return ("fail");
		}
	}
	return 0;
}