

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SmartStorageBox.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

SmartStorageBox box("s002", "http://dagfro.de:9000/api/");

void setup() {
    USE_SERIAL.begin(115200);
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
    WiFiMulti.addAP("SSID", "password");

}

void loop() {
    if((WiFiMulti.run() == WL_CONNECTED)) {
        box.postWeight(10);
        box.postRFID("E2 46 4B A0");
        box.postActivity();
    }
    delay(10000);
}

