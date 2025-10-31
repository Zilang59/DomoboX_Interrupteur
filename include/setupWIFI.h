#include <arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <Update.h>

#define setupWifiH true

#include "web_content.h" // Generation des variables contenant le site web ce fichier est généré lors du build, pas de panique si il y a une erreur sur cette ligne avant le premier build

const byte DNS_PORT = 53;
#define SERVER_PORT 81 // Besoin de ne pas utiliser le port 80 car conflictuel avec Alexa (FauxMo)
DNSServer dnsServer;
WebServer server(SERVER_PORT);
WebServer hotspot(SERVER_PORT);

// Variable pour le wifi
  bool IsConnectedToWifi = false;
  bool hotspotActive = true;
  unsigned long lastReconnectAttempt = 0;
  const unsigned long reconnectInterval = 30000; // 30s
  unsigned long wifiConnectedTime = 0;  // moment où le WiFi a été connecté

  wifi_power_t wifiPower = WIFI_POWER_19_5dBm;
  wifi_power_t hotspotPower = WIFI_POWER_19_5dBm;
  String macWifi = WiFi.macAddress();
  String ID_MAC = "";
  String hostname_ID = "";


bool connectToWiFi() {
  DEBUG_PRINT("\n--CONNEXION AU WIFI----------------------\n");

  ID_MAC = getUniqueESP32ID();
  hostname_ID = Dispo_basename + "_" + ID_MAC;

  File configFile = SPIFFS.open(WIFI_FILE, "r");
  if (!configFile) {
    DEBUG_PRINTLN("Erreur d'ouverture du fichier de configuration");
    return false;
  }

  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, configFile);
  configFile.close();

  if (error) {
    DEBUG_PRINTLN("Erreur de lecture du fichier JSON");
    return false;
  }

  JsonArray wifiNetworks = doc["wifi_networks"].as<JsonArray>();
  for (JsonObject network : wifiNetworks) {
    String ssid = network["ssid"];
    String password = decryptPassword(network["mdp"]);
    String MAC_rec = network["MAC_ADRESS"];

    if (MAC_rec == "0") {
      // Si pas de MAC enregistrée, on l'initialise avec la MAC actuelle
      network["MAC_ADRESS"] = macWifi;
      File file = SPIFFS.open(WIFI_FILE, "w");
      if (!file) {
        DEBUG_PRINTLN("Erreur d'ouverture du fichier pour écriture");
        return true;
      }
      serializeJson(doc, file);
      file.close();
      DEBUG_PRINTLN("Initialisation de la MAC dans wifi.json");
    }

    if (ssid == "0") {
      DEBUG_PRINT("SSID = 0, aucun réseau configuré.\n");
      return false;
    }


    WiFi.disconnect(true);
    delay(100);
    WiFi.mode(WIFI_AP_STA);
    WiFi.setHostname(hostname_ID.c_str());
    WiFi.begin(ssid.c_str(), password.c_str());

    
    DEBUG_PRINT("Connexion au réseau ");
    DEBUG_PRINT(ssid);
    DEBUG_PRINT("...");

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
      if (millis() - startAttemptTime >= 2000) {
        DEBUG_PRINT("..impossible\n");
        break;
      }
      delay(100);
    }

    if (WiFi.status() == WL_CONNECTED) {
      // Nom du dispositif sur le réseau
      DEBUG_PRINT("..OK\n");
      wifiConnectedTime = millis();

      // Démarrer le serveur HTTP
      startWebServer();

      // Affichage des résultats dans le port série
      DEBUG_PRINT("Connecté à : ");
      DEBUG_PRINTLN(WiFi.SSID());
      DEBUG_PRINT("Adresse IP : ");
      DEBUG_PRINTLN(WiFi.localIP());
      DEBUG_PRINT("Hostname   : ");
      DEBUG_PRINTLN(WiFi.getHostname());

      // Démarrer le serveur mDNS
      if (MDNS.begin(String(Dispo_basename) + "_" + ID_MAC)) {
          DEBUG_PRINT("mDNS       : ");
          DEBUG_PRINTLN(String(Dispo_basename) + "_" + ID_MAC + ".local");
          MDNS.addService("http", "tcp", SERVER_PORT);
      } else {
          DEBUG_PRINTLN("mDNS       : erreur");
      }

      // 🔹 Mise à jour du fichier JSON avec l'adresse IP locale
      for (JsonObject net : wifiNetworks) {
        if (net["ssid"] == ssid) {
          net["IP_local"] = WiFi.localIP().toString(); // Mise à jour IP
          net["port"] = String(SERVER_PORT); // Mise à jour du port
          net["hostname"] = String(WiFi.getHostname()); // Mise à jour du hostname
          net["DNS"] = String(WiFi.getHostname()); // Mise à jour du DNS
          net["mDNS"] = String(WiFi.getHostname()) + ".local"; // Mise à jour du mDNS
          break;
        }
      }

      // 🔹 Écriture du fichier mis à jour
      File file = SPIFFS.open(WIFI_FILE, "w");
      if (!file) {
        DEBUG_PRINTLN("Erreur d'ouverture du fichier pour écriture");
        return true;
      }
      serializeJson(doc, file);
      file.close();
      DEBUG_PRINTLN("Mise à jour de l'IP locale dans wifi.json");

      return true;
    }
  }

  return false;
}
String getUniqueESP32ID() {
    String mac = macWifi;                   // Ex: "E4:65:B8:84:60:C0"
    mac.replace(":", "");                  // Supprime les ":"
    mac.toUpperCase();                     // Optionnel, pour uniformité
    return mac.substring(mac.length() - 6); // Prend les 6 derniers caractères : 8460C0
}


