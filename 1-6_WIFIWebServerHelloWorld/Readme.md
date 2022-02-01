와이파이 연결
======================
와이파이에는 AP 모드와 Station 모두 두가지가 있다.

## 1.1 AP (Access Point) 모드?
AP모드란 공유기에서 내부 IP를 할당하지 않고 전달만 하게 하는 역할(허브, Hub)을 수행하기 위해 되는 모드입니다. ... AP모드가 된, 즉 허브가 된 공유기는 추가적인 포트포워딩 설정이나 DMZ설정 등 인터넷 설정을 적용받지 않으며 신호만 전달해 주는 역할을 합니다. 간단하게 집에서 사용하는 공유기라 생각하면 됩니다.

## 1.2 Station 모드
기존 AP(공유기)에 접속하는 방식이다. 공유기에서 DHCP로 IP를 할당 받을 수도 있고, 공유기 대역의 IP를 Static으로 설정할 수도 있다. 우리가 평상시 가정에서 사용하는 것이 AP 모드 입니다.

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
