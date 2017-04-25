#ifndef ButtonRow_h
#define ButtonRow_h


class ButtonRow {
public:
    ButtonRow(int analogPin, int analogButtonValues[], int threshold);

    int getPushedButton();

private:
    int _sensorValue;
    int _threshold;
    int _analogPin;
    int _analogButtonValues[];
    int _analogButtonValuesLength;

};


#endif //ButtonRow_h
