#include<s3servo.h>
s3servo servo1;
s3servo servo2;

void setup() 
{
servo1.attach(9);  
servo2.attach(10); 
}

void loop() 
{
 servo1.write(180); 
 servo2.write(0); 
}
