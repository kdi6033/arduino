#include "Nextion.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "i2rAp";
const char* password = "00000000";
ESP8266WebServer server(80);

NexButton b0 = NexButton(0,1,"b0");
NexButton b1 = NexButton(0,2,"b1");
NexButton b2 = NexButton(0,3,"b2");
NexButton b3 = NexButton(0,4,"b3");
NexButton b4 = NexButton(0,5,"b4");
NexButton b5 = NexButton(0,6,"b5");
NexButton b6 = NexButton(0,7,"b6");
NexButton b7 = NexButton(0,8,"b7");

NexTouch *nex_listen_list[] =
{
  &b0,
  &b1,
  &b2,
  &b3,
  &b4,
  &b5,
  &b6,
  &b7,
  NULL
};

int count=0;
char msg[100];
bool bB[8]={false},bChange=false;
int color=864;

String s,s_head;
void handleRoot() {
  s="";
  s=s+"{\"sw\":[" +bB[0]+","+bB[1]+","+bB[2]+","+bB[3]+","+bB[4]+","+bB[5]+","+bB[6]+","+bB[7] +"] }";
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
  int iBno=0;
  if(bB[iBno]==false) {
    bB[iBno]=true;
    color=2016;
    sprintf (msg, "b0.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    sprintf (msg, "b0.txt=\"1 on\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
  else {
    bB[iBno]=false;
    color=864;
    sprintf (msg, "b0.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    //Serial1.print(msg);
    sprintf (msg, "b0.txt=\"1 off\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
}

void b1PushCallback(void *ptr) {
  int iBno=1;
  if(bB[iBno]==false) {
    bB[iBno]=true;
    color=2016;
    sprintf (msg, "b1.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    sprintf (msg, "b1.txt=\"2 on\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
  else {
    bB[iBno]=false;
    color=864;
    sprintf (msg, "b1.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    //Serial1.print(msg);
    sprintf (msg, "b1.txt=\"2 off\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
}

void b2PushCallback(void *ptr) {
  int iBno=2;
  if(bB[iBno]==false) {
    bB[iBno]=true;
    color=2016;
    sprintf (msg, "b2.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    sprintf (msg, "b2.txt=\"3 on\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
  else {
    bB[iBno]=false;
    color=864;
    sprintf (msg, "b2.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    //Serial1.print(msg);
    sprintf (msg, "b2.txt=\"3 off\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
}

void b3PushCallback(void *ptr) {
  int iBno=3;
  if(bB[iBno]==false) {
    bB[iBno]=true;
    color=2016;
    sprintf (msg, "b3.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    sprintf (msg, "b3.txt=\"4 on\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
  else {
    bB[iBno]=false;
    color=864;
    sprintf (msg, "b3.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    //Serial1.print(msg);
    sprintf (msg, "b3.txt=\"4 off\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
}

void b4PushCallback(void *ptr) {
  int iBno=4;
  if(bB[iBno]==false) {
    bB[iBno]=true;
    color=2016;
    sprintf (msg, "b4.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    sprintf (msg, "b4.txt=\"5 on\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
  else {
    bB[iBno]=false;
    color=864;
    sprintf (msg, "b4.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    //Serial1.print(msg);
    sprintf (msg, "b4.txt=\"5 off\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
}

void b5PushCallback(void *ptr) {
  int iBno=5;
  if(bB[iBno]==false) {
    bB[iBno]=true;
    color=2016;
    sprintf (msg, "b5.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    sprintf (msg, "b5.txt=\"6 on\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
  else {
    bB[iBno]=false;
    color=864;
    sprintf (msg, "b5.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    //Serial1.print(msg);
    sprintf (msg, "b5.txt=\"6 off\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
}

void b6PushCallback(void *ptr) {
  int iBno=6;
  if(bB[iBno]==false) {
    bB[iBno]=true;
    color=2016;
    sprintf (msg, "b6.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    sprintf (msg, "b6.txt=\"7 on\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
  else {
    bB[iBno]=false;
    color=864;
    sprintf (msg, "b6.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    //Serial1.print(msg);
    sprintf (msg, "b6.txt=\"7 off\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
}

void b7PushCallback(void *ptr) {
  int iBno=7;
  if(bB[iBno]==false) {
    bB[iBno]=true;
    color=2016;
    sprintf (msg, "b7.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    sprintf (msg, "b7.txt=\"8 on\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
  }
  else {
    bB[iBno]=false;
    color=864;
    sprintf (msg, "b7.bco=%d%c%c%c",color,0xff,0xff,0xff);
    Serial1.print(msg);
    //Serial1.print(msg);
    sprintf (msg, "b7.txt=\"8 off\"%c%c%c",0xff,0xff,0xff);
    Serial1.print(msg);
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
  b1.attachPush(b1PushCallback);
  b2.attachPush(b2PushCallback);
  b3.attachPush(b3PushCallback);
  b4.attachPush(b4PushCallback);
  b5.attachPush(b5PushCallback);
  b6.attachPush(b6PushCallback);
  b7.attachPush(b7PushCallback);
  setupWifi();
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  //초기 스위치를 모두 off 설정한다.
  for(int i=0;i<8;i++) {
    color=864;
    sprintf (msg, "b%d.bco=%d%c%c%c",i,color,0xff,0xff,0xff);
    Serial1.print(msg);
    Serial1.print(msg);
    sprintf (msg, "b%d.txt=\"%d off\"%c%c%c",i,i,0xff,0xff,0xff);
    Serial1.print(msg);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  nexLoop(nex_listen_list);
  /*
  if(bChange==true) {
      count++;
      SetText();
      bChange=false;
  }
  */
}
