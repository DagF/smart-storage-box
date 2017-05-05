#include "Arduino.h"
#include "SmartStorageBox.h"
#include "ESP8266HTTPClient.h"



SmartStorageBox::SmartStorageBox(String name, String host){
	_name = name;
	_host = host;
}

bool SmartStorageBox::postWeight(int weight){
	return _post("box/weight/", "{\"box\": \""+_name+"\",\"value\": "+String(weight)+"}");
}

bool SmartStorageBox::postRFID(int rfid){
	return _post("box/rfid/", "{\"box\": \""+_name+"\",\"value\": \""+String(rfid)+"\"}");
}

bool SmartStorageBox::postActivity(){
	return _post("box/activities/", "{\"box\": \""+_name+"\"}");
}

bool SmartStorageBox::_post(String path, String data){
	bool success = true;
  	HTTPClient http;
        http.begin(_host + path);
        http.addHeader("Content-Type", "application/json");
        int httpCode = http.POST(data);
        if(httpCode <= 0) {
		success = false;
        }
        http.end(); 
	return success;
}
