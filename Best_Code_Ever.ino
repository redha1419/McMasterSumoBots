//Motor 1
int pinR1 = 8; //Direction
int pinR2 = 9; //Direction
int pinPWMA = 3; //Speed

//Motor 2
int pinL1 = 10; //Direction
int pinL2 = 11; //Direction
int pinPWMB = 5; //Speed

// Sonar Left
int pinLEcho = 7;
int pinLTrig = 6;

// Sonar Right
int pinREcho = 13;
int pinRTrig = 12;

long durationL,durationR, distanceR,distanceL;

//Constants to help remember the parameters
static boolean turnCW = 0;  //for motorDrive function
static boolean turnCCW = 1; //for motorDrive function
static boolean motor1 = 0;  //for motorDrive, motorStop, motorBrake functions
static boolean motor2 = 1;  //for motorDrive, motorStop, motorBrake functions


//For QTI
long result = 0;

int pinFR = A4;
int pinFL = A3;
int pinBR = A1;
int pinBL = A2;

//Ernest made us do it , and thank god lol
int i;
int val;
int redpin=A0;

void setup() {
  // put your setup code here, to run once:

  pinMode(pinPWMA, OUTPUT);
  pinMode(pinR1, OUTPUT);
  pinMode(pinR2, OUTPUT);

  pinMode(pinPWMB, OUTPUT);
  pinMode(pinL1, OUTPUT);
  pinMode(pinL2, OUTPUT);

  pinMode(pinLTrig, OUTPUT);
  pinMode(pinLEcho, INPUT);


  pinMode(pinRTrig, OUTPUT);
  pinMode(pinREcho, INPUT);

  pinMode(redpin,OUTPUT);
  Serial.begin(9600);
  delay(5000);

}


void loop() {
  // put your main code here, to run repeatedly:
  forward();
  if (RCduration(pinFR)||RCduration(pinFL)||RCduration(pinBR)||RCduration(pinBL)){
    if (RCduration(pinFR)||RCduration(pinFL)){
      if (RCduration(pinFR)){
        back();
        delay(500);
        turn(0,320);
      }
      else{
        back();
        delay(500);
        turn(1,320);
      }
    }
    else{
      if(RCduration(pinBR)||RCduration(pinBL)){
        if(RCduration(pinBR)){
          scurry(0,500);
        }
        else{
          scurry(1,500);
        }
      }
    }
  }
  else{
    chase(sonar());
  }
}


void motorDrive(boolean motorNumber, boolean motorDirection, int motorSpeed){
  /*
  This Drives a specified motor, in a specific direction, at a specified speed:
    - motorNumber: motor1 or motor2 ---> Motor 1 or Motor 2
    - motorDirection: turnCW or turnCCW ---> clockwise or counter-clockwise
    - motorSpeed: 0 to 255 ---> 0 = stop / 255 = fast
  */

  boolean pinIn1;  //Relates to R1 or L1 (depending on the motor number specified)

  //Specify the Direction to turn the motor
  //Clockwise: R1/L1 = HIGH and R2/L2 = LOW
  //Counter-Clockwise: R1/L1 = LOW and R2/L2 = HIGH
  if (motorDirection == turnCW){
    pinIn1 = HIGH;
  }
  else{
    pinIn1 = LOW;
  }

  //Select the motor to turn, and set the direction and the speed
  if(motorNumber == motor1){
    digitalWrite(pinR1, pinIn1);
    digitalWrite(pinR2, !pinIn1);  //This is the opposite of the R1
    analogWrite(pinPWMA, motorSpeed);
  }
  else{
    digitalWrite(pinL1, pinIn1);
    digitalWrite(pinL2, !pinIn1);  //This is the opposite of the L1
    analogWrite(pinPWMB, motorSpeed);
  }

}


