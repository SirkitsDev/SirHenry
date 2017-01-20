#include <Servo.h>
#include <NewPing.h>
#include <SirHenry.h>

/* Sir Henry example5.
 *  By Cobus Truter (20 Jan 2017)
 *  
 *  Demo code for 21 January 2017
 *  
 */
 
SirHenry bot;

int head_straight = 0; // Head alignment offset
int repeat_right = 0;
int repeat_left = 0;
long rand_num;
uint8_t bumper_arr[4];

void setup() {
  Serial.begin(115200); // Bluetooth module setup to communicate at
  bot.rotateHead(head_straight); // Reset head 
  bot.colourEye(0,0,0); //White
  randomSeed(analogRead(0));
}

void loop() {
  
  if (bot.left_bumper() == 0 || bot.front_bumper() == 0 ||  bot.rear_bumper() == 0 ||  bot.right_bumper() == 0){ // IF front or back bumper switch is pressed
    bot.colourEye(255,0,0); //Red
    delay(250);
  }

  int dist = bot.getDist();
  if (dist < 20){
      bot.colourEye(40,205,240); //Turquiose blue
  } else if ((dist >= 20)&&(dist<=45)) {
      bot.colourEye(240,40,255); //Purpleish
  } else if ((dist >= 45)&&(dist<=75)) {
      bot.colourEye(140,255,40); //Yellowish-Green
  } else{
     bot.colourEye(0,0,0); //OFF
  }

  if (dist < 10){
    
    bot.rotateHead(head_straight); // Reset head 
    delay(150);
    if (dist < 10){
          rand_num = random(100);
      if ((rand_num > 49 || repeat_left >= 2) && repeat_right < 2){
        repeat_right++;
        repeat_left = 0;
        bot.rotateHead(-40); //Turn head right
        delay(150);
        dist = bot.getDist();
        if (dist < 10){
            bot.rotateHead(-75); //Turn head far right
            delay(150);
        }
      } else{
        repeat_left++;
        repeat_right = 0;
        bot.rotateHead(40); //Turn head left
        delay(150);
        dist = bot.getDist();
        if (dist < 10){
            bot.rotateHead(75); //Turn head far left
            delay(150);
        }
      }
    }//End first dist sense
  }//End main IF
} // End main
