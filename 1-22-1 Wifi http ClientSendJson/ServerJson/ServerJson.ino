#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>


const char* ssid = "i2rAp01";
const char* password = "00000000";
ESP8266WebServer server(80);

const int ledPin =  LED_BUILTIN;
//json
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;
String payload;

int count=0;
char msg[100];
bool bB0=false,bChange=false;
int color=864;

String s,s_head;
void handleRoot() {
  s="";
  s=s+"{\"sw0\":"+bB0+","+"\"count\":"+count+"}";
  server.send(200, "text/plain", s);
  //server.send(200, "text/html", s);
  
}

void handleJson() {
  digitalWrite(ledPin, LOW);
  server.arg("j").toCharArray(msg, 99);
  Serial.println(msg);
  deserializeJson(doc,msg);
  root = doc.as<JsonObject>();
  int Rvalue = root["count"];
  Serial.println(Rvalue);

  count++;
  s="";
  s=s+"{\"sw0\":"+bB0+","+"\"count\":"+count+"}";
  server.send(200, "text/plain", s);
  //server.send(200, "text/html", s);
  digitalWrite(ledPin, HIGH);
  
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

void SetText() {
   sprintf (msg, "t0.txt=\"count %d \"%c%c%c",count,0xff,0xff,0xff);
   //Serial1.print(msg); 
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial1.begin(9600);
  setupWifi();
  server.on("/", handleRoot);
  server.on("/json", handleJson);
  server.begin();
  Serial.println("HTTP server started");
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

}
