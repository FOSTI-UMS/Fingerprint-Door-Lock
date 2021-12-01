# How to Install Door-Lock System

Powered By FOSTI UMS

✨ Door lock system with fingerprint✨  

## Installation
### Langkah 1 :
Menyiapkan alat dan bahan ;
Bahan :
1. Modul NodeMCU ESP8266 
2. Kabel Jumper
3. Modul Fingerprint

Alat :
1. Menyediakan 2 device minimal 1 komputer/leptop dan 1 handphone
2. Micro-USB to USB cable
3. Breadboard

Menginstall software yang dibutuhkan pada perangkat komputer ;
1. VisualCodeStudio/text editor lainnya
2. Ekstensi platform io (pada VisualCodeStudio)

### Langkah 2 :
Menghubungkan semua port mengikuti skema di bawah ini

![](https://raw.githubusercontent.com/imaana/door-lock/main/assets/picture1.jpeg)

### Langkah 3 :
Menghubungkan NodeMCU yang sudah dihubungkan ke semua port ke PC melalui kabel USB

![](https://raw.githubusercontent.com/imaana/door-lock/main/assets/picture2.jpeg)

### Langkah 4 :
- Membuka software VisualCodeStudio/text editor lainnya
- Menginstall ekstensi platform io
- Membuka platform io
- Membuat project baru pada platform io

Menginstall modul-modul dibawah ini :
1. Arduino
2. ESP8266WiFi
3. SoftwareSerial
4. WiFiClient
5. ESP8266WebServer
6. Adafruit_Fingerprint
7. SPI
8. SD
9. ArduinoJson

Salin kode pada file src/main.cpp

![](https://raw.githubusercontent.com/imaana/door-lock/main/assets/picture4.jpeg)

![](https://raw.githubusercontent.com/imaana/door-lock/main/assets/picture5.jpeg)

![](https://raw.githubusercontent.com/imaana/door-lock/main/assets/picture6.jpeg)


> Note: `--capt-add=SYS-ADMIN` is required for PDF rendering.


## License

MIT

**FOSTI UMS 2021**

