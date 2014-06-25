byte byteRead;

int firstSensor = 0;    // first analog sensor
int secondSensor = 0;   // second analog sensor
const int num = 4;            // number of times pressure should be checked
int averageFirst = 0;
int averageSecond = 0;


void setup() {                
// Turn the Serial Protocol ON
  Serial.begin(9600);
}


void loop() {
   /*  check if data has been sent from the computer: */
  if (Serial.available()) {
    /* read the most recent byte */
    byteRead = Serial.read();
    
    /*Listen for a comma which equals byte code # 44 */
    if(byteRead==97){
      for(int i=0; i<num; i++){
        averageFirst += analogRead(A0);
        delay(10);
      }
      Serial.println(averageFirst/num);
    }else if(byteRead==98){
      for(int i=0; i<num; i++){
        averageSecond += analogRead(A1);
        delay(10);
      }
      Serial.println(averageSecond/num);
    }else if(byteRead==63){
      Serial.write("1\n");
    }else{
      /*ECHO the value that was read, back to the serial port. */
      Serial.write("Error\n");
    }
  }
}
