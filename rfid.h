/*
  Library made for getting UID from RFID

  Maker: Sivert Hatletveit


  SDA   D10(grå)
  SCK   D13(blå) 
  MOSI  D11(grønn)
  MISO  D12(gul)
  IRQ   NC
  RST   D9(hvit)
  
*/

#include <Arduino.h>

#ifndef Rfid_h
#define Rfid_h




class Rfid {
  
  public:
    Rfid();
    void beginRFID();
    unsigned long getUID();
  private:
    void printHex(byte *buffer, byte bufferSize);
    byte nuidPICC[4];// Init array that will store new NUID
    String str;
};




#endif
