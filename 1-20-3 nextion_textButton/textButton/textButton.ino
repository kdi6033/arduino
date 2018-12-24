#include "Nextion.h"

NexButton b0 = NexButton(0,2,"b0");
NexTouch *nex_listen_list[] =
{
  &b0,
  NULL
};

int value=0;
char msg[100];
bool bB0=false;
int color;
unsigned long previousMillis = 0; 
const long interval = 1000;    

void b0PushCallback(void *ptr) {
  value=0;
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
  value=0;
}

void SetText() {
   sprintf (msg, "t0.txt=\"val %d \"%c%c%c",value,0xff,0xff,0xff);
   Serial1.print(msg); 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  b0.attachPush(b0PushCallback);
  Serial.println("start");
}

void loop() {
  // put your main code here, to run repeatedly:
  nexLoop(nex_listen_list);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    value++;
    SetText();
  }
}
