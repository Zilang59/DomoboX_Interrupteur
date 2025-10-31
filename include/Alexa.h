#include <Arduino.h>
#include "fauxmoESP.h"
fauxmoESP fauxmo;

#define AlexaH true


void SetupAlexa() {
  // démarrer le serveur Alexa
    fauxmo.createServer(true);
    fauxmo.setPort(80); // Port par défaut 80 modifier le port si vous utilisez un serveur web sur le port 80 !
    fauxmo.enable(true);


  // Ouvrir le nom du dispositif
    const char *fileName = "/wifi.json";
    StaticJsonDocument<2000> doc;
    
    if (SPIFFS.exists(fileName))
    {
      File file = SPIFFS.open(fileName, "r");
      if (!file)
      {
        DEBUG_PRINTLN("Erreur d'ouverture du fichier pour lecture");
      }

      // Lire le contenu du fichier JSON
      size_t size = file.size();
      if (size > 1024)
      {
        DEBUG_PRINTLN("Fichier trop grand");
      }

      std::unique_ptr<char[]> buf(new char[size]);
      file.readBytes(buf.get(), size);

      // Désérialiser le JSON
      DeserializationError error = deserializeJson(doc, buf.get());
      if (error)
      {
        DEBUG_PRINTLN("Erreur de désérialisation du JSON");
      }

      // Extraire les valeurs
      ID_ALEXA = doc["wifi_networks"][0]["hostname"].as<String>();

      // Fermer le fichier
      file.close();
    }
    else
    {
      DEBUG_PRINTLN("Le fichier WIFI.json n'existe pas.");
    }


  // ajouter les appareils
    fauxmo.addDevice(ID_ALEXA.c_str());

    Alexa_Setuped = true;

  // fonction de rappel (callback) lorsqu’une commande d’Alexa est reçue
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
      static unsigned long lastCall = 0;
      unsigned long now = millis();

      if (now - lastCall < 2000) return; // ignore si moins de 2 secondes depuis le dernier appel
      lastCall = now;


      DEBUG_PRINTLN("[ALEXA] Appareil #" + String(device_id) + " (" + String(device_name) + ") état : " + (state ? "ON" : "OFF") + " - valeur : " + String(int(value)));
      if(strcmp(device_name, ID_ALEXA.c_str()) == 0) { // Si le nom de l'appareil correspond à celui défini
        if(param.EtatLumiere != state) {
          SwitchRelais();
        }
      } 
    });
}


void sendtoAlexa() {
  // Notifier Alexa de l'état actuel
    fauxmo.setState("0",ID_ALEXA.c_str(), param.EtatLumiere);
}