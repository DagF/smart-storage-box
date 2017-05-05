#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//acc
#include <SparkFun_ADXL345.h>

ADXL345 adxl = ADXL345();

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ButtonRow.h>

#include <SmartStorageBox.h>

SmartStorageBox box("s002", "http://172.24.1.4:9000/api/");

#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);
//ok, back, up, down, reset(up + down)
#define OK 0
#define BACK 1
#define UP 2
#define DOWN 3
#define RESET 4
int buttonValues[] = {645, 997, 745, 955, 988};
int size = sizeof(buttonValues) / sizeof(buttonValues[0]);
int threshold = 10;
ButtonRow buttonRow(A0, buttonValues, size, threshold);

bool popup_active = false;

#define MENU_MAIN  1
#define MENU_RFID  2
#define MENU_WEIGHT 3
#define MENU_INFO_CONTENT  4
#define MENU_INFO_USE  5
#define MENU_ACTIVE  6
#define MENU_MAIN_INFO_CONTENT 7
#define MENU_MAIN_INFO_USE 8
#define MENU_MAIN_INFO_WEIGHT 9

int current_menu = MENU_MAIN;
int prev_menu = MENU_MAIN;


#define SENSOR_RESPONSE_NO_VALUE  0
#define SENSOR_RESPONSE_WEIGHT_CHANGED  1
#define SENSOR_RESPONSE_RFID_VALUE  2
#define SENSOR_RESPONSE_ACTIVE 3

String user_names[] = {"Dag", "Sivert", "Thomesine"};
unsigned long user_rfid[] = {294938465,1,1};
String last_used_by = user_names[0];

String tools[] = {"Hammer", "caliper", "Tape"};
unsigned long tool_weight[] = {300, 8000, 80};
bool tool_present[] ={true, false, true};
unsigned long tools_rfid[] = {19360,1,1};


static const unsigned char PROGMEM
protoboxLogo[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x38, 0x70, 0x8F, 0x3E, 0xDC, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x88, 0x84, 0x88, 0x49, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x88, 0xC4, 0x88, 0x6A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x89, 0x44, 0x88, 0x6A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x89, 0x44, 0x88, 0x5A, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x89, 0xE4, 0x88, 0x5A, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x11, 0x89, 0x24, 0x88, 0x49, 0x26, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x3F, 0x73, 0x3F, 0x3E, 0xE8, 0xC6, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00,
0xBC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0xC0, 0x00, 0x00, 0x00, 0x00,
0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x70, 0x00, 0x00, 0x00, 0x00,
0x81, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x00, 0x00, 0x00, 0x00,
0x80, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00,
0x98, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xC1, 0xC0, 0x00, 0x00, 0x00,
0xD6, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x70, 0x70, 0x00, 0x00, 0x00,
0x71, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x10, 0x1C, 0x00, 0x00, 0x00,
0x01, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x07, 0x00, 0x00, 0x00,
0x01, 0x00, 0x1C, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xB0, 0x01, 0xC0, 0x00, 0x00,
0x01, 0x00, 0x07, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x50, 0x00, 0x70, 0x00, 0x00,
0x01, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1C, 0x8F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x80, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8C, 0x70, 0x00, 0x00, 0x20, 0x00, 0x18, 0xE0, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8C, 0x18, 0x00, 0x00, 0x60, 0x00, 0x18, 0x30, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8C, 0x18, 0x00, 0x00, 0x60, 0x00, 0x18, 0x30, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8C, 0x1B, 0x61, 0xE0, 0xFC, 0x3C, 0x18, 0x30, 0x78, 0xC1, 0x88, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8C, 0x1B, 0xC7, 0x38, 0x60, 0xE7, 0x18, 0x61, 0xCE, 0x63, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8C, 0x73, 0x06, 0x18, 0x60, 0xC3, 0x1F, 0x81, 0x86, 0x36, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8F, 0xC3, 0x0C, 0x0C, 0x61, 0x81, 0x98, 0x63, 0x03, 0x14, 0x08, 0x00, 0x00,
0x01, 0x80, 0x1E, 0x8C, 0x03, 0x0C, 0x0C, 0x61, 0x81, 0x98, 0x33, 0x03, 0x1C, 0x08, 0x00, 0x00,
0x00, 0xC0, 0x1E, 0x8C, 0x03, 0x0C, 0x0C, 0x61, 0x81, 0x98, 0x33, 0x03, 0x1C, 0x08, 0x00, 0x00,
0x00, 0x60, 0x1E, 0x8C, 0x03, 0x0C, 0x0C, 0x61, 0x81, 0x98, 0x33, 0x03, 0x36, 0x08, 0x00, 0x00,
0x00, 0x38, 0x0E, 0x8C, 0x03, 0x06, 0x18, 0x60, 0xC3, 0x18, 0x31, 0x86, 0x36, 0x08, 0x00, 0x00,
0x00, 0x0C, 0x02, 0x8C, 0x03, 0x07, 0x38, 0x70, 0xE7, 0x18, 0xE1, 0xCE, 0x63, 0x08, 0x07, 0x0C,
0x00, 0x07, 0x00, 0x8C, 0x03, 0x01, 0xE0, 0x18, 0x3C, 0x1F, 0x80, 0x78, 0xC1, 0x88, 0x05, 0x12,
0x00, 0x01, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xB1, 0x12,
0x00, 0x00, 0x60, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xA2, 0x12,
0x00, 0x00, 0x38, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xA1, 0x12,
0x00, 0x00, 0x0C, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x45, 0x12,
0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x47, 0x4C,
0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


int GREEN_LEDS = D6;
int RED_LEDS = D5;

void init_led() {
    pinMode(GREEN_LEDS, OUTPUT);
    pinMode(RED_LEDS, OUTPUT);
}

void init_serial_communication() {
    Serial.begin(9600);
    Serial.println("ESP LOADING");
    Wire.begin(SDA, SCL);
    Wire.setClockStretchLimit(15000);
}

void init_display() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
    display.clearDisplay();
    display.drawBitmap(1, 1, protoboxLogo, 128, 64, 1);
    display.display();
    delay(500);
    display.clearDisplay();
    display.display();
}

