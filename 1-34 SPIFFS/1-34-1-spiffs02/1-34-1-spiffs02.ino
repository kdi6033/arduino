// Very basic Spiffs example, writing 10 strings to SPIFFS filesystem, and then read them back
// For SPIFFS doc see : https://github.com/esp8266/Arduino/blob/master/doc/filesystem.rst
// Compiled in Arduino 1.6.7. Runs OK on Wemos D1 ESP8266 board.

#include "FS.h"

void setup() {
  Serial.begin(115200);
  Serial.println("\nVery basic Spiffs example, writing 10 lines to SPIFFS filesystem, and then read them back");
  SPIFFS.begin();
  // Next lines have to be done ONLY ONCE!!!!!When SPIFFS is formatted ONCE you can comment these lines out!!
  Serial.println("Please wait 30 secs for SPIFFS to be formatted");
  SPIFFS.format();
  Serial.println("Spiffs formatted");
}

void loop() {

  // open file for writing
  File f = SPIFFS.open("/f.txt", "w");
  if (!f) {
      Serial.println("file open failed");
  }
  Serial.println("====== Writing to SPIFFS file =========");
  // write 10 strings to file
  for (int i=1; i<=10; i++){
    f.print("Millis() : ");
    f.println(millis());
    Serial.println(millis());
  }

  f.close();

  // open file for reading
  f = SPIFFS.open("/f.txt", "r");
  if (!f) {
      Serial.println("file open failed");
  }  Serial.println("====== Reading from SPIFFS file =======");
  // write 10 strings to file
  for (int i=1; i<=10; i++){
    String s=f.readStringUntil('\n');
    Serial.print(i);
    Serial.print(":");
    Serial.println(s);
  }

  // wait a few seconds before doing it all over again
  delay(3000);
 
}
