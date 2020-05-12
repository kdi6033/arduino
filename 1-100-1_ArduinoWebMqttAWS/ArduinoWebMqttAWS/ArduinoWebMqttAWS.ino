#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include <time.h>
#include <ArduinoJson.h>
#define TRIGGER_PIN 0 // trigger pin 0(D3) 2(D4)

ESP8266WebServer server(80);

//json을 위한 설정
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

// AP 설정을 위한 변수
const char *softAP_ssid = "candle-";
const char *softAP_password = "";
char ssid[32] = "";
char password[32] = "";
/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

/* AP 설정을 위한 변수 선언 */
String sAP_ssid,sChipID;
char cAP_ssid[20];
char cChipID[20]; // Json에서 chipid를 고유 ID로 사용

boolean bConnect=0;
unsigned long lastConnectTry = 0;
const int led = 2;
int bootMode=0; //0:station  1:AP

//mqtt 통신 변수
const char *thingId = "";          // 사물 이름 (thing ID) 
const char *host = "******-ats.iot.us-east-2.amazonaws.com"; // AWS IoT Core 주소
const char* outTopic = "outCandleAWS"; // 사용자가 결정해서 기록
const char* inTopic = "inCandleAWS"; // 사용자가 결정해서 기록
long now;
int sA0 = A0; // Analog Input
int nowBlindState=0; //현재 브라인드 상태

long lastMsg = 0;
char msg[100];
int value = 0;

String inputString;

// 사물 인증서 (파일 이름: xxxxxxxxxx-certificate.pem.crt)
const char cert_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUPC+AxKKjE2XKCR1t7NN+xslY32owDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMDUwNjA3MTcy
MFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOzAWz2sVLhF/O/GoCaJ
XsHFjwcwkJtrTtJr3h/Oj35EmHL8F+odBXFiOlEFq8giqL+pFvG0xe0ppb5u0pLe
fnsDtVhD0Vtvm6zn84vIMJwry6ddrEkZkrD068l65swSB///cIWN2cs4kt4z1UjY
cSjdr25lu8E9dLnhRLQ9qG2AfF60kciWItWDnDGhxgJgPGwXllA/2zgEb6qNLMXE
S0VmrVJ0wvUDf7bJ7xXx2RFNywZ5rxKtZAnlHntZ605cvlKmx/D+JmnZ1nBKcnjD
5OYTjmwUD9dhbcUF/6tjeGLxACY6ztcgaKn9gfitbL6ekG0FM/6SFMx3U+LSKSJO
-----END CERTIFICATE-----
)EOF";
// 사물 인증서 프라이빗 키 (파일 이름: xxxxxxxxxx-private.pem.key)
const char key_str[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEA7MBbPaxUuEX878agJolewcWPBzCQm2tO0mveH86PfkSYcvwX
6h0FcWI6UQWryCKov6kW8bTF7Smlvm7Skt5+ewO1WEPRW2+brOfzi8gwnCvLp12s
SRmSsPTryXrmzBIH//9whY3ZyziS3jPVSNhxKN2vbmW7wT10ueFEtD2obYB8XrSR
yJYi1YOcMaHGAmA8bBeWUD/bOARvqo0sxcRLRWatUnTC9QN/tsnvFfHZEU3LBnmv
Eq1kCeUee1nrTly+UqbH8P4madnWcEpyeMPk5hOObBQP12FtxQX/q2N4YvEAJjrO
1yBoqf2B+K1svp6QbQUz/pIUzHdT4tIpIk4kAQIDAQABAoIBAGxF/HRUH9q2h1rR
J+T35hIQW70aBMhPZxjspAU/3ngMvvu3NuimpRqZpbnwMgOxdiNRHWElH2+7IEXb
QQpSHt37cDARs+lxPWzyF3vR7gk+LkE4odY8j83F9R+LyYmCpY36MGej2/sIwIJG
ikBqXDYoTFbcx+U0SfHIiDc2C5Un0IYRHSYGf9q9ptz8+YnAnGCSkFr3W2fBClQd
EgyY6RYxnzQFoGqdoVYqp6BS9zDkqo0cTU1dQYXJfBEx9RbwE8YCl84Pkq7uR+kT
O/zaKEP3jCoHAtQgBtlfHLnakTeWT0oc2aeucMZR9gv/V+m0TCiyCMBA2r5RWQKg
PsPLAVECgYEA+V2sRgMFZHFMbHoYmYpmIInIYlQkx+K4fWfCGRV0TCa2rxa6PzQH
-----END RSA PRIVATE KEY-----
)EOF";
// Amazon Trust Services(ATS) 엔드포인트 CA 인증서 (서버인증 > "RSA 2048비트 키: Amazon Root CA 1" 다운로드)
const char ca_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
-----END CERTIFICATE-----
)EOF";

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  //Serial.setDebugOutput(true);
  
  // AP 이름 자동으로 만듬
  sChipID=String(ESP.getChipId(),HEX);
  sChipID.toCharArray(cChipID,sChipID.length()+1);
  sAP_ssid=String(softAP_ssid)+String(ESP.getChipId(),HEX);
  sAP_ssid.toCharArray(cAP_ssid,sAP_ssid.length()+1);
  softAP_ssid=&cAP_ssid[0];
  thingId=&sChipID[0]; //mqtt 통신에서 사용
  Serial.println("chipID:");
  Serial.println(thingId);
  loadCredentials();
  
  if(ssid[0]==0)
    setupAp();
  else
    connectWifi();
  
  server.on("/", handleRoot);
  server.on("/wifi", handleWifi);
  server.on("/wifisave", handleWifiSave);
  server.on("/scan", handleScan);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

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
  const char* rChip = root["chip"];
  //snprintf (msg, 100, "{%s %d %d %d %d}",cChipID,Bright,R,G,B);
  //Serial.println(msg);

  if(!rChip)
    return;

  //if(strcmp(rChip,cChipID)==0) {
    //Bright = root["Bright"];
  //}
}

