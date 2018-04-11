/*
 ip로 접속
 버튼 핸들링
 */
#include <ESP8266WiFi.h>

const char* ssid = "i2r";
const char* password = "00000000";
const int led1Pin =  4;
int bLED1 = 0; 
const int led2Pin =  0;
boolean bLED2 = false; 

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  // set the digital pin as output:
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  digitalWrite(led1Pin, HIGH);
  digitalWrite(led2Pin, HIGH);  
  
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
 
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  if (req.indexOf("/?led1=1") > 0) {
    bLED1 = 1;
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, HIGH);
    Serial.println(F("The up light of LED1 is ON")); 
  }
  else if (req.indexOf("/?led1=2") > 0){
    bLED1 = 2;
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, LOW);
    Serial.println(F("The up low of LED1 is ON"));  
  }
  else if (req.indexOf("/?led1=3") > 0){
    bLED1 = 3;
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    Serial.println(F("The up low of LED1 is ON"));  
  }
  else if (req.indexOf("/?led1=0") > 0){
    bLED1 = 0;
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, HIGH);
    Serial.println(F("The up low of LED1 is OFF")); 
  }

  // Prepare the response
          client.println("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n");
          client.println("<body>");
          client.println("<br />");
          client.println("Light State"); 
          
          client.println(F("<form method=\"get\">")); 
          if(bLED1 == 0) { 
            client.println(F("<input type=\"hidden\" name=\"led1\" value=\"1\">")); 
            client.println(F("<input type=\"button\" name=\"button1\" value=\"Light1  OFF\" onclick=\"submit();\" style=\"height:150px; width:300px;  font-size:50px; \" >")); 
          }
          else if(bLED1 == 1) { 
            client.println(F("<input type=\"hidden\" name=\"led1\" value=\"2\">")); 
            client.println(F("<input type=\"button\" name=\"button1\" value=\"Light1  ON\" onclick=\"submit();\" style=\"height:150px; width:300px;  font-size:50px; \" >")); 
          }
          else if(bLED1 == 2) { 
            client.println(F("<input type=\"hidden\" name=\"led1\" value=\"3\">")); 
            client.println(F("<input type=\"button\" name=\"button1\" value=\"Light2  ON\" onclick=\"submit();\" style=\"height:150px; width:300px;  font-size:50px; \" >")); 
          }
          else if(bLED1 == 3) { 
            client.println(F("<input type=\"hidden\" name=\"led1\" value=\"0\">")); 
            client.println(F("<input type=\"button\" name=\"button1\" value=\"All Light  ON\" onclick=\"submit();\" style=\"height:150px; width:300px;  font-size:50px; \" >")); 
          }
          client.println(F("</form>")); 

          client.println("</body>");
          client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed  
}
