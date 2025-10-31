#include <Arduino.h>
#include <ArduinoJson.h>


String ID_ALEXA = Dispo_basename;
uint32_t TimeForHotspot = 1000 * 60 * 5; // 5 minutes
uint32_t ESPStarted = 0;
bool hotspotDisabled = false;

bool old_ValiseOuverte = false;



// DEBUG sur le port série
    #ifdef DEBUG
        #define DEBUG_PRINT(x) Serial.print(x)
        #define DEBUG_PRINTLN(x) Serial.println(x)
        const bool DEBUGGING = true;
    #else
        #define DEBUG_PRINT(x)
        #define DEBUG_PRINTLN(x)
        const bool DEBUGGING = false;
    #endif



