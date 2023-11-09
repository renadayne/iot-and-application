/*
    Board pin | NodeMCU GPIO | 	Arduino IDE
          A- 										1 												5 or D1
          A+ 										3 												0 or D3
          B- 										2 												4 or D2
          B+ 										4 												2 or D4
*/

void setup()
{
  Serial.begin(9600);
  pinMode(5, INPUT); //D1
  pinMode(4, INPUT); //D2
  pinMode(0, OUTPUT); //D3 red
  pinMode(2, OUTPUT); //D4 green
  pinMode(2, OUTPUT);
}

void loop()
{
  int humid = analogRead(5);
  Serial.print("Humidity:");
  Serial.println(humid);
  

  int gas = analogRead(4);

  if(gas == 1023) {
    digitalWrite(0, LOW);
    digitalWrite(2, HIGH);
    Serial.println("Gas-concentration: LOW");
  }
  else {
    digitalWrite(0, HIGH);
    digitalWrite(2, LOW);
    Serial.println("Gas-concentration: HIGH");
  }
  Serial.println();
  delay(2000);
}

