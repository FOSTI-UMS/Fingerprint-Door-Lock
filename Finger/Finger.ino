#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


//Fingerprint scanner Pins
#define Finger_Rx 14 //D5
#define Finger_Tx 12 //D6

const char* ssid = "nodemcu";
const char* password = "00000000";

ESP8266WebServer server(80);

SoftwareSerial mySerial(Finger_Rx, Finger_Tx);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int FingerID = 0;     // The Fingerprint ID from the scanner 
uint8_t id;
bool lampu_on = true;
void setup(){
  Serial.begin(115200);
   pinMode(LED_BUILTIN, OUTPUT);
    // set the data rate for the sensor serial port
  finger.begin(57600);
  Serial.println("\n\nAdafruit finger detect test");
  digitalWrite(LED_BUILTIN, HIGH); 
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
 WiFi.mode(WIFI_AP);  
 WiFi.softAP(ssid, password);
 IPAddress myIP = WiFi.softAPIP();
 
   Serial.print(myIP);


 
  // Start the server
  
  server.on("/", [](){
    server.send(200, "text/html", "Home Fingerprint FOSTI<br><br>"
    "<a type='button' href='/enrollmodeon'>Enroll</a>");
    digitalWrite(LED_BUILTIN, HIGH); 
  });
    server.on("/enrollmodeon", [](){
      server.send(200, "text/html", "MODE ENROLL ON<br><br>"
      "<a type='button' href='/enrollmodeoff'>Enroll Off</a><br><br>"
      "<a type='button' href='/'>Back</a><br>");
      digitalWrite(LED_BUILTIN, LOW);

    
  });
    server.on("/enrollmodeoff", [](){
      server.send(200, "text/html", "MODE ENROLL OFF<br><br>"
      "<a type='button' href='/'>Back</a>");
      digitalWrite(LED_BUILTIN, HIGH);


    
  });
  server.begin();
  Serial.println("Server started");


}

void loop()                     // run over and over again
{
  server.handleClient();
  getFingerprintID();
  delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}
