unsigned int counter = 0;
unsigned long previousMillis = 0;     
const long interval = 2000; 
String inputString = "";         // 받은 문자열

void tick();
void serial2Event();
void setup();

void setup() {
  Serial.begin(19200);

  // Serial1.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  Serial2.begin(19200, SERIAL_8N1, 26, 27); //RS-232
  //Serial2.begin(19200, SERIAL_8N1, 16, 17); //RS-485

}

void loop() {
  tick();
  serial2Event();
}

void tick() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    counter++;
    Serial2.println(String(counter));
  }  
}

void serial2Event() {
  while(Serial2.available()) {
    char ch = Serial2.read();
    //Serial.print(ch,HEX);
    //Serial.print(" ");
    Serial.write(ch);
  }
}
