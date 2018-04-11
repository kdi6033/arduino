/*
 ip로 접속
 버튼 핸들링
 */
#include <ESP8266WiFi.h>

const char* ssid = "i2r";
const char* password = "00000000";
const int ledPin =  2;
boolean bLED = false; 

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
  
  // Wait until the client sends some data
  Serial.println("new client");
 
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  if (req.indexOf("/?do=ON") > 0) {
    bLED = true;
    digitalWrite(ledPin, LOW);
    Serial.println(F("LED is ON")); 
  }
  else if (req.indexOf("/?do=OFF") > 0) {
    bLED = false;
    digitalWrite(ledPin, HIGH);
    Serial.println(F("LED is OFF")); 
  }

  // Prepare the response
          client.println("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n");
          client.println("<body>");
          client.println("<br />");
          //client.println(F("    <form method=\"post\">")); 
          client.println(F("    <form method=\"get\">")); 
          if(!bLED) { 
            bLED=true;
            client.println(F("      <input type=\"hidden\" name=\"do\" value=\"ON\">")); 
            client.println(F(" LED     <input type=\"button\" name=\"button1\" value=\"ON\" onclick=\"submit();\" >")); 
          } 
          else { 
            bLED=false;
            client.println(F("      <input type=\"hidden\" name=\"do\" value=\"OFF\">")); 
            client.println(F(" LED     <input type=\"button\" name=\"button1\" value=\"OFF\" onclick=\"submit();\">")); 
          } 
          client.println(F("    </form>")); 
          client.println("</body>");
          client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed  
}
