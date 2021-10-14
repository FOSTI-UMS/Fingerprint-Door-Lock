#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_Fingerprint.h>
#include "SPI.h"
#include "SD.h"
#include <ArduinoJson.h>


//PIN SENSOR FINGER PRINT
#define Finger_Rx 14 //D5
#define Finger_Tx 12 //D6
//PIN BUZZER
int buzzer = 13;

//CONST
const char* ssid = "fosti-sekre";
const char* password = "00000000";
const int chipSelect = D8;
bool ENROLL_MODE = false;
String ADMIN_PASSWORD = "12345";
bool IS_LOCKED = true;
int fpBlink = 500;
int id_baru = 0;
String nama_baru = "";


//WEBSERVER OBJECT
ESP8266WebServer server(80);

//SERIAL FINGER PRINT
SoftwareSerial mySerial(Finger_Rx, Finger_Tx);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int FingerID = 0;
uint8_t id;


// FUNGSI STRING IS TERKUNCI
String str_isTerkunci() {
  return IS_LOCKED ? "TERKUNCI" : "TERBUKA";
}

// FUNGSI STRING LINK BUKA KUNCI LEWAT WEB
String str_lock_link() {
  return IS_LOCKED ? "<a class=\"btn button-biru\" href=\"/unlock\" type=\"button\">BUKA KUNCI</a>" : "<a class=\"btn button-biru\" href=\"/lock\" type=\"button\">TUTUP KUNCI</a>";
}

// HEX
void printHex(int num, int precision) {
  char tmp[16];
  char format[128];

  sprintf(format, "%%.%dX", precision);

  sprintf(tmp, format, num);
  Serial.print(tmp);
}

// WEB HALAMAN UTAMA
String homepage() {
  return "<!doctype html>\n"
"<html lang=\"en\">\n"
"  <head>\n"
"    <!-- Required meta tags -->\n"
"    <meta charset=\"utf-8\">\n"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
"\n"
"<style>\n"
".button-biru {\n"
"  background-color: blue; /* Green */\n"
"  border: none;\n"
"  color: white;\n"
"  padding: 15px 32px;\n"
"  text-align: center;\n"
"  text-decoration: none;\n"
"  display: inline-block;\n"
"  font-size: 16px;\n"
"\n"
".button-merah {\n"
"  background-color: red; /* Green */\n"
"  border: none;\n"
"  color: white;\n"
"  padding: 15px 32px;\n"
"  text-align: center;\n"
"  text-decoration: none;\n"
"  display: inline-block;\n"
"  font-size: 16px;\n"
"\n"
".button-hijau {\n"
"  background-color: green; /* Green */\n"
"  border: none;\n"
"  color: white;\n"
"  padding: 15px 32px;\n"
"  text-align: center;\n"
"  text-decoration: none;\n"
"  display: inline-block;\n"
"  font-size: 16px;\n"
"}\n"
"</style>\n"
"    <title>Hello, world!</title>\n"
"  </head>\n"
"  <body>\n"
"    <h1>Hello Admin FOSTI</h1>\n"
"<div class=\"d-grid gap-2\">\n"
"<a href=\"/enrollpage\" class=\"btn button-hijau\" type=\"button\">DAFTAR USER</a>\n"
"<a href=\"/enrollpage\" class=\"btn button-hijau\" type=\"button\">ENROLL BARU</a>\n"
+ str_lock_link() +
"</div>\n"
"  </body>\n"
"</html>";
}