bool loop_Wifi() {
  unsigned long now = millis();

  if (WiFi.status() == WL_CONNECTED) {
    if (!IsConnectedToWifi) {
      IsConnectedToWifi = true;
      wifiConnectedTime = now;  // mémoriser l'heure de connexion
      DEBUG_PRINTLN("Connexion WiFi détectée, désactivation du hotspot dans 2 minutes");
      APIForceUpdated = true;
    }

    // On ne coupe le hotspot que si 2 minutes se sont écoulées
    if (hotspotActive && now - wifiConnectedTime >= 2 * 60 * 1000) {
      WiFi.softAPdisconnect(true);
      hotspot.stop();
      hotspotActive = false;
      DEBUG_PRINTLN("Hotspot désactivé");
    }

    if(!Alexa_Setuped) SetupAlexa();
    if(!OTA_Setuped) setupOTA();

    #ifdef setupOTAH
      handleOTA();
    #endif
    #ifdef AlexaH
      fauxmo.handle();
    #endif
    server.handleClient();
    
    if (hotspotActive) {
        dnsServer.processNextRequest();
        hotspot.handleClient();       // hotspot toujours actif
    }

  } else {
    if (IsConnectedToWifi) {
      IsConnectedToWifi = false;
      DEBUG_PRINTLN("Perte de connexion WiFi !");
    }

    if (now - lastReconnectAttempt >= reconnectInterval) {
      lastReconnectAttempt = now;
      DEBUG_PRINTLN("Tentative de reconnexion au WiFi...");
      connectToWiFi();
    }

    if (!hotspotActive) {
      setupHotspot();
      hotspotActive = true;
      DEBUG_PRINTLN("Hotspot activé (pas de WiFi)");
    }
    
    dnsServer.processNextRequest();
    hotspot.handleClient();
  }

  return IsConnectedToWifi;
}



