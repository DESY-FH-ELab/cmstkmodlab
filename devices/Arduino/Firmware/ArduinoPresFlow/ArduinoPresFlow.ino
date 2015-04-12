#include <Arduino.h>

String inputString = "";
boolean stringComplete = false;

const int channelA = A0; // first analog sensor
const int channelB = A1; // second analog sensor
const int channelC = A2; // third analog sensor
const int num = 4;       // number of times pressure should be checked
int average;

void readSensor(const char * prefix, int channel)
{
  average = 0;
  for (int i=0; i<num; ++i) {
    average += analogRead(channel);
    delay(10);
  }
  Serial.print(prefix);
  Serial.println(average/num);
}

void setup()
{
  // set reference for ADC; should be changed to EXTERNAL eventually
  analogReference(DEFAULT);

  // this line is for Leonardo's, it delays the serial interface
  // until the terminal window is opened
  while (!Serial);
  Serial.begin(9600);

  inputString.reserve(32);
}

void loop()
{
  if (stringComplete) {

    if (inputString.startsWith("PA")) {
      readSensor("PA,", channelA);
    } else if (inputString.startsWith("PB")) {
      readSensor("PB,", channelB);
    } else if (inputString.startsWith("PC")) {
      readSensor("PC,", channelC);
    } else if (inputString.startsWith("ID")) {
      Serial.println("ID,ArduinoPres");
    } else {
      Serial.println("ERROR");
    }

    inputString = "";
    stringComplete = false;
  }
}

void serialEvent()
{
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
