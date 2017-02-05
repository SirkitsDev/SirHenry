/*
  SirHenry.cpp - Library file allowing rudimentary use of the robot known as SirHenry, developed by Sirkits Pty Ltd.
  Written by J.D Retief and J.C Truter 2/2017

  ## TODO - add explanations of each function:
  ## TODO - document functions properly

  Functions:
    *void moveForward(int dist);
    *void moveBackward(int dist);
    *void turnLeft();
    *void turnRight();
    *void colourEye(uint8_t red, uint8_t blue, uint8_t green);
    *int getDist();
    *void rotateHead();
	* TODO: Add the rest of methods here

  Setup Details:
    * Left Bumper is connected to pin A0
    * Front Bumper is connected to pin A1
    * Right bumper is connected to pin A2
    * Back bumper is connected to pin A3
    * 
*/

// Necessary Includes:
#include "Arduino.h"
#include "SirHenry.h"
#include <NewPing.h>
#include "Servo.h"

// Defining Pin Connections:
#define redPin 6
#define bluePin 5
#define greenPin 3
#define servoPin 9
#define MotorABrakePin 10
#define MotorBBrakePin 11
#define MotorADirectionPin 12
#define MotorBDirectionPin 13
#define triggerPin A5
#define echoPin A4
#define leftPin A0
#define frontPin A1
#define rightPin A2
#define hindPin A3

// MOTOR A is the right side motor, and MOTOR B is the left side motor.

// Globally accessible objects; 
Servo myServo;
NewPing sonar(triggerPin,echoPin, 300); //Trigger, Echo, maxDist(cm)

SirHenry::SirHenry(void){
  // Setting up pin modes:
  
  // RGB LED pins:
  pinMode(redPin, OUTPUT); // red pin
  pinMode(bluePin, OUTPUT); // blue pin
  pinMode(greenPin, OUTPUT); //green pin
  
  // Motor Control pins:
  pinMode(MotorABrakePin,OUTPUT);       //Channel A Brake Pin
  pinMode(MotorBBrakePin,OUTPUT);       //Channel B Brake Pin
  pinMode(MotorADirectionPin,OUTPUT);      //Channel A Direction Pin
  pinMode(MotorBDirectionPin,OUTPUT);      //Channel B Direction Pin
  
  // Bumper pins:
  pinMode(leftPin,INPUT);
  pinMode(frontPin,INPUT);
  pinMode(rightPin,INPUT);
  pinMode(hindPin,INPUT);

  // Servo:
  pinMode(servoPin,OUTPUT);
}


// <START> -- PRIVATE METHODS --

void SirHenry::motorAControl(uint8_t dir, int speed, int time){
	/*
	* This function controls motor A (right side). It sets the direction of rotation, speed 
	* and time that the motor is active.
	* 
	* Params:
	* 	uint8_t dir: '0' for forwards (clockwise rotation) and any other integer (e.g. '1') to 
	* 	go backwards (anti-clockwise rotation).
	* 	int speed: Between 0 and 255. 255 is maximum speed.
	* 	int time: The time in milliseconds that the motor turns
	* 
	* Returns:
	* 	None
	* 
	* */
	
  // Making sure that the speed integer is between 0 and 255
  if (speed > 255){
	  speed = 255;
  } else if (speed < 0){
	  speed = 0;
  }
  
  // Sending signals to the motor
  if(dir == 0){ // Forwwards (clockwise)
  
    digitalWrite(MotorADirectionPin,LOW); // Direction pin (MN pin) = LOW
    analogWrite(MotorABrakePin,speed); // Write pwm signal (between 0 and 255) to 'EN' pin (motorABrakePin)
  }

  else{ // Else go backwards (anti-clockwise)
    digitalWrite(MotorADirectionPin,HIGH); // Direction pin (MN pin) = HIGH
    analogWrite(MotorABrakePin,speed); // Write pwm signal (between 0 and 255) to 'EN' pin (motorABrakePin)
  }
	
  // Letting the motor run for 'time' milliseconds
  delay(time);
  // Stop the motor
  analogWrite(MotorABrakePin,0);
}

