
int v=0;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println();
  Serial.print("z0.val="); //처음 실행문은 전송되지 않음 Error
  Serial.print(v);  
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.print("z0.val="); 
  Serial.print(v);  
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  v=v+20;
  delay(2000);
}
