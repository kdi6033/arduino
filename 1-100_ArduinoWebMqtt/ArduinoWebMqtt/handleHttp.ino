/** Handle root or redirect to captive portal */
void handleMqttTx() {
  Serial.println("Send Mqtt");
  char dd[2];
  server.arg("d").toCharArray(dd, 2);
  Serial.println(dd);
  server.arg("p").toCharArray(password, sizeof(password) - 1);
  server.sendHeader("Location", "/", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  server.arg("d").toCharArray(cno, sizeof(cno) - 1);
  no=atoi(cno);
  sprintf (msg, "\{ \"command\":\"read\", \"device\":%d}",(int)no);
  Serial.println(msg);
  client.publish(outTopic, msg);
  delay(2000);
}
void handleMqttRx() {

}
void handleRoot() {
  Serial.println("Server Html");
  Serial.println ( WiFi.localIP() ); 
  Serial.println ( ch[10] );
  Serial.println ( ch[11] );
  Serial.println ( ch[12] );
  Serial.println ( ch[13] );
  
  String s1= String(ssid);
  if(bJson == true) {
    s=sHead+"<meta http-equiv='refresh' content='1'/>";
  }
  else {
    s=sHead;
  }
  bJson = false;
  s=s+"<h1>더핏 건강관리</h1>";
  s=s+"<form method='POST' action='mqtttx'>"
    +"<p>기기번호 <input type='text' placeholder='디바이스번호' name='d'/></p>"
    +"          <input type='submit' value='        저    장        '/></form>";
  s=s+"기기번호 : "+ String(device) + "<br>"; 
  for(int i=0;i<10;i++) {
    s=s+"ch"+i+" : "+ String(ch[i]) + "<br>"; 
  }
  s=s+"Pulse(sec) : " + String(ch[10]) + "<br>";
  s=s+"P/W(usec) : " + String(ch[11]) + "<br>"; 
  s=s+"Pause(sec) : " + String(ch[12]) + "<br>";
  s=s+"Freq(Hz) : " + String(ch[13]) + "<br>";
  s=s+"<p><a href='/'>메인 홈페이지로 가기</a>.</p>";
  if(setAP==1) {
    s=s+"아래 공유기 이름과 주소가 연결되었으면 주소를 선택한 후에 설정에서 사용 하시는 인터넷 공유기를 선택하세요. <br>";
    s=s+"Connected IP : "+String(ssid)+"  "+"<p><a href='http://"+WiFi.localIP().toString()+"'/>"+WiFi.localIP().toString()+"</a></p>";
  }
  s=s+"<p><a href='/wifi'>공유기를 바꾸려면 누르세요.</a></p>";
  setAP=0;
  server.send(200, "text/html", s); // Empty content inhibits Content-length header so we have to close the socket ourselves.
}

void handleWifi() {
  String s1= String(ssid);
  s=sHead+"<h1>Wifi 사양</h1>";
  if (server.client().localIP() == apIP) {
    Serial.println(String(softAP_ssid));
    s=s+String("<p>연결된 AP: ") + sAP_ssid + "</p>";
  } else {
    s=s+"<p>연결된 와이파이 " + String(ssid) + "</p>";
  }
  s=s+"<table><tr><th align='left'>SoftAP 사양</th></tr>";
  s=s+"<tr><td>SSID " + String(softAP_ssid) + "</td></tr>";
  s=s+"<tr><td>IP   " + toStringIp(WiFi.softAPIP()) + "</td></tr>"+"</table>";
  s=s+"<br /><table><tr><th align='left'>WLAN 사양</th></tr>";
  s=s+"<tr><td>SSID " + String(ssid) + "</td></tr>";
  s=s+"<tr><td>IP   " + toStringIp(WiFi.localIP()) + "</td></tr>"+"</table>";
  
  s=s+"<br /><table><tr><th align='left'>검색된 와이파이 </th></tr>";
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      s=s+"\r\n<tr><td>SSID " + WiFi.SSID(i) + String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":" *") + " (" + WiFi.RSSI(i) + ")</td></tr>";
    }
  } else {
    s=s+"<tr><td>No WLAN found</td></tr>";
  }
  s=s+"</table>";
  s=s+"<p><a href='/wifi'>와이파이가 없으면 다시 검색하세요.</a></p>";
  
  s=s+"<form method='POST' action='wifisave'><h4>연결하려는 와이파이 입력</h4>"
    +"<p>와이파이 이름 <input type='text' placeholder='와이파이 이름' name='n'/></p>"
    +"<p>비  밀  번  호<input type='password' placeholder='비밀번호' name='p'/></p>"
    +"          <input type='submit' value='        저    장        '/></form>"
    +"<p><a href='/'>메인 홈페이지로 가기</a>.</p>";
  server.send(200, "text/html", s);
  setAP=1;
}

/** Handle the WLAN save form and redirect to WLAN config page again */
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
  saveCredentials();
  connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname)+".local")) {
    Serial.print("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

void handleNotFound() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }
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
