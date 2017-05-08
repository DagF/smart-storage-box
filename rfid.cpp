#include <Arduino.h>

#include "rfid.h"

#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;


Rfid::Rfid() {

  Serial.println("rfid starting");
}


void Rfid::beginRFID() {


  SPI.begin(); // Init SPI bus

  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  //printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}
unsigned long Rfid::getUID() {
  // Look for new cards

  if ( ! rfid.PICC_IsNewCardPresent()) {
    //Serial.print("0");
    return 0;
  }
  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial()) {
    //Serial.print("0");
    return 0;
  }
  /*if (rfid.uid.uidByte[0] != nuidPICC[0] ||
      rfid.uid.uidByte[1] != nuidPICC[1] ||
      rfid.uid.uidByte[2] != nuidPICC[2] ||
      rfid.uid.uidByte[3] != nuidPICC[3] ) {
    //Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

    //Serial.println(F("The NUID tag is:"));
    //Serial.print(F("In hex: "));
    Serial.print("{'rfid': '");
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println("'}");
    //Serial.println();
    //Serial.print(F("In dec: "));
    //printDec(rfid.uid.uidByte, rfid.uid.size);
    //Serial.println();
    }
    else Serial.println(F("Card read previously."));*/
  unsigned long uid=0;
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
    uid+= nuidPICC[i]<< (8*(3-i));
  }

  //printHex(rfid.uid.uidByte, rfid.uid.size);

  

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  return uid;
}
void Rfid::printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "0" : "");
    Serial.print(buffer[i], HEX);
  }
}


