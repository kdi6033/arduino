#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#define TRIGGER_PIN 0 // trigger pin 0(D3) 2(D4)
/* Set these to your desired credentials. */
const char *softAP_ssid = "i2r-";
const char *softAP_password = "";
char ssid[32] = "";
char password[32] = "";
/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
ESP8266WebServer server(80);

/* AP 설정을 위한 변수 선언 */
String sAP_ssid,sChipID;
char cAP_ssid[20];
char cChipID[10]; // Json에서 chipid를 고유 ID로 사용

boolean connect;
const int led = 2;
int bootMode=0; //0:station  1:AP


void setup(void) {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  
  // AP 이름 자동으로 만듬
  sChipID=String(ESP.getChipId(),HEX);
  sChipID.toCharArray(cChipID,sChipID.length()+1);
  sAP_ssid=String(softAP_ssid)+String(ESP.getChipId(),HEX);
  sAP_ssid.toCharArray(cAP_ssid,sAP_ssid.length()+1);
  softAP_ssid=&cAP_ssid[0];
  //clientName=softAP_ssid; //mqtt 통신에서 사용

  loadCredentials();
  //Serial.println("******************");
  //Serial.println(ssid[0]);
  if(ssid[0]==0)
    setupAp();
  else
    connectWifi();

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

void setupAp() {
  bootMode=1;
  Serial.println("AP Mode");
  Serial.println(softAP_ssid);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  digitalWrite(led, 0); //상태 led로 표시
}

int connectWifi() {
  bootMode=0;
  Serial.println("STATION Mode");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); //이 모드를 설정 않해야 AP가 살아있습니다.
  WiFi.begin(ssid, password);

  //referance: https://www.arduino.cc/en/Reference/WiFiStatus
  //WL_NO_SHIELD:255 WL_IDLE_STATUS:0 WL_NO_SSID_AVAIL:1 WL_SCAN_COMPLETED:2
  //WL_CONNECTED:3 WL_CONNECT_FAILED:4 WL_CONNECTION_LOST:5 WL_DISCONNECTED:6
  int connRes = WiFi.waitForConnectResult();
  Serial.print ( "connRes: " );
  Serial.println ( connRes );
  if(connRes == WL_CONNECTED){
    Serial.println("well connected");
    Serial.println(WiFi.localIP());
  }
  else
    WiFi.disconnect();
  // 상태 led로 표시
  for(int i=0;i<5;i++) {
    digitalWrite(led, 0);
    delay(500);
    digitalWrite(led, 1);
    delay(500);
  }
  return connRes;
}

void loop(void) {
  server.handleClient();

  if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();
}
