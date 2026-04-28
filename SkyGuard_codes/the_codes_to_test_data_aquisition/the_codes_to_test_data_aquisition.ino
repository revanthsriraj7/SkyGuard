#include <BluetoothSerial.h>
BluetoothSerial serialBT;

void setup() 
{
Serial.begin(9600);
serialBT.begin("SkyGuard");
}

void loop() 
{
serialBT.print("23.017691");  
serialBT.print("|");
serialBT.println("72.567448");  
delay(5000);
serialBT.print("12.017691");  
serialBT.print("|");
serialBT.println("72.567448");  
delay(5000);
serialBT.print("13.017691");  
serialBT.print("|");
serialBT.println("43.567448");  
delay(5000);
serialBT.print("23.017691");  
serialBT.print("|");
serialBT.println("65.567448");  
delay(5000);
serialBT.print("83.017691");  
serialBT.print("|");
serialBT.println("72.567448");  
delay(5000);
}
