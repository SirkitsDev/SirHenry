#include <Servo.h>
#include <NewPing.h>
#include <SirHenry.h>
SirHenry bot;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int dist = 0;
  
  bot.colourEye(232,32,32);
  bot.moveForeward(1);
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
