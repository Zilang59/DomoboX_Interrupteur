#include <Arduino.h>
#include "HLK-LD2410S.h"

// Pin definitions
constexpr int RX_PIN = 19;
constexpr int TX_PIN = 18;
constexpr int INDICATOR_PIN = 10;
constexpr size_t SERIAL_NUMBER_BUFFER_SIZE = 16;

// Sensor object
LD2410S ld2410s(Serial1, RX_PIN, TX_PIN);

// Firmware version variables
uint16_t firmwareMajor = 0;
uint16_t firmwareMinor = 0;
uint16_t firmwarePatch = 0;

// Serial number buffer
char serialNumberBuffer[SERIAL_NUMBER_BUFFER_SIZE];

// Common parameters
uint32_t farDistance = 0;
uint32_t nearDistance = 0;
uint32_t delayTime = 0;
uint32_t freqStatus = 0;
uint32_t freqDistance = 0;
uint32_t responseTime = 0;

// Resultats
  bool PersonDetected = false;
  bool motionFromLD2410 = false;
  bool motionFromPin = false;
  uint16_t distanceFromLD2410 = 0;

void SetupLD2410() {
  DEBUG_PRINT("\n--INITIALISATION LD2410-----------------\n");
  // Dans la librairie HLK-LD2410S.cpp il ne faut pas hésiter à modifier les paramètres par défaut dans la fonction writeGenericParametersCommand() (ligne 405)
    // const uint8_t farthest_distance[6] = {0x05, 0x00, 0x01, 0x00, 0x00, 0x00};    // 1m
    // const uint8_t nearest_distance[6] = {0x0A, 0x00, 0x00, 0x00, 0x00, 0x00};     // 0.0m
    // const uint8_t delay_time[6] = {0x06, 0x00, 0x0A, 0x00, 0x00, 0x00};           // 10s
    // const uint8_t freq_status_report[6] = {0x02, 0x00, 0x28, 0x00, 0x00, 0x00};   // 4.0 Hz
    // const uint8_t freq_distance_report[6] = {0x0C, 0x00, 0x28, 0x00, 0x00, 0x00}; // 4.0Hz
    // const uint8_t response_speed[6] = {0x0B, 0x00, 0x0A, 0x00, 0x00, 0x00};       // response normal
  // Serial2.begin(256000, SERIAL_8N1, RX_PIN, TX_PIN);
  ld2410s.begin();
  delay(100);

  // LD2410_SetCustomParams(param.LD2410S_farthest, param.LD2410S_nearest, param.LD2410S_delaySec, param.LD2410S_freqHz, param.LD2410S_response);

  Lire_FirmwareVersion();
  Lire_SerialNumber();
  Lire_Parametres();

  

  LD2410_Setuped = true;

  DEBUG_PRINT("-----------------------------------------\n");
}

void Lire_FirmwareVersion() {
  if (ld2410s.readFirmwareVersion(firmwareMajor, firmwareMinor, firmwarePatch)) {
    DEBUG_PRINT("Firmware Version: ");
    DEBUG_PRINT(firmwareMajor);
    DEBUG_PRINT(".");
    DEBUG_PRINT(firmwareMinor);
    DEBUG_PRINT(".");
    DEBUG_PRINTLN(firmwarePatch);
  } else {
    DEBUG_PRINTLN("Failed to read firmware version");
  }
}
void Lire_SerialNumber() {
  if (ld2410s.readSerialNumber(serialNumberBuffer, sizeof(serialNumberBuffer))) {
    DEBUG_PRINT("Serial Number: ");
    DEBUG_PRINTLN(serialNumberBuffer);
  } else {
    DEBUG_PRINTLN("Failed to read serial number.");
  }
}
void Lire_Parametres() {
  if (ld2410s.readCommonParametersCommand(farDistance, nearDistance, delayTime, freqStatus, freqDistance, responseTime)) {
    DEBUG_PRINTLN("Parametres enregistrés dans le LD2410 :");
    DEBUG_PRINT("--FarDistance = ");
    DEBUG_PRINTLN(farDistance);
    DEBUG_PRINT("--nearDistance = ");
    DEBUG_PRINTLN(nearDistance);
    DEBUG_PRINT("--delayTime = ");
    DEBUG_PRINTLN(delayTime);
    DEBUG_PRINT("--freqStatus = ");
    DEBUG_PRINTLN(freqStatus);
    DEBUG_PRINT("--freqDistance = ");
    DEBUG_PRINTLN(freqDistance);
    DEBUG_PRINT("--responseTime = ");
    DEBUG_PRINTLN(responseTime);
  } else {
    DEBUG_PRINTLN("Failed to read common parameters");
  }
}


