#define BLYNK_TEMPLATE_ID "TMPL6sU_mp0Hg"
#define BLYNK_TEMPLATE_NAME "Test Blynk"
#define BLYNK_AUTH_TOKEN "S4doE35XQWDS2Tttrp-mGfK8kS6TE8t4"
#include "ESP8266WiFi.h"

#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "MayChi";
char pass[] = "maichi0709";

int sensorVal;
int sensorVal2;
int normalLed;
int warningLed;
BlynkTimer timer;

void myTimer() 
{
  sensorVal = analogRead(5);
  sensorVal2 = digitalRead(4);
  normalLed = digitalRead(2);
  warningLed = digitalRead(0);
  Blynk.virtualWrite(V0, sensorVal);  
  Blynk.virtualWrite(V1, sensorVal2);
  Blynk.virtualWrite(V2, normalLed);
  Blynk.virtualWrite(V3, warningLed);
}

void setup() {
  Serial.begin(9600);

  pinMode(5, INPUT); //D1
  pinMode(4, INPUT); //D2
  pinMode(0, OUTPUT); //D3 red
  pinMode(2, OUTPUT); //D4 green
  pinMode(2, OUTPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimer);
}

BLYNK_WRITE(V0) // Executes when the value of virtual pin 0 changes
{
  if(param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    digitalWrite(4,HIGH);  // Set digital pin 2 HIGH
  }
  else
  {
    // execute this code if the switch widget is now OFF
    digitalWrite(4,LOW);  // Set digital pin 2 LOW    
  }
}

void loop()
{
  int humid = analogRead(5);
  Serial.print("Humidity:");
  Serial.println(humid);
  

  int gas = digitalRead(4);

  if(gas == 1) {
    digitalWrite(0, HIGH);
    digitalWrite(2, LOW);
    Serial.println("Gas-concentration: HIGH");
  }
  else {
    digitalWrite(0, LOW);
    digitalWrite(2, HIGH);
    Serial.println("Gas-concentration: LOW");
  }
  Serial.println();
  Blynk.run();
  timer.run();
}
