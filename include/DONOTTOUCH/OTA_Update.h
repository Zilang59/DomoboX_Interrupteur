#include <ArduinoOTA.h>

#define setupOTAH true

void setupOTA() {
  DEBUG_PRINT("\n--PARAMETRAGE DE OTA---------------------\n");
  // Gestion de l'adresse MAC
    String macWifi_OTA = WiFi.macAddress();
    macWifi_OTA.replace(":", "");                  // Supprime les ":"
    macWifi_OTA.toUpperCase();                     // Optionnel, pour uniformité
    macWifi_OTA = macWifi_OTA.substring(macWifi_OTA.length() - 6); // Prend les 6 derniers caractères : 8460C0
    String hostname_IDOTA = Dispo_basename + "_" + macWifi_OTA;
    DEBUG_PRINT("OTA Hostname : ");
    DEBUG_PRINTLN(hostname_IDOTA);
    DEBUG_PRINT("OTA Password : ");
    DEBUG_PRINTLN("9z8c49z4");
  
  DEBUG_PRINT("-----------------------------------------\n");


  ArduinoOTA.setPort(3232);               // Port defaults to 3232
  ArduinoOTA.setHostname(hostname_IDOTA.c_str());     // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setPassword("9z8c49z4");     // No authentication by default
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else { // U_SPIFFS
        type = "filesystem";
        SPIFFS.end(); // On arrête SPIFFS avant une mise à jour du système de fichiers
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + String(type));
    })
    .onEnd([]() {
      Serial.println("\nEnd");

      delay(1000);
      ESP.restart();  // Redémarre pour charger la nouvelle partition
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.print("Progress: ");
      Serial.print(progress / (total / 100));
      Serial.println("%");
    })
    .onError([](ota_error_t error) {
      Serial.print("Error[");
      Serial.print(error);
      Serial.print("]: ");
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
      else {
        Serial.print("Unknown error: ");
        Serial.println(error);
      }
    });

  ArduinoOTA.begin();

  OTA_Setuped = true;
}

void handleOTA() {
  ArduinoOTA.handle();
}