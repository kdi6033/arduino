#include "FS.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define TRIGGER_PIN 0 // trigger pin 0(D3) 2(D4)

char ssid[40] = "";
char password[50] = "";
char softAP_ssid[40] = "i2r-";
const char softAP_password[40] = "";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
ESP8266WebServer server(80);

int bootMode=0; //0:station  1:AP
const int led = 2;
unsigned long now,lastConnectTry = 0,count=0;

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  Serial.begin(115200);

  Serial.println("mac address");
  //이름 자동으로 생성
  uint8_t mac[6]="";
  WiFi.macAddress(mac);
  sprintf(softAP_ssid,"i2r-%02x%02x%02x%02x%02x%02x%c",mac[5], mac[4], mac[3], mac[2], mac[1], mac[0],0);
  Serial.println(softAP_ssid);
  
  readConfig();
  if(ssid[0]==0)
    bootWifiAp();
  else
    bootWifiStation();
  Serial.println("Boot Mode : "+bootMode);

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/wifi", handleWifi);
  server.on("/wifisave", handleWifiSave);
  server.on("/scan", handleScan);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void bootWifiAp() {
  bootMode=1; //0:station  1:AP
  /* Soft AP network parameters */
  Serial.println("AP Mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  digitalWrite(led, 0); //상태 led로 표시
  // 접속 성공이면 led 5초간 점등 후 소등
  digitalWrite(led, 0);
  delay(5000);
  digitalWrite(led, 1);
}

void bootWifiStation() {
  //referance: https://www.arduino.cc/en/Reference/WiFiStatus
  //WL_NO_SHIELD:255 WL_IDLE_STATUS:0 WL_NO_SSID_AVAIL:1 WL_SCAN_COMPLETED:2
  //WL_CONNECTED:3 WL_CONNECT_FAILED:4 WL_CONNECTION_LOST:5 WL_DISCONNECTED:6
  //WiFi 연결
  bootMode=0; //0:station  1:AP
  Serial.println("Station Mode");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    //공장리셋
    if ( digitalRead(TRIGGER_PIN) == LOW ) 
      factoryDefault();
  }
  digitalWrite(led, 0);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // 접속 성공이면 led 5번 점등
  for(int i=0;i<5;i++) {
    digitalWrite(led, 0);
    delay(500);
    digitalWrite(led, 1);
    delay(500);
  }
}

void loop() {
  server.handleClient();

  now = millis();
  //와이파이 연결되면 1초 간격으로 점등
  unsigned int sWifi = WiFi.status();
  if(now >= (lastConnectTry + 1000)) {
    Serial.println ( WiFi.localIP() );
    lastConnectTry=now;
    count++;
    if(count>=3600*24)
      count=0;
  }

  //공장리셋
  if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();
}
