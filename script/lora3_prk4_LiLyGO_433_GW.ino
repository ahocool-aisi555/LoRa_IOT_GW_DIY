#include <SPI.h>
#include <LoRa.h>
//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <PubSubClient.h>

//Gunakan pin ini untuk TTGO, sesuaikan jika berbeda
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26

//OLED pins
#define OLED_SDA 21
#define OLED_SCL 22 
#define OLED_RST -1
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

// Wi-Fi credentials
const char* ssid = "Gak_Perlu_Ngebut";
const char* password = "klengcinot";

// MQTT broker configuration
const char* mqtt_server = "mqtt.eclipseprojects.io";
const int mqtt_port = 1883;
const char* temp_topic = "/undiksha/temp";
const char* humi_topic = "/undiksha/humi";
const char* rssi_topic = "/undiksha/rssi";


// Wi-Fi client and MQTT client
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup_wifi() {
  Serial.println("Connecting to Wi-Fi...");

  display.setCursor(0,20);
  display.print("Connect Wifi.. ");
  display.display();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  display.print("..OK! ");
  display.display();
}


// Function to generate a random client name
String generateRandomClientName() {
  String prefix = "ESP32_LoRa_Client_";
  uint32_t randomValue = random(1000, 9999); // Generate a random number between 1000 and 9999
  return prefix + String(randomValue);
}

void reconnect_mqtt() {

  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Koneksi ke MQTT ..");
    display.display();
    
    // Generate a random client name
    String clientId = generateRandomClientName();
    Serial.print("Generated MQTT Client ID: ");
    Serial.println(clientId);

    // Connect to MQTT broker with the random client name
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Connected to MQTT broker");
      display.setCursor(0,10);
      display.print("Koneksi MQTT OK!");
      display.display();
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds...");
      display.setCursor(0,10);
      display.print("Koneksi MQTT Gagal!");
      display.display();
      delay(5000);
    }
  }
}


void setup()
{
    // Seed the random number generator
  randomSeed(analogRead(34)); // Use an unconnected analog pin for randomness
  Serial.begin(9600);
  while (!Serial);

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA GW UNDIKSHA");
  display.setCursor(0,10);
  display.println("Initialising.. ");
  display.display();
  
  Serial.println("LoRa Gateway Testing ..");


  // Initialize Wi-Fi
  setup_wifi();

  // Initialize MQTT client
  mqttClient.setServer(mqtt_server, mqtt_port);

  // Initialize LoRa
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(433E6)) { // Set frequency to 433 MHz
    Serial.println("LoRa initialization failed!");
    while (1);
  }
  Serial.println("LoRa initialized successfully.");

  display.setCursor(0,30);
  display.println("Lora Modul OK ! ");
  display.display();

}

void loop() {
  // Reconnect to MQTT broker if disconnected
  if (!mqttClient.connected()) {
    reconnect_mqtt();
  }
  mqttClient.loop();

  // Check for incoming LoRa packets
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incomingData = "";
    while (LoRa.available()) {
      incomingData += (char)LoRa.read();
    }

    // Get RSSI value
    int rssi = LoRa.packetRssi();

        // Debugging: Print raw received data
    Serial.print("Raw received data: ");
    Serial.println(incomingData);

    // Parse temperature and humidity as integers
    int tempInt = 0, humidityInt = 0;
    int parsed = sscanf(incomingData.c_str(), "Temp:%d,Humidity:%d", &tempInt, &humidityInt);

    if (parsed == 2) { // Ensure both values were parsed successfully
      // Convert integers back to floats
      float temperature = tempInt / 100.0;
      float humidity = humidityInt / 100.0;

      Serial.print("Parsed Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C, Humidity: ");
      Serial.print(humidity);
      Serial.print("%, RSSI=");
      Serial.println(rssi);

      display.clearDisplay();
      display.setCursor(0,0);
      display.print("Publish Ke MQTT");
      display.setCursor(0,10);
      display.print("Temp : ");
      display.print(temperature);
      display.println(" C");
      display.print("Humidity : ");
      display.print(humidity);
      display.println(" %");
      display.print("Rssi : ");
      display.print(rssi);
      display.print(" dBm");
      display.display();

    // Publish data to MQTT broker
    mqttClient.publish(temp_topic, String(temperature).c_str());
    mqttClient.publish(humi_topic, String(humidity).c_str());
    mqttClient.publish(rssi_topic, String(rssi).c_str());

    Serial.println("Published data to MQTT broker.");
  }
 }
}
