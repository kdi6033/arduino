#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
//for LED status
#include <Ticker.h>
Ticker ticker;

#define TRIGGER_PIN 0 // trigger pin 0(D3) 2(D4)

String sChipId="";
char cChipId[40]="";
ESP8266WebServer server(80);

int bootMode=0; //0:station  1:AP
int LED = LED_BUILTIN,ledOn=0;
unsigned long now,lastConnectTry = 0,count=0;

void tick()
{
  //toggle state
  digitalWrite(LED, !digitalRead(LED));     // set pin to the opposite state
}

void setup() {
 
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(LED, OUTPUT);
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);

  // AP 이름 자동으로 만듬 i2r-chipid
  sChipId = "i2r-"+String(ESP.getChipId(),HEX);
  sChipId.toCharArray(cChipId,sChipId.length()+1);
  Serial.println(sChipId);

  //WiFiManager
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  WiFiManager wm;
  // wm.resetSettings(); //reset settings - for testing

  if (!wm.autoConnect(cChipId)) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected... :)");
  ticker.detach();
  //keep LED on

  
  digitalWrite(LED, LOW);

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/scan", handleScan);
  server.on("/wifi", handleWifi);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  now = millis();
  //와이파이 연결되면 1초 간격으로 점등
  unsigned int sWifi = WiFi.status();
  if(now >= (lastConnectTry + 3000)) {
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