void startWebServer() {
  WiFi.setTxPower(wifiPower);
  server.on("/", HTTP_GET, [&](){ accueil(&server); });
  server.on("/accueil", HTTP_GET, [&](){ accueil(&server); });
  server.on("/informations", HTTP_GET, [&](){ informations(&server); });
  server.on("/option", HTTP_GET, [&](){ option(&server); });
  server.on("/reset", HTTP_GET, resetESP);
  server.on("/wifi", HTTP_GET, [&](){ gestion_wifi(&server); });
  server.on("/parametres", HTTP_GET, [&](){ parametres(&server); });
  server.on("/ld2410_info", HTTP_GET, [&](){ ld2410_info(&server); });
  server.on("/parameter_info", HTTP_GET, [&](){ parameter_info(&server); });
  server.on("/resetparam", HTTP_GET, resetPARAMETRE);
  server.on("/update_firmware", HTTP_GET, [&](){ update_firmware(&server); });
  server.on("/restart", HTTP_GET, []() {
      server.send(200, "text/html", "<h1>Redémarrage en cours...</h1>");
      DEBUG_PRINTLN("Redémarrage de l'ESP...");
      delay(1000); // Laisser le temps au client de recevoir la réponse
      ESP.restart(); // Redémarrer l'ESP
  });
  server.onNotFound([]() {
      String path = server.uri();          // Récupérer le chemin de l'URL
      File file = SPIFFS.open(path, "r");  // Ouvrir le fichier correspondant
      if (!file) {
          server.send(404, "text/plain", "Fichier non trouvé");
          return;
      }

      // Déterminer le type MIME
      String contentType = "text/plain";  // Valeur par défaut
      if (path.endsWith(".html")) contentType = "text/html";
      else if (path.endsWith(".png")) contentType = "image/png";
      else if (path.endsWith(".jpg") || path.endsWith(".jpeg")) contentType = "image/jpeg";
      else if (path.endsWith(".gif")) contentType = "image/gif";
      else if (path.endsWith(".css")) contentType = "text/css";
      else if (path.endsWith(".js")) contentType = "application/javascript";
      else if (path.endsWith(".svg")) contentType = "image/svg+xml";


      // server.sendHeader("Cache-Control", "public, max-age=31536000");  // Mettre en cache pendant 1 an
      server.streamFile(file, contentType);
      file.close();
  });

  server.begin();
  DEBUG_PRINT("HTTP       : activé\n");
}
void setupHotspot() {
  String hotspotName = Dispo_basename + "_" + ID_MAC;
  WiFi.softAP(hotspotName.c_str(), nullptr, 1, 0, 10);
  WiFi.setTxPower(hotspotPower);

  DEBUG_PRINT("Hotspot    : ");
  DEBUG_PRINTLN(hotspotName);
  DEBUG_PRINT("Adresse IP : ");
  DEBUG_PRINTLN(WiFi.softAPIP().toString());
  
  // 🔹 DNS captive portal
  dnsServer.start(53, "*", WiFi.softAPIP());

  hotspot.on("/", HTTP_GET, [&](){ gestion_wifi(&hotspot); });
  hotspot.on("/wifi", HTTP_GET, [&](){ gestion_wifi(&hotspot); });
  hotspot.on("/accueil", HTTP_GET, [&](){ accueil(&hotspot); });
  hotspot.on("/informations", HTTP_GET, [&](){ informations(&hotspot); });
  hotspot.on("/option", HTTP_GET, [&](){ option(&hotspot); });
  hotspot.on("/parametres", HTTP_GET, [&](){ parametres(&hotspot); });
  hotspot.on("/ld2410_info", HTTP_GET, [&](){ ld2410_info(&hotspot); });
  hotspot.on("/parameter_info", HTTP_GET, [&](){ parameter_info(&hotspot); });
  hotspot.on("/reset", HTTP_GET, resetESP);
  hotspot.on("/resetparam", HTTP_GET, resetPARAMETRE);
  hotspot.on("/restart", HTTP_GET, []() {
          hotspot.send(200, "text/html", "<h1>Redémarrage en cours...</h1>");
          DEBUG_PRINTLN("Redémarrage de l'ESP...");
          delay(1000); // Laisser le temps au client de recevoir la réponse
          ESP.restart(); // Redémarrer l'ESP
  });
  hotspot.onNotFound([]() {
    String path = hotspot.uri();          // Récupérer le chemin de l'URL
    File file = SPIFFS.open(path, "r");  // Ouvrir le fichier correspondant
    if (!file) {
      hotspot.send(404, "text/plain", "Fichier non trouvé");
      return;
    }

    // Déterminer le type MIME
    String contentType = "text/plain";  // Valeur par défaut
    if (path.endsWith(".html"))
      contentType = "text/html";
    else if (path.endsWith(".png"))
      contentType = "image/png";
    else if (path.endsWith(".jpg") || path.endsWith(".jpeg"))
      contentType = "image/jpeg";
    else if (path.endsWith(".gif"))
      contentType = "image/gif";
    else if (path.endsWith(".css"))
      contentType = "text/css";
    else if (path.endsWith(".js"))
      contentType = "application/javascript";

    // hotspot.sendHeader("Cache-Control", "public, max-age=31536000");  // Mettre en cache pendant 1 an
    hotspot.streamFile(file, contentType);
    file.close();
  });
    
  hotspot.begin();
  hotspotActive = true;
}

