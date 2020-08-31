/* ver 1.1
 * 입력된 ip 주소가 없으면 기기는 AP 모드로 시작하며 Led에 항시 불이 들어온다.
 * 공유기 설정에서 i2r-***** 로 되어있는 ap 공유기를 선택한다. 비밀번호는 00000000 이다.
 * 공유기를 입력하기 위해 크롬의 주소창에 http://192.168.4.1 을 입력한다.
 * 공유기 설정에서 공유기 정보를 입력 후에 이름과 주소가 정상적으로 출력되면 기기를 리셋한다.
 * ip 주소는 EEPROM에 저장되고 Station 모드로 시작되고 정상적으로 통신에 연결되면
 * Led 가 0.5 간격으로 5번 깜박이고 꺼진다.
 * 공장 초기상태로 하려면 D3 핀을 GND와 연결한다.
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#define TRIGGER_PIN 0 // trigger pin 0(D3) 2(D4)
/* Set these to your desired credentials. */
char *softAP_ssid = "i2r-";
char *softAP_password = "";
char ssid[32] = "";
char password[32] = "";
/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
ESP8266WebServer server(80);

/* AP 설정을 위한 변수 선언 */
String sAP_ssid="",sMac="";
char cAP_ssid[20];
char cMac[15]; // Json에서 mac address를 고유 ID로 사용

boolean connect;
unsigned long lastConnectTry = 0;
const int led = 2;
int bootMode=0; //0:station  1:AP


void setup(void) {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  //이름 자동으로 생성
  uint8_t mac[6]="";
  WiFi.macAddress(mac);
  sprintf(cMac,"%02x%02x%02x%02x%02x%02x%c",mac[5], mac[4], mac[3], mac[2], mac[1], mac[0],0);
  sMac=String(cMac);
  sAP_ssid=String(softAP_ssid)+sMac;
  sAP_ssid.toCharArray(cAP_ssid,sAP_ssid.length()+1);
  softAP_ssid=&cAP_ssid[0];
  clientName=softAP_ssid; //mqtt 통신에서 사용
  Serial.println(sMac);
  Serial.println(softAP_ssid);

  loadCredentials();
  if(ssid[0]==0)
    setupAp();
  else
    connectWifi();

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/wifi", handleWifi);
  server.on("/wifisave", handleWifiSave);
  server.on("/scan", handleScan);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void setupAp() {
  bootMode=1;
  Serial.println("AP Mode");
  Serial.println(softAP_ssid);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  digitalWrite(led, 0); //상태 led로 표시
}

int connectWifi() {
  bootMode=0;
  Serial.println("STATION Mode");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); //이 모드를 설정 않해야 AP가 살아있습니다.
  //IPAddress ip(192, 168, 0, 201); // this 3 lines for a fix IP-address
  //IPAddress gateway(192, 168, 0, 1);
  //IPAddress subnet(255, 255, 255, 0);
  //WiFi.config(ip, gateway, subnet); // before or after Wifi.Begin(ssid, password);
  WiFi.begin(ssid, password);

  //referance: https://www.arduino.cc/en/Reference/WiFiStatus
  //WL_NO_SHIELD:255 WL_IDLE_STATUS:0 WL_NO_SSID_AVAIL:1 WL_SCAN_COMPLETED:2
  //WL_CONNECTED:3 WL_CONNECT_FAILED:4 WL_CONNECTION_LOST:5 WL_DISCONNECTED:6
  int connRes = WiFi.waitForConnectResult();
  Serial.print ( "connRes: " );
  Serial.println ( connRes );
  if(connRes == WL_CONNECTED){
    Serial.println("well connected");
    Serial.println(WiFi.localIP());
  }
  else
    WiFi.disconnect();
  // 상태 led로 표시
  for(int i=0;i<5;i++) {
    digitalWrite(led, 0);
    delay(500);
    digitalWrite(led, 1);
    delay(500);
  }
  return connRes;
}

void loop(void) {
  server.handleClient();

  long now = millis();
  //6초에 한번 와이파이 끊기면 다시 연결
  unsigned int sWifi = WiFi.status();
  if (sWifi == WL_IDLE_STATUS && now > (lastConnectTry + 60000) && strlen(ssid) > 0 ) {
    lastConnectTry=now;
    Serial.println ( "Connect requested" );
    connectWifi();
  }

  if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();
}
