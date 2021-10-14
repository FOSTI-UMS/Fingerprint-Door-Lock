#include "nada.h"
#include "Arduino.h"

void toneUp(int buzzer) {
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
void toneDown(int buzzer) {
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
void toneRegistered(int buzzer) {
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
void toneError(int buzzer) {
    //Nada info Error
    tone(buzzer, 2000);
    delay(2000);
    noTone(buzzer);
}
