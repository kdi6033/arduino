#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <WiFiClient.h>

String sChipId="";
char cChipId[40]="";

char msg[100];
int Type=0; //Type : 0-OnOff전등  1-color전등  2-디밍전등
int On=0; // Bringht Red Green Blue
int i=0;

//sonoff
int gpio13Led = LED_BUILTIN;
//int gpio13Led = 13;
int gpio12Relay = 12;

ESP8266WebServer server(80);

void setup() {
  //sonoff
  pinMode(gpio13Led, OUTPUT);
  pinMode(gpio12Relay, OUTPUT);
  digitalWrite(gpio13Led, HIGH);
  digitalWrite(gpio12Relay,LOW);  
  
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  Serial.begin(115200);
  WiFiManager wm;
  
  // AP 이름 자동으로 만듬 chipid
  sChipId = "i2r-"+String(ESP.getChipId(),HEX);
  sChipId.toCharArray(cChipId,sChipId.length()+1);
  Serial.println(sChipId);

  //reset settings - wipe credentials for testing
  //wm.resetSettings();

  if (!wm.autoConnect(cChipId)) {
    Serial.println("failed to connect and hit timeout");
    ESP.restart();
    delay(1000);
  }

  Serial.print("connected... :");
  Serial.print(WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/scan", handleScan);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
    server.handleClient();
    
}
