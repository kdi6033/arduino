#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

/* Set these to your desired credentials. */
const char *softAP_ssid = "ap_i2r";
const char *softAP_password = "00000000";
/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

ESP8266WebServer server(80);

const int led = 2;
String s="";
int P4[4]={0};
int P0[6]={0};
int lastRead=0,autoRead=0;
boolean stringComplete = false; 
String inputString;
char msg[50];

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  Serial.begin(9600);
  Serial1.begin(9600);
  
  setupAp();
  server.on("/", handleRoot);
  server.on("/onoffP40", handleOnOffP40);
  server.on("/onoffP41", handleOnOffP41);
  server.on("/onoffP42", handleOnOffP42);
  server.on("/onoffP43", handleOnOffP43);
  server.on("/read", handleRead);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void setupAp() {
  Serial.println(softAP_ssid);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

void loop(void) {
  server.handleClient();

  long now = millis();
  if ((now - lastRead > 1000) && autoRead==1){
    serialEvent();
    lastRead = now;
    String s="";
    s=char(5);
    s+="00RSS0104%PW0";
    s+=char(4);
    Serial1.print(s);
  }
}

void serialEvent() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    inputString.toCharArray(msg, inputString.length());
    // clear the string:
    inputString = "";
    stringComplete = false;
    if(msg[3]=='R') {
        byte inC=msg[13];
        byte inC1=msg[12];
        P0[0]=bitRead(inC,0);
        P0[1]=bitRead(inC,1);
        P0[2]=bitRead(inC,2);
        P0[3]=bitRead(inC,3);
        P0[4]=bitRead(inC1,0);
        P0[5]=bitRead(inC1,1);
        GoHome();
    }
  }
  
  if(Serial.available() == false) 
    return;
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
  }
  stringComplete = true;
}

void plcOut() {
  //int out=P40+P41*2+P42*4+P43*8;
  String out =  String(P4[0]+P4[1]*2+P4[2]*4+P4[3]*8, HEX);
  String s="";
  s=char(5);
  s+="00WSS0104%PW4000";
  s+=out;
  s+=char(4);
  Serial1.print(s);
}
