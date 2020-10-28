// html->string 변환사이트
// http://davidjwatts.com/youtube/esp8266/esp-convertHTM.html#
// https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
String webHead="<html> <head> <meta name='viewport' content='width=device-width, initial-scale=1.0'/> <meta http-equiv='Content-Type' content='text/html;charset=utf-8' /> <style> body { background: #eab0dc; font-family: \"Lato\", sans-serif; } .button { border: none; border-color:black; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; } .buttonMenu { padding: 5px 24px; margin-left:20%; background-color:black; border: none; border-color:black; color:white; text-align: left; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; } .sidenav { height: 100%; width: 0; position: fixed; z-index: 1; top: 0; left: 0; background-color: #111; overflow-x: hidden; transition: 0.5s; padding-top: 60px; } .sidenav a { padding: 8px 8px 8px 32px; text-decoration: none; font-size: 18px; color: #818181; display: block; transition: 0.3s; } .sidenav a:hover { color: #f1f1f1; } .sidenav .closebtn { position: absolute; top: 0; right: 25px; font-size: 36px; margin-left: 50px; } .button-on {border-radius: 100%; background-color: #4CAF50;} .button-off {border-radius: 100%;background-color: #707070;} </style> </head> <body> <script> function openNav() { document.getElementById(\"mySidenav\").style.width = \"150px\"; } function closeNav() { document.getElementById(\"mySidenav\").style.width = \"0\"; } </script>";
String webTail="\n<p><a href='/wifi'>네트웍공유기변경-누른후 와이파설정부터 다시하세요</a></p></body>\n</html>";

void handleRoot() {
  String s=""; 
  s+="<div id='mySidenav' class='sidenav'>";
  s+="<a href='javascript:void(0)' class='closebtn' onclick='closeNav()'>&times;</a>";
  s+="<a href='#'>About</a>";
  s+="<a href='#'>메뉴얼</a>";
  s+="<a href='http://i2r.link'>김동일홈피</a>";
  s+="<form action='/act'>";
  s+="<input type='hidden' name='ipNo' value='10'>";
  s+="<button type='submit' name='act' value='1' class='buttonMenu'>켜기</button>";
  s+="<button type='submit' name='act' value='0' class='buttonMenu'>끄기</button>";
  s+="</form>";
  s+="</div>";
  s+="<span style='font-size:30px;cursor:pointer' onclick='openNav()'>&#9776; </span>";
  s+="메뉴열기";

  s+="<h3 style='color:SteelBlue;'>&emsp;김동일유튜브 <a href='http://i2r.link' style='color:SteelBlue;'>http://i2r.link</a> </h3>";
  s+="<br>";
  if(ledOn==1) 
    s=s+"<a href='off'> <button class='button button-on' ></button></a>";
  else 
    s=s+"<a href='on'> <button class='button button-off' ></button></a>";
  s=s+"<br><br>AP & IP :&emsp;"+sChipId+"&emsp;"+WiFi.localIP().toString();
  server.send(200, "text/html", webHead+s+webTail);
}

void handleAct() {
  int ipNo=server.arg("ipNo").toInt();
  int act=server.arg("act").toInt();
  Serial.println(ipNo);
  Serial.println(act);
  if(act==1) {
    ledOn=1;
    digitalWrite(LED, 0);
  }
  else {
   ledOn=0;
    digitalWrite(LED, 1);   
  }
  GoHome();
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

void handleWifi() {
  WiFiManager wm;
  wm.resetSettings();
  wm.resetSettings();
  ESP.reset();
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
