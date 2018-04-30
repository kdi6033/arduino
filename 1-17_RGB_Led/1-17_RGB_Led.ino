int ledRed=12; // D6 IO12 핀을 사용
int ledGreen=13; // D7 IO13 핀을 사용
int ledBlue=15; // D8 IO15 핀을 사용
int timeIn=1000; 
void setup() {
  // put your setup code here, to run once:
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
}

void loop() {
  analogWrite(ledRed, 1023);
  analogWrite(ledGreen, 0);
  analogWrite(ledBlue, 0);
  delay(timeIn);
                         
  analogWrite(ledRed, 0);
  analogWrite(ledGreen, 1023);
  analogWrite(ledBlue, 0);
  delay(timeIn); 
  
  analogWrite(ledRed, 0);
  analogWrite(ledGreen, 0);
  analogWrite(ledBlue, 1023);
  delay(timeIn);
}