void SirHenry::motorBControl(uint8_t dir, int speed, int time){
	/*
	* This function controls motor B (left side). It sets the direction of rotation, speed 
	* and time that the motor is active.
	* 
	* Params:
	* 	uint8_t dir: '0' for forwards (clockwise rotation) and any other integer (e.g. '1') to 
	* 	go backwards (anti-clockwise rotation).
	* 	int speed: Between 0 and 255. 255 is maximum speed.
	* 	int time: The time in milliseconds that the motor turns
	* 
	* Returns:
	* 	None
	* 
	* */
	
  // Making sure that the speed integer is between 0 and 255
  if (speed > 255){
	  speed = 255;
  } else if (speed < 0){
	  speed = 0;
  }
  
  // Making sure the time is positive
  if (time <= 0){
	  time = 0;
  }
  
  // Sending signals to the motor
  if(dir == 0){ // Forwards (clockwise)

    digitalWrite(MotorBDirectionPin,LOW); // Direction pin (MN pin) = LOW
    analogWrite(MotorBBrakePin,speed); // Write pwm signal (between 0 and 255) to 'EN' pin (motorABrakePin)
  }

  else{ // Else turn backwards (anti-clockwise)
    digitalWrite(MotorBDirectionPin,HIGH); // Direction pin (MN pin) = HIGH
    analogWrite(MotorBBrakePin,speed); // Write pwm signal (between 0 and 255) to 'EN' pin (motorABrakePin)
  }
	
  // Letting the motor run for 'time' milliseconds
  delay(time);
  // Stop the motor
  analogWrite(MotorBBrakePin,0);
}

void SirHenry::motorABControl(uint8_t dir, int speed, int time){
   /*
    * This function controls motor A and B together. It sets the direction of rotation, speed 
	* and time that the motors are active.
	* 
	* Params:
	* 	uint8_t dir: '0' for forwards (clockwise rotation) and any other integer (e.g. '1') to 
	* 	go backwards (anti-clockwise rotation).
	* 	int speed: Between 0 and 255. 255 is maximum speed.
	* 	int time: The time in milliseconds that the motor turns (must be >= 0).
	* 
	* Returns:
	* 	None
	* 
	* */
	
   // Making sure that the speed integer is between 0 and 255
  if (speed > 255){
	  speed = 255;
  } else if (speed < 0){
	  speed = 0;
  }
  
  // Making sure the time is positive
  if (time <= 0){
	  time = 0;
  }

  if(dir==0){ //Forward
	  
    digitalWrite(MotorADirectionPin,LOW); 
    analogWrite(MotorABrakePin,speed);

    digitalWrite(MotorBDirectionPin,LOW);
    analogWrite(MotorBBrakePin,speed);
  } 
  else{ //Backward
	  
    digitalWrite(MotorADirectionPin,HIGH);
    analogWrite(MotorABrakePin,speed);

    digitalWrite(MotorBDirectionPin,HIGH);
    analogWrite(MotorBBrakePin,speed);
  }
  
  // Letting the motors run for 'time' milliseconds
  delay(time);
  // Stops motors
  analogWrite(MotorABrakePin,0);
  analogWrite(MotorBBrakePin,0);
}

void SirHenry::motorA(uint8_t dir){
	  /* 
   * This fuction provides basic functionality to control Motor B.
   * Function replaced by motorAControl().
   * 
   * Look at motorAControl() for explinations of commands used below.
   * */
	
  if(dir == 0){ // Forward (clockwise)

    digitalWrite(MotorADirectionPin,LOW); 
    analogWrite(MotorABrakePin,255);
  }

  else{ // Backwards (anti-clockwise)
  
    digitalWrite(MotorADirectionPin,HIGH);
    analogWrite(MotorABrakePin,255);
  }

  delay(800);
  analogWrite(MotorABrakePin,0);
}

void SirHenry::motorB(uint8_t dir){
  /* 
   * This fuction provides basic functionality to control Motor B.
   * Function replaced by motorBControl().
   * 
   * Look at motorBControl() for explinations of commands used below.
   * */
   
  if(dir == 0){ // Forward (clockwise)
    

    digitalWrite(MotorBDirectionPin,LOW);
    analogWrite(MotorBBrakePin,255);
  }
  else{ // Backwards (anti-clockwise)
  
    digitalWrite(MotorBDirectionPin,HIGH);
    analogWrite(MotorBBrakePin,255);
  }

  delay(800);
  analogWrite(MotorBBrakePin,0);
}

void SirHenry::motorAB(uint8_t dir){
  /*
   *  This function provides basic control for motor A and B together.
   *  motorABControl() provides more customisability.
   * 
   * Params:
   * 	uint8_t dir: '0' to go forward (clockwise) and any other integer (e.g. '1') to 
   * 	go backward (anti-clockwise).
   * 
   * */

  if(dir==0){ //Forward
	  
    digitalWrite(MotorADirectionPin,LOW); 
    analogWrite(MotorABrakePin,255);

    digitalWrite(MotorBDirectionPin,LOW);
    analogWrite(MotorBBrakePin,255);
  } 
  else{ //Backward
	  
    digitalWrite(MotorADirectionPin,HIGH);
    analogWrite(MotorABrakePin,255);

    digitalWrite(MotorBDirectionPin,HIGH);
    analogWrite(MotorBBrakePin,255);
  }

  delay(800);
  analogWrite(MotorABrakePin,0);
  analogWrite(MotorBBrakePin,0);
}

