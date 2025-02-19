와이파이 연결
======================
와이파이에는 AP 모드와 Station 모두 두가지가 있다.

## 1.1 AP (Access Point) 모드?
AP모드란 공유기에서 내부 IP를 할당하지 않고 전달만 하게 하는 역할(허브, Hub)을 수행하기 위해 되는 모드입니다. ... AP모드가 된, 즉 허브가 된 공유기는 추가적인 포트포워딩 설정이나 DMZ설정 등 인터넷 설정을 적용받지 않으며 신호만 전달해 주는 역할을 합니다. 간단하게 집에서 사용하는 공유기라 생각하면 됩니다.

## 1.2 Station 모드
기존 AP(공유기)에 접속하는 방식이다. 공유기에서 DHCP로 IP를 할당 받을 수도 있고, 공유기 대역의 IP를 Static으로 설정할 수도 있다. 우리가 평상시 가정에서 사용하는 것이 AP 모드 입니다.

다음은 유튜브에 소개한 와이파이 연결 프로그램 입니다. Statin 모드로 접속하여 AP에서 DHCP로 할당된 IP로 접속하면 여기서 제공하는 웹페이지를 볼수 있습니다.
## 아두이노 소스 프로그램 

```
#include <ESP8266WiFi.h>

const char* ssid = "i2r";
const char* password = "00000000";

// Create an instance of the server

// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);
  
  // Connect to WiFi network
  Serial.println();
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
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  //while(!client.available()){
  //  delay(1);
  //}
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Prepare the response
  String s="<html>";
  s=s+"<meta name='viewport' content='width=device-width, initial-scale=1.0'/>";
  //s=s+"<meta http-equiv='refresh' content='5'/>";
  s=s+"<meta http-equiv='Content-Type' content='text/html;charset=utf-8' />";
  s=s+"<head></head><body>안녕하세요!</body></html>";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed  

}
```
## 1.3 아두이노 Station, AP 모드 기본프로그램
아두이노 프로그램에서 "파일->예제->ESP8266WiFi->WiFiClient.ino" 을 열고 와이파이 접속을 프로그램 해 봅니다.

## 1.4 아두이노 Station, AP 모드 프로그램
모든 무선통신 CPU는 고유의 mac address를 가지고 있습니다. 그러므로 제 프로그램에서는 이를 CPU를 구성하는 고유의 이름으로 사용하면 프로그램에서 중복되는 일이 없습니다. 이 이름으로 와이파이 무선에 접속합니다.
프로그램에서 ssid, password는 사용하고 있는 AP의 정보를 입력해서 사용하세요.
```
#include <ESP8266WiFi.h>

char ssid[40] = "**************";
char password[50] = "******************";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

void bootWifiAp() {
  /* Soft AP network parameters */
  Serial.println("AP Mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP("testAP", "");
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP().toString());
}

void bootWifiStation() {
  //referance: https://www.arduino.cc/en/Reference/WiFiStatus
  //WL_NO_SHIELD:255 WL_IDLE_STATUS:0 WL_NO_SSID_AVAIL:1 WL_SCAN_COMPLETED:2
  //WL_CONNECTED:3 WL_CONNECT_FAILED:4 WL_CONNECTION_LOST:5 WL_DISCONNECTED:6
  //WiFi 연결
  Serial.println("Station Mode");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP().toString());
}

void setup() {
  Serial.begin(9600);
  bootWifiAp();
  //bootWifiStation();

}

void loop() {
  // put your main code here, to run repeatedly:

}
```

## 1.5 Web 프로그램
아두이노로 웹 프로그램을 만들어 크롬에서 IP 주소를 가지고 아두이노를 직접 접속한다.
```
//wifiWebServer.ino
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

char ssid[40] = "405902-2.4G";
char password[50] = "k01033887147";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

ESP8266WebServer server(80);

void bootWifiAp() {
  /* Soft AP network parameters */
  Serial.println("AP Mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP("testAP", "");
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP().toString());
}

void bootWifiStation() {
  //referance: https://www.arduino.cc/en/Reference/WiFiStatus
  //WL_NO_SHIELD:255 WL_IDLE_STATUS:0 WL_NO_SSID_AVAIL:1 WL_SCAN_COMPLETED:2
  //WL_CONNECTED:3 WL_CONNECT_FAILED:4 WL_CONNECTION_LOST:5 WL_DISCONNECTED:6
  //WiFi 연결
  Serial.println("Station Mode");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP().toString());
}

void setup() {
  Serial.begin(9600);
  //bootWifiAp();
  bootWifiStation();
  server.on("/", handleRoot);
  
  server.begin();
}

void loop() {
  server.handleClient();

}
```
```
//handleHttp.ino
void handleRoot() {
  server.send(200, "text/html","Arduino Web Server");
}
```

