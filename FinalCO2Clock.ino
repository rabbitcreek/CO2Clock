#include <Wire.h>
#include <NDIR_I2C.h>
#include "Adafruit_EPD.h"
#include <Servo.h>
//Section for setup of EPD
#define EPD_RESET   -1 // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY    -1 // can set to -1 to not use a pin (will wait a fixed delay)
 #if defined (__AVR_ATmega32U4__) || defined(ARDUINO_SAMD_FEATHER_M0) || defined(ARDUINO_FEATHER_M4) || defined (__AVR_ATmega328P__) || defined(ARDUINO_NRF52840_FEATHER)
  #define SD_CS       5
  #define SRAM_CS     6
  #define EPD_CS      9
  #define EPD_DC      10  
#endif
int pos = 65;
//Your connection pin to the servo on the board
const int servoPin = 11;
int oldPos = 75;


Servo myservo;

/* Uncomment the following line if you are using 2.13" tricolor EPD */
Adafruit_IL0373 epd(212, 104 ,EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);
NDIR_I2C mySensor(0x4D); //Adaptor's I2C address (7-bit, default: 0x4D)

void setup()
{
  
   Serial.begin(9600);
   myservo.attach(11); 
  
  epd.begin();
  epd.setTextWrap(true);
  epd.setTextSize(6);
    if (mySensor.begin()) {
        Serial.println("Wait 10 seconds for sensor initialization...");
        delay(10000);
    } else {
        Serial.println("ERROR: Failed to connect to the sensor.");
        while(1);
    }
   
}

void loop() {
  

    if (mySensor.measure()) {
        Serial.print("CO2 Concentration is ");
        Serial.print(mySensor.ppm);
        Serial.println("ppm");
        pos = map( mySensor.ppm,400, 3000, 55,125);
        pos = constrain( pos, 55, 125);
        Serial.println(pos);
    
  if(pos>oldPos)
  {
    for(int i=oldPos; i<=pos; i++)
    {
      myservo.write(i); //turn servo by 1 degrees
      delay(50);        //delay for smoothness
    }
  }

  if(pos<oldPos)
  {
    for(int i=oldPos; i>=pos; i--)
    {
      myservo.write(i); //turn servo by 1 degrees
      delay(50);        //delay for smoothness
    }
  }

  oldPos = pos;
 
  
 
 
 //myservo.write(pos);
     
  epd.clearBuffer();
  epd.setCursor(10, 20);
  epd.setTextSize(8);
  epd.setTextColor(EPD_BLACK);
  epd.print(mySensor.ppm);
  epd.setTextSize(2);
  epd.setCursor(80, 80);
  epd.setTextColor(EPD_RED);
  epd.print("PPM");
  epd.display();
  
  

  delay(3*1000);
 
    } else {
        Serial.println("Sensor communication error.");
    }

    delay(1000);
    
}
