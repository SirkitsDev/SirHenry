#include <Servo.h>
#include <NewPing.h>
#include <SirHenry.h>

/* Sir Henry example3.
 *  By Cobus Truter (Dec 2016)
 *  
 *  This example aims to provide basic autonomous navigaion.
 *  Updated and slightly improved version of example2.ino.
 *  
 *  TODO: Explanation.
 */
 
SirHenry bot;

int closest_dist = 23;
int head_straight = 10; // Head alignment offset. Adjust accordingly
int clear_path_dist = 30;

void setup() {
  Serial.begin(115200);
  bot.rotateHead(head_straight); // Reset head
}

void loop() {
  
  int dist = bot.getDist();
  if (dist < closest_dist){
      bot.colourEye(255,0,0); //Red
  } else if ((dist >= closest_dist)&&(dist<=60)) {
      bot.colourEye(255,255,0); //Yellow
  } else{
      bot.colourEye(0,255,0); //Green
  }

  if (dist < closest_dist){ //If really close to an object
    
    bot.stop();
    bot.rotateHead(-85); //Turn head right
    delay(100);
    dist = bot.getDist();

    if (dist > clear_path_dist){ // Check if path to the right is clear
      bot.rotateHead(head_straight); //Reset head
      bot.colourEye(0,255,0); //Green
      bot.turnRight();
    } 
    else{
      bot.rotateHead(85); //Turn head left
      delay(250);
      
      if (dist > clear_path_dist){ // Check if path to the left is clear
        bot.rotateHead(head_straight); //Reset head
        bot.colourEye(0,255,0); //Green
        bot.turnLeft();
      } else{ // If front, left and right is obstructed

        while (true){
          bot.moveBackward(1); // Reverse
          dist = bot.getDist(); 

          if (dist > clear_path_dist){ // Check if path to the left is clear
            bot.rotateHead(head_straight); //Reset head
            bot.colourEye(0,255,0); //Green
            bot.turnLeft();
            break;
          } else{
            bot.rotateHead(-85); //Turn head right
            delay(100);
            dist = bot.getDist();
            if (dist > clear_path_dist){ // Check if path to the right is clear
              bot.rotateHead(head_straight); //Reset head
              bot.colourEye(0,255,0); //Green
              bot.turnRight();
              break;
            }
          }
        }
      }
    }
    
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