void LD2410_SetCustomParams(uint8_t farthest, uint8_t nearest, uint8_t delaySec, uint8_t freqHz, uint8_t response) {
    // format des paquets attendu par la librairie
    // uint8_t farthest_distance[6]       = {0x05, 0x00, farthest, 0x00, 0x00, 0x00};
    // uint8_t nearest_distance[6]        = {0x0A, 0x00, nearest, 0x00, 0x00, 0x00};
    // uint8_t delay_time[6]              = {0x06, 0x00, delaySec, 0x00, 0x00, 0x00};
    // uint8_t freq_status_report[6]      = {0x02, 0x00, freqHz, 0x00, 0x00, 0x00};
    // uint8_t freq_distance_report[6]    = {0x0C, 0x00, freqHz, 0x00, 0x00, 0x00};
    // uint8_t response_speed[6]          = {0x0B, 0x00, response, 0x00, 0x00, 0x00};

    // DEBUG_PRINTLN("[LD2410] Application des paramètres personnalisés...");

    // envoie directement les commandes au module (via Serial2 déjà utilisé par la lib)
    // Serial2.begin(256000, SERIAL_8N1, RX_PIN, TX_PIN);
    // Serial2.write(farthest_distance, 6);
    // Serial2.write(nearest_distance, 6);
    // Serial2.write(delay_time, 6);
    // Serial2.write(freq_status_report, 6);
    // Serial2.write(freq_distance_report, 6);
    // Serial2.write(response_speed, 6);
    // ld2410s.begin();

    // DEBUG_PRINTLN("[LD2410] Paramètres envoyés !");
    if (ld2410s.writeParametersCommand(farthest, nearest, delaySec, freqHz, response)) {
      DEBUG_PRINTLN("[LD2410] Paramètres envoyés !");
      Lire_Parametres();
    } else {
      DEBUG_PRINTLN("Problème d'envoie des paramètres !");
    }
}


String getLD2410Info() {
    LireLD2410();
    
    // Créer le JSON
    StaticJsonDocument<200> doc;
    doc["motion"] = motionFromLD2410;
    doc["pin"] = motionFromPin;
    doc["distance"] = distanceFromLD2410;
    doc["PersonDetected"] = PersonDetected;

    String jsonResponse;
    serializeJson(doc, jsonResponse);
    return jsonResponse; // Retourne le JSON sous forme de String
}
void LireLD2410() {
  motionFromLD2410 = ld2410s.isMotionDetected();
  motionFromPin   = digitalRead(INDICATOR_PIN);
  distanceFromLD2410 = ld2410s.getDistance();

  if(distanceFromLD2410 != 0) {
    DEBUG_PRINT("Target Distance: ");
    DEBUG_PRINT(distanceFromLD2410);
    DEBUG_PRINTLN(" cm");

    DEBUG_PRINT("Motion : ");
    DEBUG_PRINT(motionFromLD2410 ? "Motion" : "Static");
    DEBUG_PRINT(" - ");
    DEBUG_PRINT(motionFromPin ? "Detected" : "None");

    if(distanceFromLD2410 < 300) {
      DEBUG_PRINTLN(" - Il y a quelqu'un");
      PersonDetected = true;
    } else {
      PersonDetected = false;
      DEBUG_PRINTLN("");
    }
  }
}