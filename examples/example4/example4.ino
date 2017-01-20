#include <Servo.h>
#include <NewPing.h>
#include <SirHenry.h>

/* Sir Henry example4.
 *  By Cobus Truter (8 Jan 2017)
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
 *  
 *  Up: '1'
 *  Down: '0'
 *  Left: 'a'
 *  Right 'b'
 *  
 *  Square: '3'
 *  Triangle: '4'
 *  X: '5'
 *  Circle: '6'
 *  
 */

SirHenry bot;

char data = 0; 
int head_straight = 11; // Head alignment offset

void setup() {
  Serial.begin(115200); // Bluetooth module setup to communicate at
  bot.rotateHead(head_straight); // Reset head 
}

void loop() {
 if(Serial.available() > 0) 
   {
      data = Serial.read();        // Read the data (1st character only)
      Serial.print(data);          // Print data value for debugging purposes
      Serial.print("\n");

      // Controlling robot depending on data value
      if(data == '1') // If data is '1', move bot forward.
        bot.moveForward(1);
      else if(data == '0') // If data is '0', move bot backwards, etc.
        bot.moveBackward(1);
      else if(data == 'a')
        bot.turnLeft();
      else if(data =='b')
        bot.turnRight();

      else if (data == '3')
        bot.rotateHead(85); //Turn head left
      else if (data == '4')
        bot.colourEye(255,0,0); //Red
      else if (data == '5')
        bot.rotateHead(-85); //Turn head right
      else if (data == '6')
        bot.colourEye(0,255,0); //Green
      else{
        Serial.print("Unknown command!");
      }
        
   }
}
