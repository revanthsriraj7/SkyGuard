#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <BluetoothSerial.h>
BluetoothSerial serialBT;

#define ss 5
#define rst 14
#define dio0 2
String Storage;
String received;
String data1;
String data2;


void setup() 
{
  Serial.begin(9600);
  serialBT.begin("SkyGuard");
  pinMode(4, OUTPUT);
  
  LoRa.setPins(ss, rst, dio0);
  LoRa.begin(433E6); 
}

void loop() 
{
  int value = HIGH;
  int value1 = HIGH;
  int value2 = HIGH;
  int value3 = HIGH;
  
  LoRa.beginPacket();
  LoRa.print("down");
  LoRa.endPacket();
  
  if (value == LOW) 
  {
    LoRa.beginPacket();
    LoRa.print("up");
    LoRa.endPacket();
  }
  else if (value1 == LOW) 
  {
    LoRa.beginPacket();
    LoRa.print("down");
    LoRa.endPacket();
  }
  else if (value2 == LOW) 
  {
    LoRa.beginPacket();
    LoRa.print("hold");
    LoRa.endPacket();
  }
  else if (value3 == LOW) 
  {
    LoRa.beginPacket();
    LoRa.print("leave");
    LoRa.endPacket();
  }

  int packetSize = LoRa.parsePacket(); //to recive data
  if (packetSize) 
  {
    received = LoRa.readString();
    digitalWrite(4, HIGH);
    delay(500);
    digitalWrite(4, LOW);
    delay(500);
    int splitIndex = received.indexOf(','); //we will send logi and lati with , in between 
    if (splitIndex != -1)
      {
        data1 = received.substring(0, splitIndex);  //longi
        data2 = received.substring(splitIndex + 1); //lati
      }
    }
    serialBT.print(data1);  
    serialBT.print("|");
    serialBT.println(data2); 
    serialBT.print("|"); 
    delay(500);
  }
