#include <Arduino.h>

void printMemorySummary() {
    Serial.print("\n--POINT MEMOIRE--------------------------\n");
    // RAM
    Serial.printf("RAM   : %.2f / %.2f Ko\n", (ESP.getHeapSize() - ESP.getFreeHeap()) / 1024.0, ESP.getHeapSize() / 1024.0);
    if (psramFound()) {
        Serial.printf("PSRAM  : %.2f / %.2f Mo\n", ESP.getFreePsram() / (1024.0 * 1024), ESP.getPsramSize() / (1024.0 * 1024));
    }

    // Flash (programme)
    // Serial.printf("Taille totale de la partition OTA : %.2f Mo\n", (ESP.getFlashChipSize() - ESP.getSketchSize()) / (1024.0 * 1024));
    Serial.printf("FLASH  : %.2f / %.2f Mo\n", ESP.getSketchSize() / (1024.0 * 1024), ESP.getFreeSketchSpace() / (1024.0 * 1024));

    // Flash (SPIFFS)
    if (SPIFFS.begin(true)) {
        Serial.printf("SPIFFS : %.2f / %.2f Mo\n", SPIFFS.usedBytes() / (1024.0 * 1024), SPIFFS.totalBytes() / (1024.0 * 1024));
    }

    // Flash totale avec détails des partitions
    uint32_t flash_total = ESP.getFlashChipSize();
    
    Serial.printf("Total  : %.2f Mo\n", flash_total / (1024.0 * 1024));
    Serial.print("-----------------------------------------\n");
}

String formatTime(uint16_t totalSeconds) {
    uint8_t minutes = totalSeconds / 60;
    uint8_t seconds = totalSeconds % 60;

    String formattedTime = String(minutes) + " : " + (seconds < 10 ? "0" : "") + String(seconds);
    return formattedTime;
}