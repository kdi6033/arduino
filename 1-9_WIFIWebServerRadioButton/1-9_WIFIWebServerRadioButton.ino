/*
 ip로 접속
 라디오버튼 핸들링
 */
#include <ESP8266WiFi.h>

const char* ssid = "i2r";
const char* password = "00000000";
const int ledPin =  2;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  
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
  Serial.println("new client");

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  Serial.println(req.indexOf("/led.cgi?status=1"));
  if (req.indexOf("/led.cgi?status=1") > 0) 
    digitalWrite(ledPin, LOW);
   else if(req.indexOf("/led.cgi?status=0") > 0) 
    digitalWrite(ledPin, HIGH);

  // Prepare the response
  //표준 http 응답 헤더 전송 시작
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          
          //본문(상황에 맞는 웹 페이지) 전달
          //client.println("<!DOCTYPE HTML>");, HTML5 사용시
          client.println("<html>");  //웹 페이지 작성은 HTML사용
          client.println("<body>");
   //색 선택 부분 만들기
          client.println("<br />");
          client.println("<FORM method=\"get\" action=\"/led.cgi\">");
          client.println("<P> <INPUT type=\"radio\" name=\"status\" value=\"1\">RED");
          client.println("<P> <INPUT type=\"radio\" name=\"status\" value=\"2\">GREEN");
          client.println("<P> <INPUT type=\"radio\" name=\"status\" value=\"3\">BLUE");
          client.println("<P> <INPUT type=\"radio\" name=\"status\" value=\"0\">OFF");
          client.println("<P> <INPUT type=\"submit\" value=\"Submit\"> </FORM>");

          client.println("</body>");
          client.println("</html>");


  delay(1);
  Serial.println("Client disonnected");

}
