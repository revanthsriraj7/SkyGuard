#include<ESP32Servo.h>
#include <BluetoothSerial.h>

BluetoothSerial serialBT;
Servo left;
Servo right;

void setup() 
{
  Serial.begin(9600);
  serialBT.begin("Mech_Sphere");
  left.attach(12);
  right.attach(27);
}

void loop()
{
 if (serialBT.available()) 
 {
    String command = serialBT.readStringUntil('\n');
    command.trim(); 
    
    if (command == "forward")
    {
     left.write(98);
     right.write(82);
    } 

    else if (command == "backward")
    {
     left.write(82);
     right.write(98);
    } 

    else if (command == "lefti")
    {
     left.write(98);
     right.write(98);
    } 

    else if (command == "righti")
    {
     left.write(82);
     right.write(82);
    } 
    
    else if (command == "stop")
    {
     left.write(90);
     right.write(90);
    } 
 }

}
