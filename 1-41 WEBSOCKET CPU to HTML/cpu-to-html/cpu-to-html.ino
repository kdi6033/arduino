//참조유튜브 https://www.youtube.com/watch?v=tGR5zqN9M2E
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> 
#include <WebSocketsServer.h>
#include <Adafruit_Sensor.h>
#include <Ticker.h>

Ticker ticker;
// Initialize network parameters
static const char ssid[] = "i2r";
static const char password[] = "00000000";

// Instantiate server objects
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Declare/initialize timer variables
Ticker timer;
int counter=0;

// Raw string literal for containing the page to be sent to clients
char webpage[] PROGMEM = R"=====(
<html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'/> <meta http-equiv='Content-Type' content='text/html;charset=utf-8' />
  <script>
    var Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onmessage = function(event){
        // receive the color data from the server
        var data = JSON.parse(event.data);
        console.log(data.temperature);
        document.getElementById("temperature").innerHTML = data.temperature;
        if(data.iButton=='1')
          document.getElementById("led").innerHTML = "<button style='background-color:lime'>on</button>";
        if(data.iButton=='0')
          document.getElementById("led").innerHTML = "<button style='background-color:grey'>off</button>";
      }
    }  
  </script>
</head>
<body onload="javascript:init()">
  <h4>CPU 데이터를 웹페이지로</h4>
  tick() 함수에서 1초 간격으로 json 형식 데이터를 script에서 이를 처리하여 웹페이지에 표시
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <div id="led">%led%</div>
</body>
</html>
)=====";

void tick()
{
  Serial.println ( WiFi.localIP() );
  counter++;
  int iButton;
  iButton=counter%2;
  String json = "{\"temperature\":";
  json += counter;
  json += ",\"humidity\":";
  json += 10+counter;
  json += ",\"iButton\":";
  json += iButton;
  json += "}";
  //Serial.println(json); // DEBUGGING
  webSocket.broadcastTXT(json.c_str(), json.length());
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  if(type == WStype_TEXT)
  {
    // processs any returned data
    Serial.printf("payload [%u]: %s\n", num, payload);
  }
}

void setup() {
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("IP Address (AP): "); Serial.println(WiFi.localIP());

  // define the routes in which the server is accessible
  server.on("/",[](){
    server.send_P(200, "text/html", webpage);  
  });

  // initialize server and websockets
  server.begin();
  webSocket.begin();
  
  ticker.attach(3, tick);  //0.1 초도 가능
  //ticker.detach();
  
  // handling incoming messages on the websocket
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
  server.handleClient();  

}
