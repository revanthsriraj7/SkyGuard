#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <FastLED.h>

#define LED_PIN     25
#define LED_COUNT   8

CRGB leds[LED_COUNT];

Servo servo;
Servo servor;
Servo servol;

#define ss 5
#define rst 14
#define dio0 2

int pos = 55;


void setup()
{
  Serial.begin(9600);
  Serial.println("Attachment");
  pinMode(26, INPUT);
  LoRa.setPins(ss, rst, dio0);
  LoRa.begin(433E6);
  
  servo.attach(33); 
  servol.attach(32);
  servor.attach(13); 
  
  servo.write(pos);
  servor.write(180);
  servol.write(0);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(250); 
  white();
}

void loop()
{
  int value1 = digitalRead(26);
  int packetSize = LoRa.parsePacket();
  
  if (packetSize)
  {
    String received = LoRa.readString();
    Serial.println(received);
    
   if (received == "up") 
  {
    Serial.println("Servo Up");
    pos = pos + 5;
    servo.write(pos);
    if(pos >= 90)
     {
      pos = 90;
     }
  }
  else if (received == "down") 
  {
    Serial.println("Servo Down");
    pos = pos - 5;
    servo.write(pos);
     
     if(pos <= 0)
     {
      pos = 0;
     }
  }
   
   
   else if (received == "hold") 
  {
   servor.write(70); 
   servol.write(110); 
  }

   else if (received == "leave") 
  {
   servor.write(180);
   servol.write(0); 
  }
  
  }
 if (value1 == HIGH) 
  {
    LoRa.beginPacket();
    LoRa.print("13.117474,");
    LoRa.print("77.655568");
    Serial.println("cordinates sent!");
    LoRa.endPacket();
    alert();
  }
 else
 {
  white(); 
 }
  
}

void white()
{
  leds[0] = CRGB(225, 255, 255); //wait
  leds[1] = CRGB(225, 255, 255);
  leds[2] = CRGB(225, 255, 255);
  leds[3] = CRGB(225, 255, 255);
  leds[4] = CRGB(225, 255, 255);
  leds[5] = CRGB(225, 255, 255);
  leds[6] = CRGB(225, 255, 255);
  leds[7] = CRGB(225, 255, 255);
  FastLED.show();  
  delay(50);
}

void alert()
{
  leds[0] = CRGB(255, 0, 0); //red
  leds[1] = CRGB(255, 0, 0);
  leds[2] = CRGB(255, 0, 0);
  leds[3] = CRGB(255, 0, 0);
  leds[4] = CRGB(255, 0, 0);
  leds[5] = CRGB(255, 0, 0);
  leds[6] = CRGB(255, 0, 0);
  leds[7] = CRGB(255, 0, 0);
  FastLED.show(); 
  delay(50); 
}