void accueil(WebServer* activeServer) {
  // Clignotement pour montrer qu'on est bien sur ce dispositif
    blink.start(hexToNeopixelbus("#ffffff"), 5, 20);

  // Mise en mémoire de la bonne page a afficher
  String html = "";
  html = SiteWeb_Accueil;
  
  // Remplacement des marqueurs pour afficher des variables
    String DispoName = Dispo_basename + "_" + ID_MAC;
    html.replace("%DISPO_NAME%", DispoName);
    html.replace("%DISPLAY_MENU_ADMIN%", param.Admin_site ? "display:block;" : "display:none;");
    html.replace("%DISPLAY_MENU_TACTILE%", param.BOUTON_TACTILE_Connected ? "display:block;" : "display:none;");

    html.replace("%COULEUR_ON%", String(param.CouleurLedOn));
    html.replace("%COULEUR_OFF%", String(param.CouleurLedOff));

    html.replace("%MODETACTILE%", String(param.BoutonTactile ? "checked='checked'" : ""));
    html.replace("%ETATLAMPE%", String(param.EtatLumiere ? "icon-on" : "icon-off"));
    
    html.replace("%DISPLAY_LD2410%", param.LD2410S_Connected ? "display:block;" : "display:none;");
    html.replace("%LD2410_ON_DETECT%", param.LD2410S_Activate_on ? "checked='checked'" : "");
    html.replace("%LD2410_OFF_NODETECT%", param.LD2410S_Activate_off ? "checked='checked'" : "");
    html.replace("%LD2410_NIGHT_ONLY%", param.LD2410S_Activate_alltime ? "" : "checked='checked'");
    html.replace("%LD2410_SENSIBILITE%", String(param.LD2410S_sensibilite));

    html.replace("%SUNSET%", param.Sunset);
    html.replace("%SUNRISE%", param.Sunrise);
    html.replace("%LD2410_NIGHT_ONLY_ONOFF%", param.LD2410S_Activate_alltime ? "" : "on");

  // Send le site web
    activeServer->setContentLength(html.length());
    activeServer->send(200, "text/html", html); 
}
void gestion_wifi(WebServer* activeServer) {
  // Clignotement pour montrer qu'on est bien sur ce dispositif
    blink.start(hexToNeopixelbus("#ffffff"), 5, 20);

  String html = "";
  html = SiteWeb_GestionWifi;

  html.replace("%DISPO_NAME%", Dispo_basename);
  html.replace("%DISPLAY_MENU_ADMIN%", param.Admin_site ? "display:block;" : "display:none;");
  
  if(IsConnectedToWifi) {
    html.replace("%SSID_CONNECTED%", WiFi.SSID());
    html.replace("%WIFI_RSSI%", String(WiFi.RSSI()));
  } else {
    html.replace("%SSID_CONNECTED%", "");
    html.replace("%WIFI_RSSI%", "0");
  }

  activeServer->setContentLength(html.length());
  activeServer->send(200, "text/html", html);
}

