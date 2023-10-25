int pwm = 5;

void setup() {
  Serial.begin(9600);
  pinMode(pwm, OUTPUT);
}

void loop() {
  for (int i = 0; i <= 255; i++)
  {
    analogWrite(pwm, i);
  }
  delay(2000);
}
