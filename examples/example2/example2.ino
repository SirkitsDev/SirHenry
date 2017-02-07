#include <Servo.h>
#include <NewPing.h>
#include <SirHenry.h>

/* Sir Henry example2.
 *  By Cobus Truter (Dec 2016)
 *  
 *  This example aims to provide basic autonomous navigaion.
 *  
 *  TODO: Explanation.
 */
 
// Servo rotation limitation variables. Adjust according to your own results from basic_testing.ino.
const uint8_t maxAngle = 85; // Maximum allowable angle of servo rotation
const int minAngle = -75; // Minimum allowable angle of servo rotation
const int head_straight = -10; // Head alignment offset. Adjust accordingly

const uint8_t closest_dist = 20; // Distance at which an object is deemed 'too close'
const uint8_t clear_path_dist = 25; // If there is no objects present within this distance, the path is deemed clear
uint16_t dist;

SirHenry bot(maxAngle,minAngle);

void setup() {
  Serial.begin(115200);
  bot.rotateHead(head_straight); // Reset head to center
  delay(250);
}

void loop() {
  
  dist = bot.getDist();

  // Adjusting colour of LED according to distance from object
  if (dist <= closest_dist){
      bot.colourEye(255,0,0); //Red
  } else if ((dist > closest_dist)&&(dist <= clear_path_dist)) {
      bot.colourEye(255,140,0); //Orange
  } else if ((dist > clear_path_dist)&&(dist <= 100)){
      bot.colourEye(255,255,0); //Yellow
  } else bot.colourEye(0,255,0); //Green
  
  if (dist < closest_dist){ //If too close to an object
   
    bot.stop();
    bot.rotateHead(-85); //Turn head right
    delay(250);
    dist = bot.getDist();

    if (dist > clear_path_dist){ // Check if path to the right is clear
      bot.rotateHead(head_straight); //Reset head
      bot.turnRight();
    } 
    else{
      bot.rotateHead(85); //Turn head left
      delay(400);
      dist = bot.getDist();
      
      if (dist > clear_path_dist){ // Check if path to the left is clear
        bot.rotateHead(head_straight); //Reset head
        bot.turnLeft();
      } else{ // If front, left and right is obstructed

        while (true){
          bot.rotateHead(head_straight); //Reset head
          bot.moveBackward(1); // Reverse
          bot.rotateHead(85); //Turn head left
          delay(250);
          dist = bot.getDist(); 

          if (dist > clear_path_dist){ // Check if path to the left is clear
            bot.rotateHead(head_straight); //Reset head
            bot.turnLeft();
            break;
          } else{
            bot.rotateHead(-85); //Turn head right
            delay(250);
            dist = bot.getDist();
            if (dist > clear_path_dist){ // Check if path to the right is clear
              bot.rotateHead(head_straight); //Reset head
              bot.turnRight();
              break;
            }
          }
        }
      }
    }
    // TODO: Decent bumper interaction
  } else if (bot.front_bumper() == 0){ //If front bumper hits object not seen by 'eyes'
    bot.moveBackward(1);
    if (bot.rear_bumper() == 0){ // If object is behind robot
      bot.moveForward(1);
    }
    bot.turnRight();
    
  } else{ // If no object in front of robot and no collision
     bot.move(255);
  }
}
