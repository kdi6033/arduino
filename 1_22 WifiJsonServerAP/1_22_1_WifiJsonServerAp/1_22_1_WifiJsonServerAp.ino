#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "i2rAp";
const char* password = "00000000";

ESP8266WebServer server(80);

int count=0;
String s,s_head;

void handleRoot() {
  count++;
  s="";
  s=s+"{\"data\":"+count+","+"\"데이타\":"+(count+1)+"}";
  server.send(200, "text/plain", s);
  //server.send(200, "text/html", s);
  
}

void setupWifi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("HTTP server started");
}

void setup(void){
  Serial.begin(115200);
  setupWifi();
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
