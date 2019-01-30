#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "images.h"

//OLED pins to ESP32 GPIOs via this connecthin:
//OLED_SDA -- GPIO4
//OLED_SCL -- GPIO15
//OLED_RST -- GPIO16
SSD1306  display(0x3c, 4, 15);

int count=0;
String ss;

void setup() {
  DisplaySetup();
  Serial.begin(9600);
  Serial.println();
  Serial.println();
}

void DisplaySetup() {
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void drawFontFaceDemo() {
    display.clear();
    ss="";
    ss="count: ";
    ss=ss+count;
    Serial.println(ss);
    display.drawString(0, 0, ss);
    display.display();
}
void loop() {
  // put your main code here, to run repeatedly:
  drawFontFaceDemo();
  count++;
  delay(1000);
}