void option(WebServer* clientServer) {       // Fonction automatique quand l'utilisateur change des options
  if(clientServer->hasArg("parametre")) {
    if(clientServer->arg("parametre") == "1") { // Couleur de la LED
        String colorOn = clientServer->arg("colorOn");
        String colorOff = clientServer->arg("colorOff");

        // Sauvegarder ou utiliser les couleurs selon le besoin
        DEBUG_PRINTLN("Couleur Allumée : " + colorOn);
        DEBUG_PRINTLN("Couleur Eteinte : " + colorOff);

        param.CouleurLedOn = colorOn;
        param.CouleurLedOff = colorOff;
    
        // Réponse JSON pour confirmer la mise à jour
        clientServer->send(200, "application/json", "{\"status\":\"success\"}");

        delay(100);

        modifJson("String", "CouleurLedOn", param.CouleurLedOn, PARAMETRE_FILE);
        modifJson("String", "CouleurLedOff", param.CouleurLedOff, PARAMETRE_FILE);

        refresh_LEDColor = true;
    }
    if(clientServer->arg("parametre") == "2") { // Mode tactile ou physique
      String newmodetactile = clientServer->arg("modetactile");
      if(newmodetactile == "1") param.BoutonTactile = true;
      else param.BoutonTactile = false;

      // Réponse JSON pour confirmer la mise à jour
      clientServer->send(200, "application/json", "{\"status\":\"success\"}");

      delay(100);

      modifJson("bool", "BoutonTactile", param.BoutonTactile ? "true" : "false", PARAMETRE_FILE);
    }
    if(clientServer->arg("parametre") == "3") { // Passage en mode administrateur sur le site
      if(clientServer->hasArg("admin")) {
        if(param.Admin_site) param.Admin_site = false;
        else param.Admin_site = true;

        // Réponse JSON pour confirmer la mise à jour
        clientServer->send(200, "application/json", "{\"status\":\"success\"}");

        delay(100);

        modifJson("bool", "Admin_site", param.Admin_site ? "true" : "false", PARAMETRE_FILE);
      }
      else if(clientServer->hasArg("module_up") && param.Admin_site) {
        if(param.BOUTON_UP_Connected) param.BOUTON_UP_Connected = false;
        else param.BOUTON_UP_Connected = true;

        // Réponse JSON pour confirmer la mise à jour
        clientServer->send(200, "application/json", "{\"status\":\"success\"}");

        delay(100);

        modifJson("bool", "BOUTON_UP_Connected", param.BOUTON_UP_Connected ? "true" : "false", PARAMETRE_FILE);
      }
      else if(clientServer->hasArg("module_down") && param.Admin_site) {
        if(param.BOUTON_DOWN_Connected) param.BOUTON_DOWN_Connected = false;
        else param.BOUTON_DOWN_Connected = true;

        // Réponse JSON pour confirmer la mise à jour
        clientServer->send(200, "application/json", "{\"status\":\"success\"}");

        delay(100);

        modifJson("bool", "BOUTON_DOWN_Connected", param.BOUTON_DOWN_Connected ? "true" : "false", PARAMETRE_FILE);
      }
      else if(clientServer->hasArg("module_tactile") && param.Admin_site) {
        if(param.BOUTON_TACTILE_Connected) param.BOUTON_TACTILE_Connected = false;
        else param.BOUTON_TACTILE_Connected = true;

        // Réponse JSON pour confirmer la mise à jour
        clientServer->send(200, "application/json", "{\"status\":\"success\"}");

        delay(100);

        modifJson("bool", "BOUTON_TACTILE_Connected", param.BOUTON_TACTILE_Connected ? "true" : "false", PARAMETRE_FILE);
      }
      else if(clientServer->hasArg("module_ld2410") && param.Admin_site) {
        if(param.LD2410S_Connected) param.LD2410S_Connected = false;
        else param.LD2410S_Connected = true;

        // Réponse JSON pour confirmer la mise à jour
        clientServer->send(200, "application/json", "{\"status\":\"success\"}");

        delay(100);

        modifJson("bool", "LD2410S_Connected", param.LD2410S_Connected ? "true" : "false", PARAMETRE_FILE);
      }
      else if(clientServer->hasArg("nombre_relais") && param.Admin_site) {
        if(param.TWO_RELAIS_Connected) param.TWO_RELAIS_Connected = false;
        else param.TWO_RELAIS_Connected = true;

        // Réponse JSON pour confirmer la mise à jour
        clientServer->send(200, "application/json", "{\"status\":\"success\"}");

        delay(100);

        modifJson("bool", "TWO_RELAIS_Connected", param.TWO_RELAIS_Connected ? "true" : "false", PARAMETRE_FILE);
      }
      else { clientServer->send(200, "application/json", "{\"status\":\"forbidden\"}"); }
    }
    if(clientServer->arg("parametre") == "4") { // allume ou éteint la lumière
      if(clientServer->hasArg("switchlight")) {
        String switchlight = clientServer->arg("switchlight");
        if(switchlight == "1") {
          SwitchRelais();
        }
        // Réponse JSON pour confirmer la mise à jour
        clientServer->send(200, "application/json", "{\"status\":\"success\",\"EtatLumiere\":\"" + String(param.EtatLumiere ? "1" : "0") + "\"}");
      }
    }
    if(clientServer->arg("parametre") == "5") { // Paramètres du LD2410
      if(clientServer->hasArg("LD2410_on_detect")) {
        String newstate = clientServer->arg("LD2410_on_detect");
        if(newstate == "1") param.LD2410S_Activate_on = true;
        else param.LD2410S_Activate_on = false;
        modifJson("bool", "LD2410S_Activate_on", param.LD2410S_Activate_on ? "true" : "false", PARAMETRE_FILE);
        delay(100);
        // Réponse JSON pour confirmer la mise à jour
        clientServer->send(200, "application/json", "{\"status\":\"success\"}");

        if(param.LD2410S_Activate_on && !LD2410_Setuped) SetupLD2410();
      }
      if(clientServer->hasArg("ld2410_off_nodetect")) {
        String newstate = clientServer->arg("ld2410_off_nodetect");
        if(newstate == "1") param.LD2410S_Activate_off = true;
        else param.LD2410S_Activate_off = false;
        modifJson("bool", "LD2410S_Activate_off", param.LD2410S_Activate_off ? "true" : "false", PARAMETRE_FILE);
        delay(100);
        // Réponse JSON pour confirmer la mise à jour
        clientServer->send(200, "application/json", "{\"status\":\"success\"}");

        if(param.LD2410S_Activate_off && !LD2410_Setuped) SetupLD2410();
      }
      if(clientServer->hasArg("ld2410_night_only")) {
        if(param.LD2410S_Activate_off || param.LD2410S_Activate_on) {
          String newstate = clientServer->arg("ld2410_night_only");
          if(newstate == "1") param.LD2410S_Activate_alltime = false;
          else param.LD2410S_Activate_alltime = true;
          modifJson("bool", "LD2410S_Activate_alltime", param.LD2410S_Activate_alltime ? "true" : "false", PARAMETRE_FILE);
          delay(100);
          // Réponse JSON pour confirmer la mise à jour
          clientServer->send(200, "application/json", "{\"status\":\"success\"}");
        } else {
          clientServer->send(200, "application/json", "{\"status\":\"error\"}");
        }
      }
      if(clientServer->hasArg("ld2410_sensi")) {
        if(param.LD2410S_Activate_on || param.LD2410S_Activate_off) {
          String newstate = clientServer->arg("ld2410_sensi");
          param.LD2410S_sensibilite = newstate.toInt();
          modifJson("uint8_t", "LD2410S_sensibilite", String(param.LD2410S_sensibilite), PARAMETRE_FILE);
          delay(100);
          // Réponse JSON pour confirmer la mise à jour
          clientServer->send(200, "application/json", "{\"status\":\"success\"}");

          if(!LD2410_Setuped) SetupLD2410();

          if(param.LD2410S_sensibilite < 50) { param.LD2410S_farthest = 1; param.LD2410S_nearest = 0; }
          else if(param.LD2410S_sensibilite < 100) { param.LD2410S_farthest = 3; param.LD2410S_nearest = 0; }
          else if(param.LD2410S_sensibilite < 150) { param.LD2410S_farthest = 5; param.LD2410S_nearest = 1; }
          else if(param.LD2410S_sensibilite < 200) { param.LD2410S_farthest = 7; param.LD2410S_nearest = 1; }
          else { param.LD2410S_farthest = 10; param.LD2410S_nearest = 1; }
          LD2410_SetCustomParams(param.LD2410S_farthest, param.LD2410S_nearest, param.LD2410S_delaySec, param.LD2410S_freqHz, param.LD2410S_response);

          modifJson("uint8_t", "LD2410S_farthest", String(param.LD2410S_farthest), PARAMETRE_FILE);
          modifJson("uint8_t", "LD2410S_nearest", String(param.LD2410S_nearest), PARAMETRE_FILE);
        } else {
          clientServer->send(200, "application/json", "{\"status\":\"error\"}");
        }
      }
    }
  }
}
void parametres(WebServer* clientServer) {   // Fonction automatique quand l'utilisateur changes les parametres wifi
  if(clientServer->hasArg("ssid")) {
    String ssid_to_modify = clientServer->arg("ssid");

    if(clientServer->hasArg("delete")) {
      File file = SPIFFS.open(WIFI_FILE, "r");
      if (!file) {
          DEBUG_PRINTLN("Impossible d'ouvrir le fichier JSON.");
          clientServer->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Cannot open file\"}");
          return;
      }

      size_t fileSize = file.size();
      std::unique_ptr<char[]> buffer(new char[fileSize + 1]);
      file.readBytes(buffer.get(), fileSize);
      buffer[fileSize] = '\0';
      file.close();

      DynamicJsonDocument doc(2048);
      DeserializationError error = deserializeJson(doc, buffer.get());
      if (error) {
          DEBUG_PRINTLN("Erreur lors du parsing JSON.");
          clientServer->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON format\"}");
          return;
      }

      JsonArray wifiNetworks = doc["wifi_networks"];
      bool found = false;
      for (size_t i = 0; i < wifiNetworks.size(); i++) {
          if (wifiNetworks[i]["ssid"].as<String>() == ssid_to_modify) {
              if (wifiNetworks.size() == 1) {
                  wifiNetworks[i]["ssid"] = "0";
                  wifiNetworks[i]["mdp"] = "0";
                  wifiNetworks[i]["IP_local"] = "0";
                  wifiNetworks[i]["MAC_ADRESS"] = "0";
                  wifiNetworks[i]["DNS"] = "0";
                  wifiNetworks[i]["mDNS"] = Dispo_basename + ".local";
                  wifiNetworks[i]["port"] = "80";
              } else {
                  wifiNetworks.remove(i);
              }
              found = true;
              break;
          }
      }

      if (!found) {
          DEBUG_PRINTLN("SSID non trouvé.");
          clientServer->send(404, "application/json", "{\"status\":\"error\",\"message\":\"SSID not found\"}");
          return;
      }

      file = SPIFFS.open(WIFI_FILE, "w");
      if (!file) {
          DEBUG_PRINTLN("Impossible d'écrire dans le fichier JSON.");
          clientServer->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Cannot write file\"}");
          return;
      }

      serializeJson(doc, file);
      file.close();

      DEBUG_PRINT("SSID supprimé : ");
      DEBUG_PRINTLN(ssid_to_modify);
      clientServer->send(200, "application/json", "{\"status\":\"success\"}");
    }
    if(clientServer->hasArg("modifymdp")) {
        String mdp_to_modify = encryptPassword(clientServer->arg("modifymdp"));

        File file = SPIFFS.open(WIFI_FILE, "r");
        if (!file) {
          clientServer->send(500, "application/json", "{\"status\":\"error\", \"message\":\"Impossible d'ouvrir le fichier JSON\"}");
            return;
        }

        DynamicJsonDocument doc(4096);
        DeserializationError error = deserializeJson(doc, file);
        file.close();
        if (error) {
          clientServer->send(500, "application/json", "{\"status\":\"error\", \"message\":\"Erreur de parsing JSON\"}");
            return;
        }

        JsonArray wifi_networks = doc["wifi_networks"];
        bool ssid_found = false;
        bool empty_slot_used = false;

        for (JsonObject network : wifi_networks) {
            if (network["ssid"] == ssid_to_modify) {
                network["mdp"] = mdp_to_modify;
                ssid_found = true;
                break;
            }
        }

        if (!ssid_found) {
            for (JsonObject network : wifi_networks) {
                if (network["ssid"] == "0") {
                    network["ssid"] = ssid_to_modify;
                    network["mdp"] = mdp_to_modify;
                    empty_slot_used = true;
                    break;
                }
            }
            if (!empty_slot_used) {
                JsonObject new_network = wifi_networks.createNestedObject();
                new_network["ssid"] = ssid_to_modify;
                new_network["mdp"] = mdp_to_modify;
                new_network["hostname"] = hostname_ID;
                new_network["IP_local"] = "0";
                new_network["MAC_ADRESS"] = "0";
                new_network["DNS"] = "0";
                new_network["mDNS"] = hostname_ID + ".local";
                new_network["port"] = String(SERVER_PORT);
            }
        }

        file = SPIFFS.open(WIFI_FILE, "w");
        if (!file) {
          clientServer->send(500, "application/json", "{\"status\":\"error\", \"message\":\"Impossible de sauvegarder le fichier JSON\"}");
            return;
        }

        if (serializeJson(doc, file) == 0) {
          clientServer->send(500, "application/json", "{\"status\":\"error\", \"message\":\"Erreur d'écriture dans le fichier JSON\"}");
            file.close();
            return;
        }

        file.close();
        DEBUG_PRINT("SSID modifié ou ajouté : ");
        DEBUG_PRINTLN(ssid_to_modify);

        if (connectToWiFi()) { delay(1); }

        clientServer->send(200, "application/json", "{\"status\":\"success\"}");
    }
  }
}

