//i2c communication
#include <Wire.h>
#define ANSWERSIZE 22
String answer = "I2CVirker";

//verider som skal sendes
unsigned long rfidValue = 0;
unsigned long weight = 0;


//scale
#include "HX711.h"
HX711 scale;

//rfid
#include "rfid.h"
Rfid rfid1;


void setup() {
  //i2c communication
  Wire.begin(9);
  Wire.onRequest(requestEvent); // data request to slave
  Wire.onReceive(receiveEvent); // data slave received
  Serial.begin(9600);
  Serial.println("I2C Slave ready!");
  rfid1.beginRFID();

  //scale
  scale.begin(4, 5);

}

void receiveEvent(int countToRead) {
  while (0 < Wire.available()) {
    byte x = Wire.read();
  }
  //Serial.println("Receive event");
}


void requestEvent() {
  String stringToSend = "";
  stringToSend += 'w';

  //Serial.println(weight);
  for (int i = 9; i >= 0 ; i--) {
    if (weight < pow(10, i))
      stringToSend += '0';
  }

  stringToSend += weight;


  //Serial.println(rfidValue);
  stringToSend += 'r';
  for (int i = 9; i >= 0 ; i--) {
    if (rfidValue < pow(10, i))
      stringToSend += '0';
  }
  stringToSend += rfidValue;

  byte response[stringToSend.length()];
  for (byte i = 0; i < stringToSend.length(); i++) {
    response[i] = (byte)stringToSend.charAt(i);
  }


  Serial.print("stringToSend: ");
  Serial.println(stringToSend);
  Wire.write(response, sizeof(response));
}

void loop() {
  //delay(50);
  weight = scale.read();
  rfidValue = rfid1.getUID();
  //Serial.println(rfidValue);
  //weight= scale.read_average(1);
}
