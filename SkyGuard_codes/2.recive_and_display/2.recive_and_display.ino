#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <BluetoothSerial.h>

BluetoothSerial serialBT;

#define ss 5
#define rst 14
#define dio0 2

String received;
String data1;
String data2;

void setup() 
{
  Serial.begin(9600);
  serialBT.begin("SkyGuard");
  pinMode(4, OUTPUT);
  
  LoRa.setPins(ss, rst, dio0);
  
  if (!LoRa.begin(433E6)) 
  {
    Serial.println("LoRa initialization failed!");
    while (1);
  }
  Serial.println("LoRa initialized");
}

void loop() 
{
  // Check for Bluetooth commands
  if (serialBT.available()) {
    String command = serialBT.readStringUntil('\n');
    command.trim(); // Remove any extra whitespace or newline characters
    
    if (command == "up")
    {
      LoRa.beginPacket();
      LoRa.print("up");
      LoRa.endPacket();
      Serial.println("Camera Up");
    } 
    else if (command == "down")
    {
      LoRa.beginPacket();
      LoRa.print("down");
      LoRa.endPacket();
      Serial.println("Camera Down");
    } 

    
    else if (command == "hold")
    {
      LoRa.beginPacket();
      LoRa.print("hold");
      LoRa.endPacket();
      Serial.println("Gripper lock");
    } 
    else if (command == "leave") 
    {
      LoRa.beginPacket();
      LoRa.print("leave");
      LoRa.endPacket();
      Serial.println("gripper unlock");
    } 

    else if (command == "remove") 
    {
      LoRa.beginPacket();
      LoRa.print("leave");
      LoRa.endPacket();
      Serial.println("Mechsphere leave");
    } 

    else if (command == "attach") 
    {
      LoRa.beginPacket();
      LoRa.print("leave");
      LoRa.endPacket();
      Serial.println("Mechsphere lock");
    } 
  }

  // Receive data from LoRa
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {
    received = LoRa.readString();
    digitalWrite(4, HIGH);
    delay(500);
    digitalWrite(4, LOW);
    delay(500);

    int splitIndex = received.indexOf(','); // Assume longitude and latitude are separated by a comma
    if (splitIndex != -1)
    {
      data1 = received.substring(0, splitIndex);  // Longitude
      data2 = received.substring(splitIndex + 1); // Latitude
    }

    // Send received data back via Bluetooth
    serialBT.print(data1);  
    serialBT.print("|");
    serialBT.println(data2); 
    serialBT.print("|");
    delay(500);
  }
}