void resetESP() {
  DEBUG_PRINTLN("Suppression de tous les fichiers SPIFFS...");

  // Liste des fichiers à supprimer
  const char* filesToDelete[] = {WIFI_FILE,PARAMETRE_FILE};
  
  // Parcours de la liste des fichiers
  for (int i = 0; i < sizeof(filesToDelete) / sizeof(filesToDelete[0]); i++) {
    const char* fileName = filesToDelete[i];
    
    // Essayer de supprimer chaque fichier
    if (SPIFFS.exists(fileName)) {
      if (SPIFFS.remove(fileName)) {
        DEBUG_PRINT("Fichier supprimé : ");
        DEBUG_PRINTLN(fileName);
      } else {
        DEBUG_PRINT("Erreur de suppression pour le fichier : ");
        DEBUG_PRINTLN(fileName);
      }
    } else {
      DEBUG_PRINT("Fichier non trouvé : ");
      DEBUG_PRINTLN(fileName);
    }
  }

  DEBUG_PRINTLN("Tous les fichiers spécifiés ont été supprimés.");
  ESP.restart(); // Redémarrage de l'ESP après la suppression
}
void resetPARAMETRE() {
  DEBUG_PRINTLN("Suppression des parametres...");

  // Liste des fichiers à supprimer
  const char* filesToDelete[] = {PARAMETRE_FILE};
  
  // Parcours de la liste des fichiers
  for (int i = 0; i < sizeof(filesToDelete) / sizeof(filesToDelete[0]); i++) {
    const char* fileName = filesToDelete[i];
    
    // Essayer de supprimer chaque fichier
    if (SPIFFS.exists(fileName)) {
      if (SPIFFS.remove(fileName)) {
        DEBUG_PRINT("Fichier supprimé : ");
        DEBUG_PRINTLN(fileName);
      } else {
        DEBUG_PRINT("Erreur de suppression pour le fichier : ");
        DEBUG_PRINTLN(fileName);
      }
    } else {
      DEBUG_PRINT("Fichier non trouvé : ");
      DEBUG_PRINTLN(fileName);
    }
  }

  DEBUG_PRINTLN("Tous les fichiers spécifiés ont été supprimés.");
  ESP.restart(); // Redémarrage de l'ESP après la suppression
}


