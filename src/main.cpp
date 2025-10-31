#include <Arduino.h>
#include <WebServer.h>

// Variables générales
    String Dispo_basename = "Interrupteur"; //Pas de caractere speciaux, espaces...
    String Dispo_version = "1.0.1"; // Version du code
    #define DEBUG  // Mets en commentaire cette ligne pour désactiver les Serial.print()
    // Commande pour éviter de voir des erreurs lié au fichier .h : pio init --ide vscode



// Include Généraux
  #include <DONOTTOUCH/VariablesGlobales.h>
  #include <Variables.h>
  #include <LED_RGB.h>     // Gestion des LEDs choisir celui adapté au led (LED_RGB.h ou LED_RGBW.h) ou commenter pour ne pas utiliser
  #include "AllFunction.h" // Fichier généré automatiquement pour éviter les erreurs de "fonction non déclarées"
  #include "DONOTTOUCH/setupSPIFFS.h"   // Gestion du système de fichier SPIFFS
  #include "DONOTTOUCH/OTA_Update.h"    // Gestion des mises à jour OTA (setupWIFI.H doit etre activé)
  #include "DONOTTOUCH/Cryptage.h"      // Gestion du cryptage des mots de passe
  #include "Fonctions.h"
  #include <pinout.h>
  #include <Alexa.h>                  // Utilisation de Alexa (setupWIFI.H doit etre activé)
  #include <setupWIFI.h>              // Le include doit être placé après Alexa.h et OTA_update.h

// Include diverses
  #include <LD2410.h>                 // Utilisation du détecteur de présence LD2410 (pinout.h doit etre modifié)
  #include <API.h>                    // Permet d'obtenir des informations depuis des API internet (météo, heure, etc...)

void setup() {
  // Initialisation de la communication série
    delay(100);
    #ifdef DEBUG
        Serial.begin(115200);
        delay(50);
    #endif

  // Setup des inputs outputs
    SetupInOut();
    delay(10);

  // Paramètrage de la LED
    #ifdef ledrgbH
      LED_setup();
      
      // Utilisation du Breath dans la suite
        currentColor = strip.GetPixelColor(0);
        breath.setColor(currentColor); // initialisation du breath
        breath.setSpeed(10); // vitesse du breath
      delay(10);
    #endif

  // Paramètrage des fichiers et des paramètres mémoire
    #ifdef setupSpiffsH
      DEBUG_PRINT("\n--MONTAGE SPIFFS-------------------------\n");
      setupSPIFFS();
      setupSPIFFS_wififile();     // Mettre en commentaire si on ne veux pas gérer de réseau wi-fi
      setupSPIFFS_parametres();   // Mettre en commentaire si on ne veux pas de paramètres

      if(param.Version != Dispo_version) {
        param.Version = Dispo_version;
        DEBUG_PRINTLN("Mise à jour de la version : " + Dispo_version);
        modifJson("String", "Version", Dispo_version, PARAMETRE_FILE);
      }
      
      DEBUG_PRINT("-----------------------------------------\n");
      delay(10);
    #endif

  // Essayer de se connecter à un réseau Wi-Fi
    #ifdef setupWifiH
      if (!connectToWiFi()) {
          blinkBlocking(hexToNeopixelbus("#ff0000"), 3, 150);
          setupHotspot(); // On génère la page de gestion des réseaux Wifi
          IsConnectedToWifi = false;
          delay(10);
      } else {
          blinkBlocking(hexToNeopixelbus("#00ff00"), 3, 150);
          IsConnectedToWifi = true;
          setupHotspot(); // On génère quand meme le hotspot au cas ou on aurais perdu l'ip ou des informations, il sera visible 2 minutes puis désactiver dans la fonction "loop_wifi"
          wifiConnectedTime = millis();
          #ifdef AlexaH
            SetupAlexa();
          #endif
          delay(10);
          getGeoInfo();
          SunriseSunset();
          APIUpdated = true;
      }
      DEBUG_PRINT("-----------------------------------------\n");
      delay(10);
    #endif

  // Activation du OTA si il est défini et qu'on utilise le wifi
    #if defined(setupOTAH) && defined(setupWifiH)
      setupOTA();
    #endif

  // Initialisation du détecteur de présence LD2410
    if(param.LD2410S_Connected) {
      SetupLD2410();
      delay(10);
    }

  // Finalisation du Setup
    #ifdef DEBUG
        printMemorySummary();
    #endif
    DEBUG_PRINTLN(" =====> Setup OK");
    ESPStarted = millis();

  delay(50);
}

void loop() {
  // Gestion des connexions
    #if defined(setupWifiH)
      loop_Wifi();  // Inclu Alexa et Mise a jour OTA si défini
      if(param.LD2410S_Connected) { updateAPIDatas(); }
    #endif

  // Gestion des Boutons et de l'état de la lumière
    lectureCapteurLumiere();  // Permet de définir l'état de la lumière via une lecture sur l'entrée annalogique du capteur de lumière
    gestionBoutons();         // Permet de gérer les actions lors d'un appuis sur les boutons

  // Gestion de la LED
    #ifdef ledrgbH
      // Gestion de la couleur (en fonction de l'état de la lampe)
        static bool old_light_etat = false;
        static bool first_run = true;
        if(old_light_etat != param.EtatLumiere || first_run || refresh_LEDColor) { // Définition de la couleur de la LED en fonction de l'état de la lumière
            if(IsConnectedToWifi) {
              if(!hotspotActive) {
                DefineLed(hexToNeopixelbus(param.EtatLumiere ? param.CouleurLedOn : param.CouleurLedOff),0,NUM_LEDS-1);
              } else {
                breath.setColor(hexToNeopixelbus(param.EtatLumiere ? param.CouleurLedOn : param.CouleurLedOff));
              }
            } else {
              breath.setColor(hexToNeopixelbus(param.EtatLumiere ? param.CouleurLedOn : param.CouleurLedOff));
            }
            old_light_etat = param.EtatLumiere;
            first_run = false;
            if(refresh_LEDColor) refresh_LEDColor = false;
        }

      // Gestion de l'effet (en fonction de la connexion WiFi)
        if(IsConnectedToWifi) {
          if(!hotspotActive) {  
            if(breath.enabled && !blink.active) { breath.enabled = false; }
          } else {
            if(!breath.enabled && !blink.active) { breath.enabled = true; }
          }
        } else {
          if(!breath.enabled && !blink.active) { breath.enabled = true; }
        }

        breath.update();  // Update des fonction spéciales pour la LED
        blink.update();   // Update des fonction spéciales pour la LED
    #endif

  // Gestion du détecteur de présence LD2410
    ld2410s.loop();

  // Tâches à exécuter toutes les secondes
    static unsigned long lastCheck_1sec = 0;
    if (millis() - lastCheck_1sec >= 1000) { // Choses qui se font toutes les seconde
      lastCheck_1sec = millis();

      // if(param.LD2410S_Connected) { LireLD2410(); }
    }

    static unsigned long lastCheck_30sec = 0;
    if (millis() - lastCheck_30sec >= 30000) { // Choses qui se font toutes les 30 secondes
      lastCheck_30sec = millis();
      
      // Gestion des APIs
        updateAPIDatas();     // Mise à jour quotidienne des API
        resetAPIDailyFlag();  // Réinitialisation du flag de mise à jour quotidienne pour les APIs
    }

  delay(1);
}