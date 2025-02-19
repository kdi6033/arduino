#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 48      // RGB LED가 연결된 핀 번호
#define NUM_PIXELS 1    // 제어할 LED 개수
#define BUTTON_PIN 41   // 버튼이 연결된 핀 번호

Adafruit_NeoPixel strip(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

const char *ssid = "ESP32-AP";      // 서버 ESP32 AP SSID
const char *password = "12345678";  // AP 비밀번호
const char *serverIP = "192.168.4.1"; // AP의 기본 IP
const int serverPort = 80;

int count = 0; // 전송할 카운트 값

// RGB 색상 설정 함수
void setColor(uint8_t red, uint8_t green, uint8_t blue) {
  strip.setPixelColor(0, strip.Color(red, green, blue)); // 첫 번째 LED의 색상 설정
  strip.show();                                         // 설정한 색상을 출력
}

void setup() {
    Serial.begin(115200);
    strip.begin();         // NeoPixel 초기화
    strip.show();          // LED 끄기
    pinMode(BUTTON_PIN, INPUT_PULLUP); // 버튼을 풀업 입력으로 설정

    WiFi.begin(ssid, password); // AP에 연결

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi 연결 완료!");
}

void loop() {
    WiFiClient client;

    if (client.connect(serverIP, serverPort)) {
        Serial.print("서버 연결됨, count: ");
        Serial.println(count);

        client.println(String(count)); // count 값 전송
        count++; // count 증가
        delay(2000); // 2초마다 전송
    } else {
        Serial.println("서버 연결 실패");
        delay(1000);
    }

    // 버튼이 눌리면 (LOW 상태)
    if (digitalRead(BUTTON_PIN) == LOW) {
        Serial.println("버튼 눌림: 파란색 LED ON");
        setColor(0, 0, 255); // 파란색 LED 켜기
        delay(100);          // 0.1초 대기
        setColor(0, 0, 0);   // LED 끄기
        Serial.println("파란색 LED OFF");
    }
}