void informations(WebServer* activeServer) {
  // Clignotement pour montrer qu'on est bien sur ce dispositif
    blink.start(hexToNeopixelbus("#ffffff"), 5, 20);

  // Mise en mémoire de la bonne page a afficher
  String html = "";
  html = SiteWeb_Informations;
  
  // Remplacement des marqueurs pour afficher des variables
    String DispoName = Dispo_basename + "_" + ID_MAC;
    html.replace("%DISPO_NAME%", DispoName);
    html.replace("%DISPO_BASENAME%", Dispo_basename);
    html.replace("%DISPLAY_MENU_ADMIN%", param.Admin_site ? "display:block;" : "display:none;");


    
    html.replace("%VERSION_FIRMWARE%", param.Version);
    html.replace("%MODULE_UP%", param.BOUTON_UP_Connected ? "Oui" : "Non");
    html.replace("%MODULE_DOWN%", param.BOUTON_DOWN_Connected ? "Oui" : "Non");
    html.replace("%MODULE_TACTILE%", param.BOUTON_TACTILE_Connected ? "Oui" : "Non");
    html.replace("%MODULE_LD2410%", param.LD2410S_Connected ? "Oui" : "Non");
    html.replace("%MODULE_RELAIS%", param.TWO_RELAIS_Connected ? "2" : "1");

    

  // Send le site web
    activeServer->setContentLength(html.length());
    activeServer->send(200, "text/html", html); 
}