// -- PRIVATE METHODS -- <END>


// <START> -- PUBLIC METHODS --

void SirHenry::move(int speed){
	/* 
	 * Function simply enables both motors at the same time
	 * to move forward (clockwise) at the specified speed
	 * 
	 * */
	 
	// Making sure that the speed integer is between 0 and 255
	if (speed > 255){
		speed = 255;
	} else if (speed < 0){
		speed = 0;
	}
	
	digitalWrite(MotorADirectionPin,LOW); 
	analogWrite(MotorABrakePin,speed);
	digitalWrite(MotorBDirectionPin,LOW);
	analogWrite(MotorBBrakePin,speed);
}

void SirHenry::stop(){
	
  /*
   * Function stops both motors
   * 
   * */
  analogWrite(MotorABrakePin,0);
  analogWrite(MotorBBrakePin,0);
}

void SirHenry::moveForward(int dist){
/* Method instructs both motors to rotate forward for a 'dist' amount of times.
 * */
  for(uint8_t i = 0; i<dist;i++){
    motorAB(0);
  }
}

void SirHenry::moveBackward(int dist){
/* Method instructs both motors to rotate backward for a 'dist' amount of times.
 * */
  for(uint8_t i = 0; i<dist;i++){
    motorAB(1);
  }
}


void SirHenry::turnLeft(void){
/* This method instructs the robot to turn LEFT by instructing the right-side motor to turn clockwise
 * and left-side motor to turn anti-clockwise at maximum speed for 0.525 seconds.
 * */
 
  digitalWrite(MotorADirectionPin,LOW); 
  digitalWrite(MotorBDirectionPin,HIGH);
  
  analogWrite(MotorABrakePin,255);
  analogWrite(MotorBBrakePin,255);
  
  delay(525);
  
  analogWrite(MotorABrakePin,0);
  analogWrite(MotorBBrakePin,0);
}

void SirHenry::turnLeft(int rightSpeed, int leftSpeed, int time){
/* This method is used to make a left-hand turn with the robot. The speed of both motors and the anount 
 * of time the wheels spin is adjustable. Thus the speed and angle of the turn can be controlled.
 * 
 * Params:
 *   int rightSpeed: Speed of motor A (right side). Value between 0 and 255.
 *   int leftSpeed: Speed of motor B (left side). Value between 0 and 255.
 *   int time: Amount of time in milliseconds that the motors will rotate.
 * 
 * Returns:
 *   None
 * */
 
	// Making sure that the speed integers are between 0 and 255
  if (rightSpeed > 255){
	  rightSpeed = 255;
  } else if (rightSpeed < 0){
	  rightSpeed = 0;
  }
	
  if (leftSpeed > 255){
	  leftSpeed = 255;
  } else if (leftSpeed < 0){
	  leftSpeed = 0;
  }
	
	if (time < 0) time = 0;

  digitalWrite(MotorADirectionPin,LOW); 
  digitalWrite(MotorBDirectionPin,HIGH);
  
  analogWrite(MotorABrakePin,rightSpeed);
  analogWrite(MotorBBrakePin,leftSpeed);
  
  delay(time);
  
  analogWrite(MotorABrakePin,0);
  analogWrite(MotorBBrakePin,0);
}

void SirHenry::turnRight(void){
/* This method instructs the robot to turn RIGHT by instructing the right-side motor to turn anti-clockwise
 * and left-side motor to turn clockwise at maximum speed for 0.5 seconds.
 * */

  digitalWrite(MotorADirectionPin,HIGH);
  digitalWrite(MotorBDirectionPin,LOW);
  
  analogWrite(MotorABrakePin,255);
  analogWrite(MotorBBrakePin,255);
  
  delay(525);
  
  analogWrite(MotorABrakePin,0);
  analogWrite(MotorBBrakePin,0);
}

