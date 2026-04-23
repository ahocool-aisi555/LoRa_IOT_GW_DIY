//by : Nyoman Yudi Kurniawan
//for : TRSE - Undiksha - 2025
//www.aisi555.com

#include <SPI.h>
#include <LoRa.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"



// LoRa pins
#define LORA_CS_PIN 10
#define LORA_RST_PIN 9
#define LORA_DIO0_PIN 2

// OLED settings
#define I2C_ADDRESS 0x3C
#define RST_PIN -1

SSD1306AsciiWire oled;


void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial monitor to open

  // Initialize OLED

  Wire.begin();
  Wire.setClock(400000L);
  
  oled.begin(&Adafruit128x64, I2C_ADDRESS);

  
  oled.setFont(Arial14);
  oled.clear();
  oled.println("Initializing Gateway...");


  // Initialize LoRa
  SPI.begin();
  LoRa.setPins(LORA_CS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
  if (!LoRa.begin(433E6)) { // Set frequency to 433 MHz
    Serial.println("LoRa initialization failed!");
    while (1);
  }
  Serial.println("LoRa initialized successfully.");

  oled.println("Lora Gateway OK !");
  delay(2000);
}

void loop() {
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
    //Serial.print("Raw received data: ");
    //Serial.println(incomingData);

    // Parse temperature and humidity as integers
    int tempInt = 0, humidityInt = 0;
    int parsed = sscanf(incomingData.c_str(), "Temp:%d,Humidity:%d", &tempInt, &humidityInt);

    if (parsed == 2) { // Ensure both values were parsed successfully
      // Convert integers back to floats
      float temperature = tempInt / 100.0;
      float humidity = humidityInt / 100.0;

      Serial.print("{ \"Temp\": ");
      Serial.print(temperature);
      Serial.print(", \"Humi\": ");
      Serial.print(humidity);
      Serial.print(", \"Rssi\": ");
      Serial.print(rssi);
      Serial.println("}");
    
          // Update OLED with received data
      oled.clear();
      oled.println("Gateway is Receiving..");
      oled.print("Temp: ");
      oled.print(temperature);
      oled.println(" C");
      oled.print("Humidity: ");
      oled.print(humidity);
      oled.println(" %");
      oled.print("Rssi: ");
      oled.print(rssi);
      oled.println(" dBm");

    
    } else {
      Serial.println("Failed to parse data!");
    }
  }
}