void ld2410_info(WebServer* activeServer) {
  DEBUG_PRINTLN("Demande des informations du LD2410 depuis la page d'accueil du site");
  String data = getLD2410Info(); // Appel de la fonction du LD2410
  activeServer->send(200, "application/json", data);
}
void parameter_info(WebServer* activeServer) {
  DEBUG_PRINTLN("Demande de parametre depuis la page d'accueil du site");
  String data = ReadParametreFile();
  activeServer->send(200, "application/json", data);
}

void update_firmware(WebServer* activeServer) {
    String fileName = activeServer->arg("file");
    if (fileName == "") {
        activeServer->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Aucun fichier fourni\"}");
        return;
    }

    String url = "http://novalys.ovh/UpdateArduino/" + fileName;
    DEBUG_PRINT("Téléchargement depuis : ");
    DEBUG_PRINTLN(url);
    activeServer->send(200, "application/json", "{\"status\":\"success\"}");
    delay(100);

    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == 200) {
        int contentLength = http.getSize();
        WiFiClient* stream = http.getStreamPtr();

        if (Update.begin(contentLength)) {
            size_t written = Update.writeStream(*stream);
            if (written == contentLength) {
                DEBUG_PRINTLN("Mise à jour terminée !");
            } else {
                DEBUG_PRINTLN("Erreur d'écriture du firmware");
            }

            if (Update.end(true)) {
                DEBUG_PRINTLN("Redémarrage...");
                ESP.restart();
            } else {
                DEBUG_PRINTLN("Erreur Update.end()");
            }
        } else {
            DEBUG_PRINTLN("Impossible de commencer la mise à jour");
        }
    } else {
        DEBUG_PRINT("Erreur HTTP : ");
        DEBUG_PRINTLN(httpCode);
    }

    http.end();
}
