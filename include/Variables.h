#include <Arduino.h>

// Inclure le fichier de configuration des tokens (non versionné)
#include "config_tokens.h"

// Définition centralisée des paramètres enregistrées dans le SPIFFS (utiliser dans le script param.nomdelavariable)
    #define PARAM_LIST \
    X(bool, relais1, false) \
    X(bool, relais2, false) \
    X(bool, BoutonTactile, false) \
    X(bool, EtatLumiere, false) \
    X(bool, Admin_site, false) \
    X(bool, BOUTON_UP_Connected, false) \
    X(bool, BOUTON_DOWN_Connected, true) \
    X(bool, BOUTON_TACTILE_Connected, false) \
    X(bool, TWO_RELAIS_Connected, false) \
    X(bool, LD2410S_Connected, false) \
    X(bool, LD2410S_Activate_on, false) \
    X(bool, LD2410S_Activate_off, false) \
    X(bool, LD2410S_Activate_alltime, false) \
    X(uint8_t, LD2410S_sensibilite, 255) \
    X(uint8_t, LD2410S_farthest, 1) \
    X(uint8_t, LD2410S_nearest, 0) \
    X(uint8_t, LD2410S_delaySec, 10) \
    X(uint8_t, LD2410S_freqHz, 40) \
    X(uint8_t, LD2410S_response, 10) \
    X(String, Version, "1.0.0") \
    X(String, CouleurLedOn, "#00FF00") \
    X(String, CouleurLedOff, "#FF0000") \
    X(float, latitude, 48.866667) \
    X(float, longitude, 2.333333) \
    X(String, Sunrise, "08:00") \
    X(String, Sunset, "19:00") \
    X(bool, empty_variable, false)

// Ne pas toucher, sert au variable du PARAM_LIST
    struct Parametres {
        #define X(type, name, defaultValue) type name = defaultValue;
        PARAM_LIST
        #undef X

        void toJson(JsonDocument& doc) {
            #define X(type, name, defaultValue) doc[#name] = name;
            PARAM_LIST
            #undef X
        }

        void fromJson(JsonDocument& doc) {
            #define X(type, name, defaultValue) if (doc.containsKey(#name)) name = doc[#name].as<type>();
            PARAM_LIST
            #undef X
        }
    };
    Parametres param;


    bool refresh_LEDColor = false;
    bool APIUpdated = false, APIForceUpdated = false;
    String APIUpdateClock = "04:00";

    // Token GitHub pour accéder au dépôt privé (configuré dans config_tokens.h)
    String GITHUB_TOKEN = GITHUB_TOKEN_VALUE;


    bool LD2410_Setuped = false;
    bool Alexa_Setuped = false;
    bool OTA_Setuped = false;