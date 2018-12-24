int value=0;
char msg[100];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial.println("start");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  value++;
  sprintf (msg, "t0.txt=\"%d \"%c%c%c",value,0xff,0xff,0xff);
  Serial1.print(msg);
  delay(2000);
}