// WEB HALALAMAN PENDAFTARAN SIDIK JARI
String enrollpage() {
    int sisaSlot = 0;
    String pilihanSlot = "";
    for (int id = 1; id < 120; id++) {
        uint8_t p = finger.loadModel(id);
        if (p == FINGERPRINT_OK) {

        } else {
            pilihanSlot += "  \t\t<option value=\""+String(id)+"\">Slot "+String(id)+"</option>\n";
            sisaSlot++;
        }
    }
  return "<!doctype html>\n"
"<html lang=\"en\">\n"
"  <head>\n"
"    <!-- Required meta tags -->\n"
"    <meta charset=\"utf-8\">\n"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
"\n"
"\n"
"<style>\n"
".button-biru {\n"
"  background-color: blue; /* Green */\n"
"  border: none;\n"
"  color: white;\n"
"  padding: 15px 32px;\n"
"  text-align: center;\n"
"  text-decoration: none;\n"
"  display: inline-block;\n"
"  font-size: 16px;\n"
"}\n"
"\n"
".button-merah {\n"
"  background-color: red; /* Green */\n"
"  border: none;\n"
"  color: white;\n"
"  padding: 15px 32px;\n"
"  text-align: center;\n"
"  text-decoration: none;\n"
"  display: inline-block;\n"
"  font-size: 16px;\n"
"}\n"
"\n"
".button-hijau {\n"
"  background-color: green; /* Green */\n"
"  border: none;\n"
"  color: white;\n"
"  padding: 15px 32px;\n"
"  text-align: center;\n"
"  text-decoration: none;\n"
"  display: inline-block;\n"
"  font-size: 16px;\n"
"}\n"
"</style>"
"    <title>Hello, world!</title>\n"
"  </head>\n"
"  <body>\n"
"<form action=\"/tambahuser\">\n"
"<table>\n"
"\t<tr>\n"
"\t\t<td>\n"
"\t\tSISA SLOT\n"
"\t\t</td>\n"
"\t\t<td>"+String(sisaSlot)+"</td>\n"
"\t</tr>\n"
"\t<tr>\n"
"\t\t<td>\n"
"\t\tID\n"
"\t\t</td>\n"
"\t\t<td>\n"
"\t<select class=\"form-select\" name=\"id\">\n"

"" + pilihanSlot + ""
"\t</select>\n"
"\t\t</td>\n"
"\t</tr>\n"
"\n"
"\t<tr>\n"
"\t\t<td>\n"
"\t\tNAMA\n"
"\t\t</td>\n"
"\t\t<td>\n"
"\t\t<input type=\"text\" class=\"form-control\" name=\"nama\">\n"
"\t\t</td>\n"
"\t</tr>\n"
"\n"
"\t<tr>\n"
"\t\t<td>\n"
"\t\tPASSWORD ADMIN\n"
"\t\t</td>\n"
"\t\t<td>\n"
"\t\t<input type=\"text\" class=\"form-control\"  name=\"password\">\n"
"\t\t</td>\n"
"\t</tr>\n"
"\n"
"\t<tr>\n"
"\t\t<td>\n"
"\t\t\n"
"\t\t</td>\n"
"\t\t<td>\n"
"\t\t  <input type=\"submit\" class=\"form-control\"  name=\"submit\"/>\n"
"\t\t</td>\n"
"\t</tr>\n"
"</table>\n"
"\n"
"</form>"

"  </body>\n"
"</html>";
}



// COMPOSER NADA BUZZER
void toneUp() {
  //Nada info naik
  tone(buzzer, 1000);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 1500);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 2000);
  delay(100);
  noTone(buzzer);
}
void toneDown() {
  //Nada info turun
  tone(buzzer, 2000);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 1500);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 1000);
  delay(100);
  noTone(buzzer);
}
void toneRegistered() {
  //Nada info terdaftar
  tone(buzzer, 2000);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 1500);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 1000);
  delay(2000);
  noTone(buzzer);
}
void toneError() {
    //Nada info Error
    tone(buzzer, 2000);
    delay(2000);
    noTone(buzzer);
}

void openLock() {
  // NADA NDADA CHAKK
  toneUp();
  IS_LOCKED = false;
  for (int i = 0; i < 5; i++)
  {
    
    finger.LEDcontrol(true);
    delay(500);
    finger.LEDcontrol(false);
    delay(500);

  }
  IS_LOCKED = true;
  toneDown();
  
}


// FUNGSI SCAN FINGER OPEN LOCK JIKA COCOK
uint8_t scanFinger() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gambar diambil");      
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Tidak ada jari terdeteksi");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Koneksi Error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Prosses Error");
      return p;
    default:
      Serial.println("Error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gambar dikonversi");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Gmbar tidak jelas");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("koneksi error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Tidak dapat mendeteksi fitur");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Tidak dapat mendeteksi fitur");
      return p;
    default:
      Serial.println("Error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Ditemukan Kecocokan");
    openLock();
    return p;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Koneksi Error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Tidak ditemukan kecocokan");
    toneError();
    return p;
  } else {
    Serial.println("Error");
    return p;
  }

  // COCOK
  Serial.print("ID #"); Serial.print(finger.fingerID);
  Serial.print("NILAI "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// FUNGSI SCAN FINGER UNTUK ENROLL
uint8_t getFingerprintEnroll(int id, String nama) {
                   
  
  int p = -1;
  Serial.println(id);
  while (p != FINGERPRINT_OK) {


    delay(50);    
    tone(buzzer, 2000);                 
    finger.LEDcontrol(false);
    delay(50);  
    noTone(buzzer);

    finger.LEDcontrol(true);
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gmabr diambil");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Koneksi Error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Penggambaran Error");
      break;
    default:
      Serial.println("Error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gambar dikonversi");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Gambar tidak jelas");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Koneksi error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Tidak ditemukan sidik");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Tidak ditemukan sidik");
      return p;
    default:
      Serial.println("Error");
      return p;
  }

  Serial.println("Angkat Jari");
  toneUp();
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Letakan jari yang sama");
  while (p != FINGERPRINT_OK) {
    
    delay(50);    
    tone(buzzer, 2000);                 
    finger.LEDcontrol(false);
    delay(50);  
    noTone(buzzer);


    finger.LEDcontrol(true);
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gambar diambil");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("koneksi error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Penggambaran Error");
      break;
    default:
      Serial.println("Error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Gambar dikonversi");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Gambar tidak jelas");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("koneksi error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("tidak ditemukan sidik");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("tidak ditemukan sidik");
      return p;
    default:
      Serial.println("Error");
      return p;
  }

  // OK converted!
  Serial.print("Membuat model FP");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Cocok !!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Koneksi error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Repeat tidak cocok");
    return p;
  } else {
    Serial.println("Error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Disimoan sebagai " + nama);
    
    toneRegistered();

    //SIMPAN KEY VALUE JSON ID

    //SET MODE ENROLL FALSE
    ENROLL_MODE = false;

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Koneksi error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("TIdak bisa menyimpan");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Memori Flash Error");
    return p;
  } else {
    Serial.println("Error");
    return p;
  }

  return true;
}


