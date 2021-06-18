//참조유튜브 https://www.youtube.com/watch?v=ROeT-gyYZfw
#include "FS.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

ESP8266WebServer server;
WebSocketsServer webSocket = WebSocketsServer(81);

char* ssid = "i2r";
char* password = "00000000";

char Head[] PROGMEM = R"=====(
<html>
  <head>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
    <meta http-equiv='Content-Type' content='text/html;charset=utf-8' />
    <style>
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
        .button-on {border-radius: 100%; background-color: #4CAF50;}
        .button-off {border-radius: 100%;background-color: #707070;}
        .button-ledon {border-radius: 100%; padding: 10px; font-size: 8px; margin: 0px 0px; background-color: #ff4500;}
        .button-ledoff {border-radius: 100%; padding: 10px; font-size: 8px; background-color: #707070;}
  </style>
)=====";

char HeadScript[] PROGMEM = R"=====(
  <script>
    var Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onmessage = function(event){
        document.getElementById("rxConsole").value += event.data;
      }
    }
    function sendText(){
      Socket.send(document.getElementById("txBar").value);
      document.getElementById("txBar").value = "";
    }
    function sendBrightness(){
      Socket.send("#"+document.getElementById("brightness").value);
    } 
    function sendAct(valueIn){
      Socket.send(valueIn);
    }
    function sendButton(valueIn){
      var led = document.getElementById('led');
      if(valueIn=="1")
        led.style.backgroundColor = 'lime';
      if(valueIn=='0')
        led.style.backgroundColor = 'grey';
      Socket.send(valueIn);
    }
    function sendOption(){
      document.getElementById("carSelect").value = document.getElementById("cars").value;
      Socket.send("$"+document.getElementById("cars").value);
    }
    function changeColor(newColor) {
      var elem = document.getElementById('para');
      elem.style.color = newColor;
    }            
  </script>
</head>
)=====";

char Body[] PROGMEM = R"=====(
<body onload="javascript:init()">
  <br>websocket 설명
  <br>전송된 문자
  <div><textarea id="rxConsole"></textarea></div>
  <hr/>
  전송할 문자를 임력 후 리턴을 치세요
  <div><input type="text" id="txBar" onkeydown="if(event.keyCode == 13) sendText();" /></div>
  <hr/>
  0~1023 정수 값을 CPU 변수로 전송
  <div><input type="range" min="0" max="1023" value="512" id="brightness" oninput="sendBrightness()" /></div>  
  <div> <a href="/" id="onAct" onclick="sendAct('act-1');" style="text-decoration:none">행동1</a> </div>  
  <div> <button id="onButton" onclick="sendButton('1');">on</button> &emsp; <button id="offButton" onclick="sendButton('0');">off</button>  </div>  
  <br>
  <div> <button id="led">LED</div>  
  <br>
  <label for="cars">Choose a car:</label>
  <div>
  <select id="cars" onclick="sendOption();">
    <option value="volvo">Volvo</option>
    <option value="saab">Saab</option>
    <option value="opel">Opel</option>
    <option value="audi">Audi</option> 
  </select>
  </div>
  <div><input type="text" id="carSelect" /></div>
)=====";

char Tail[] PROGMEM = R"=====(
    </body>
</html>
)=====";


void setup()
{
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void handleRoot() {
  String s;
  s=FPSTR(Head);
  s+=FPSTR(HeadScript);
  s+=FPSTR(Body);
  s+=FPSTR(Tail);
  server.send(200, "text/html", s);
};

void loop()
{
  webSocket.loop();
  server.handleClient();
  if(Serial.available() > 0){
    char c[] = {(char)Serial.read()};
    webSocket.broadcastTXT(c, sizeof(c));
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if(type == WStype_TEXT){
    if(payload[0] == '#'){
      uint16_t brightness = (uint16_t) strtol((const char *) &payload[1], NULL, 10);
      brightness = 1024 - brightness;
      Serial.print("brightness= ");
      Serial.println(brightness);
    }
    else if(payload[0] == '$'){
      for(int i = 0; i < length; i++)
        Serial.print((char) payload[i]);
      Serial.println();
    }
    else if(payload[0] == '%'){
      for(int i = 0; i < length; i++)
        Serial.print((char) payload[i]);
      Serial.println();
    }
    else{
      for(int i = 0; i < length; i++)
        Serial.print((char) payload[i]);
      Serial.println();
    }
  }
}
