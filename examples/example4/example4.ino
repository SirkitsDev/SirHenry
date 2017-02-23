#include <Servo.h>
#include <NewPing.h>
#include <SirHenry.h>

// DO NOT FLASH SKETCH!!!
// PLEASE NOTE - VERY IMPORTANT: THIS SKETCH IS STILL IN DEVELOPMENT AND IS NOT YET TO BE USED.
// THIS WAS NOT TESTED YET AND IS MOST POSSIBLY RIDDLED WITH BUGS

/* Sir Henry example4.
 *  By Cobus Truter (Feb 2017)
 *  
 *  This example aims to provide simple control of the robot via Bluetooth communication.
 *  
 *  IMPORTANT: Remember to disconnect the bluetooth module from the Arduino before uploading sketches to the Arduino.
 *             Reconnect the bluetooth module after the sketch has been uploaded.
 *  
 *  SETUP:
 *  - Download the "Arduino Bluetooth Controller" app from Google Play Store and pair with the robot (Sir_H)
 *  - Connect to 'Sir_H' in "Controller Mode"
 *  - Configure the controller as follows:
 */
 
// DO NOT FLASH SKETCH!!!
// PLEASE NOTE - VERY IMPORTANT: THIS SKETCH IS STILL IN DEVELOPMENT AND IS NOT YET TO BE USED.
// THIS WAS NOT TESTED YET AND IS MOST POSSIBLY RIDDLED WITH BUGS
 
// Servo rotation limitation variables. Adjust according to your own results from basic_testing.ino.
const uint8_t maxAngle = 85; // Maximum allowable angle of servo rotation
const int minAngle = -75; // Minimum allowable angle of servo rotation
const int head_straight = 0; // Head alignment offset. Adjust accordingly

const uint8_t closest_dist = 25; // Distance at which an object is deemed 'too close'
const uint8_t clear_path_dist = 25; // If there is no objects present within this distance, the path is deemed clear
uint16_t dist;
uint8_t bumper_arr[4];
char data = 0;
char STATE = '1';

/* ROBOT STATES:
 * 1 - Normal control mode. No objects detected, no autonomous control.  
 * 2 - Collision/object detected. Autonomous control activated to evade object(s).
 */

SirHenry bot(maxAngle,minAngle);

void setup() {
  Serial.begin(115200); // Bluetooth module setup to communicate at
  bot.rotateHead(head_straight); // Reset head 
  delay(250);
}

void loop() {
 
  // Collision detection
  bot.detect(bumper_arr);
  dist = bot.getDist(7);

  // Setting state 2 if needed
  if (dist <= closest_dist || bumper_arr[0] == 0 || bumper_arr[1] == 0 || bumper_arr[2] == 0 || bumper_arr[3] == 0) STATE = '2';

  // Adjusting colour of LED according to distance from object
  if (dist <= closest_dist){
      bot.colourEye(255,0,0); //Red
  } else if ((dist > closest_dist)&&(dist <= clear_path_dist)) {
      bot.colourEye(255,140,0); //Orange
  } else if ((dist > clear_path_dist)&&(dist <= 100)){
      bot.colourEye(255,255,0); //Yellow
  } else bot.colourEye(0,255,0); //Green

  // State handling
  if (STATE == '1'){ // Manual control active
   if(Serial.available() > 0){ 
        data = Serial.read(); // Read the data (1st character only)
        Serial.print(data); // Print data value for debugging purposes
        Serial.print("\n");
  
        // Controlling robot depending on data value
        // Upper-case letters are commands sent on press start of button
        // Lower-case letters are commands sent on release of button
  
        // TODO: UPDATE LIBRARY TO INCLUDE CONSTANT TURNING FUNCTIONS
        //       UPDATE LIBRARY TO INCLUDE CONSTANT BACKWARDS FUNCTION
        
        switch(data){
          case 'A':
            bot.move();
            break;
          case 'a':
            bot.stop();
            break;
          case 'B':
            bot.moveBackward(1); // make constant fucntion
            break;
          case 'C':
            bot.turnLeft();  // make constant fucntion
            break;
          case 'D':
            bot.turnRight();  // make constant fucntion
            break;
        } 
     }
  } 
  else if(STATE == '2'){
    bot.stop();
    if (dist <= closest_dist){ //If too close to an object
      switch (scoutPath()){
        case 'c':
          bot.move(255);
          break;
        case 'l':
          bot.turnLeft();
          bot.move(255);
          break;
        case 'r':
          bot.turnRight();
          bot.move(255);
          break;
        case 'n':
          escape();
          break;
      }
    } // End distance handling
    
    if(bumper_arr[0] == 0 || bumper_arr[1] == 0 || bumper_arr[2] == 0 || bumper_arr[3] == 0){ // If a collision is detected
    bot.stop();
    
    if (bumper_arr[0] == 0){ // Front bumper
      bot.moveBackward(1);
      if (bumper_arr[2] == 0){ //If front and right bumper
        bot.turnLeft();
        bot.move(255);
        
      } else if (bumper_arr[3] == 0){ //If front and left bumper
          bot.turnRight();
          bot.move(255);
        
      } else{ //Just front bumper
          bot.rotateHead(maxAngle); //Rotate head left
          delay(100);
          uint16_t leftDist = bot.getDist(10);
          delay(400);
          bot.rotateHead(minAngle); //Rotate head right
          uint16_t rightDist = bot.getDist(10);
          
          if (rightDist >= leftDist) bot.turnRight();
          else bot.turnLeft();
          bot.move(255);
        }
        
    } else if (bumper_arr[1] == 0){ // If rear bumper
      
        if (bumper_arr[2] == 0) //If rear and right bumper
          bot.turnLeft();
        else if (bumper_arr[3] == 0) //If rear and left bumper
          bot.turnRight();
        switch (scoutPath()){
          case 'c':
            bot.move(255);
            break;
          case 'l':
            bot.turnLeft();
            bot.move(255);
            break;
          case 'r':
            bot.turnRight();
            bot.move(255);
            break;
          case 'n':
            escape();
            break;
          }
      } else if (bumper_arr[2] == 0){ // If right bumper
          bot.turnLeft();
          bot.move(255);
      } else if (bumper_arr[3] == 0){ // If left bumper
          bot.turnRight();
          bot.move(255);
      }
    } //End collision detection
  } 
}

char scoutPath(){
  // This function determines which path (left, right, center or None) is the optimal path to take
  char result = 'n';
  uint16_t resultDist;
  bot.rotateHead(head_straight); //Reset head to center
  uint16_t centerDist = bot.getDist(10);
  bot.rotateHead(maxAngle); //Rotate head left
  delay(100);
  uint16_t leftDist = bot.getDist(10);
  delay(400);
  bot.rotateHead(minAngle); //Rotate head right
  uint16_t rightDist = bot.getDist(10);
  
  if ((centerDist >= leftDist) && (centerDist >= rightDist)) {result = 'c'; resultDist = centerDist;}
  else if ((leftDist >= centerDist) && (leftDist >= rightDist)) {result = 'l'; resultDist = leftDist;}
  else if ((rightDist >= centerDist) && (rightDist >= leftDist)) {result = 'r'; resultDist = rightDist;}
  
  bot.rotateHead(head_straight); //Reset head to center
  
  if (result == 'n') return 'n';
  else if (resultDist >= clear_path_dist) return result;
  else return 'n';
}

void escape(){ // Temporary function. TODO: Resolve 'stuck' situations.
  while (true){
    bot.colourEye(255,0,0); //Red
    delay(500);
    bot.colourEye(0,0,0); //Off
    delay(500);
  }
}
