#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "i2r";
const char* password = "00000000";

ESP8266WebServer server(80);

int count=0;
String s;

void handleRoot() {
  count++;
  s="";
  s=s+"{\"data\":"+count+","+"\"데이타\":"+(count+1)+"}";
  server.send(200, "text/plain", s);
  //server.send(200, "text/html", s);
  
}

void setupWifi() {
  WiFi.mode(WIFI_STA);
  IPAddress ip(192, 168, 0, 251); // DHCP 서버에서 IP 할당을 1~250 까지 한다.
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet); //Wifi.Begin(ssid, password); 전에 선언
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup(void){
  Serial.begin(115200);
  setupWifi();
  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
