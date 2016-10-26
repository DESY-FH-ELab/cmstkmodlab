/*
  Dimmer

 Demonstrates the sending data from the computer to the Arduino board,
 in this case to control the brightness of an LED.  The data is sent
 in individual bytes, each of which ranges from 0 to 255.  Arduino
 reads these bytes and uses them to set the brightness of the LED.

 The circuit:
 LED attached from digital pin 9 to ground.
 Serial connection to Processing, Max/MSP, or another serial application

 created 2006
 by David A. Mellis
 modified 30 Aug 2011
 by Tom Igoe and Scott Fitzgerald

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Dimmer

 */

const int ledPin = 13;      // the pin that the LED is attached to
String command;

void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  // initialize the ledPin as an output:
  pinMode(ledPin, OUTPUT);
}

void loop() {

if(Serial.available())
{  
    char c = (char)Serial.read();

    if(c   == '\n')
       {
       parseCommand(command);
       command = "";
       }
    else{
        command += c;
        } 
}


}



void parseCommand(String com)
{

  String part1;
  String part2;


 part1 = com.substring(0,com.indexOf(","));
 part2 = com.substring(com.indexOf(",") +1 );

 int expo_time = part1.toInt();
 int intensity = part2.toInt();

 analogWrite(ledPin, intensity);
 delay(expo_time);
 digitalWrite(ledPin, LOW);

 
 Serial.println(part1);
 Serial.println(",");
 Serial.println(part2);

}

 
