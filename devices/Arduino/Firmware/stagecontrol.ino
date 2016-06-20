#include <Stepper.h>

int in1Pin = 2;
int in2Pin = 3;
int in3Pin = 8;
int in4Pin = 9;

Stepper motor(200, in1Pin, in3Pin);  
String command;


void setup() {
  // put your setup code here, to run once:

    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
    pinMode(in3Pin, OUTPUT);
    pinMode(in4Pin, OUTPUT);
    digitalWrite(in2Pin,HIGH);
    digitalWrite(in4Pin,HIGH);

    Serial.begin(9600);


}

void loop() {
  // put your main code here, to run repeatedly:

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

  //PINON 3
  //PINOFF 3

 part1 = com.substring(0,com.indexOf(","));
 part2 = com.substring(com.indexOf(",") +1 );

 int distance = part1.toInt();
 int speed = part2.toInt();
 float acc = 0.1;
 int new_speed = speed;

 for (int inc = 1; inc < 200; inc++){

   if (inc < 100){ 
    new_speed = new_speed + (speed*acc);
   }
   else{
    new_speed = new_speed - (speed*acc);
      } 

      Serial.println(new_speed);
      motor.setSpeed(new_speed);
      motor.step(distance);  
  }

 
      Serial.println(part1);
      Serial.println(",");
      Serial.println(part2);

}


