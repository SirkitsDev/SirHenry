#include <Servo.h>
#include <NewPing.h>
#include <SirHenry.h>
SirHenry bot;

/* First basic test of components.
 *  By JD Retief (Dec 2016).
 *  
 *  TODO - Replaced by basic_testing.ino
 */

void setup() {
  Serial.begin(115200);
}

void loop() {
  int dist = 0;
  
  bot.colourEye(232,32,32);
  bot.moveForward(1);
  bot.moveBackward(1);
  bot.colourEye(191,64,191);
  bot.turnLeft();
  bot.turnRight();
  bot.rotateHead(-85);
  bot.colourEye(0,255,128);
  delay(500);
  bot.rotateHead(0);
  delay(500);
  bot.rotateHead(85);
  bot.colourEye(64,0,255);
  delay(500);
  dist = bot.getDist();
  Serial.println(dist);
  delay(200);
}
