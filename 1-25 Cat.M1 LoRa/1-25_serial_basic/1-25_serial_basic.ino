unsigned long previousMillis = 0;  
const long interval = 3000; //단위 ms
void setup() {
  // initialize serial:
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println();
  Serial.println("Test Strat");
}

void loop() {
  serialEvent();
  SendText();
}

void SendText() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.println("AT");
    Serial1.write("AT");
    Serial1.write(0x0d);
    Serial1.write(0x0a);
  }
}

void serialEvent() {
  if(Serial.available() == false)
    return;
  String inChar;
  while (Serial.available()) {
    // get the new byte:
    inChar += (char)Serial.read();
  }
  Serial.println(inChar);
}
