#include <M5Stack.h>
#include <Wire.h>
#define STEPMOTOR_I2C_ADDR 0x70

char buf[100];

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  Wire.begin();
  Serial.begin(115200);
  m5.Lcd.setTextSize(2);

  M5.Lcd.println("StepMotor Test: 0x70");

  delay(1000);
  SendCommand(STEPMOTOR_I2C_ADDR, "G1 X.5Y1Z00 F500");
  delay(1000);
  SendCommand(STEPMOTOR_I2C_ADDR, "G1 X0Y0Z0 F400");
}

void SendByte(byte addr, byte b) {
  Wire.beginTransmission(addr);
  Wire.write(b);
  Wire.endTransmission();
}

void SendCommand(byte addr, char *c) {
  Wire.beginTransmission(addr);
  while ((*c) != 0) {
    Wire.write(*c);
    c++;
  }
  Wire.write(0x0d);
  Wire.write(0x0a);
  Wire.endTransmission();
}

void loop() {
  // put your main code here, to run repeatedly:
}
