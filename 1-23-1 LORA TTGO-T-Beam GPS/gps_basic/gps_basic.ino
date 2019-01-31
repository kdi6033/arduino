void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, 12, 15);   //17-TX 18-RX
}

void loop() {
  // put your main code here, to run repeatedly:
  serialEvent();
}

void serialEvent() {
  if(Serial1.available() == false)
    return;
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    Serial.write(inChar);
  }
}
