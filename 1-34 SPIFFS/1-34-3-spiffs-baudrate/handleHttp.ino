// html->string 변환사이트 다음 유튜브 참조하세요 https://youtu.be/eY4K7Wu7fgc
// 변환 사이트
// http://davidjwatts.com/youtube/esp8266/esp-convertHTM.html#
// https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
String webHeadMonit="<html> <head> <meta http-equiv='refresh' content='1; url='http://localhost' method='get' /> <meta name='viewport' content='width=device-width, initial-scale=1.0'/> <meta http-equiv='Content-Type' content='text/html;charset=utf-8' /> <style> body { background: #eab0dc; font-family: \"Lato\", sans-serif; } .button { border: none; border-color:black; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; } .buttonMenu { padding: 5px 24px; margin-left:20%; background-color:black; border: none; border-color:black; color:white; text-align: left; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; } .sidenav { height: 100%; width: 0; position: fixed; z-index: 1; top: 0; left: 0; background-color: #111; overflow-x: hidden; transition: 0.5s; padding-top: 60px; } .sidenav a { padding: 8px 8px 8px 32px; text-decoration: none; font-size: 18px; color: #818181; display: block; transition: 0.3s; } .sidenav a:hover { color: #f1f1f1; } .sidenav .closebtn { position: absolute; top: 0; right: 25px; font-size: 36px; margin-left: 50px; } .button-on {border-radius: 100%; background-color: #4CAF50;} .button-off {border-radius: 100%;background-color: #707070;} .button-ledon {border-radius: 100%; padding: 10px; font-size: 8px; margin: 0px 0px; background-color: #ff4500;} .button-ledoff {border-radius: 100%; padding: 10px; font-size: 8px; background-color: #707070;} </style> </head> <body> <script> function openNav() { document.getElementById(\"mySidenav\").style.width = \"150px\"; } function closeNav() { document.getElementById(\"mySidenav\").style.width = \"0\"; } </script>";
String webHead="<html> <head> <meta name='viewport' content='width=device-width, initial-scale=1.0'/> <meta http-equiv='Content-Type' content='text/html;charset=utf-8' /> <style> body { background: #eab0dc; font-family: \"Lato\", sans-serif; } .button { border: none; border-color:black; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; } .buttonMenu { padding: 5px 24px; margin-left:20%; background-color:black; border: none; border-color:black; color:white; text-align: left; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; } .sidenav { height: 100%; width: 0; position: fixed; z-index: 1; top: 0; left: 0; background-color: #111; overflow-x: hidden; transition: 0.5s; padding-top: 60px; } .sidenav a { padding: 8px 8px 8px 32px; text-decoration: none; font-size: 18px; color: #818181; display: block; transition: 0.3s; } .sidenav a:hover { color: #f1f1f1; } .sidenav .closebtn { position: absolute; top: 0; right: 25px; font-size: 36px; margin-left: 50px; } .button-on {border-radius: 100%; background-color: #4CAF50;} .button-off {border-radius: 100%;background-color: #707070;} .button-ledon {border-radius: 100%; padding: 10px; font-size: 8px; margin: 0px 0px; background-color: #ff4500;} .button-ledoff {border-radius: 100%; padding: 10px; font-size: 8px; background-color: #707070;} </style> </head> <body> <script> function openNav() { document.getElementById(\"mySidenav\").style.width = \"150px\"; } function closeNav() { document.getElementById(\"mySidenav\").style.width = \"0\"; } </script>";
String webTail="</body> </html>";

String webMenu="";
String webHtml="";

void handleRoot() {
  int monit=0;
  makeHtml();
  if(monit==1)
    server.send(200, "text/html", webHeadMonit+webHtml+webTail);
  else
    server.send(200, "text/html", webHead+webHtml+webTail);
}


