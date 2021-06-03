#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include "FS.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>

Ticker ticker;

#define TRIGGER_PIN 0 // trigger pin 0(D3) 2(D4)

char myBaudrate[50] = "";
char mac[20];  //mac address
String sMac;

int count=0,countScaned=0,actIp[255]={0};

ESP8266WebServer server(80);

int act=0;
const int led = 2;
//unsigned long now,lastConnectTry = 0,count=0;

void tick()
{
  Serial.println(myBaudrate);
}

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  WiFiManager wm;

  //이름 자동으로 생성
  byte hardwareMAC[12];  //WIFI MAC address
  WiFi.macAddress(hardwareMAC);
  sprintf(mac,"%02x%02x%02x%02x%02x%02x",hardwareMAC[5], hardwareMAC[4], hardwareMAC[3], hardwareMAC[2], hardwareMAC[1], hardwareMAC[0]);  
  sMac=mac;

  //wifi manager 참조유튜브  https://youtu.be/mJFd7g4jedw  //https://youtu.be/EzEG_2HcOQo
  //reset settings - wipe credentials for testing
  //wm.resetSettings();
  
  char i2rMac[30];
  sprintf(i2rMac,"i2r-%02x%02x%02x%02x%02x%02x",hardwareMAC[5], hardwareMAC[4], hardwareMAC[3], hardwareMAC[2], hardwareMAC[1], hardwareMAC[0]);  
  if (!wm.autoConnect(i2rMac)) {
    Serial.println("failed to connect and hit timeout");
    ESP.restart();
    delay(1000);
  }
  Serial.print("connected... :");
  Serial.print(WiFi.localIP().toString());

  readConfig();
  Serial.begin(atoi(myBaudrate));
  
  ticker.attach(2, tick);  //0.1 초도 가능
  //ticker.detach();
  
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/scan", handleScan);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  //공장리셋
  if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();
}
