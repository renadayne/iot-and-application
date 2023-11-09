#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* ssid = "NOPIXELS";
const char* password = "99999999";
const char* mqtt_server = "test.mosquitto.org"; //Initial MQTT broker

WiFiClient espClient; //Object
PubSubClient client(espClient); //Object
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];

float temp;
int ledReceived;

int DHTPIN = 4; // D2
int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to Wi-Fi");

  WiFi.mode(WIFI_STA); // Mode user - standalone
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  ledReceived = atoi((char*)payload);
  Serial.print("Led Mode: ");
  Serial.print(ledReceived);
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish("Sensor", "hello IoT Gateway...");
      client.subscribe("Control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();  

  pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(2, OUTPUT);
  pinMode(5, OUTPUT); // D1
  setup_wifi();
  client.setServer(mqtt_server, 1883); // default port
  client.setCallback(callback);
}

void loop() {
  digitalWrite(5, ledReceived);
  temp = dht.readTemperature();
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  unsigned long now = millis();

  if (now - lastMsg > 1000) {
    lastMsg = now;
    //convert temp to char
    char get[20]; 
    dtostrf(temp, 4, 2, get);

    snprintf (msg, MSG_BUFFER_SIZE, get); //merge temp(string type) to msg
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("Sensor", msg);
  }
  Serial.println();
  //digitalWrite(0, ledReceived);
  
  delay(3000);
}
