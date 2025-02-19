#include <WiFi.h>

const char *ssid = "ESP32-AP";      // AP SSID
const char *password = "12345678";  // AP 비밀번호

// Output pin numbers
const int outputPins[21] = {7,6,5,4,3,2,1,8,9,10,11,12,13,14,15,16,21,38,47,18,17};

WiFiServer server(80);

void setup() {
    Serial.begin(115200);
    WiFi.softAP(ssid, password); // SoftAP 설정
    Serial.print("AP IP 주소: ");
    Serial.println(WiFi.softAPIP());

    // 모든 핀을 출력으로 설정
    for (int i = 0; i < 21; i++) {
        pinMode(outputPins[i], OUTPUT);
        digitalWrite(outputPins[i], LOW); // 초기 상태 OFF
    }
    delay(100);
    controlPins(0);

    server.begin();
}

// count 값을 받아서 핀을 ON/OFF하는 함수
void controlPins(int count) {
  for (int i = 0; i < 21; i++) {
    digitalWrite(outputPins[i], HIGH);
    delay(10);  
    digitalWrite(outputPins[i], LOW);
    delay(10); 
  }
}

void loop() {
    WiFiClient client = server.available(); // 클라이언트 접속 확인
    if (client) {
        //Serial.println("클라이언트 연결됨");
        while (client.connected()) {
            if (client.available()) {
                String receivedData = client.readStringUntil('\n'); // 데이터 수신
                Serial.print("수신 데이터: ");
                Serial.println(receivedData);
                // 숫자 변환
                int count = receivedData.toInt();
                controlPins(count); // 핀 제어 함수 호출
            }
        }
        client.stop(); // 클라이언트 연결 종료
        //Serial.println("클라이언트 연결 종료");
    }
}

