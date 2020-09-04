#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "SimpleTimer.h"
#include <ArduinoJson.h>

SimpleTimer timer;

// 아래의 6개설정은 사용자 환경에 맞게 수정하세요.
const char* ssid = "i2r"; // 와이파이 AP, 또는 스마트폰의 핫스판 이름
const char* password = "00000000";  // 와이파이 AP, 또는 스마트폰의 핫스판 이름
const char* mqtt_server = "broker.mqtt-dashboard.com"; //브로커 주소
const char* outTopic = "/kdi/inTopic"; // 이름이 중복되지 않게 설정 기록
const char* inTopic = "/kdi/outTopic"; // 이름이 중복되지 않게 설정 기록
const char* clientName = "";  // setup 함수에서 자동생성
String sChipID;
char cChipID[20];

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];

long count=0;

//GPS 처리
double TargetLat = 37.816707; //GPS Data
double TargetLon = 126.801219;
double lat=0,lon=0;
double distanceKm=0,distanceMiles=0,distanceFeet=0;

//json을 위한 설정
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

void setup() {
  Serial.begin(9600);
  setup_wifi();

  //이름 자동으로 생성
  sChipID=String(ESP.getChipId(),HEX);
  sChipID.toCharArray(cChipID,sChipID.length()+1);
  clientName=&cChipID[0];
  Serial.println(clientName);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //timer.setInterval(1000,gpsData);
}

void setup_wifi() {
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
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  deserializeJson(doc,payload);
  root = doc.as<JsonObject>();
  lat = root["lat"];
  lon = root["lon"];
  getDistance(lat,lon,TargetLat,TargetLon);
  Serial.println(distanceKm);

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

void gpsData() {
  //GPS를 연결하면 다음을 추가 한다.
  //lat=gps.location.lat();
  //lon=gps.location.lng();

  //가상데이타 보냄
  count++;
  lat=TargetLat+count*0.001;
  lon=TargetLon+count*0.001;
  snprintf(msg, 50, "{\"id\":\"%s\", \"lat\":%f, \"lon\":%f}",cChipID, lat,lon);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish(outTopic, msg);
}

// 이함수 참조사이트 : https://forum.arduino.cc/index.php?topic=281493.0
void getDistance(double lat, double lon, double targetLat, double targetLon)
{
  float latRad, lonRad;
  float flat, flon;
  float tlat, tlon;
  float tlatRad, tlonRad;
  float midLat, midLon;
  int samples=0;

  flat = lat;
  flon = lon;
  tlat = targetLat;
  tlon = targetLon;

  //convert decimal degree into radian
  latRad = flat * 0.017453293;
  lonRad = flon * 0.017453293;
  tlatRad = tlat * 0.017453293;
  tlonRad = tlon * 0.017453293;

  midLat = tlatRad - latRad;
  midLon = tlonRad - lonRad;

  //Calculate the distance in KM
  float latSin = sin((latRad - tlatRad)/2);
  float lonSin = sin((lonRad - tlonRad)/2);
  distanceKm = 2 * asin(sqrt((latSin*latSin) + cos(latRad) * cos(tlatRad) * (lonSin * lonSin)));
  distanceKm = distanceKm * 6371;

  //convert to miles
  distanceMiles = distanceKm * 0.621371192;
  distanceFeet = distanceMiles *5280;
}

void loop() {
  timer.run();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
