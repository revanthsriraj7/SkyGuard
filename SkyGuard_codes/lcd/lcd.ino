 #include <Wire.h>           
 #include <LiquidCrystal_I2C.h>    
 LiquidCrystal_I2C lcd(0x27,16,2);   

 void setup()  
  { 
     
   lcd.begin();    
   lcd.backlight();  
   
  }  
 void loop()   
  {  
   lcd.setCursor(0,0);  
   lcd.print("NEWS");  
   lcd.setCursor(0,1);  
   lcd.print("jgseifgh");  
   delay(5000);
   lcd.clear();
  }  
