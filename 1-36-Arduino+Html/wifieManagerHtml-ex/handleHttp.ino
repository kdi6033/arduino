// html->string 변환 https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
String webHead="<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <meta http-equiv='Content-Type' content='text/html;charset=utf-8' />\n        <style>\n            .button {\n                border: none;\n                color: white;\n                padding: 20px;\n                text-align: center;\n                text-decoration: none;\n                display: inline-block;\n                font-size: 16px;\n                margin: 4px 2px;\n                cursor: pointer;\n            }\n            .button-on {border-radius: 100%; background-color: #4CAF50;}\n            .button-off {border-radius: 100%;background-color: #707070;}\n        </style>\n    </head>\n    <body>\n        <h1>김동일 웹서버 i2r.link</h1>";
String webTail="\n<p><a href='/wifi'>네트웍공유기변경-누른후 와이파설정부터 다시하세요</a></p></body>\n</html>";

//html 소스프로그램 : https://www.w3schools.com/jsref/tryit.asp?filename=tryjsref_onclick
//참조유튜브 : https://www.youtube.com/watch?v=lyoBWH92svk
const char webpage1[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <body>
    <h1>The onclick Event 01</h1>
    <p>The onclick event is used to trigger a function when an element is clicked on.</p>
    <p>Click the button to trigger a function that will output "Hello World" in a p element with id="demo".</p>
    <button onclick="myFunction()">Click me</button>
    <p id="demo"></p>
    <script>
    function myFunction() {
      document.getElementById("demo").innerHTML = "Hello World";
    }
    </script>
  </body>
</html>
)=====";


//참조유튜브 : https://www.youtube.com/watch?v=FyPZ29wgD7Y
// text 변환 사이트 : http://davidjwatts.com/youtube/esp8266/esp-convertHTM.html#
//                  https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
const char webpage2[] PROGMEM={"<!DOCTYPE html> <html> <body> <h1>The onclick Event 02</h1> <p>The onclick event is used to trigger a function when an element is clicked on.</p> <p>Click the button to trigger a function that will output \"Hello World\" in a p element with id=\"demo\".</p> <button onclick=\"myFunction()\">Click me</button> <p id=\"demo\"></p> <script> function myFunction() { document.getElementById(\"demo\").innerHTML = \"Hello World\"; } </script> </body> </html>"};
String webpage3="<!DOCTYPE html> <html> <body> <h1>The onclick Event 03</h1> <p>The onclick event is used to trigger a function when an element is clicked on.</p> <p>Click the button to trigger a function that will output \"Hello World\" in a p element with id=\"demo\".</p> <button onclick=\"myFunction()\">Click me</button> <p id=\"demo\"></p> <script> function myFunction() { document.getElementById(\"demo\").innerHTML = \"Hello World\"; } </script> </body> </html>";

void handleRoot() {
  //server.send(200, "text/html", webpage1);
  //server.send(200, "text/html", webpage2);
  server.send(200, "text/html", webpage3);
}

void handleWifi() {
  WiFiManager wm;
  wm.resetSettings();
  wm.resetSettings();
  ESP.reset();
}

void handleOn() {
  ledOn=1;
  digitalWrite(LED, 0);
  GoHome();
}

void handleOff() {
  ledOn=0;
  digitalWrite(LED, 1);
  GoHome();
}

void GoHome() {
  String s,ipS;
  //IPAddress ip;
  ipS=toStringIp(WiFi.localIP());
  s="<meta http-equiv='refresh' content=\"0;url='http://"+ipS+"/'\">";
  server.send(200, "text/html", s);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleScan() {
  String s;
  s="{\"chipId\":\""+sChipId+"\",\"ip\":\""+WiFi.localIP().toString()+"\"}";
  server.send(200, "text/html", s);
}
