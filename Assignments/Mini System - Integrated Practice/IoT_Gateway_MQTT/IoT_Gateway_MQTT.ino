#define BLYNK_TEMPLATE_ID "TMPL6sU_mp0Hg"
#define BLYNK_TEMPLATE_NAME "Test Blynk"
#define BLYNK_AUTH_TOKEN "S4doE35XQWDS2Tttrp-mGfK8kS6TE8t4"
#include "ESP8266WiFi.h"
#include <PubSubClient.h>
#include <DHT.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "khactinhgioitre";
const char* password = "12356789";
const char* mqtt_server = "test.mosquitto.org"; //Initial MQTT broker

WiFiClient espClient; //Object
PubSubClient client(espClient); //Object
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];

float tempReceived;
char led; 

// MQTT SETUP/////////////////////////////////////////////////////////////////////////////////////////
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
  Serial.print("] ");
  tempReceived = atof((char*)payload);
  Serial.print("Temp Received: ");
  Serial.print(tempReceived);
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
      client.publish("Control", "hello IoT Node...");
      client.subscribe("Sensor");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
// BLYNK SETUP////////////////////////////////////////////////////////////////////////////////////////////////////
BlynkTimer timer;

void myTimer() 
{
  Blynk.virtualWrite(V0, tempReceived);  
}

BLYNK_WRITE(V1) // Executes when the value of virtual pin 0 changes
{
  if(param.asInt() == 1)
  {
    led = 1;
  }
  else
  {
    led = 0;
  }
}

void setup() {
  Serial.begin(9600);

  setup_wifi();
  client.setServer(mqtt_server, 1883); // default port
  client.setCallback(callback);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  timer.setInterval(1000L, myTimer);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  unsigned long now = millis();

  if (now - lastMsg > 2000) {
    lastMsg = now;

    char get[20]; 
    dtostrf(led, 4, 2, get); 

    snprintf (msg, MSG_BUFFER_SIZE, get); //merge temp(string type) to msg
    Serial.print("Publish message - Led mode: ");
    Serial.println(msg);
    client.publish("Control", msg);
  }

  Serial.println();
  Blynk.run();
  timer.run();
  delay(2000);
}



/*
Blynk Configuration:

Mã đầu tiên xác định các thông tin cần thiết để kết nối với Blynk IoT platform. Điều này bao gồm ID mẫu, tên mẫu và mã xác thực.
WiFi và MQTT Configuration:

Điều này được thực hiện giống với mã bạn đã đưa ra trong câu trước, đặt tên SSID và mật khẩu của mạng Wi-Fi và máy chủ MQTT (ban đầu là "test.mosquitto.org").
Global Variables:

Biến toàn cục được khai báo để lưu trữ các giá trị nhiệt độ (tempReceived) và trạng thái của LED (led).
MQTT Setup:

Hàm setup_wifi được sử dụng để kết nối ESP8266 với mạng Wi-Fi. Sau khi kết nối thành công, nó sẽ seed bộ sinh số ngẫu nhiên.
Hàm callback được gọi khi một tin nhắn MQTT được nhận. Nó giải mã dữ liệu từ tin nhắn và lưu trữ giá trị nhiệt độ trong biến tempReceived.
Hàm reconnect cố gắng kết nối lại với máy chủ MQTT nếu kết nối bị mất.
Blynk Setup:

Trong hàm setup, Blynk được kết nối bằng cách gọi Blynk.begin với thông tin xác thực và thông tin mạng.
Hàm myTimer được đặt để gửi giá trị nhiệt độ đến một biểu đồ Blynk cứ sau mỗi giây. Điều này được xác định bởi timer.setInterval.
Loop:

Hàm loop chạy trong vòng lặp vô hạn và có các phần sau:
Kiểm tra xem kết nối với máy chủ MQTT còn hoạt động hay không, và thực hiện kết nối lại nếu cần.
Gửi giá trị trạng thái LED thông qua MQTT đến máy chủ MQTT. Giá trị LED được chuyển đổi thành chuỗi và gửi đi qua chủ đề "Control".
Cập nhật giá trị Blynk sử dụng hàm Blynk.run và gửi giá trị nhiệt độ đến biểu đồ Blynk.
*/