void SirHenry::turnRight(int rightSpeed, int leftSpeed, int time){
/* This method is used to make a right-hand turn with the robot. The speed of both motors and the anount 
 * of time the wheels spin is adjustable. Thus the speed and angle of the turn can be controlled.
 * 
 * Params:
 *   int rightSpeed: Speed of motor A (right side). Value between 0 and 255.
 *   int leftSpeed: Speed of motor B (left side). Value between 0 and 255.
 *   int time: Amount of time in milliseconds that the motors will rotate.
 * 
 * Returns:
 *   None
 * */
	
	// Making sure that the speed integers are between 0 and 255
  if (rightSpeed > 255){
	  rightSpeed = 255;
  } else if (rightSpeed < 0){
	  rightSpeed = 0;
  }
	
  if (leftSpeed > 255){
	  leftSpeed = 255;
  } else if (leftSpeed < 0){
	  leftSpeed = 0;
  }
	
	if (time < 0) time = 0;
	
  digitalWrite(MotorADirectionPin,HIGH); 
  digitalWrite(MotorBDirectionPin,LOW);
  
  analogWrite(MotorABrakePin,rightSpeed);
  analogWrite(MotorBBrakePin,leftSpeed);
  
  delay(time);
  
  analogWrite(MotorABrakePin,0);
  analogWrite(MotorBBrakePin,0);
}


void SirHenry::moveForwardDetailed(int speed, int time){
/* Method instructs both motors to rotate forward for a 'dist' amount of times.
 * */
  motorABControl(0, speed, time);
}

void SirHenry::moveBackwardDetailed(int speed, int time){
/* Method instructs both motors to rotate backward for a 'dist' amount of times.
 * */
  motorABControl(1, speed, time);
}


void SirHenry::colourEye(uint8_t red, uint8_t green, uint8_t blue){
/* 	This method controls the RGB LED of the robot.
 *  A desired RGB parameters has to be passed to the method in the following format: (R, G, B)
 * 	The method then sets the values of the individual pins on the LED module
 *
 * 	Params:
 * 		uint8_t red: Red value of RGB tuple
 * 		uint8_t green: Green value of RGB tuple
 * 		uint8_t blue: Blue value of RGB tuple
 *
 * 	Output:
 * 		None
 *
 * 	Some RGB combinations:
        191,191,191 -white
        0,0,0       - OFF
        0,255,0     -pure green
        255,0,0     -pure red
        0,0,255     -pure blue
        232,32,32   -pink
        28,200,28   -nice green
        191,64,191  -light purple
        255,128,0   -bright yellow
        255,0,128   -bright purple
        0,255,128   -cyan
        0,128,255   -light blue
        128,0,255   -bluish-purple
        64,0,255    -violet-ish
        0,64,128    -light-cyan
        128,64,64   -purplish-silver
 */
 
// Setting the corresponding pins' values
// 255-(value) since LED module's colours are 'inverted'
 if((red>=0)&&(red<=255))
	analogWrite(redPin,255-red);
  
 if((green>=0)&&(green<=255))
	analogWrite(greenPin,255-green);

 if((blue>=0)&&(blue<=255))
	analogWrite(bluePin,255-blue);
}

void SirHenry::rotateHead(int angle){
  // Angle in degrees
  // Rotates to that angle position. Not rotating that amount of degrees from current point
  
  // Making sure angle is between -85 and 85 degrees
  if (angle > 85)
	  angle = 85;
  if (angle < -85)
	  angle = -85;
  
  myServo.attach(servoPin);

  if((angle>=-85)&&(angle<=85)){ // Again make sure angle is between -85 and 85 before adjusting servo
	myServo.write(90+angle);
	delay(200);
  } 
}

int SirHenry::getDist(){

  int echo = sonar.ping_median(5);
  int dist = sonar.convert_cm(echo);
  return dist;
}

//Bumper methods
uint8_t SirHenry::front_bumper(){

    if(digitalRead(frontPin) == HIGH)
      return 1;
    else
      return 0;
}

uint8_t SirHenry::rear_bumper(){

    if(digitalRead(hindPin) == HIGH)
      return 1;
    else
      return 0;
}

uint8_t SirHenry::left_bumper(){

    if(digitalRead(leftPin) == HIGH)
      return 1;
    else
      return 0;
}

uint8_t SirHenry::right_bumper(){

    if(digitalRead(rightPin) == HIGH)
      return 1;
    else
      return 0;
}

void SirHenry::detect(uint8_t bumper_arr[]){
    
    bumper_arr[0] = digitalRead(frontPin);
    bumper_arr[1] = digitalRead(hindPin);
    bumper_arr[2] = digitalRead(rightPin);
    bumper_arr[3] = digitalRead(leftPin);
}

// -- PUBLIC METHODS -- <END>