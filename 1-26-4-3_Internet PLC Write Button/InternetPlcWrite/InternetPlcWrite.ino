#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "i2r";
const char* password = "00000000";

ESP8266WebServer server(80);

const int led = 2;
String s="";
int P4[4]={0};

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  Serial.begin(9600);
  Serial1.begin(9600);
  
  connectWifi();
  server.on("/", handleRoot);
  server.on("/onoffP40", handleOnOffP40);
  server.on("/onoffP41", handleOnOffP41);
  server.on("/onoffP42", handleOnOffP42);
  server.on("/onoffP43", handleOnOffP43);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void connectWifi() {
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

void loop(void) {
  server.handleClient();
}

void plcOut() {
  //int out=P40+P41*2+P42*4+P43*8;
  String out =  String(P4[0]+P4[1]*2+P4[2]*4+P4[3]*8, HEX);
  String s="";
  s=char(5);
  s+="00WSS0104%PW4000";
  s+=out;
  s+=char(4);
  Serial1.print(s);
}