//SETUP WEB SERVER ENDPOINTS
void setupEndpoints() {
    server.on("/", [](){
        server.send(200, "text/html",
        homepage()
      );
    });

    server.on("/unlock", [](){
      IS_LOCKED = false;
      openLock();
      server.sendHeader("Location", String("/"), true);
      server.send(302, "text/plane",""); 
    });
    server.on("/lock", [](){
      IS_LOCKED = true;
      server.sendHeader("Location", String("/"), true);
      server.send(302, "text/plane",""); 
    });

    server.on("/enrollpage", [](){
        server.send(200, "text/html", enrollpage());
    });

    server.on("/tambahuser", [] () {

        // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
        if (server.arg("id") != "" && server.arg("nama") != "" && server.arg("password") != "") {
          int id = server.arg("id").toInt();
          String nama = server.arg("nama");
          String password = server.arg("password");
          if (password == ADMIN_PASSWORD) {

            id_baru = id;
            nama_baru = nama;
            ENROLL_MODE = true;
          } else {
            tone(buzzer, 5000);
            delay(5000);
            noTone(buzzer);
            ENROLL_MODE = false;
          }
          
        } else {
            tone(buzzer, 5000);
            delay(5000);
            noTone(buzzer);
            ENROLL_MODE = false;
        }

        server.send(200, "text/html", "<h1>IKUTI NADA INTRUKSI, jika lampu berkelip cepat letakan jari anda di sensor, lalu akan ada tanda suara setelah itu sensor akan berkedip lalu letakan lagi jari yang sama untuk verifikasi</h1>");
    });

    server.begin();
}



void setup() {
  Serial.begin(115200);
  Serial.print("Menyalakan Wifi .. AP");
  WiFi.mode(WIFI_AP);  
  //WiFi.softAP(ssid, password); //Dengan Password
    
  WiFi.softAP(ssid); //Tanpa Password ketika sudah dipasang jangan gunakan ini
  IPAddress myIP = WiFi.softAPIP();
  Serial.print(myIP);

  setupEndpoints();
  server.begin();
  Serial.println("Server Start");
  
  finger.begin(57600);
  Serial.println("\n\nAdafruit finger");

  if (finger.verifyPassword()) {
    Serial.println("Sensor OK!");
  } else {
    Serial.println("Sensor Error");
    //while (1) { delay(1); }
  }

  //setup sdcard untuk oengembangan selanjutnya
  // if (!SD.begin(chipSelect)) {
  //   Serial.println("Initialization failed!");
  //   while (1);
  // }
}


void jsontoOBJ() {
  char json[] = "{'data':[{'finger':'54','nama':Husni Thamrin,'nim':'L200190002'},{'finger':'54','nama':Dimas,'nim':'L200190002'}]}";

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);

  for (int i = 0; i < doc["data"].size(); i++)
  {
    String nama = doc["data"][i]["nama"];
    Serial.println(nama);
  }
}

void loop() {
  server.handleClient();
  Serial.flush();
  //jsontoOBJ();
  Serial.println(finger.getImage());
  toneRegistered();
  //BLINKING
  finger.LEDcontrol(true);
  // delay(fpBlink);                       
  // finger.LEDcontrol(false);
  // delay(fpBlink);      


  //CHECK MODE
  if (ENROLL_MODE) {
    // SCAN UNTUK ENROLL
    getFingerprintEnroll(id_baru, nama_baru);
  } else {
    // SCAN UNTUK MEMBUKA
    scanFinger();
  }

  //WRITE DATA ke sd card untuk pengembangan selanjutnya
  // File dataFile = SD.open("anggota.txt", FILE_WRITE);

  // if (dataFile) {
  //   dataFile.println("DATA");
  //   dataFile.close();
  //   Serial.println("OK!!");
  // }
  
  // else {
  //   Serial.println("error opening datalog.txt");
  // }
  delay(20);
} 