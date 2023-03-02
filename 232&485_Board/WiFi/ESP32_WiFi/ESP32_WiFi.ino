/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "사용하시는 와이파이 이름";
const char* password = "사용하시는 와이파이 비밀번호";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output232state = "off";
String output485state = "off";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  Serial2.begin(115200, SERIAL_8N1, 16, 17); //RS-485
  Serial1.begin(115200, SERIAL_8N1, 26, 27); //RS-232

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
            
            if (header.indexOf("GET /232/on") >= 0) {
              Serial.println("232 send");
              output232state = "on";
              Serial1.print("232offA");
            } else if (header.indexOf("GET /232/off") >= 0) {
              Serial.println("232 send");
              output232state = "off";
              Serial1.print("232onA");
            } else if (header.indexOf("GET /485/on") >= 0) {
              Serial.println("485 send");
              output485state = "on";
              Serial2.print("485offB");
            } else if (header.indexOf("GET /485/off") >= 0) {
              Serial.println("485 send");
              output485state = "off";
              Serial2.print("485onB");
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
            client.println("<body><h1>RS232 RS485 WiFi test</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            //client.println("<p>RS 232 - " + output232state + "</p>");
            client.println("<p>RS 232</p>");
            // If the output232state is off, it displays the ON button       
            if (output232state=="off") {
              client.println("<p><a href=\"/232/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/232/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            //client.println("<p>RS 485 - " + output485state + "</p>");
            client.println("<p>RS 485</p>");
            // If the output485state is off, it displays the ON button       
            if (output485state=="off") {
              client.println("<p><a href=\"/485/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/485/off\"><button class=\"button button2\">OFF</button></a></p>");
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
