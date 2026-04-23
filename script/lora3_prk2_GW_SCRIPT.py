# by : Nyoman Yudi Kurniawan
# for : TRSE - Undiksha - 2025
# www.aisi555.com

import serial
import json
import paho.mqtt.client as mqtt
import ssl

# Serial port configuration
SERIAL_PORT = "COM6"  # Replace with your serial port (e.g., /dev/ttyUSB0 on Linux)
BAUD_RATE = 9600

# MQTT broker configuration
MQTT_BROKER = "broker.hivemq.com" #ganti test.mosquitto.org jika brokernya error
MQTT_PORT = 1883
TEMP_TOPIC = "/undiksha/temp"
HUMI_TOPIC = "/undiksha/humi"
RSSI_TOPIC = "/undiksha/rssi"

# Initialize serial connection
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)


def on_publish(client,userdata,result):
    print("data terkirim ke broker")
    pass


# Initialize MQTT client

client = mqtt.Client()

client.on_publish = on_publish
client.connect(MQTT_BROKER, MQTT_PORT)

print("Connected to MQTT broker and serial port.")

try:
    while True:
        # Read data from serial port
        line = ser.readline().decode('utf-8').strip()
        if line.startswith("{") and line.endswith("}"):  # Check if it's a JSON string
            try:
                # Parse JSON data
                data = json.loads(line)
                temperature = data.get("Temp")
                humidity = data.get("Humi")
                rssi = data.get("Rssi")

                if temperature is not None and humidity is not None and rssi is not None:
                    print(f"Received: Temperature={temperature}, Humidity={humidity}, RSSI={rssi}")

                    # Publish to MQTT broker
                    client.publish(TEMP_TOPIC, str(temperature))
                    client.publish(HUMI_TOPIC, str(humidity))
                    client.publish(RSSI_TOPIC, str(rssi))
                    print(f"Published: {TEMP_TOPIC}={temperature}, {HUMI_TOPIC}={humidity}, {RSSI_TOPIC}={rssi}")
            except json.JSONDecodeError:
                print("Failed to decode JSON data.")
except KeyboardInterrupt:
    print("Exiting...")
finally:
    ser.close()
    client.disconnect()
