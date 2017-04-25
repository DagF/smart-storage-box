#include <Arduino.h>
#include "ButtonRow.h"

SmartStorageBox::SmartStorageBox(String name, String host){
    _name = name;
    _host = host;
}

ButtonRow::ButtonRow(int analogPin, int analogButtonValues[], int threshold);
    _analogButtonValues = analogButtonValues;
    _analogButtonValuesLength = sizeof(_analogButtonValues) / sizeof(_analogButtonValues[0]);
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
