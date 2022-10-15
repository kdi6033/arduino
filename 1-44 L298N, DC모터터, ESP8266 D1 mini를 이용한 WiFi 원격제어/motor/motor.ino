int ENA = D8;
int INA1 = D7;
int INA2 = D6;
int cmd = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);

  digitalWrite(ENA, LOW);
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, LOW);

  Serial.println("DC motor test");

  Serial.println("Forward ");
  digitalWrite(ENA, HIGH);
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  delay(3000);
  
  Serial.println("Backward");
  digitalWrite(ENA, HIGH);
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  delay(3000);
  
  Serial.println("stop");
  digitalWrite(ENA, LOW);
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, LOW);
  delay(1000);
  
}

void loop()
{
  Serial.println("Forward-PWM");
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  for(int i=300;i<1000;i+=50) {
    Serial.println(i);
    analogWrite(ENA, i);
    delay(2000);
  }

  Serial.println("Backward-PWM");
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  for(int i=300;i<1000;i+=50) {
    Serial.println(i);
    analogWrite(ENA, i);
    delay(2000);
  }
}
