#include "Nextion.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "i2rAp";
const char* password = "00000000";
ESP8266WebServer server(80);

NexButton b0 = NexButton(0,2,"b0");
NexTouch *nex_listen_list[] =
{
  &b0,
  NULL
};

int count=0;
char msg[100];
bool bB0=false,bChange=false;
int color=864;

String s,s_head;
void handleRoot() {
  s="";
  s=s+"{\"sw0\":"+bB0+","+"\"count\":"+count+"}";
  server.send(200, "text/plain", s);
  //server.send(200, "text/html", s);
  
}

void setupWifi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("HTTP server started");
}


void b0PushCallback(void *ptr) {
  bChange=true;
  if(bB0==false) {
    bB0=true;
    color=2016;
    sprintf (msg, "b0.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    sprintf (msg, "b0.txt=\"on\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
    //Serial.println("p40 on");
  }
  else {
    bB0=false;
    color=864;
    sprintf (msg, "b0.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    //Serial1.print(msg);
    sprintf (msg, "b0.txt=\"off\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
    //Serial.println("p40 off");
  }
}

void SetText() {
   sprintf (msg, "t0.txt=\"count %d \"%c%c%c",count,0xff,0xff,0xff);
   Serial1.print(msg); 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  b0.attachPush(b0PushCallback);
  setupWifi();
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  nexLoop(nex_listen_list);
  if(bChange==true) {
      count++;
      SetText();
      bChange=false;
    }
}
