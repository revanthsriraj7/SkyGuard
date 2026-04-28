#include <ESP32Servo.h>
Servo servo;   // the set angle at the very end looking stright down is 0 and otimum angle is form 0 to 60 in between 90max
Servo servor;
Servo servol;

void setup() 
{
servo.attach(33); 
servor.attach(13);
servol.attach(32);
 
servo.write(0);

servor.write(180);
servol.write(0);
}

void loop() 
{
 servor.write(70); 
 servol.write(110); 
 delay(2000);
 //servor.write(180);
 //servol.write(0);
 //delay(2000);

}
