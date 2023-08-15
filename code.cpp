#include "SPI.h"
#include "TFT_22_ILI9225.h"

// Include font definition files
// NOTE: These files may not have all characters defined! Check the GFXfont def
// params 3 + 4, e.g. 0x20 = 32 = space to 0x7E = 126 = ~

#include <../fonts/FreeSans12pt7b.h>
#include <../fonts/FreeSans24pt7b.h>
#if defined (ARDUINO_ARCH_STM32F1)
#define TFT_RST PA1
#define TFT_RS  PA2
#define TFT_CS  PA0 // SS
#define TFT_SDI PA7 // MOSI
#define TFT_CLK PA5 // SCK
#define TFT_LED 0 // 0 if wired to +5V directly
#elif defined(ESP8266)
#define TFT_RST 4   // D2
#define TFT_RS  5   // D1
#define TFT_CLK 14  // D5 SCK
//#define TFT_SDO 12  // D6 MISO
#define TFT_SDI 13  // D7 MOSI
#define TFT_CS  15  // D8 SS
#define TFT_LED 2   // D4     set 0 if wired to +5V directly -> D3=0 is not possible !!
#elif defined(ESP32)
#define TFT_RST 26  // IO 26
#define TFT_RS  25  // IO 25
#define TFT_CLK 14  // HSPI-SCK
//#define TFT_SDO 12  // HSPI-MISO
#define TFT_SDI 13  // HSPI-MOSI
#define TFT_CS  15  // HSPI-SS0
#define TFT_LED 0   // 0 if wired to +5V directly
SPIClass hspi(HSPI);
#else
#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 3   // 0 if wired to +5V directly
#endif
#define TFT_BRIGHTNESS 200 // Initial brightness of TFT backlight (optional)
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);
int16_t x=0, y=0, w, h;

