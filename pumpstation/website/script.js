var switch_0 = document.getElementById("switch_0");
var switch_1 = document.getElementById("switch_1");
var switch_2 = document.getElementById("switch_2");
var switch_3 = document.getElementById("switch_3");
var switch_4 = document.getElementById("switch_4");

//Create an array for easy access later
var Switches = [ switch_0, switch_1, switch_2, switch_3, switch_4];

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