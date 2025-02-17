#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "i2r";          // Wi-Fi SSID
const char* password = "00000000"; // Wi-Fi 비밀번호

WebServer server(80); // HTTP 서버 포트 80번 사용
const int ledPin = 47; // GPIO 47번 핀 사용
bool ledState = false; // LED 상태 저장

// 웹페이지 (HTML + JavaScript)
const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32-S3 Web Server</title>
    <script>
        function sendCommand(state) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/toggle?state=" + state, true);
            xhr.send();
        }
    </script>
</head>
<body>
    <h1>ESP32-S3 GPIO 47 Control</h1>
    <button onclick="sendCommand('on')">ON</button>
    <button onclick="sendCommand('off')">OFF</button>
</body>
</html>
)rawliteral";

// "/" 경로 요청 시 HTML 페이지 전송
void handleRoot() {
    server.send(200, "text/html", MAIN_page);
}

// GPIO 47 ON/OFF 제어
void handleToggle() {
    if (server.hasArg("state")) {
        String state = server.arg("state");
        if (state == "on") {
            digitalWrite(ledPin, HIGH);
            ledState = true;
        } else if (state == "off") {
            digitalWrite(ledPin, LOW);
            ledState = false;
        }
    }
    server.send(200, "text/plain", ledState ? "ON" : "OFF");
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    pinMode(ledPin, OUTPUT); // GPIO 47번 핀 출력으로 설정
    digitalWrite(ledPin, LOW); // 초기 상태 OFF

    // MDNS 설정
    if (MDNS.begin("esp32")) {
        Serial.println("MDNS responder started");
    }

    // HTTP 라우트 설정
    server.on("/", handleRoot);
    server.on("/toggle", handleToggle);
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}

