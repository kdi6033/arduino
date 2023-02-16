// 4채널 보드 v1.5
// 릴레이 출력핀 확인 소스 입니다. 2023-02-14

#define RELAY1_PIN 33
#define RELAY2_PIN 32
#define RELAY3_PIN 27
#define RELAY4_PIN 26

void setup() {
pinMode(RELAY1_PIN, OUTPUT);  //릴레이 출력핀 세팅
pinMode(RELAY2_PIN, OUTPUT);
pinMode(RELAY3_PIN, OUTPUT);
pinMode(RELAY4_PIN, OUTPUT);

digitalWrite(RELAY1_PIN,0);   //전체 off
digitalWrite(RELAY2_PIN,0);
digitalWrite(RELAY3_PIN,0);
digitalWrite(RELAY4_PIN,0);
delay(500);
}

void loop() {  
digitalWrite(RELAY1_PIN,1);   // 1~4 릴레이 on/off
delay(1000);
digitalWrite(RELAY1_PIN,0);
delay(1000);

digitalWrite(RELAY2_PIN,1);
delay(1000);
digitalWrite(RELAY2_PIN,0);
delay(1000);

digitalWrite(RELAY3_PIN,1);
delay(1000);
digitalWrite(RELAY3_PIN,0);
delay(1000);

digitalWrite(RELAY4_PIN,1);
delay(1000);
digitalWrite(RELAY4_PIN,0);
delay(1000);
}
