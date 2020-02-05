unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 3000;           // interval at which to blink (milliseconds)

void setup() {
  // initialize serial:
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    Serial1.println("test");

  }
  serialEvent();
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    Serial.println(inChar);

  }
}
