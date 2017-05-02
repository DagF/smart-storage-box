#include <Arduino.h>
#include <ButtonRow.h>


ButtonRow buttonRow(A0, {879, 181, 321, 985, 1024}, 5);

void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.print(buttonRow.getPushedButton());
    Serial.print("\n");
    delay(1000);
}