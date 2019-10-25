/** Handle root or redirect to captive portal */

void handleRoot() {
  //Serial.println("Server Html");
  //Serial.println ( WiFi.localIP() );
  String s=""; 
  String s1= String(ssid);
  s="<meta name='viewport' content='width=device-width, initial-scale=1.0'/>";
  if(autoRead==1)
    s=s+"<meta http-equiv='refresh' content='5'/>";
  s=s+"<meta http-equiv='Content-Type' content='text/html;charset=utf-8' />";
  s=s+"<meta http-equiv='Content-Type' content='text/html;charset=utf-8' />";
  s=s+"<h1>김동일 웹서버 i2r</h1>";
  s=s+"<br>출력버튼<br>";
  if(P4[0]==1)
    s=s+"<a href=\"onoffP40\"><button style='background-color:Lime; color:blue;'>40 ON</button></a>";
  else 
    s=s+"<a href=\"onoffP40\"><button style='background-color:DarkGreen; color:white;'>40 OFF</button></a>";
  s=s+"&emsp;";

  if(P4[1]==1)
    s=s+"<a href=\"onoffP41\"><button style='background-color:Lime; color:blue;'>41 ON</button></a>";
  else 
    s=s+"<a href=\"onoffP41\"><button style='background-color:DarkGreen; color:white;'>41 OFF</button></a>";
  s=s+"&emsp;";

  if(P4[2]==1)
    s=s+"<a href=\"onoffP42\"><button style='background-color:Lime; color:blue;'>42 ON</button></a>";
  else 
    s=s+"<a href=\"onoffP42\"><button style='background-color:DarkGreen; color:white;'>42 OFF</button></a>";
  s=s+"&emsp;";

  if(P4[3]==1)
    s=s+"<a href=\"onoffP43\"><button style='background-color:Lime; color:blue;'>43 ON</button></a>";
  else 
    s=s+"<a href=\"onoffP43\"><button style='background-color:DarkGreen; color:white;'>43 OFF</button></a>";
  s=s+"<br><br>";

  
  if(autoRead==1)
    s=s+"<a href=\"read\"><button style='background-color:Tomato; color:blue;'>IN port Read On</button></a>";
  else 
    s=s+"<a href=\"read\"><button style='background-color:DimGray; color:white;'>IN port Read OFF</button></a>";
  s=s+"<br><br>";

  //s=s+"<a href=\"read\"><button style='background-color:Tomato; color:white;'>IN port Read</button></a>";
  //s=s+"<br><br>";

  if(P0[0]==1)
    s=s+"<button style='background-color:Lime; color:blue;'>00</button>";
  else 
    s=s+"<button style='background-color:DarkGreen; color:white;'>00</button>";
  s=s+"&nbsp;";
  if(P0[1]==1)
    s=s+"<button style='background-color:Lime; color:blue;'>01</button>";
  else 
    s=s+"<button style='background-color:DarkGreen; color:white;'>01</button>";
  s=s+"&nbsp;";
  if(P0[2]==1)
    s=s+"<button style='background-color:Lime; color:blue;'>02</button>";
  else 
    s=s+"<button style='background-color:DarkGreen; color:white;'>02</button>";
  s=s+"&nbsp;";
  if(P0[3]==1)
    s=s+"<button style='background-color:Lime; color:blue;'>03</button>";
  else 
    s=s+"<button style='background-color:DarkGreen; color:white;'>03</button>";
  s=s+"&nbsp;";
  if(P0[4]==1)
    s=s+"<button style='background-color:Lime; color:blue;'>04</button>";
  else 
    s=s+"<button style='background-color:DarkGreen; color:white;'>04</button>";
  s=s+"&nbsp;";
  if(P0[5]==1)
    s=s+"<button style='background-color:Lime; color:blue;'>05</button>";
  else 
    s=s+"<button style='background-color:DarkGreen; color:white;'>05</button>";
  s=s+"<br><br>";

  server.send(200, "text/html", s);
}

void GoHome() {
  IPAddress ip = WiFi.localIP();
  String s,ipS;
  ipS=String(ip[0])+"."+String(ip[1])+"."+String(ip[2])+"."+String(ip[3]);
    s="<meta http-equiv='refresh' content=\"0;url='http://"+ipS+"/'\">";
  server.send(200, "text/html", s);
}

void handleRead() {
  if(autoRead==1) 
    autoRead=0;
  else 
    autoRead=1;
  GoHome();
}

void handleOnOffP40() {
  if(P4[0]==1) 
    P4[0]=0;
  else 
    P4[0]=1;
  plcOut();
  GoHome();
  delay(100);
}

void handleOnOffP41() {
  if(P4[1]==1) 
    P4[1]=0;
  else 
    P4[1]=1;
  plcOut();
  GoHome();
  delay(100);
}

void handleOnOffP42() {
  if(P4[2]==1) 
    P4[2]=0;
  else 
    P4[2]=1;
  plcOut();
  GoHome();
  delay(100);
}

void handleOnOffP43() {
  if(P4[3]==1) 
    P4[3]=0;
  else 
    P4[3]=1;
  plcOut();
  GoHome();
  delay(100);
}

void handleNotFound() {
  digitalWrite(led, 1);
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
  digitalWrite(led, 0);
}