void motorBrake(boolean motorNumber){
  /*
  This "Short Brake"s the specified motor, by setting speed to zero
  */

  if (motorNumber == motor1){
    analogWrite(pinPWMA, 0);
  }
  else{
    analogWrite(pinPWMB, 0);
  }

}


void motorStop(boolean motorNumber){
  /*
  This stops the specified motor by setting both IN pins to LOW
  */
  if(motorNumber == motor1) {
    digitalWrite(pinR1, LOW);
    digitalWrite(pinR2, LOW);
  }
  else{
    digitalWrite(pinL1, LOW);
    digitalWrite(pinL2, LOW);
  }
}


void forward(){
  motorDrive(motor1, turnCW, 255);
  motorDrive(motor2, turnCW, 255);
}


boolean RCduration(int sensPin){
   long result = 0;
   pinMode(sensPin, OUTPUT);       // make pin OUTPUT
   digitalWrite(sensPin, HIGH);    // make pin HIGH to discharge capacitor - study the schematic
   delay(1);                       // wait a  ms to make sure cap is discharged

   pinMode(sensPin, INPUT);        // turn pin into an input and duration till pin goes low
   digitalWrite(sensPin, LOW);     // turn pullups off - or it won't work
   while(digitalRead(sensPin)){    // wait for pin to go low
      result++;
   }

  if (result <= 100){
    return true;
  }
  else{
    return false;
  }
  // report results
}

int sonar(){
  digitalWrite(pinLTrig, LOW);
  delayMicroseconds(2);

  digitalWrite(pinLTrig, HIGH);
  delayMicroseconds(10);

  digitalWrite(pinLTrig, LOW);
  durationL = pulseIn(pinLEcho, HIGH);

  digitalWrite(pinRTrig, LOW);
  delayMicroseconds(2);

  digitalWrite(pinRTrig, HIGH);
  delayMicroseconds(10);

  digitalWrite(pinRTrig, LOW);
  durationR = pulseIn(pinREcho, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distanceL = durationL/58.2;
  distanceR = durationR/58.2;
  if (check()){
    return 2;
  }
  else{
    if(distanceL <= 25 && distanceR <= 25){
      return 2;
    }
    else{
      if (distanceL <= 25){
        return 0;
      }
      if(distanceR <=25){
        return 1;
      }
      else{
        return -1;
      }
    }
  }
}


void chase(int side){
  if (side==0){
    motorStop(motor2);
    motorDrive(motor1, turnCW, 255);
    delay(40);
    forward();
  }
  if(side==1){
    motorStop(motor1);
    motorDrive(motor2, turnCW, 255);
    delay(40);
    forward();
  }
  if(side ==2){
    forward();
  }
}


void turn(int direction,int duration){
  if (direction==0){
    motorDrive(motor2, turnCCW, 255);
    motorDrive(motor1, turnCW, 255);
  }
  else if (direction){
    motorDrive(motor1, turnCCW, 255);
    motorDrive(motor2, turnCW, 255);
  }
  delay(duration/5);
  chase(sonar());
  delay(duration/5);
  chase(sonar());
  delay(duration/5);
  chase(sonar());
  delay(duration/5);
  chase(sonar());
  delay(duration/5);
  chase(sonar());
}

void scurry(int direction, int duration){
  if(direction==0){
    motorDrive(motor2, turnCCW, 255);
    motorDrive(motor1, turnCW, 255);
    delay(100);
  }
  else if(direction){
    motorDrive(motor1, turnCCW, 255);
    motorDrive(motor2, turnCW, 255);
    delay(100);
  }
  forward();
  delay(300);
  //motorStop(motor1);
  //motorStop(motor2);
}


boolean check(){
  i = analogRead(redpin);
  val = (6762/(i-9))-4;
  Serial.println(val);

  if (val < 0){
    return true;
  }
  else{
    return false;
  }
}


void back(){
  motorDrive(motor1, turnCCW, 220);
  motorDrive(motor2, turnCCW, 220);
}
