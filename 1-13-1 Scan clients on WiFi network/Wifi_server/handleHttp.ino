// html->string 변환사이트
// http://davidjwatts.com/youtube/esp8266/esp-convertHTM.html#
// https://tomeko.net/online_tools/cpp_text_escape.php?lang=en
String webHead="<html>\n<head>\n<meta name='viewport' content='width=device-width,initial-scale=1.0'/>\n<meta http-equiv='Content-Type' content='text/html;charset=utf-8' />\n<style>\n.button {\nborder: none;\n color: white;\n padding: 20px;\n text-align: center;\n text-decoration: none;\n display: inline-block;\n font-size: 16px;\n margin: 4px 2px;\n cursor: pointer;\n }\n .button-on {border-radius: 100%; background-color: #4CAF50;}\n .button-off {border-radius: 100%;background-color: #707070;}\n </style>\n </head>\n <body>\n";
String webTail="\n<p><a href='/wifi'>네트웍공유기변경-누른후 와이파설정부터 다시하세요</a></p></body>\n</html>";

void handleRoot() {
  String s; 
  s="<meta name='viewport' content='width=device-width, initial-scale=1.0'/>";
  s=s+"<meta http-equiv='Content-Type' content='text/html;charset=utf-8' />";
  s=s+"<h1>&emsp;김동일 http://i2r.link</h1>";
  for(int i=0;i<count;i++) {
    if(On[actIp[i]]==1) 
      s=s+"<form action='/off'>"+s1+(String)actIp[i]+"<input type='hidden'><input type='hidden' id='ipNo' name='ipNo' min='1' max='100' value='"+(String)actIp[i]+"';'><button type='submit' class='button button-on' >켜짐</button></form>";
    else 
      s=s+"<form action='/on'>"+s1+(String)actIp[i]+"<input type='hidden'><input type='hidden' id='ipNo' name='ipNo' min='1' max='100' value='"+(String)actIp[i]+"';'><button type='submit' class='button button-off' >꺼짐</button></form>";
  }
  s=s+"<br>&emsp;&emsp;<a href='scan'> <button >기기찿기</button></a>";
  s+="<br><br>&emsp;AP & IP :&emsp;"+sChipId+"&emsp;"+WiFi.localIP().toString();
  server.send(200, "text/html", webHead+s+webTail);
}

void handleOn() {
  int htmlValue=0;
  htmlValue=server.arg("ipNo").toInt();
  sendHttp(htmlValue,1);
  On[htmlValue]=1;
  GoHome();
}

void handleOff() {
  int htmlValue=0;
  htmlValue=server.arg("ipNo").toInt();
  sendHttp(htmlValue,0);
  On[htmlValue]=0;
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
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 404, "text/plain", message );
}

void handleScan() {
  wifiScan();
  GoHome();
  //String s;
  //s="{\"chipId\":\""+sChipId+"\",\"ip\":\""+WiFi.localIP().toString()+"\"}";
  //server.send(200, "text/plain", s);
}
