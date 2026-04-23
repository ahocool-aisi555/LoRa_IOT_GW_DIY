# LoRa IOT GateWay Do It Yourself

<img width="500" height="300" alt="Lora3 pptx" src="https://github.com/user-attachments/assets/6abefdac-3aac-4b93-8343-e6e1694293a9" />


# Selayang Pandang

Belajar Membuat Gateway IOT - LoRa - MQTT  melalui praktek langsung<br>
Saya bagikan ilmu LoRa dan LoRaWan yang saya sudah geluti sejak 2021<br>
Melalui praktek yang sudah saya laksanankan bersama TRSE -Undiksha 2025<br>
4 Modul ini semoga dapat membantu siapapun yang sedang belajar komunikasi LoRa<br><br>
Part 1 : https://github.com/ahocool-aisi555/LoRa_1<br>
Part 2 : https://github.com/ahocool-aisi555/LoRa_2

# Library

Arduino LoRa by Sandeepmistry : https://github.com/sandeepmistry/arduino-LoRa <br>
Untuk menhemat memori vs Display Oled library harap install : SSD1306Ascii


# Bahan-Bahan

1. Modul LoRa Semtech SX1278 /Ra-02 + antena <br>
2. Arduino Nano atau clonenya , disini saya pilihkan LGT8F328P-LQFP48 <br>
   Level logic 3.3V sehingga gak perlu level shifter<br>
   Silahkan baca disini untuk dapat digunakan di IDE arduino : https://github.com/dbuezas/lgt8fx<br>
3. OLED SSD1306 - 128 X 64 PIXEL<br>
4. DHT11 / DHT22 <br>
5. Potensiometer 50-100 kilo Ohm<br>
6. LilyGo / TTGO Lora - ESP32 - Ready to use Lora modul + Display <br>
7. Breadboard atau Project Board<br>

# Dasar Teori

<img width="500" height="300" alt="Lora3 pptx (1)" src="https://github.com/user-attachments/assets/5cc2b21a-a195-4f9c-844b-b58b762d4db7" />

<img width="500" height="300" alt="Lora3 pptx (2)" src="https://github.com/user-attachments/assets/8fd4a4f7-a955-4cad-8674-6c52bfa990d5" />

# Python MQTT data simulator ke IOT MQTT Panel (Smartphone)

Silahkan belajar ke blog saya : https://www.aisi555.com/2021/11/mqtt-data-simulator-menggunakan-python.html <br>
Atau ikuti script dibawah ini
```python
import paho.mqtt.client as mqtt
from time import sleep
from random import randrange

# Inisialisasi broker 
broker_address="broker.hivemq.com" 		  # bisa gunakan broker lain seperti test.mosquitto.org
broker_port=1883

def on_publish(client,userdata,result):                     #create function for callback
    print("data terkirim ke broker")
    pass

client= mqtt.Client(f'undiksha-client-{randrange(0,100)}')  #clientnya harus random
client.on_publish = on_publish                              #assign function to callback
client.connect(broker_address,broker_port)                  #establish connection
while :                                                     #loop terus sambil kirim data
    suhu = float( randrange(250,350,2) / 10)                #random suhu dan kelembaban
    humi = randrange(50,95,2)
    ret= client.publish("/testyuk/suhu",suhu)               #PUB suhu ke topik /testyuk/suhu
    sleep(3)
    ret= client.publish("/testyuk/humi",humi)               #PUB Kelembaban ke topik /testyuk/humi
    sleep(3)

```

<img width="500" height="300" alt="Lora3 pptx (5)" src="https://github.com/user-attachments/assets/b99d730b-6b6b-4f33-a83e-ebee49aca209" />

# Membaca Data Serial Dari Arduino Uno 

Data serial pada mikrokontroler/arduino yang terhubung PC langsung oleh kabel USB dapat diolah langsung dan selanjutnya di baca oleh PC dengan koding python sebagai inputan data.

```python
import serial
# Serial port configuration
SERIAL_PORT = "COM6"  # sesuaikan port pc kamu
BAUD_RATE = 9600
# Initialize serial connection
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
try:
    while True:
        # Read data from serial port
        line = ser.readline().decode('utf-8').strip()
        print(line)
except KeyboardInterrupt:
    print("Exiting...")
finally:
    ser.close()

```

# Praktek 1 - 2 Kirim Suhu Antar SX1278/Ra02 lalu ke PC dan MQTT broker

<img width="500" height="300" alt="Lora3 pptx (3)" src="https://github.com/user-attachments/assets/2c0298b9-1405-4f49-8de4-40f74b026b32" />

<img width="500" height="300" alt="Lora3 pptx (4)" src="https://github.com/user-attachments/assets/89cd1f5f-9756-4ec2-99a1-cfe84f258138" />

<img width="500" height="300" alt="Lora3 pptx (6)" src="https://github.com/user-attachments/assets/e2f83dac-629a-4c52-8601-0b4a90e9589c" />

<img width="500" height="300" alt="Lora3 pptx (7)" src="https://github.com/user-attachments/assets/d7e7b936-a6a0-4a21-841d-73019a3a2351" />

<img width="500" height="300" alt="Lora3 pptx (8)" src="https://github.com/user-attachments/assets/ece4edee-31af-40ce-bad6-dbcb87acbb62" />


# Praktek 3 - Suhu Dari DHT22 - Ra02 - Kirim ke LilyGo lanjut ke Broker MQTT via Wifi

<img width="960" height="540" alt="Lora3 pptx (9)" src="https://github.com/user-attachments/assets/d576ba88-8dc9-4e6a-95f7-4aac3351cef4" />

# Acknowledgement

Bapak Ketut Udy Ariawan ST. MT. - Koordinator Prodi Teknik Rekayasa Sistem Elektronika - Fakultas Teknik Dan Kejuruan - Universitas Pendidikan Ganesha - Singaraja Bali 
