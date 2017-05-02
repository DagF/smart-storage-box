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
}

int ButtonRow::getPushedButton() {
    _sensorValue = analogRead(_analogPin);
    for (int i = 0; i < _analogButtonValuesLength; i++) {
        if (_sensorValue < _analogButtonValues[i] + _threshold && _sensorValue > _analogButtonValues[i] - _threshold) {
            return i;
        }
    }
    return -1;
}
