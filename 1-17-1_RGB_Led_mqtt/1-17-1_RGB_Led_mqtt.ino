//1-16-1, 1-17  두개의 프로그램을 합쳐서 만듬
#include <stdlib.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// 아래의 6개설정은 사용자 환경에 맞게 수정하세요.
const char* ssid = "i2r"; // 와이파이 AP, 또는 스마트폰의 핫스판 이름
const char* password = "00000000";  // 와이파이 AP, 또는 스마트폰의 핫스판 이름
const char* mqtt_server = "broker.mqtt-dashboard.com"; //브로커 주소
const char* outTopic = "/kdi/inLight"; // 이름이 중복되지 않게 설정 기록
const char* inTopic = "/kdi/outLight"; // 이름이 중복되지 않게 설정 기록
const char* clientName = "980303Client";  // 다음 이름이 중복되지 않게 꼭 수정 바람 - 생년월일 추천

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];

//int led=4; // D2 GPIO4 핀을 사용
int ledRed=12; // D6 IO12 핀을 사용
int ledGreen=13; // D7 IO13 핀을 사용
int ledBlue=15; // D8 IO15 핀을 사용
int DutyRed=100;
int DutyGreen=100;
int DutyBlue=100;
int timeIn=1000;  // led가 깜박이는 시간을 mqtt 통신에서 전달받아 저장

void setup() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// 통신에서 문자가 들어오면 이 함수의 payload 배열에 저장된다.
void callback(char* topic, byte* payload, unsigned int length) {
  if(length < 5)
    return;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  char szHex[2];
  for (int i = 0; i < 2; i++) 
    szHex[i] = (char)payload[i];
  DutyRed = (int)strtol(szHex, NULL, 16)*4.01;
  
  for (int i = 2; i < 4; i++) 
    szHex[i-2] = (char)payload[i];
  DutyGreen = (int)strtol(szHex, NULL, 16)*4.01;
 
  for (int i = 4; i < 6; i++) 
    szHex[i-4] = (char)payload[i];
  DutyBlue = (int)strtol(szHex, NULL, 16)*4.01;
  
  Serial.print("R G B   ");
  Serial.print(DutyRed);
  Serial.print(", ");
  Serial.print(DutyGreen);
  Serial.print(", ");
  Serial.print(DutyBlue);
  Serial.println("");
}

// mqtt 통신에 지속적으로 접속한다.
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientName)) {
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

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // 들어온 Duty 값에 따라 led가 색을 낸다.
  analogWrite(ledRed, DutyRed);
  analogWrite(ledGreen, DutyGreen);
  analogWrite(ledBlue, DutyBlue);
}