ESP8266WiFiMulti WiFiMulti;

void init_wifi() {
    WiFiMulti.addAP("smart-storage", "smart-storage");
}

void setup() {
    init_acc();
    init_led();
    init_wifi();
    init_display();
    init_led();
    init_serial_communication();
}


void clear_display() {
    display.clearDisplay();
}

void reset_text() {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
}

unsigned long weight = 0;
unsigned long prev_weight = 0;
unsigned long weights[50] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int weight_counter = 0;
unsigned long rfid = 0;
unsigned long last_rfid = 0;
unsigned long last_weight_time = 0;

int read_values_from_other_arduino() {
    int response = SENSOR_RESPONSE_NO_VALUE;
    Wire.beginTransmission(9);
    Wire.write(0);
    Wire.endTransmission();
    //Serial.println("Receive data");
    Wire.requestFrom(9, 22);
    String data = "";
    while (Wire.available()) {
        char b = Wire.read();
        data += b;
    }
    String weightS = data.substring(2, 11);
    String rfidS = data.substring(13, 22);
    
    unsigned long weight_tmp = weightS.toInt();
      //Serial.print("tp ");
      //Serial.println(weight_tmp);
      //Serial.print("last ");
      //Serial.println(weights[weight_counter]);
                //Serial.println( weight_tmp);
   if(millis() - last_weight_time > 100){
    if(weight_counter < 10){
      if(weight_counter == 0){
        weights[weight_counter] = weight_tmp;
      }else if( weight_tmp > weights[weight_counter-1]){
            if((weight_tmp - weights[weight_counter-1]) > 1000){
                // Serial.println("rese1t1");
                // Serial.println(weight_tmp - weights[weight_counter]);
               weight_counter  = 0;
              }
          }
          else if(weights[weight_counter-1]> weight_tmp ){
            
            if((weights[weight_counter-1] - weight_tmp) > 1000){
              //  Serial.println("reset2");
               // Serial.println(weights[weight_counter-1] - weight_tmp);
                weight_counter  = 0;
              }
          }
        last_weight_time = millis();
        weights[weight_counter] = weight_tmp;
               // Serial.println( weights[weight_counter]);
                //Serial.println( weight_tmp);
        weight_counter++;
    } else {
        unsigned long sum_weight = 0;
        for(int i = 0; i < 10; i++){
            sum_weight = sum_weight + weights[i];
        }
        weight_tmp = sum_weight / 10;
        weight_counter = 0;
        if(weight_tmp > weight && (weight_tmp - weight) > 1000 || weight_tmp < weight && (weight-weight_tmp) > 1000){
         //  Serial.println(weight);
     // Serial.println(weight_tmp);
          prev_weight = weight;
            weight = weight_tmp;
unsigned long diff = 0;
    if(prev_weight > weight){
       diff = prev_weight - weight;      
    } else if (prev_weight < weight){
      diff = weight - prev_weight;      
    }
        
        for(int i = 0; i < 3; i++){
          if(tool_weight[i] > diff - 1000 && tool_weight[i] < diff + 1000 ){
            if(prev_weight > weight){
         tool_present[i] = false;
        
      } else if (prev_weight < weight){
        tool_present[i] = true;
      }

            break; 
          }
        }
      
      
      
      


            
            response = SENSOR_RESPONSE_WEIGHT_CHANGED;
            box.postWeight(weight);
        }
     
    }
   }
    
    rfid = rfidS.toInt();
    if(rfid > 0){
      Serial.println(rfid);
        last_rfid = rfid;
        
        for(int i = 0; i < 3; i++){
            if(last_rfid == tools_rfid[i]){
              tool_present[i] = !tool_present[i];
            }
        }
        box.postRFID(rfid);
        response = SENSOR_RESPONSE_RFID_VALUE;
    }

    return response;
}

