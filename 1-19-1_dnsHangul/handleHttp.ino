/** Handle root or redirect to captive portal */
void handleRoot() {
  Serial.println("Server Html");
  Serial.println ( WiFi.localIP() );
  String s; 
  String s1= String(ssid);
  s="<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\", http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\"/>";
  s=s+"한글을 사용해 핸드폰으로 공유기를 설정하는 예제 입니다.<br>";
  s=s+"Connected IP : "+String(ssid)+"  "+WiFi.localIP().toString()+"<p><a href='/wifi'>공유기를 바꾸려면 누르세요.</a></p>";
  server.send(200, "text/html", s); // Empty content inhibits Content-length header so we have to close the socket ourselves.
}

void handleWifi() {
  String s; 
  String s1= String(ssid);
  s="<meta name=\"viewport\" content=\"width=device-width, user-scalable=no\", meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\"/>";
  s=s+"<h1>Wifi 사양</h1>";
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
    +"<input type='text' placeholder='와이파이 이름' name='n'/>"
    +"<br /><input type='password' placeholder='비밀번호' name='p'/>"
    +"<br /><input type='submit' value='      저    장      '/></form>"
    +"<p><a href='/'>메인 홈페이지로 가기</a>.</p>";
  server.send(200, "text/html", s);
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
