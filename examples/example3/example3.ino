#include <Servo.h>
#include <NewPing.h>
#include <SirHenry.h>

/* Sir Henry example3.
 *  By Cobus Truter (Feb 2016)
 *  
 *  This example aims to provide more 'thorough' autonomous navigaion.
 *  
 *  TODO: Explanation.
 */

// Servo rotation limitation variables. Adjust according to your own results from basic_testing.ino.
const uint8_t maxAngle = 85; // Maximum allowable angle of servo rotation
const int minAngle = -75; // Minimum allowable angle of servo rotation
const int head_straight = 0; // Head alignment offset. Adjust accordingly

const uint8_t closest_dist = 30; // Distance at which an object is deemed 'too close'
const uint8_t clear_path_dist = 30; // If there is no objects present within this distance, the path is deemed clear
uint16_t dist;
char bestSide;
uint8_t bumper_arr[4];

SirHenry bot(maxAngle,minAngle);

void setup() {
  Serial.begin(115200);
  bot.rotateHead(head_straight); // Reset head to center
  delay(250);
  bot.move(255);
}

void loop() {
  
  dist = bot.getDist();
  for(int i = 0; i < 5; i++){ // To account for 0cm results caused by sound absorption
    if (dist == 0) break;
    dist = bot.getDist();
  }
  bot.detect(bumper_arr);

  // Adjusting colour of LED according to distance from object
  if (dist <= closest_dist){
      bot.colourEye(255,0,0); //Red
  } else if ((dist > closest_dist)&&(dist <= clear_path_dist)) {
      bot.colourEye(255,140,0); //Orange
  } else if ((dist > clear_path_dist)&&(dist <= 100)){
      bot.colourEye(255,255,0); //Yellow
  } else bot.colourEye(0,255,0); //Green
  
  if(bumper_arr[0] == 0 || bumper_arr[1] == 0 || bumper_arr[2] == 0 || bumper_arr[3] == 0){ // If a collision is detected
    bot.stop();
    Serial.println("Collision");
    
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

  if (dist < closest_dist){ //If too close to an object
    bot.stop();
    Serial.println("Close to object");
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
  }
} //End loop

char scoutPath(){
  // This function determines which path (left, right, center or None) is the optimal path to take
  char result = 'n';
  uint16_t resultDist;
  
  uint16_t bestSideDist;
  bot.rotateHead(head_straight); //Reset head to center
  uint16_t centerDist = bot.getDist(10);
  bot.rotateHead(maxAngle); //Rotate head left
  delay(100);
  uint16_t leftDist = bot.getDist(10);
  delay(400);
  bot.rotateHead(minAngle); //Rotate head right
  uint16_t rightDist = bot.getDist(10);
  
  if ((rightDist >= centerDist) && (rightDist >= leftDist)) {result = 'r'; resultDist = rightDist;}
  else if ((leftDist >= centerDist) && (leftDist >= rightDist)) {result = 'l'; resultDist = leftDist;}
  else if ((centerDist >= leftDist) && (centerDist >= rightDist)) {result = 'c'; resultDist = centerDist;}

  if (leftDist >= rightDist) bestSide = 'l';
  else bestSide = 'r';
  
  bot.rotateHead(head_straight); //Reset head to center
  
  if (result == 'n') return 'n';
  else if (resultDist >= clear_path_dist) return result;
  else return 'n';
}

void escape(){ // Temporary function. TODO: Resolve 'stuck' situations.

    bot.colourEye(255,0,0); //Red
    delay(500);
    bot.colourEye(0,0,0); //Off
    delay(500);
    bot.moveBackward(2);
    if (bestSide == 'l') bot.turnLeft(255,255,1500);
    else bot.turnRight(255,255,1500);
}

