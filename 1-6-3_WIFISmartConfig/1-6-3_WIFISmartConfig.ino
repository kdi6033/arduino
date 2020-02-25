//1-6 https://youtu.be/2mqQm96d8TQ 프로그램을 기초로 수정
#include <ESP8266WiFi.h>

//const char* ssid = "i2r";
//const char* password = "00000000";

// Create an instance of the server

// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(9600);

  //WiFi.begin(ssid, password);

  WiFi.beginSmartConfig();
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
  s=s+"<head></head><body>SmartConfig 접속 안녕하세요!</body></html>";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed  

}
