// 햄버거 메뉴 https://youtu.be/yPOYlaGloMk
// 웹소켙 https://youtu.be/77qg_aaFGoI   https://youtu.be/TUU0CSBfgos
// SPIFFS https://youtu.be/5wgZIapHPxs  https://youtu.be/FRIgp7JI7IY
// ota 참조유튜브 https://www.youtube.com/watch?v=UiAc3yYBsNU
#include "FS.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>

#define TRIGGER_PIN 0 // trigger pin 0(D3)
HTTPClient http;
ESP8266WebServer server(80);
//WebSocketsServer webSocket = WebSocketsServer(81);
WiFiUDP Udp;

unsigned int localUdpPort = 4210; // local port to listen on
char incomingPacket[255]; // buffer for incoming packets

int type=12; // 기기 인식번호 -> display에 사용 6= LS PLC XEC-DR14E

char ssid[40] = "";
char password[50] = "";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

char email[50] = "";
char ipMqtt[40]="";
unsigned int countTick=0;
unsigned int countMqtt=0;
unsigned int countMeasure=0;

const char* outTopic = "/i2r/outTopic"; // 이름이 중복되지 않게 설정 기록
const char* inTopic = "/i2r/inTopic"; // 이름이 중복되지 않게 설정 기록
const char* clientName = "";  // setup 함수에서 자동생성
char msg[100];
int mqttConnected=0; // 1=연결 0=끊김

String ipAct="";
char mac[20];  //mac address
String sMac;
int act=0,outPlc=0;
int bootMode=0; //0:station  1:AP
int counter=0;
String inputString = "";    // 받은 문자열
int Out[8]={0},In[10]={0};  // plc 입력과 출력 저장 
String sIn="",sInPre="";  // 입력값이 달라질 때만 mqtt로 송신
int noOut=0,valueOut=0; //mqtt로 수신된 plc출력 명령 noOut=출력핀번호 valueOut=출력값
unsigned long previousMillis = 0;     
const long interval = 1000;  
int nWifi;

int callMotor=0,vel=0,dir=0;

//json을 위한 설정
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

WiFiClient espClient;
PubSubClient client(espClient);

int ENA = D8;
int INA1 = D7;
int INA2 = D6;

void factoryDefault();
void GoHome();
void GoHomeWifi();
void handleNotFound();
void handleRoot();
void handleWifiSave();
String sensWifi(int in);
void motor();
void readConfig();
void saveConfig();
void tick();


// 통신에서 문자가 들어오면 이 함수의 payload 배열에 저장된다.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  deserializeJson(doc,payload);
  root = doc.as<JsonObject>();
  int dirValue = root["dir"];
  int velValue = root["vel"];
  dir=dirValue;
  vel=velValue;
  callMotor=1;
}

// mqtt 통신에 지속적으로 접속한다.
void reconnect() {
  if(ipMqtt[0]==0)
    return;
  Serial.println("reconnect");
  Serial.println(ipMqtt);
  //if(WiFi.status() == WL_DISCONNECTED)
  //  return;
  Serial.println("====================================");
  Serial.println(countMqtt);
  // Loop until we're reconnected
  //while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientName)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish(outTopic, "Reconnected");
      // ... and resubscribe
      client.subscribe(inTopic);
      mqttConnected=1;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 180 seconds");
      mqttConnected=0;
      // Wait 5 seconds before retrying
      //delay(5000);
    }
  //}
}

void tick()
{
  countTick++;
  if(countTick > 10000)
    countTick=0;
}

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT);
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);

  Serial.println("mac address");
  //이름 자동으로 생성
  uint8_t macH[6]="";
  WiFi.macAddress(macH);
  sprintf(mac,"%02x%02x%02x%02x%02x%02x%c",macH[5], macH[4], macH[3], macH[2], macH[1], macH[0],0);
  sMac=mac;
  clientName=mac;
  Serial.println(mac);
  
  readConfig();
  if(ssid[0]==0)
    bootWifiAp();
  else 
    bootWifiStation();
    
  server.on("/", handleRoot);
  //server.on("/wifi", handleWifi);
  server.on("/wifisave", handleWifiSave);
  //server.on("/config", handleConfig);
  //server.on("/scan", handleScan);
  server.onNotFound(handleNotFound);

  server.begin();
  //webSocket.begin();
  Serial.println("HTTP server started");

  //ticker.attach(1, tick);  //0.1 초도 가능
  if(bootMode !=1) {
    // mqtt 설정
    client.setServer(ipMqtt, 1883);
    client.setCallback(callback);
  }
  Udp.begin(localUdpPort);
}

void bootWifiAp() {
  bootMode=1; //0:station  1:AP
  /* Soft AP network parameters */
  Serial.println("AP Mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  char i2rMac[30];
  sMac="i2r-"+sMac;
  sMac.toCharArray(i2rMac, sMac.length()+1);
  WiFi.softAP(i2rMac, "");
    ipAct=WiFi.softAPIP().toString();
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(ipAct);
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
  ipAct=WiFi.localIP().toString();
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(ipAct);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  //webSocket.loop();
  server.handleClient();
  //doTick();

  //공장리셋
  if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();

  if(bootMode !=1) {
    if(callMotor==1) {
      motor();
      callMotor=0;
    }
    udpEvent();
    if(mqttConnected==1)
      client.loop();
  }

}

//1초 마다 실행되는 시간함수
void doTick() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    tick();
  }  
}

void udpEvent() {
  int packetSize = Udp.parsePacket();
  if(packetSize) {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if(len > 0) {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    deserializeJson(doc,incomingPacket);
    root = doc.as<JsonObject>();
    String mqttIp = root["mqttIp"];
    mqttIp.toCharArray(ipMqtt, mqttIp.length()+1);
    Serial.println(ipMqtt);
    saveConfig();
  }  
}

void motor() {
  Serial.println("-----------------------");
  Serial.println(dir);
  Serial.println(vel);
  if(dir==0) {
    digitalWrite(INA1, LOW);
    digitalWrite(INA2, LOW);    
  }
  else if(dir==1) {
    digitalWrite(INA1, LOW);
    digitalWrite(INA2, HIGH);
  }
  else {
    digitalWrite(INA1, HIGH);
    digitalWrite(INA2, LOW);
  }
  analogWrite(ENA, vel);
}