const int redPin = 6;
const int greenPin = 5;
const int bluePin = 3;
const int buttonPin = 7;
int analogPin = A1; // LDR connected to analog pin A1
int ledBrightness = 0;
int redValue = 0;
int greenValue = 0;
int blueValue = 0;
int buttonState = 0;
int lastButtonState = 0;
int pressCount =0;
float fc = 0;
float lux = 0;
float sensorValue = 0;
float Intensity  = 0;
void setup() {
  // put your setup code here, to run once:
 pinMode( A4, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // use internal pull-up resistor
  Serial.begin(9600);
  #if defined(ESP32)
  hspi.begin();
  tft.begin(hspi);
#else
  tft.begin();
#endif
}

void loop() {
  // put your main code here, to run repeatedly:
    buttonState = digitalRead(buttonPin);
    
      Serial.print(pressCount );
       ledBrightness = map(sensorValue, 0, 1023, 0, 254); // map the LDR value to a brightness value for the LED
       if (buttonState != lastButtonState) {
    
    if (buttonState == HIGH) {
      pressCount++;
      
      if (pressCount == 1) {
        
        
        analogWrite(redPin, ledBrightness);
        analogWrite(greenPin, ledBrightness);
        analogWrite(bluePin, ledBrightness);
        tft.drawText(50, 50, "Power_off",COLOR_BLACK);  
        tft.setBackgroundColor(COLOR_BLACK);
        tft.setFont(Terminal12x16);
        tft.drawText(70, 10, "white",COLOR_WHITE);
      delay(1000);        
       sensorValue = analogRead(analogPin);  
  
     fc = ((-0.476*sensorValue) + 324)*100 ;
    
     lux= fc/10.764;
     Intensity =  lux*((2*2)*1*3.14);
      Serial.println(fc);
      tft.drawText(50, 90, "LDR",COLOR_WHITE);
      tft.drawText(50, 110, String( sensorValue ), COLOR_WHITE); 
      tft.drawText(50, 50, "fc",COLOR_WHITE);
      tft.drawText(50, 70, String(fc), COLOR_WHITE); 
      tft.drawText(50, 130, "Lux",COLOR_WHITE);
      tft.drawText(50, 145, String(lux), COLOR_WHITE);
      tft.drawText(50, 165, "Intensity",COLOR_WHITE);
      tft.drawText(50, 185, String(Intensity), COLOR_WHITE);

      } else if (pressCount == 2) {
      analogWrite(redPin, ledBrightness);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
      tft.setFont(Terminal12x16);
      tft.drawText(70, 10, "white",COLOR_BLACK);
      tft.drawText(50, 50, "fc",COLOR_BLACK);
      tft.drawText(50, 70, String(fc), COLOR_BLACK);
      tft.drawText(50, 90, "LDR",COLOR_BLACK);
      tft.drawText(50, 110, String( sensorValue), COLOR_BLACK); 
      tft.drawText(50, 130, "Lux",COLOR_BLACK);
      tft.drawText(50, 145, String(lux), COLOR_BLACK);
      tft.drawText(50, 165, "Intensity",COLOR_BLACK);
      tft.drawText(50, 185, String(Intensity), COLOR_BLACK);
      tft.setBackgroundColor(COLOR_BLACK);
      tft.drawText(70, 10, "Red",COLOR_RED);
      tft.drawText(5, 30, "WL = 639.458 nm",COLOR_RED);
      delay(2000); 
      sensorValue = analogRead(analogPin);  

    fc = ((-0.476*sensorValue) + 324)*100 ;
    
      lux= fc/10.764;
      Intensity =  lux*((2*2)*1*3.14);
   
      Serial.println(fc);
      tft.drawText(50, 90, "LDR",COLOR_WHITE);
      tft.drawText(50, 110, String( sensorValue), COLOR_WHITE); 
      tft.drawText(50, 50, "fc",COLOR_WHITE);
      tft.drawText(50, 70, String(fc), COLOR_WHITE); 
      tft.drawText(50, 130, "Lux",COLOR_WHITE);
      tft.drawText(50, 145, String(lux), COLOR_WHITE);
       tft.drawText(50, 165, "Intensity",COLOR_WHITE);
       tft.drawText(50, 185, String(Intensity), COLOR_WHITE);

        
      } else if (pressCount == 3) {
        analogWrite(redPin, 0);
        analogWrite(greenPin, ledBrightness);
        analogWrite(bluePin, 0);
        tft.setFont(Terminal12x16);
        tft.drawText(70, 10, "Red",COLOR_BLACK);
        tft.drawText(50, 50, "fc",COLOR_BLACK);
        tft.drawText(50, 70, String(fc), COLOR_BLACK);
        tft.drawText(50, 90, "LDR",COLOR_BLACK);
        tft.drawText(50, 110, String( sensorValue), COLOR_BLACK); 
        tft.drawText(50, 130, "Lux",COLOR_BLACK);
        tft.drawText(50, 145, String(lux), COLOR_BLACK);
        tft.drawText(50, 165, "Intensity",COLOR_BLACK);
        tft.drawText(50, 185, String(Intensity), COLOR_BLACK);
        tft.setBackgroundColor(COLOR_BLACK);
     
        tft.drawText(5, 30, "WL = 639.458 nm",COLOR_BLACK);
        tft.drawText(10, 30, String(fc), COLOR_BLACK);
        tft.drawText(10, 90, String(lux), COLOR_BLACK);  
        tft.drawText(10, 80, String( sensorValue), COLOR_BLACK);    
        tft.setBackgroundColor(COLOR_BLACK);
        
        tft.drawText(70, 10, "Green",COLOR_GREEN); 
        tft.drawText(5, 30, "WL = 510.062 nm",COLOR_GREEN);    
        delay(2000);      
        sensorValue = analogRead(analogPin);
  
        fc = ((-0.476*sensorValue) + 324)*100 ;
        lux= fc/10.764;
        Intensity =  lux*((2*2)*1*3.14);
        Serial.println(fc);
        tft.drawText(50, 90, "LDR",COLOR_WHITE);
        tft.drawText(50, 110, String( sensorValue), COLOR_WHITE); 
        tft.drawText(50, 50, "fc",COLOR_WHITE);
        tft.drawText(50, 70, String(fc), COLOR_WHITE); 
        tft.drawText(50, 130, "Lux",COLOR_WHITE);
        tft.drawText(50, 145, String(lux), COLOR_WHITE);
        tft.drawText(50, 165, "Intensity",COLOR_WHITE);
        tft.drawText(50, 185, String(Intensity), COLOR_WHITE);

      } else if (pressCount == 4) {
        analogWrite(redPin, 0);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, ledBrightness);
        tft.drawText(70, 10, "Green",COLOR_BLACK); 
        tft.drawText(5, 30, "WL = 510.062 nm",COLOR_BLACK); 
        tft.drawText(50, 50, "fc",COLOR_BLACK);
        tft.drawText(50, 70, String(fc), COLOR_BLACK);
        tft.drawText(50, 90, "LDR",COLOR_BLACK);
        tft.drawText(50, 110, String( sensorValue), COLOR_BLACK); 
        tft.drawText(50, 130, "Lux",COLOR_BLACK);
        tft.drawText(50, 145, String(lux), COLOR_BLACK);
        tft.drawText(50, 165, "Intensity",COLOR_BLACK);
        tft.drawText(50, 185, String(Intensity), COLOR_BLACK);
        tft.setBackgroundColor(COLOR_BLACK); 
        tft.setFont(Terminal12x16);
        tft.drawText(70, 10, "Blue",COLOR_BLUE);     
        tft.drawText(5, 30, "WL = 493.511 nm",COLOR_BLUE);  
        delay(2500);  
        sensorValue = analogRead(analogPin);  
      
       fc = ((-0.476*sensorValue) + 324)*100 ;
        lux= fc/10.764;
        Intensity =  lux*((2*2)*1*3.14);
        Serial.println(fc);
        tft.drawText(50, 90, "LDR",COLOR_WHITE);
        tft.drawText(50, 110, String( sensorValue), COLOR_WHITE); 
        tft.drawText(50, 50, "fc",COLOR_WHITE);
        tft.drawText(50, 70, String(fc), COLOR_WHITE); 
        tft.drawText(50, 130, "Lux",COLOR_WHITE);
        tft.drawText(50, 145, String(lux), COLOR_WHITE);
        tft.drawText(50, 165, "Intensity",COLOR_WHITE);
        tft.drawText(50, 185, String(Intensity), COLOR_WHITE);

        
      } else if (pressCount == 5) {
        analogWrite(redPin, 0);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, 0);
        pressCount = 0; 
        tft.drawText(70, 10, "Blue",COLOR_BLACK); 
        tft.drawText(5, 30, "WL = 493.511 nm",COLOR_BLACK); 
        tft.drawText(50, 50, "fc",COLOR_BLACK);
        tft.drawText(50, 70, String(fc), COLOR_BLACK);
        tft.drawText(50, 90, "LDR",COLOR_BLACK);
        tft.drawText(50, 110, String( sensorValue), COLOR_BLACK); 
        tft.drawText(50, 130, "Lux",COLOR_BLACK);
        tft.drawText(50, 145, String(lux), COLOR_BLACK);
        tft.drawText(50, 165, "Intensity",COLOR_BLACK);
        tft.drawText(50, 185, String(Intensity), COLOR_BLACK);
        tft.setBackgroundColor(COLOR_BLACK); 
         tft.setFont(Terminal12x16);
        tft.drawText(50, 50, "Power_off",COLOR_WHITE);  

      }
      
    }
    lastButtonState = buttonState;
  }


 
  
  delay(100); // delay to avoid flickering

}



