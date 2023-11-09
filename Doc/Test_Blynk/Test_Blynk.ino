#define BLYNK_TEMPLATE_ID "TMPL6oh1hVyyp"
#define BLYNK_TEMPLATE_NAME "IoT Challenge 2023"
#define BLYNK_AUTH_TOKEN "pc4eUkYDBFplLPdSFrmzf63d1QtLjeLG"
#include "ESP8266WiFi.h"

#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "BingChilling";
char pass[] = "kochodau";


void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

BLYNK_WRITE(V1) // Executes when the value of virtual pin 0 changes
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
  Blynk.run();
}