X509List ca(ca_str);
X509List cert(cert_str);
PrivateKey key(key_str);
WiFiClientSecure wifiClient;
PubSubClient client(host, 8883, callback, wifiClient); //set  MQTT port number to 8883 as per //standard

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(thingId)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "hello world");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      char buf[256];
      wifiClient.getLastSSLError(buf,256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Set time via NTP, as required for x.509 validation
void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void setupAp() {
  bootMode=1;
  Serial.println("AP Mode Connecting...");
  Serial.println(softAP_ssid);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  digitalWrite(led, 0); //상태 led로 표시
}

void connectWifi() {
  bootMode=0;
  Serial.println("STATION Mode Connecting...");
  WiFi.disconnect();
  //WiFi.mode(WIFI_STA); //이 모드를 설정 않해야 AP가 살아있습니다.
  WiFi.begin(ssid, password);

  //referance: https://www.arduino.cc/en/Reference/WiFiStatus
  //WL_NO_SHIELD:255 WL_IDLE_STATUS:0 WL_NO_SSID_AVAIL:1 WL_SCAN_COMPLETED:2
  //WL_CONNECTED:3 WL_CONNECT_FAILED:4 WL_CONNECTION_LOST:5 WL_DISCONNECTED:6
  int connRes = WiFi.waitForConnectResult();
  Serial.print ( "connRes: " );
  Serial.println ( connRes );
  if(connRes == WL_CONNECTED){
    wifiClient.setTrustAnchors(&ca);
    wifiClient.setClientRSACert(&cert, &key);
    setClock();
    client.setCallback(callback);
    //Serial.println("Certifications and key are set");
    Serial.println("AWS connected");
    Serial.println(WiFi.localIP());
    // 상태 led로 표시
    for(int i=0;i<5;i++) {
      digitalWrite(led, 0);
      delay(500);
      digitalWrite(led, 1);
      delay(500);
    }
  }
  //else
  //  WiFi.disconnect();
}

void loop() {
  server.handleClient();
  
  unsigned int sWifi = WiFi.status();
  now = millis();

  //6초에 한번 와이파이 끊기면 다시 연결
  if(sWifi == WL_CONNECTED)
    doMqtt();
  else if (sWifi == WL_IDLE_STATUS && now > (lastConnectTry + 50000) && strlen(ssid) > 0 ) {
    lastConnectTry=now;
    Serial.println ( "Connect requested" );
    connectWifi();
  }

  //Factory Reset
  if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();
}

void doMqtt() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (now - lastMsg > 5000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    //float i=(float)random(0,9)/10;
    //snprintf (msg, 100, "{\"chip\":\"%s\",\"dust\":}",cChipID);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(outTopic, msg);
    //Serial.print("Heap: "); Serial.println(ESP.getFreeHeap()); //Low heap can cause problems
  }
}