## 1.6 Web 프로그램 (mac address, CSS)
- 아두이노에서 mac address를 읽어와 이를 CPU의 이름으로 사용하여 와이파이 검색창에서 이 이름이 나타나도록 한다.
- 아두이노에서 CSS를 정의 하여 입력과 출력에 사용되는 버튼을 만든다.

wifiWebServer.ino
```
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

char ssid[40] = "405902-2.4G";
char password[50] = "k01033887147";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

const char* clientName = "";  // setup 함수에서 자동생성
String ipAct="";
char mac[20];  //mac address
String sMac;
int bootMode=0; //0:station  1:AP

ESP8266WebServer server(80);

void bootWifiAp() {
  bootMode=1; //0:station  1:AP
  /* Soft AP network parameters */
  Serial.println("AP Mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  char i2rMac[30];
  sMac="i2r-"+sMac;
  sMac.toCharArray(i2rMac, sMac.length()+1);
  WiFi.softAP(i2rMac, "");
    ipAct=WiFi.softAPIP().toString();
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(ipAct);
}

void bootWifiStation() {
  //referance: https://www.arduino.cc/en/Reference/WiFiStatus
  //WL_NO_SHIELD:255 WL_IDLE_STATUS:0 WL_NO_SSID_AVAIL:1 WL_SCAN_COMPLETED:2
  //WL_CONNECTED:3 WL_CONNECT_FAILED:4 WL_CONNECTION_LOST:5 WL_DISCONNECTED:6
  //WiFi 연결
  bootMode=0; //0:station  1:AP
  Serial.println("Station Mode");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  ipAct=WiFi.localIP().toString();
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(ipAct);
}

void setup() {
  Serial.begin(9600);
  Serial.println("mac address");
  //이름 자동으로 생성
  uint8_t macH[6]="";
  WiFi.macAddress(macH);
  sprintf(mac,"%02x%02x%02x%02x%02x%02x%c",macH[5], macH[4], macH[3], macH[2], macH[1], macH[0],0);
  sMac=mac;
  clientName=mac;
  Serial.println(mac);

  //bootWifiAp();
  bootWifiStation();
  
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();

}
```

handleHttp.ino
```
String webTail="</body> </html>";
//***********************************************************************
char Head[] PROGMEM = R"=====(
<html>
  <head>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
    <meta http-equiv='Content-Type' content='text/html;charset=utf-8' />
    <style>
      table, th, td {
        padding: 4px;
      }
      body {
        background: #eab0dc;
        font-family: "Lato", sans-serif;
      }
      .button {
        border: none;
          border-color:black;
          color: white;
          padding: 20px;
          text-align: center;
          text-decoration: none;
          display: inline-block;
          font-size: 16px;
          margin: 4px 2px;
          cursor: pointer;
        }
        .buttonMenu {
          padding: 5px 24px;
          margin-left:20%;
          background-color:black;
          border: none;
          border-color:black;
          color:white;
          text-align: left;
          text-decoration: none;
          display: inline-block;
          font-size: 16px;
          margin: 4px 2px;
          cursor: pointer;
        }
        .sidenav {
          height: 100%;
          width: 0;
          position: fixed;
          z-index: 1;
          top: 0;
          left: 0;
          background-color: #111;
          overflow-x: hidden;
          transition: 0.5s;
          padding-top: 60px;
        }
        .sidenav a {
          padding: 8px 8px 8px 32px;
          text-decoration: none;
          font-size: 18px;
          color: #818181;
          display: block;
                transition: 0.3s;
        }
        .sidenav a:hover {
          color: #f1f1f1;
        }
        .sidenav .closebtn {
          position: absolute;
          top: 0;
          right: 25px;
          font-size: 36px;
          margin-left: 50px;
        }
        .button-box {background-color:#ff8000;color: white;border: none;padding: 6px 15px;}
        .button-on {border-radius: 100%; background-color: #4CAF50;}
        .button-off {border-radius: 100%;background-color: #707070;}
        .button-ledon {border-radius: 100%; padding: 10px; font-size: 8px; margin: 0px 0px; background-color: #ff4500;}
        .button-ledoff {border-radius: 100%; padding: 10px; font-size: 8px; background-color: #707070;}
  </style>
)=====";

char Body[] PROGMEM = R"=====(
  <br>입력과 출력<br>
  <button class='button button-on'></button>
  <button class='button button-off'></button>
  <button class='button button-ledon'></button>
  <button class='button button-ledoff'></button>

)=====";

void handleRoot() {
  String s;
  s=FPSTR(Head);
  s+=FPSTR(Body);
  s+=webTail;
  server.send(200, "text/html", s);
}
```