void makeHtml() {
  String s=""; 

  s+="<h3 style='color:SteelBlue;'>&emsp;김동일유튜브 <a href='http://i2r.link' style='color:SteelBlue;'>http://i2r.link</a> </h3>";

  s+="<form action='/on'>";
  s+="<input type='hidden' name='act' value='1'>";
  //s+="<input type='hidden' name='no' value='"+(String)i+"'>";
  s+="<label for='myBaudrate'>통신속도를 선택하세요:</label>";
  s+="<select id='myBaudrate' name='value'>";
    s+="<option value='300'>300</option>";
    s+="<option value='1200'>1200</option>";
    s+="<option value='2400'>2400</option>";
    s+="<option value='4800'>4800</option>";
    s+="<option value='9600'>9600</option>";
    s+="<option value='19200'>19200</option>";
    s+="<option value='38400'>38400</option>";
    s+="<option value='57600'>57600</option>";
    s+="<option value='74880'>74880</option>";
    s+="<option value='115200'>115200</option>";
    s+="<option value='230400'>230400</option>";
    s+="<option value='250000'>250000</option>";
    s+="<option value='500000'>500000</option>";
    s+="<option value='1000000'>1000000</option>";
    s+="<option value='2000000'>2000000</option>";
  s+="</select>";
  s+="<input type='submit'>";
  s+="</form>";
/*
  <form action="/action_page.php">
  <label for="cars">Choose a car:</label>
  <select id="cars" name="cars">
    <option value="volvo">Volvo</option>
    <option value="saab">Saab</option>
    <option value="fiat">Fiat</option>
    <option value="audi">Audi</option>
  </select>
  <input type="submit">
</form>
*/  

  s+="<br><br>MAC & IP :&emsp;"+sMac+"&emsp;"+WiFi.localIP().toString();
  webHtml=s;
}

void handleOn() {
  act=server.arg("act").toInt();
  //int no=server.arg("no").toInt();
  int value=server.arg("value").toInt();

  Serial.println("----------------------------");
  Serial.println(act);
  //Serial.println(no);
  Serial.println(value);

  if(act==1) {
    sprintf(myBaudrate , "%d", value);
    saveConfig();
  }

  GoHome();
}


void GoHome() {
  String s;
  s="<meta http-equiv='refresh' content=\"0;url='http://"+WiFi.localIP().toString()+"/'\">";
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

/*
void handleWifi() {
  String s; 
  String s1= String(ssid);
  s="<meta name=\"viewport\" content=\"width=device-width, user-scalable=no\", meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />";
  //s=s+"<meta http-equiv='refresh' content='5'/>";
  s=s+"<h1>Wifi 사양</h1>";
  if (server.client().localIP() == apIP) {
    Serial.println(String(softAP_ssid));
    //s=s+String("<p>연결된 AP: ") + sAP_ssid + "</p>";
    s=s+String("<p>연결된 AP: 192.168.4.1") + "</p>";
  } else {
    s=s+"<p>연결된 와이파이 " + String(ssid) + "</p>";
  }
  s=s+"<table><tr><th align='left'>SoftAP 사양</th></tr>";
  s=s+"<tr><td>SSID " + String(softAP_ssid) + "</td></tr>";
  s=s+"<tr><td>IP   " + toStringIp(WiFi.softAPIP()) + "</td></tr>"+"</table>";
  s=s+"<br /><table><tr><th align='left'>WLAN 사양</th></tr>";
  s=s+"<tr><td>SSID " + String(ssid) + "</td></tr>";
  s=s+"<tr><td>IP   " + toStringIp(WiFi.localIP()) + "</td></tr>"+"</table>";
  
  s=s+"<br /><table><tr><th align='left'>검색된 와이파이 (SSID)</th></tr>";
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) 
      s=s+"\r\n<tr><td>" + WiFi.SSID(i) + "</td></tr>";
      //s=s+"\r\n<tr><td>SSID " + WiFi.SSID(i) + String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":" *") + " (" + WiFi.RSSI(i) + ")</td></tr>";
  } 
  else 
    s=s+"<tr><td>No WLAN found</td></tr>";
  s=s+"</table>";
  s=s+"<p><a href='/wifi'>와이파이가 없으면 다시 검색하세요.</a></p>";
  
  s=s+"<form method='POST' action='wifisave'><h4>연결하려는 와이파이 입력</h4>"
    +"와이파이 이름 <input type='text' value='"+ssid+"' name='n'/>"
    +"<br/><br/>비밀번호     <input type='password' value='"+password+"' name='p'/>"
    +"<br/><br/><input type='submit' value='      저    장      '/></form>"
    +"<p><a href='/'>메인 홈페이지로 가기</a>.</p>";
  server.send(200, "text/html", s);
}

void handleWifiSave() {
  Serial.println("wifi save");
  server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(password, sizeof(password) - 1);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  saveConfig();
  //connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
  Serial.println(WiFi.waitForConnectResult());
  Serial.println(ssid);
  Serial.println(password);
  Serial.println("Reset");
  ESP.reset();
}
*/

void handleScan() {
  String s;
  //s="{\"mac\":\""+sMac+"\",\"ip\":\""+WiFi.localIP().toString()+"\"}";
  server.send(200, "text/html", s);
}
