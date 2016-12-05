This is where the Arduino based library for the robot known as "Sir Henry", developed by Sirkits Pty Ltd., lives. The library can be used to perform basic actions with the robot, such as moving forward or rotating the head.

Getting the library to work:

1. Download this repository as a .zip file.
2. Copy the downloaded file to C:\Users\<Username>\Documents\Arduino\libraries\SirHenry and unzip it, all of the files should appear in a directory named "SirHenry".
3. Create a new sketch with the Arduino IDE (available from www.arduino.cc), and add the following three lines to the top of the file:


```
#include <Servo.h>
#include <NewPing.h>
#include <SirHenry.h>
```

These lines allow the compiler to use the library.
4. An instance of the robot object can now be created by using the line:

```
SirHenry bot;
```
5. You are now ready to start writing your own programs that can make the robot do just about anything. Your imagination is the limit.
6. Just in case this all seems a bit daunting, we have included two example programs in the library to give you a better idea of how things work.

#Consider reading through the library's .cpp and .h files to get an idea of how it all works, you might even want to write your own library in time!
