#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);

String sChipId="";
char cChipId[40]="";
int count=0,actIp[255]={0};

String s1="http://192.168.0.";
char msg[100];
int Type=0; //Type : 0-OnOff전등  1-color전등  2-디밍전등
int On[255]={0}; // Bringht Red Green Blue
int i=0;


void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  Serial.begin(115200);
  WiFiManager wm;
  wm.setSTAStaticIPConfig(IPAddress(192,168,0,199), IPAddress(192,168,0,1), IPAddress(255,255,255,0));  // set static ip,gw,sn
  
  // AP 이름 자동으로 만듬 chipid
  sChipId = String(ESP.getChipId(),HEX);
  sChipId.toCharArray(cChipId,sChipId.length()+1);
  Serial.println("");
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
  wifiScan();
}

void loop() {
  server.handleClient();
}

void wifiScan() {
  int i;
  String s1="http://192.168.0.",s;

  count=0;
  for(int i=1;i<255;i++) {
    s=s1+String(i)+"/scan"; 
    Serial.println(s);
    if ((WiFi.status() == WL_CONNECTED)) {
      WiFiClient client;
      HTTPClient http;
  
      Serial.print("[HTTP] begin...\n");
      if (http.begin(client, s)) {  // HTTP
        http.setTimeout(200);       // http 응답 대기시간 설정
        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
  
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            Serial.println(payload);
            actIp[count++]=i;
          }
        } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
      } else {
        Serial.printf("[HTTP} Unable to connect\n");
      }
    }
  }  
  Serial.print("count:");
  Serial.println(count);
  for(i=0;i<count;i++)
    Serial.println(actIp[i]);
}

void sendHttp(int ipNo, int on) {
  String s=s1+String(ipNo);
  if(on==1) 
    s+="/on";
  else
    s+="/off";
  Serial.println(s);
  WiFiClient client;
  HTTPClient http;
  http.begin(client, s);
  http.setTimeout(200);
  int httpCode = http.GET();
  Serial.println(httpCode);
  http.end();
  //GoHome();

  //handleRoot();
  /*
  s=s1+"199/";
  Serial.println(s);
  WiFiClient client1;
  HTTPClient http1;
  http.begin(client1, s);
  http1.setTimeout(200);
  httpCode = http1.GET();
  Serial.println(httpCode);
  http.end();
  */
}
