#include "Nextion.h"

const int ledPin =  LED_BUILTIN;// the number of the LED pin
NexButton b0 = NexButton(0,1,"b0");
NexTouch *nex_listen_list[] =
{
  &b0,
  NULL
};

void b0PushCallback(void *ptr) {
  digitalWrite(ledPin, LOW);
  Serial.println("on");
}
void b0PopCallback(void *ptr) {
  digitalWrite(ledPin, HIGH);
  Serial.println("off");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  Serial.begin(9600);
  Serial.println("start");
  b0.attachPush(b0PushCallback);
  b0.attachPop(b0PopCallback);
}


void loop() {
  // put your main code here, to run repeatedly:
  delay(30);
  nexLoop(nex_listen_list);

}