/***
 * Reads if box is acctive
 * if active sends value to server and returns SENSOR_RESPONSE_ACTIVE
 * @return SENSOR_RESPONSE_ACTIVE | SENSOR_RESPONSE_NO_VALUE
 */
unsigned long last_active = millis();

int read_acc() {
    int response = SENSOR_RESPONSE_NO_VALUE;
    if (aks_get()) {
        if ((WiFiMulti.run() == WL_CONNECTED)) {
            box.postActivity();
        }
        response = SENSOR_RESPONSE_ACTIVE;
        last_active = millis();
        blink(GREEN_LEDS, 2, 100);
    }
    return response;
}

/***
 * Reads values from sensors.
 * Reading triggers sending data to the server
 * @return SENSOR_RESPONSE_*
 */
int manage_sensors() {
    int response = SENSOR_RESPONSE_NO_VALUE;
    response = read_acc();
    int other_arduino_sensors_response = read_values_from_other_arduino();
    if (response == SENSOR_RESPONSE_NO_VALUE) {
        response = other_arduino_sensors_response;
    }
    return response;
}

#define MENU_MAIN_OPTION_INFO_CONTENT 0
#define MENU_MAIN_OPTION_INFO_USE 1
#define MENU_MAIN_OPTION_INFO_WEIGHT 2
String menu_items[] = {
        "Info About Content", "Info About Use", "Weight raw"};
int menu_length = 3;
int in = 0;

void main_menu() {
    clear_display();
    reset_text();


//TODO move code for better button click feeling into library with delay in cycles as input
    switch (buttonRow.getPushedButton()) {
        case OK:
            blink(GREEN_LEDS, 5, 100);
            switch (in) {
                case MENU_MAIN_OPTION_INFO_CONTENT:
                    prev_menu = current_menu;
                    current_menu = MENU_MAIN_INFO_CONTENT;
       
                    break;
                case MENU_MAIN_OPTION_INFO_USE:
                    prev_menu = current_menu;
                    current_menu = MENU_MAIN_INFO_USE;
      
                    break;
                case MENU_MAIN_OPTION_INFO_WEIGHT:
                    prev_menu = current_menu;
                    current_menu = MENU_MAIN_INFO_WEIGHT;
        
                    break;
            }
            break;
        case BACK:
            blink(RED_LEDS, 5, 300);
            //TODO reset menu values
            break;
        case UP:
            if (in > 0) {
                in--;
            }
            break;
        case DOWN:
            if (in < menu_length - 1) {
                in++;
            }
            break;
        case RESET:
            //reset
            break;
    }
    int line = 0;
    for (int i = 0; i < menu_length; i++) {
        display.setCursor(0, 10 * line);
        if (i == in) {
            display.print(">");
        } else {
            display.print(" ");
        }
        display.print(menu_items[i]);
        line++;

    }
    display.display();
}

