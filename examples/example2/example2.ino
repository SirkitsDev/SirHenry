#include <Servo.h>
#include <NewPing.h>
#include <SirHenry.h>
SirHenry bot;
void setup() {
  Serial.begin(115200);
}

void loop() {

  int dist = bot.getDist();
  if (dist < 10){
      bot.colourEye(255,0,0); //Red
  } else if ((dist >= 10)&&(dist<=30)) {
      bot.colourEye(255,255,0); //Yellow
  } else{
      bot.colourEye(0,255,0); //Green
  }

  if (dist < 5){ //If really close to an object
    bot.moveBackward(1);
    bot.turnRight();
    
  } else if (bot.front_bumper() == 0){ //If front bumper hits object not seen by 'eyes'
    bot.moveBackward(1);
    if (bot.rear_bumper() == 0){ // If object is behind robot
      bot.moveForeward(1);
    }
    bot.turnRight();
    
  } else{ // If no object in front of robot and no collision
    bot.moveForeward(1);
  }
  
}
