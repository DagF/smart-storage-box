#include <ButtonRow.h>

/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution

Code edited by Andrew R. from HobbyTransform for this example.
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

int buttonValues[] = {879, 181, 321, 985, 1024};
int size = sizeof(buttonValues) / sizeof(buttonValues[0]);
ButtonRow buttonRow(A0, buttonValues, size, 5);

void setup()   {             
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  delay(3000);
  display.clearDisplay();
  display.display();

    Serial.begin(9600);
    
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
}

void blink(int pin, int times, int d){
 for(int j = 0; j < times; j++){
  digitalWrite(pin, LOW);
  delay(d);
  digitalWrite(pin, HIGH);
  delay(d); 
 }
  digitalWrite(pin, LOW);
}

int last = 0;

void loop() {

      // scanner inn 300 ms delay blinking 
      // gått ut på dato 500ms delay blinking
      // når reggistrere 2
    Serial.print(buttonRow.getPushedButton());
    Serial.print("\n");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      int i = buttonRow.getPushedButton();
      if(i >= 0){
      last = i; 
        }
      switch(last){
        case 0:
          blink(D6, 5, 100);
          display.print("one");
          break;
        case 1:
          blink(D6, 5, 300);
          display.print("two");
          break;
        case 2:
          blink(D5, 5, 500);
          display.print("three");
          break;
        case 3:
          blink(D5, 5, 400);
          display.print("four");
          break;
        case 4:
          blink(D5, 5, 500);
          display.print("five");
          break;
      }
      display.display();


     display.clearDisplay();
      display.print(analogRead(A0));
     display.display();
      delay(100);
     
}


