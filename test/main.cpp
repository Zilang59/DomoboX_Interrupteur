#include <Arduino.h>
#include "AllFunction.h" // Fichier généré automatiquement pour éviter les erreurs de "fonction non déclarées"

#include "afficheur_6D.h"
#include "INMP441.h"



void setup() {
  Serial.begin(115200);
  setupI2S();
  delay(10);
  setupTM1367();
  delay(10);
}

void loop() {
  if (getStableFrequency(freq, amp)) {
    int8_t digits[6];
    intToDigits((int)freq, digits);
    int8_t points[6] = {0, 0, 0, 0, 0, 0};
    afficher_Digits(digits,points);
  }
  delay(50);
}