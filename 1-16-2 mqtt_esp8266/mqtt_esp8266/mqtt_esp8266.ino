#include <ESP8266WiFi.h>
#include <PubSubClient.h>

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

//int led=4; // D2 GPIO4 핀을 사용
int led=BUILTIN_LED; // D1 mini에 있는 led를 사용
int timeIn=1000;  // led가 깜박이는 시간을 mqtt 통신에서 전달받아 저장

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  setup_wifi();

  //이름 자동으로 생성
  sChipID=String(ESP.getChipId(),HEX);
  sChipID.toCharArray(cChipID,sChipID.length());
  clientName=&cChipID[0];
  Serial.println(clientName);
  /* Topic 이름 자동생성 ChipId/outTopic  ChipId/inTopic
  String s;
  s=sChipID+"/outTopic";
  s.toCharArray(cChipID,s.length());
  outTopic=&cChipID[0];
  s=sChipID+"/inTopic";
  s.toCharArray(cChipID,s.length());
  inTopic=&cChipID[0];
  */

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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

  // payload로 들어온 문자를 정수로 바꾸기 위해 String inString에 저장후에
  // toInt() 함수를 사용해 정수로 바꾸어 timeIn에 저장한다.
  String inString="";
  for (int i = 0; i < length; i++) {
    inString += (char)payload[i];
  }
  timeIn=inString.toInt();
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

  // 들어온 timeIn 값에 따라 led가 점멸하게 한다.
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(timeIn);                       // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(timeIn); 
}