int prev_menu_active =0;
unsigned long menu_start = 0;

void active_menu() {
    clear_display();
    reset_text();
    display.print("Hey! Who moved me?");
    display.display();
    if ((millis() - menu_start) > 2000) {
        current_menu = prev_menu_active;
        popup_active = false;
    }
}

void rfid_menu() {

    String item = "UNKNOWN";
    clear_display();
    reset_text();
    display.println("RFID");
    display.println(last_rfid);
    for(int i = 0; i < 3; i++){
        if(last_rfid == user_rfid[i]){
    display.print("User: ");
    display.print( user_names[i]);
            break;
        }
    }
    for(int i = 0; i < 3; i++){
        if(last_rfid == tools_rfid[i]){
    display.print("Tool: ");
    display.println( tools[i]);
    if(tool_present[i]){
      
      display.print("Returned");
    } else {
      
      display.print("Removed");
    }
            break;
        }
    }
    //tool_present
    display.display();

    if ((millis() - menu_start) > 2000) {
        current_menu = prev_menu_active;
        popup_active = false;
    }
}


/*

String tools[] = {"Hammer", "caliper", "Tape"};
unsigned long tool_weight[] = {300, 8000, 80};
bool tool_present[] ={true, true, true};
unsigned long tools_rfid[] = {19360,1,1};
*/
void weight_menu() {
    clear_display();
    reset_text();
    unsigned long diff = 0;
    display.println("weight");
    if(prev_weight > weight){
       diff = prev_weight - weight;
    display.print(diff);
    display.println("g removed");
       
      
    } else if (prev_weight < weight){
      diff = weight - prev_weight;
    display.print(diff);
    display.println("g added");
      
    }
    
    if(diff > 2000){
      
      for(int i = 0; i < 3; i++){
        if(tool_weight[i] > diff - 1000 && tool_weight[i] < diff + 1000 ){
          
          display.println(tools[i]);
          break; 
        }
      }
    }
    display.display();
    if ((millis() - menu_start) > 2000) {
        current_menu = prev_menu_active;
        popup_active = false;
    }
}


void main_menu_info_content() {
    clear_display();
    reset_text();
    display.print("Tools:");
    int ind = 1;
    for(int i = 0; i < 3; i++){
        if(tool_present[i]){
            display.setCursor(0, (ind) * 10);
            display.print(tools[i]);
            ind++;
        }
    }
    display.display();
    switch (buttonRow.getPushedButton()) {
        case BACK:
            current_menu = prev_menu;
            break;
    }
}

void main_menu_info_use() {
    clear_display();
    reset_text();
    display.print("Contact information:");
    display.setCursor(0, 20);
    display.print("Name: Sivert");
    display.setCursor(0, 30);
    display.print("TLF: 815 493 00");
    display.setCursor(0, 40);
    unsigned long last_used = (millis() - last_active) / (1000);
    display.print("Last used: ");
    display.print(last_used);
    display.print(" s");
    display.setCursor(0, 50);
    display.print("Last used by:: ");
    display.print(last_used_by);
    display.display();
    switch (buttonRow.getPushedButton()) {
        case BACK:
            current_menu = prev_menu;
            break;
    }
}

