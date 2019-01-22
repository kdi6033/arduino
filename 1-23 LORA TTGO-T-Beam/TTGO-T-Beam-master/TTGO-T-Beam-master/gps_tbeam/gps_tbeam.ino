void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, 12, 15);   //RX TX 
}

void loop()
{
  while (Serial1.available() > 0){
    // get the byte data from the GPS
    byte gpsData = Serial1.read();
    Serial.write(gpsData);
  }
}
