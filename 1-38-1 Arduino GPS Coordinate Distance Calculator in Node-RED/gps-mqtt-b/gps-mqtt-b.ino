#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <arduino-timer.h>

auto timer = timer_create_default(); // create a timer with default settings

// 아래의 6개설정은 사용자 환경에 맞게 수정하세요.
const char* mqtt_server = "broker.mqtt-dashboard.com"; //브로커 주소
const char* outTopic = "/kdi/outTopic"; // 이름이 중복되지 않게 설정 기록
const char* inTopic = "/kdi/inTopic"; // 이름이 중복되지 않게 설정 기록
const char* clientName = "";  // setup 함수에서 자동생성
String sChipId;
char cChipId[20];

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];

long count=0;

//GPS 처리
double TargetLat = 37.816707; //GPS Data
double TargetLon = 126.801219;
double lat=0,lon=0;

void setup() {
  Serial.begin(115200);
  // 타이머  millis 마다 동작
  timer.every(5000, pulse);

  //이름 자동으로 생성
  sChipId=String(ESP.getChipId(),HEX);
  sChipId.toCharArray(cChipId,sChipId.length()+1);
  clientName=&cChipId[0];
  Serial.println(clientName);

  //WiFiManager
  WiFiManager wm;
  //wm.startConfigPortal("");
  //wm.resetSettings();
  if (!wm.autoConnect(cChipId)) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// 통신에서 문자가 들어오면 이 함수의 payload 배열에 저장된다.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
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

bool pulse(void *) {
  if(WiFi.status()==WL_CONNECTED) {
    //가상데이타 보냄
    count++;
    lat=TargetLat+(float)count*0.001;
    lon=TargetLon+(float)count*0.001;
    snprintf(msg, 75, "{\"id\":\"%s\", \"lat\":%f, \"lon\":%f}",cChipId, lat,lon);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(outTopic, msg);
  }
  return true; 
}

void loop() {
  timer.tick(); // tick the timer

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
