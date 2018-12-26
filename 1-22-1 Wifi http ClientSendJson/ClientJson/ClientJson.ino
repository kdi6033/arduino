#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "i2rAp";
const char* password = "00000000";

const int ledPin =  LED_BUILTIN;// the number of the LED pin
String payload;
int count=0;
String s;

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
  digitalWrite(ledPin, HIGH);
  Serial.begin(9600);
  setupWifi();
}

void loop(void){
  HTTPClient http;    //Declare object of class HTTPClient

  count++;
  s="http://192.168.4.1/json?j={\"count\":";
  s = s + count + "}";
  http.begin(s);   
  
  int httpCode = http.GET();            //Send the request
  payload = http.getString();    //Get the response payload
 
  //Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
  delay(3000);  //GET Data at every 5 seconds
}