void main_menu_weight(){

    clear_display();
    reset_text();
    display.print(weight);
    display.display();
    switch (buttonRow.getPushedButton()) {
        case BACK:
            current_menu = prev_menu;
            break;
    }
}
void loop() {
    update_blink();
//TODO loop til connected to wifi with text connecting to network

    int sensor_response = manage_sensors();
    if (sensor_response != SENSOR_RESPONSE_NO_VALUE) {
        int next_menu = current_menu;
        if (sensor_response == SENSOR_RESPONSE_ACTIVE) {
            next_menu = MENU_ACTIVE;
        }
        if (sensor_response == SENSOR_RESPONSE_RFID_VALUE) {
            next_menu = MENU_RFID;
        }
        if (sensor_response == SENSOR_RESPONSE_WEIGHT_CHANGED) {
            next_menu = MENU_WEIGHT;
        }
        if (!popup_active) {
            popup_active = true;
            prev_menu_active = current_menu;
            current_menu = next_menu;
            menu_start = millis();
        }
    }

    switch (current_menu) {
        case MENU_MAIN:
            main_menu();
            break;
        case MENU_MAIN_INFO_CONTENT:
       
            main_menu_info_content();
            break;
        case MENU_MAIN_INFO_USE:
            main_menu_info_use();
            break;
        case MENU_ACTIVE:
            active_menu();
            break;
        case MENU_RFID:
            rfid_menu();
            break;
        case MENU_WEIGHT:
            weight_menu();
            break;
        case MENU_MAIN_INFO_WEIGHT:
            main_menu_weight();
            break;
        default:
            main_menu();
    }

}

boolean aks_get() {
    // getInterruptSource clears all triggered actions after returning value
    // Do not call again until you need to recheck for triggered actions

    byte interrupts = adxl.getInterruptSource();
/*
  // Free Fall Detection
  if (adxl.triggered(interrupts, ADXL345_FREE_FALL)) {
    //Serial.println("*** FREE FALL ***");
    //add code here to do when free fall is sensed
  }

  // Inactivity
  if (adxl.triggered(interrupts, ADXL345_INACTIVITY)) {
    //Serial.println("*** INACTIVITY ***");
    //add code here to do when inactivity is sensed
  }
*/
    // Activity
    if (adxl.triggered(interrupts, ADXL345_ACTIVITY)) {
        return true;
    }
/*
  // Double Tap Detection
  if (adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)) {
    //Serial.println("*** DOUBLE TAP ***");
    //add code here to do when a 2X tap is sensed
  }

  // Tap Detection
  if (adxl.triggered(interrupts, ADXL345_SINGLE_TAP)) {
    //Serial.println("*** TAP ***");
    //add code here to do when a tap is sensed
  }*/
    return false;
}

//AKS
void init_acc() {
    adxl.powerOn();                     // Power on the ADXL345

    adxl.setRangeSetting(16);           // Give the range settings
    // Accepted values are 2g, 4g, 8g or 16g
    // Higher Values = Wider Measurement Range
    // Lower Values = Greater Sensitivity

    adxl.setSpiBit(
            0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
    // Default: Set to 1
    // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library

    adxl.setActivityXYZ(1, 0,
                        0);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
    adxl.setActivityThreshold(25);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

    adxl.setInactivityXYZ(1, 0,
                          0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
    adxl.setInactivityThreshold(25);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
    adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?

    adxl.setTapDetectionOnXYZ(0, 0,
                              1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)

    // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
    adxl.setTapThreshold(50);           // 62.5 mg per increment
    adxl.setTapDuration(15);            // 625 μs per increment
    adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
    adxl.setDoubleTapWindow(200);       // 1.25 ms per increment

    // Set values for what is considered FREE FALL (0-255)
    adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
    adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment

    // Setting all interupts to take place on INT1 pin
    //adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);"
    // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
    // This library may have a problem using INT2 pin. Default to INT1 pin.

    // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
    adxl.InactivityINT(1);
    adxl.ActivityINT(1);
    adxl.FreeFallINT(1);
    adxl.doubleTapINT(1);
    adxl.singleTapINT(1);
}


int blink_color = 0;
int blinks_left = 0;
int blink_count = 0;
int blink_delay = 0;
unsigned long blink_start = 0;

void blink(int pin, int times, int d) {
    blink_color = pin;
    blinks_left = times;
    blink_count = 0;
    blink_delay = d;
    blink_start = millis();
}

void update_blink() {
    unsigned long now = millis();
    if (blinks_left > 0) {


        if (((blink_count * 2) * blink_delay) < (now - blink_start)) {
            digitalWrite(blink_color, HIGH);
            blink_count++;
            blinks_left--;
        } else if (((blink_count * 2 - 1) * blink_delay) < (now - blink_start)) {
            digitalWrite(blink_color, LOW);
        }

    } else {
        digitalWrite(blink_color, LOW);
    }
}
