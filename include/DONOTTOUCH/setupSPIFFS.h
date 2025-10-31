#include <SPIFFS.h>

#define setupSpiffsH true

#define WIFI_FILE "/wifi.json"
#define PARAMETRE_FILE "/parametres.json"

void setupSPIFFS() {
  DEBUG_PRINT("SPIFFS.");
  while (!SPIFFS.begin(true)) {
    DEBUG_PRINT(".");
    delay(1000);
  }
  DEBUG_PRINTLN(".Ok");
}
void setupSPIFFS_wififile() {
  const char* fileName = WIFI_FILE;

  // Vérifier si le fichier existe
  if (SPIFFS.exists(fileName)) {
    DEBUG_PRINTLN("wifi.json       : existe déjà");
    return;
  }

  // Créer un document JSON avec un réseau par défaut
  StaticJsonDocument<1024> doc;
  JsonArray wifiNetworks = doc.createNestedArray("wifi_networks");



  JsonObject defaultNetwork = wifiNetworks.createNestedObject();
  defaultNetwork["ssid"] = "0";
  defaultNetwork["mdp"] = "0";
  defaultNetwork["hostname"] = "0";
  defaultNetwork["IP_local"] = "0";
  defaultNetwork["MAC_ADRESS"] = "0";
  defaultNetwork["DNS"] = "0";
  defaultNetwork["mDNS"] = "0";
  defaultNetwork["port"] = "80";

  // Ouvrir le fichier pour l'écriture
  File file = SPIFFS.open(fileName, "w");
  if (!file) {
    DEBUG_PRINTLN("wifi.json       : Erreur d'ouverture du fichier pour écriture");
    return;
  }

  // Sérialiser le document JSON dans le fichier
  if (serializeJson(doc, file) == 0) {
    DEBUG_PRINTLN("wifi.json       : Erreur de sérialisation du JSON dans le fichier");
  } else {
    DEBUG_PRINTLN("wifi.json       : Créé avec succès");
  }

  // Fermer le fichier
  file.close();
}
void setupSPIFFS_parametres() {
    StaticJsonDocument<512> doc;
    bool modified = false;

    // Charger le fichier s'il existe
    if (SPIFFS.exists(PARAMETRE_FILE)) {
        File file = SPIFFS.open(PARAMETRE_FILE, "r");
        if (!file) {
            DEBUG_PRINTLN("parametres.json : Erreur d'ouverture du fichier pour lecture");
            return;
        }

        DeserializationError error = deserializeJson(doc, file);
        file.close();

        if (error) {
            DEBUG_PRINTLN("parametres.json : Erreur de désérialisation du JSON");
            return;
        }

        // Charger les valeurs existantes
        DEBUG_PRINTLN("parametres.json : Lecture du fichier qui existe déjà");
        param.fromJson(doc);
    } else {
        DEBUG_PRINTLN("parametres.json : Le fichier n'existe pas, il sera créé.");
        modified = true;
    }

    // Vérifier et ajouter les nouvelles variables si elles manquent
    #define X(type, name, defaultValue) \
        if (!doc.containsKey(#name)) { doc[#name] = param.name; modified = true; }
    PARAM_LIST
    #undef X

    // Sauvegarder uniquement si des modifications ont été faites
    if (modified) {
        File file = SPIFFS.open(PARAMETRE_FILE, "w");
        if (!file) {
            DEBUG_PRINTLN("parametres.json : Erreur d'ouverture du fichier pour écriture");
            return;
        }
        serializeJson(doc, file);
        file.close();
        DEBUG_PRINTLN("parametres.json : Fichier mis à jour avec les nouvelles variables.");
    }
}

void deleteFile(const char* fileName) {
  if (SPIFFS.begin(true)) { // Monte le système de fichiers, formatte s'il le faut
    if (SPIFFS.exists(fileName)) { // Vérifie si le fichier existe
      if (SPIFFS.remove(fileName)) {
        DEBUG_PRINTLN("Fichier supprimé avec succès !");
      } else {
        DEBUG_PRINTLN("Échec de la suppression du fichier.");
      }
    } else {
      DEBUG_PRINTLN("Fichier introuvable.");
    }
  } else {
    DEBUG_PRINTLN("Erreur : Impossible de monter le système de fichiers.");
  }
}


void modifJson(String type, String key, String newValue, String fileName) {
  // Charger le fichier JSON
  File file = SPIFFS.open(fileName, "r+");
  if (!file) {
      DEBUG_PRINTLN("Erreur d'ouverture du fichier JSON");
      return;
  }

  // Charger et analyser le contenu du fichier JSON
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
      DEBUG_PRINTLN("Erreur de lecture du fichier JSON");
      file.close();
      return;
  }

  // Fermer le fichier après lecture
  file.close();

  // Modifier la valeur en fonction du type
  if (type == "String") {
      doc[key].set(newValue);
  }
  else if (type == "int" || type == "uint8_t" || type == "uint16_t" || type == "uint32_t") {
      // On convertit en int ou en uint (selon le type)
      unsigned long intValue = newValue.toInt();  // Convertir en entier
      if (type == "uint8_t") {
          doc[key].set((uint8_t)intValue);  // On cast en uint8_t
      } else if (type == "uint16_t") {
          doc[key].set((uint16_t)intValue);  // On cast en uint16_t
      } else if (type == "uint32_t") {
          doc[key].set((uint32_t)intValue);  // On cast en uint32_t
      } else {
          doc[key].set(intValue);  // Pour les types int classiques
      }
  }
  else if (type == "bool") {
      doc[key].set(newValue == "true");
  }
  else if (type == "float") {
      doc[key].set(newValue.toFloat());
  }
  else {
      DEBUG_PRINTLN("Type inconnu");
      return;
  }

  // Sauvegarder les modifications dans le fichier JSON
  file = SPIFFS.open(fileName, "w");
  if (!file) {
      DEBUG_PRINTLN("Erreur d'ouverture du fichier pour écriture");
      return;
  }
  serializeJson(doc, file);
  file.close();
  DEBUG_PRINTLN("Fichier JSON mis à jour : " + String(key) + " = " + String(newValue));
}

String ReadParametreFile() {
  File file = SPIFFS.open(PARAMETRE_FILE, "r");
  if (!file) return "{}";  // si le fichier n’existe pas

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();
  if (error) return "{}";

  String jsonResponse;
  serializeJson(doc, jsonResponse);
  return jsonResponse;
}
