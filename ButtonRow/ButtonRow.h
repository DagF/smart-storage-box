#ifndef ButtonRow_h
#define ButtonRow_h


class ButtonRow {
public:
    ButtonRow(int analogPin, int analogButtonValues[], int size, int threshold);

    int getPushedButton();

private:
    int _sensorValue;
    int _threshold;
    int _analogPin;
    int _analogButtonValues[16];
    int _analogButtonValuesLength;

};


#endif //ButtonRow_h
