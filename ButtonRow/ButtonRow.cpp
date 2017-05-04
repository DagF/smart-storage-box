#include <Arduino.h>
#include "ButtonRow.h"

ButtonRow::ButtonRow(int analogPin, int analogButtonValues[], int size, int threshold) {
    _analogButtonValuesLength = size;
    for (int i = 0; i < _analogButtonValuesLength; i++) {
        _analogButtonValues[i] = analogButtonValues[i];
    }
    _analogPin = analogPin;
    _threshold = threshold;
    _sensorValue = 0;
    _last_button_pressed = -1;
    _last_button_pressed_time = 0;
}

int ButtonRow::getPushedButton() {
    _sensorValue = analogRead(_analogPin);
    for (int i = 0; i < _analogButtonValuesLength; i++) {
        if (_sensorValue < _analogButtonValues[i] + _threshold && _sensorValue > _analogButtonValues[i] - _threshold) {
            if (last_button_pressed != i || millis() - _last_button_pressed_time > 30) {
                _last_button_presed = i;
                _last_button_pressed_time = millis();
                return i;
            }
            break;
        }
    }
    return -1;
}
