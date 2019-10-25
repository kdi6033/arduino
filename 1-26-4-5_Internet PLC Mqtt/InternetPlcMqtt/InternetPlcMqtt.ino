#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

//json을 위한 설정
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

const char* ssid = "i2r";
const char* password = "00000000";

//mqtt 통신 변수
const char* mqtt_server = "broker.mqtt-dashboard.com"; //브로커 주소
const char* outTopic = "/kdi/outTopic"; // 이름이 중복되지 않게 설정 기록
const char* inTopic = "/kdi/inTopic"; // 이름이 중복되지 않게 설정 기록
const char* clientName = "";  // setup 함수에서 자동생성
String sChipID; // mac address를 문자로 기기를 구분하는 기호로 사용
char cChipID[20];
int value=0;
WiFiClient espClient;
PubSubClient client(espClient);

//plc 제어 변수
const int led = 2;
String s="";
int P4[4]={0};
int P0[6]={0};
int lastRead=0,lastMqtt=0,autoRead=0;
boolean stringComplete = false; 
String inputString;
char msg[50];

ESP8266WebServer server(80);

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  Serial.begin(9600);
  Serial1.begin(9600);

  //이름 자동으로 생성
  uint8_t chipid[6]="";
  WiFi.macAddress(chipid);
  sprintf(cChipID,"%02x%02x%02x%02x%02x%02x%c",chipid[5], chipid[4], chipid[3], chipid[2], chipid[1], chipid[0],0);
  sChipID=String(cChipID);
  
  connectWifi();
  server.on("/", handleRoot);
  server.on("/onoffP40", handleOnOffP40);
  server.on("/onoffP41", handleOnOffP41);
  server.on("/onoffP42", handleOnOffP42);
  server.on("/onoffP43", handleOnOffP43);
  server.on("/read", handleRead);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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

// 통신에서 문자가 들어오면 이 함수의 payload 배열에 저장된다.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    Serial1.print((char)payload[i]);
  }
  Serial.println();
}

// mqtt 통신에 지속적으로 접속한다.
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(cChipID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "Reconnected");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop(void) {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  server.handleClient();

  long now = millis();
  /*
  if ((now - lastMqtt > 3000)){
    lastMqtt = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(outTopic, msg);
  }
  */
  if ((now - lastRead > 1000)){
    lastRead = now;
    serialEvent();
    if(autoRead==1) {
      String s="";
      s=char(5);
      s+="00RSS0104%PW0";
      s+=char(4);
      Serial1.print(s);
    }
  }
}

void serialEvent() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    inputString.toCharArray(msg, inputString.length());
    client.publish(outTopic, msg);
    // clear the string:
    inputString = "";
    stringComplete = false;
    if(msg[3]=='R') {
        byte inC=msg[13];
        byte inC1=msg[12];
        P0[0]=bitRead(inC,0);
        P0[1]=bitRead(inC,1);
        P0[2]=bitRead(inC,2);
        P0[3]=bitRead(inC,3);
        P0[4]=bitRead(inC1,0);
        P0[5]=bitRead(inC1,1);
        GoHome();
    }
  }
  
  if(Serial.available() == false) 
    return;
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    //Serial.print(inChar);
    // add it to the inputString:
    inputString += inChar;
  }
  stringComplete = true;
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
