/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/
/*
 4채널 릴레이보드 WiFi 테스트 소스입니다.
 보드 2개를 사용하는 BLE 테스트와 달리
 보드 1개만 사용하시면 됩니다

 보드 업로드 후 시리얼 모니터에 나타나는 ip에 접속해주세요
 버튼으로 릴레이 제어가 가능합니다.
 */

#include <WiFi.h>

#define RELAY1_PIN 33
#define RELAY2_PIN 32
#define RELAY3_PIN 27
#define RELAY4_PIN 26

const char* ssid = "mecha1203";
const char* password = "mecha1203";

WiFiServer server(80);

String header;

String relay1state = "off";
String relay2state = "off";
String relay3state = "off";
String relay4state = "off";

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  pinMode(RELAY1_PIN, OUTPUT);  //릴레이 출력핀 세팅
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);

  digitalWrite(RELAY1_PIN,0);   //전체 off
  digitalWrite(RELAY2_PIN,0);
  digitalWrite(RELAY3_PIN,0);
  digitalWrite(RELAY4_PIN,0);
  delay(500);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /relay1/on") >= 0) {
              digitalWrite(RELAY1_PIN,1);
              relay1state = "on";
            } else if (header.indexOf("GET /relay1/off") >= 0) {
              digitalWrite(RELAY1_PIN,0);
              relay1state = "off";
            } else if (header.indexOf("GET /relay2/on") >= 0) {
              digitalWrite(RELAY2_PIN,1);
              relay2state = "on";
            } else if (header.indexOf("GET /relay2/off") >= 0) {
              digitalWrite(RELAY2_PIN,0);
              relay2state = "off";
            } else if (header.indexOf("GET /relay3/on") >= 0) {
              digitalWrite(RELAY3_PIN,1);
              relay3state = "on";
            } else if (header.indexOf("GET /relay3/off") >= 0) {
              digitalWrite(RELAY3_PIN,0);
              relay3state = "off";
            } else if (header.indexOf("GET /relay4/on") >= 0) {
              digitalWrite(RELAY4_PIN,1);
              relay4state = "on";
            } else if (header.indexOf("GET /relay4/off") >= 0) {
              digitalWrite(RELAY4_PIN,0);
              relay4state = "off";
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>4Relay WiFi test</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            //client.println("<p>RS 232 - " + output232state + "</p>");
            client.println("<p>Relay 1</p>");
            // If the output232state is off, it displays the ON button       
            if (relay1state=="off") {
              client.println("<p><a href=\"/relay1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/relay1/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            //client.println("<p>RS 485 - " + output485state + "</p>");
            client.println("<p>Relay 2</p>");
            // If the output485state is off, it displays the ON button       
            if (relay2state=="off") {
              client.println("<p><a href=\"/relay2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/relay2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>Relay 3</p>");
            if (relay3state=="off") {
              client.println("<p><a href=\"/relay3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/relay3/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>Relay 4</p>");
            if (relay4state=="off") {
              client.println("<p><a href=\"/relay4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/relay4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
