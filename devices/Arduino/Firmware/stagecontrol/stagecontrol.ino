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
  String str1;
  String str2;
  String str3;
  String str4;


  //PINON 3
  //PINOFF 3

 part1 = com.substring(0,com.indexOf(":"));
 part2 = com.substring(com.indexOf(":") +1 );

 str1  = part1.substring(0, part1.indexOf(","));
 str2  = part1.substring(part1.indexOf(",")+1);
 str3  = part2.substring(0, part2.indexOf(","));
 str4  = part2.substring(part2.indexOf(",")+1);

 int initial_speed = 10;
 int target_speed = str1.toInt();
 int max_acceleration = str2.toInt();
 int scan_distance = str3.toInt();
 int steps_per_mm = str4.toInt();

 //convert from mm to n steps
 target_speed = target_speed*steps_per_mm;  
 max_acceleration = max_acceleration*steps_per_mm;
 scan_distance = scan_distance*steps_per_mm;

 Serial.println(target_speed);
 Serial.println(max_acceleration);
 Serial.println(scan_distance);
 Serial.println(steps_per_mm);

 float acc = 0.1;
 int new_speed = initial_speed;

 for (int inc = 1; inc < 30; inc++){

   if (inc < 10){ 
      new_speed = new_speed + (target_speed/10);
      motor.setSpeed(new_speed);
      motor.step(scan_distance/20);  
   }
   else if (inc >= 10 && inc < 20){
      motor.step(scan_distance/10);      
   } 
   else{
    new_speed = new_speed - (target_speed/10);
    motor.setSpeed(new_speed);
    motor.step(scan_distance/20); 
      } 
 
     
  }

 
      Serial.println(part1);
      Serial.println(",");
      Serial.println(part2);

}


