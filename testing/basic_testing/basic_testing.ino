#include <Servo.h>
#include <NewPing.h>
#include <SirHenry.h>

/* Basic testing of Sir Henry.
 *  By Cobus Truter (Feb 2017)
 *  
 *  This sketch provides a basic framework to test and debug the hardware connections of Sir Henry. 
 *  
 *  
 *  TODO: Explination
 *  
 */

SirHenry bot(85,-75);
String MENU_MSG = "\n  ------ MAIN MENU ------ \nChoose a test to run: \n0 - Head alignment \n1 - Distance sensor test \n2 - Bumper switch test \n3 - RGB LED test \n4 - Movement test\n\nEnter 'x' to return to the Main Menu at any time. \nEnter '!' at any time to exit program.";
String dataString = "";
char colourStates[7] = {'r', 'g', 'b', 'y', 'p', 'o', 'w'};

void setup() {
  Serial.begin(115200);
  bot.colourEye(255,255,255); // White
  Serial.println(MENU_MSG);
}

void loop() {
  if(Serial.available() > 0){
    char choice = Serial.read(); // Read single char

    switch (choice){
      case '0': 
        // Head alignment test
        Serial.println("\n  ------ HEAD ALIGNMENT ------");
        dataString = "";
        while (true){
          
          if (Serial.available() > 0){
            dataString = Serial.readString();
            Serial.println(dataString);
            dataString.trim();
          }
          
          if (dataString == "x") {
            Serial.println(MENU_MSG);
            break;
          } else if (dataString == "!") exit_program();
          else if (isInt(dataString)){
            
              int angle = dataString.toInt();
              Serial.print("Angle received: ");
              Serial.println(angle);
  
              // Make sure angle is between -85 and 85 degrees
              if (angle > 85) angle = 85;
              if (angle < -85) angle = -85;
              Serial.print("Setting servo to: ");
              Serial.println(angle);
              bot.rotateHead(angle);
              dataString = "";
          }
        }
        break;
        
      case '1':
        // Ultrasonic distance meter test
        uint16_t totalDist, shortestDist, longestDist, avgDist;
        Serial.println("\n  ------ DISTANCE SENSOR TEST ------");
        Serial.println("\nEnter 's' to start the test.\nMake sure the sensor is stationary and facing a large, flat surface!");
        while (true){
          if (Serial.available() > 0){
            
            char inputChar = Serial.read(); // Reads only first character
            if (inputChar == 's'){
              uint16_t initialDist = bot.getDist();
              totalDist = 0; avgDist = 0;
              shortestDist = initialDist; longestDist = initialDist;
              for (uint8_t i = 0; i < 50; i++){
                uint16_t dist = bot.getDist();
                Serial.print(dist);
                Serial.println("cm.");
                totalDist += dist;
                if (dist < shortestDist) shortestDist = dist;
                else if(dist > longestDist) longestDist = dist;
              }
              avgDist = totalDist/50.0;
              Serial.print("\nTest finished. \nAverage distance:  ");
              Serial.print(avgDist);
              Serial.println("cm.");
              Serial.print("Max overshoot:  ");
              Serial.print(longestDist - avgDist);
              Serial.println("cm.");
              Serial.print("Max undershoot:  ");
              Serial.print(avgDist - shortestDist);
              Serial.println("cm.");
              Serial.println("\nEnter 's' to run the test again.");
              
            } else if (inputChar == 'x'){
              Serial.println(MENU_MSG);
              break;
            } else if (inputChar == '!') exit_program();
          }
        }
        break;
      case '2':
        // Bumper switch test
        Serial.println("\n  ------ BUMPER SWITCH TEST ------");
        Serial.println("\nPress the bumper switches individually - the result will be displayed.");
        uint8_t bumper_arr[4];
        while (true){
          if (Serial.available() > 0){
            char inputChar = Serial.read();
            if (inputChar == 'x'){
              Serial.println(MENU_MSG);
              break;
            } else if (inputChar == '!') exit_program();
          }
          bot.detect(bumper_arr);
          if(bumper_arr[0] == 0) Serial.println("FRONT bumper pressed.");
          if(bumper_arr[1] == 0) Serial.println("REAR bumper pressed.");
          if(bumper_arr[2] == 0) Serial.println("RIGHT bumper pressed.");
          if(bumper_arr[3] == 0) Serial.println("LEFT bumper pressed.");
        }
        break;
      case '3':
        // RGB LED test
        Serial.println("\n  ------ BUMPER SWITCH TEST ------");
        Serial.println("\nPress 'c' to switch to the next colour. \nThe LED's colour should be the same as the displayed colour name.");
        //Serial.println("\nWHITE - (255,255,255)");
        bot.colourEye(255,255,255);
        uint8_t pos; pos = 0;
        while (true){
          if (Serial.available() > 0){
            char inputChar = Serial.read();
            if (inputChar == 'x'){
              Serial.println(MENU_MSG);
              break;
            } else if (inputChar == '!') exit_program();
            else if (inputChar == 'c'){
              switch(colourStates[pos]){
                case 'r':
                  Serial.println("RED - (255,0,0)");
                  bot.colourEye(255,0,0);
                  pos++;
                  break;
                case 'g':
                  Serial.println("GREEN - (0,255,0)");
                  bot.colourEye(0,255,0);
                  pos++;
                  break;
                case 'b':
                  Serial.println("BLUE - (0,0,255)");
                  bot.colourEye(0,0,255);
                  pos++;
                  break;
                case 'y':
                  Serial.println("YELLOW - (255,128,0)");
                  bot.colourEye(255,128,0);
                  pos++;
                  break;
                case 'p':
                  Serial.println("BRIGHT PURPLE - (255,0,128)");
                  bot.colourEye(255,0,128);
                  pos++;
                  break;
                case 'w':
                  Serial.println("WHITE - (255,255,255)");
                  bot.colourEye(255,255,255);
                  pos = 0;
                  break;
                case 'o':
                  Serial.println("OFF - (0,0,0)");
                  bot.colourEye(0,0,0);
                  pos ++;
                  break;
              }
            }
          }
        }
        break;
      case '4':
        // Movement test
        Serial.println("\n  ------ MOVEMENT TEST ------");
        Serial.println("\n'f' - Move forward\n'b' - Move backward\n'l' - Turn left\n'r' - Turn right");
        while (true){
          if (Serial.available() > 0){
            char inputChar = Serial.read();
            if (inputChar == 'x'){
              Serial.println(MENU_MSG);
              break;
            } else if (inputChar == '!') exit_program();
            else if (inputChar == 'f') bot.moveForward(1);
            else if (inputChar == 'b') bot.moveBackward(1);
            else if (inputChar == 'l') bot.turnLeft(255,255,1000);
            else if (inputChar == 'r') bot.turnRight(255,255,1000);
          }
        }
        break;
      case '!':
        Serial.println("main exit");
        exit_program();
        break;
      default: 
        break;
    }
  }
}

boolean isInt(String str){
  if(str.charAt(0) == '\0') return false;
  if(str.charAt(0) == '+' || str.charAt(0) == '-') str = &str[1];
  
  for(int i = 0; i < str.length(); i++){
    // Check each char to see if it is a digit
    if(str.charAt(i) < '0' || str.charAt(i) > '9') return false;
  } 
  return true;
}

void exit_program(){
  Serial.println("Exiting...");
  bot.colourEye(0,0,0);
  delay(100);
  exit(0);
}

