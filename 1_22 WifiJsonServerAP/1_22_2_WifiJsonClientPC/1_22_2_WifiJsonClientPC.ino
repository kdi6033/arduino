#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "i2r";
const char* password = "00000000";

int count=0;

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
  Serial.begin(115200);
  setupWifi();
}

void loop(void){
  WiFiClient client;
  HTTPClient http;    //Declare object of class HTTPClient

  count++; 
  String  s ="http://117.16.177.40:1880/json?a=1&count=";
  //String  s ="http://localhost:1880/json?a=1&count=";
  s+=count;
  Serial.println(s);
  http.begin(client,s);
 
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload
 
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection
  delay(5000);  //GET Data at every 5 seconds
}
