/** Is this an IP? */
boolean isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

/** IP to String? */
String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}


void readConfig() {
  SPIFFS.begin();
  File f = SPIFFS.open("/config.txt", "r");
  if (!f) {
      Serial.println("file open failed");
  }  Serial.println("====== Reading from SPIFFS file =======");
  String s=f.readStringUntil('\n');
  s.toCharArray(ssid,s.length());
  ssid[s.length()]=0;
  s=f.readStringUntil('\n');
  s.toCharArray(password,s.length());
  password[s.length()]=0;
  f.close();
  SPIFFS.end();
  Serial.println("ssid: "+String(ssid));
  Serial.println("pw: "+String(password));
}

void saveConfig() {
  SPIFFS.begin();
  // Next lines have to be done ONLY ONCE!!!!!When SPIFFS is formatted ONCE you can comment these lines out!!
  //Serial.println("Please wait 30 secs for SPIFFS to be formatted");
  //SPIFFS.format();

  // open file for writing
  File f = SPIFFS.open("/config.txt", "w");
  if (!f) {
      Serial.println("file open failed");
  }
  f.println(ssid);
  f.println(password);
  f.close();
  SPIFFS.end();
  ESP.reset();
}

// trigger pin 0(D3) 2(D4)
void factoryDefault() {
    Serial.println("AP mode as Factory Deafault");
    Serial.println("Please wait over 3 min");
    SPIFFS.begin();
    SPIFFS.format();
    SPIFFS.end();
    ESP.reset();
}
