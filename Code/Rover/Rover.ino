/*
 * License: GPL V3
 * Code by Chris Dagher for ENGR 280 Spring 2018
 *    Control a rover through an arena by receiving commands
 *    from another Arduino over serial. Parsing the commands 
 *    will result in moving the motors according to the 
 *    different reads.
 */

#define EN             8
#define leftMotorDir   7
#define leftMotorStep  4
#define rightMotorDir  5
#define rightMotorStep 2
#define armMotorStep   3
#define armMotorDir    6

int serial = 5;

int armState = 0;

const int motorSpeed = 1000;  // time delay between step pulses

int cycles = 0;

void motorBoot() {  // not using a function to drive motors, just manually programme with for loops
  //
  digitalWrite(leftMotorDir, HIGH);
  digitalWrite(rightMotorDir, HIGH);
  for (int i = 0; i < 100; i++) {  // move forward
    digitalWrite(leftMotorStep, HIGH);
    digitalWrite(rightMotorStep, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(leftMotorStep, LOW);
    digitalWrite(rightMotorStep, LOW);
    delayMicroseconds(motorSpeed);
  }
  //
  delay(25);
  //
  digitalWrite(leftMotorDir, LOW);
  digitalWrite(rightMotorDir, LOW);
  for (int j = 0; j < 100; j++) {  // move backward
    digitalWrite(leftMotorStep, HIGH);
    digitalWrite(rightMotorStep, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(leftMotorStep, LOW);
    digitalWrite(rightMotorStep, LOW);
    delayMicroseconds(motorSpeed);
  }
  //
  delay(25);
  //
  digitalWrite(leftMotorDir, HIGH);
  digitalWrite(rightMotorDir, LOW);
  for (int k = 0; k < 100; k++) {  // turn clockwise
    digitalWrite(leftMotorStep, HIGH);
    digitalWrite(rightMotorStep, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(leftMotorStep, LOW);
    digitalWrite(rightMotorStep, LOW);
    delayMicroseconds(motorSpeed);
  }
  //
  delay(25);
  //
  digitalWrite(leftMotorDir, LOW);
  digitalWrite(rightMotorDir, HIGH);
  for (int z = 0; z < 100; z++) {  // turn counter-clockwise
    digitalWrite(leftMotorStep, HIGH);
    digitalWrite(rightMotorStep, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(leftMotorStep, LOW);
    digitalWrite(rightMotorStep, LOW);
    delayMicroseconds(motorSpeed);
  }
  //
  delay(25);
}

void setup() {
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);  // turn on motors
  pinMode(leftMotorDir, OUTPUT);
  pinMode(leftMotorStep, OUTPUT);
  pinMode(rightMotorDir, OUTPUT);
  pinMode(rightMotorStep, OUTPUT);
  Serial.begin(9600);  // open serial communication
  while (!Serial) {
    ; // wait for serial to become available
  }
  //motorBoot();  // used for debugging motors
}

void moveMotors(int stepPin1 = -1, int dirPin1 = -1, int stepPin2 = -1, int dirPin2 = -1, int motorDirection = -1) {
  // coordinated move of two steppers
  // -1 means do nothing with that pin
  digitalWrite(EN, LOW);  // turn on motors after being shut down
  switch (motorDirection) {
    case 0:  // move forward
      digitalWrite(dirPin1, LOW);
      digitalWrite(dirPin2, HIGH);
      break;
    case 1:  // move backward
      digitalWrite(dirPin1, HIGH);
      digitalWrite(dirPin2, LOW);
      break;
  }
  digitalWrite(stepPin1, HIGH);
  digitalWrite(stepPin2, HIGH);
  delayMicroseconds(motorSpeed);
  digitalWrite(stepPin1, LOW);
  digitalWrite(stepPin2, LOW);
  delayMicroseconds(motorSpeed);  // move one step
}

void moveArm(int steps = 0, int dir = 0) {  // moves the front arm of the rover
  for (int i = 0; i <= steps; i++) {
    moveMotors(armMotorStep, armMotorDir, -1, -1, dir);
  }
}

void serialRead() {  // reads incoming serial commands
  int incomingByte = Serial.read();  // read an integer
  serial = incomingByte;  // write it to the global variable
  //Serial.println(serial);  // send a confirmation command to the controller
  return;
}

void serialInterpret() {  // determines what to do with commands
  switch (serial) {
    case 1:         // move forward
      moveRover(0);
      break;
    case 49:
      moveRover(0);
      break;
    case 2:         // move backward
      moveRover(1);
      break;
    case 50:
      moveRover(1);
      break;
    case 3:         // rotate clockwise
      turnRover(0);
      break;
    case 51:
      turnRover(0);
      break;
    case 4:         // rotate counter clockwise
      turnRover(1);
      break;
    case 52:
      turnRover(1);
      break;
    case 6:         // move the arm   currently does nothing
      break;
    case 54:
      break;
    case 7:         // move left motor forward   disabled
      break;
    case 8:         // move left motor backward   disabled
      break;
    case 9:         // move right motor forward   disabled
      break;
    case 11:        // move right motor backward   disabled
      break;
    case 5:         // rest & wait
      break;
    case 53:
      break;
    case 10:        // ignore the enter key when USB debugging
      break;
  }
  delay(1);  // wait to avoid confusion between parsing
  serial = 5;  // reset serial read
  return;
}

void moveRover(int roverDirection) {  // coordinated movement of the rover
  moveMotors(leftMotorStep, leftMotorDir, rightMotorStep, rightMotorDir,roverDirection);
}

void turnRover(int turnDirection) {  // coordinated turning of the rover
  switch (turnDirection) {
    case 0:  // turn clockwise
      moveMotors(leftMotorStep, leftMotorDir, -1, -1, 0);
      moveMotors(-1, -1, rightMotorStep, rightMotorDir, 1);
      break;
    case 1:  // turn counter-clockwise
      moveMotors(leftMotorStep, leftMotorDir, -1, -1, 1);
      moveMotors(-1, -1, rightMotorStep, rightMotorDir, 0);
      break;
  }
}

void loop() {
  cycles =+ 1;
  serialRead();  // read serial commands
  serialInterpret();  // interpret commands and move motors
  if (cycles >= 200000) {
    digitalWrite(EN,HIGH);  // turn off motors after a period of inactivity
    cycles = 0;  // reset inavtivity counter
  }
  else {
    ;
  }
  //delay(500);  // used for USB debugging
}

