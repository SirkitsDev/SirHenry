/*
  SirHenry.h - header file for the library used for controlling the robot known as SirHenry, developed by Sirkits Pty Ltd.
  Written by J.D Retief and J.C Truter - 12/2016
  ## todo - add explanations of each function:

  Functions:
    *void moveForeward(int dist);
    *void moveBackward(int dist);
    *void turnLeft();
    *void turnRight();
    *void colourEye(uint8_t red, uint8_t blue, uint8_t green);
    *int getDist();
    *void rotateHead();
*/

#ifndef SirHenry_h
#define SirHenry_h

#include "Arduino.h"
#include "Servo.h"
#include "NewPing.h"

class SirHenry
{
  public:
  
  // Here go all of the publicly accessible functions. All of them must use a camelBack naming 
  // convention as per the Arduio library style guide. See www.arduino.cc/en/Hacking/LibraryTutorial for more details.
  
    SirHenry();
	void move(int speed);
	void stop();
    void moveForward(int dist);
    void moveBackward(int dist);
	void moveForwardDetailed(int speed, int time);
	void moveBackwardDetailed(int speed, int time);
    void turnLeft(void);
    void turnRight(void);
    void colourEye(uint8_t red, uint8_t blue, uint8_t green);
    int getDist(void);
	int getAvgDist(void);
    void rotateHead(int angle);
	uint8_t front_bumper();
	uint8_t rear_bumper();
	uint8_t left_bumper();
	uint8_t right_bumper();
    void detect(uint8_t bumper_arr[]);
  private:
    void motorA(uint8_t dir);
    void motorB(uint8_t dir);
    void motorAB(uint8_t dir);
	void motorAControl(uint8_t dir, int speed, int time);
	void motorBControl(uint8_t dir, int speed, int time);
	void motorABControl(uint8_t dir, int speed, int time);
    
};


#endif
