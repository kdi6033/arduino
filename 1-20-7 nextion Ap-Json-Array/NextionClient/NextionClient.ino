#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "i2rAp";
const char* password = "00000000";

const int ledPin =  LED_BUILTIN;// the number of the LED pin
//json
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;
String payload;
bool sw[8]={false};

void DoJason() {
  deserializeJson(doc,payload);
  root = doc.as<JsonObject>();
  bool bSw = root["sw"][0]; // 스위치에 따라 배열 값 * 선택 입력 root["sw"][*]
  if(bSw==true)
    digitalWrite(ledPin, LOW);
  else
    digitalWrite(ledPin, HIGH);
 
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
}


void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup(void){
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  setupWifi();
}

void loop(void){
  HTTPClient http;    //Declare object of class HTTPClient
 
  http.begin("http://192.168.4.1/");     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  payload = http.getString();    //Get the response payload
 
  //Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
  DoJason();

  http.end();  //Close connection
  delay(2000);  //GET Data at every 5 seconds
}
