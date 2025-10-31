#include <Arduino.h>

// Définition des broches et configurations
    #define BOUTON_UP 6
    #define BOUTON_DOWN 7
    #define BOUTON_TACTILE 0

    #define CAPTEUR_LUMIERE 3

    #define RELAIS1 4
    #define RELAIS2 5
    

void SetupInOut() {
    pinMode(BOUTON_UP, INPUT);
    pinMode(BOUTON_DOWN, INPUT);
    pinMode(BOUTON_TACTILE, INPUT);

    pinMode(RELAIS1, OUTPUT);
    pinMode(RELAIS2, OUTPUT);
    digitalWrite(RELAIS1, LOW); // Relais inactif au démarrage
    param.relais1 = false;
    if(param.TWO_RELAIS_Connected) {
        digitalWrite(RELAIS2, HIGH); // Relais actif au démarrage
        param.relais2 = true;
    }
}

void gestionBoutons() {
    static bool lastStateUp = LOW;
    static bool lastStateDown = LOW;
    static bool lastStateTactile = LOW;

    static unsigned long pressTimeUp = 0;
    static unsigned long pressTimeDown = 0;
    static unsigned long pressTimeTactile = 0;

    const unsigned long LONG_PRESS = 2000; // 2 seconde

    // BOUTON UP
        if (param.BOUTON_UP_Connected && !param.BoutonTactile) {
            bool currentStateUp = digitalRead(BOUTON_UP);
            // début appui
            if (lastStateUp == LOW && currentStateUp == HIGH) {
                pressTimeUp = millis();
            }
            // appui long détecté dès que dépassé
            if (currentStateUp == HIGH && (millis() - pressTimeUp >= LONG_PRESS)) {
                ESP.restart();
            }
            // appui court (au relâchement)
            if (lastStateUp == HIGH && currentStateUp == LOW) {
                unsigned long duration = millis() - pressTimeUp;
                if (duration < LONG_PRESS) {
                    blink.start(hexToNeopixelbus("#ffffff"), 3, 20);
                    SwitchRelais();
                }
            }
            lastStateUp = currentStateUp;
        }

    // BOUTON DOWN
        if (param.BOUTON_DOWN_Connected && !param.BoutonTactile) {
            bool currentStateDown = digitalRead(BOUTON_DOWN);
            if (lastStateDown == LOW && currentStateDown == HIGH) {
                pressTimeDown = millis();
            }
            if (currentStateDown == HIGH && (millis() - pressTimeDown >= LONG_PRESS)) {
                ESP.restart();
            }
            if (lastStateDown == HIGH && currentStateDown == LOW) {
                unsigned long duration = millis() - pressTimeDown;
                if (duration < LONG_PRESS) {
                    blink.start(hexToNeopixelbus("#ffffff"), 3, 20);
                    SwitchRelais();
                }
            }
            lastStateDown = currentStateDown;
        }


    // BOUTON TACTILE
        if (param.BOUTON_TACTILE_Connected && param.BoutonTactile) {
            bool currentStateTactile = digitalRead(BOUTON_TACTILE);
            // Détection de l’appui
            if (lastStateTactile == LOW && currentStateTactile == HIGH) {
                pressTimeTactile = millis();
            }
            // Appui long → redémarrage dès dépassement du temps
            if (currentStateTactile == HIGH && (millis() - pressTimeTactile >= LONG_PRESS)) {
                ESP.restart();
            }
            // Relâchement → si appui court, action
            if (lastStateTactile == HIGH && currentStateTactile == LOW) {
                unsigned long duration = millis() - pressTimeTactile;
                if (duration < LONG_PRESS) {
                    blink.start(hexToNeopixelbus("#ffffff"), 3, 20);
                    SwitchRelais();
                }
            }
            lastStateTactile = currentStateTactile;
        }
}

void lectureCapteurLumiere() {
    static unsigned long lastTime = 0;
    static bool old_light_etat = false;
    static bool firstRun = true;

    static int readings[5] = {0};
    static int index = 0;

    unsigned long now = millis();
    if (now - lastTime >= 50) { // lecture toutes les 50 ms
        readings[index] = analogRead(CAPTEUR_LUMIERE);
        index = (index + 1) % 5;
        lastTime = now;
    }

    // calcul de la moyenne
    long sum = 0;
    for (int i = 0; i < 5; i++) sum += readings[i];
    int moyenne = sum / 5;

    static unsigned long lastUpdate = 0;
    if (now - lastUpdate >= 500) { // mise à jour toutes les 0.5s
        param.EtatLumiere = (moyenne >= 10); // seuil ajusté selon tes mesures

        if (old_light_etat != param.EtatLumiere || firstRun) {
            firstRun = false;
            modifJson("bool", "EtatLumiere", param.EtatLumiere ? "true" : "false", "/parametres.json");
            old_light_etat = param.EtatLumiere;
            sendtoAlexa();

            DEBUG_PRINT("Etat actuel de la lumière : ");
            DEBUG_PRINT(String(moyenne));
            DEBUG_PRINT(" (");
            DEBUG_PRINT(param.EtatLumiere ? "ON" : "OFF");
            DEBUG_PRINTLN(")");
        }
        lastUpdate = now;
    }
}



void SwitchRelais() {
    if(param.TWO_RELAIS_Connected) { // Mode avec deux relais
        if((param.relais1 && param.relais2) || (!param.relais1 && !param.relais2)) { // Les deux sont ON ou OFF
            SwitchRelais2();
        } else {    // Un seul est ON
            SwitchRelais1();
            SwitchRelais2();
        }
    } else { // Mode avec un seul relais
        SwitchRelais1();
    }
}
void SwitchRelais1() {
    if(param.relais1) {
        param.relais1 = false;
        digitalWrite(RELAIS1, LOW);
    } else {
        param.relais1 = true;
        digitalWrite(RELAIS1, HIGH);
    }
    // modifJson("bool", "relais1", param.relais1 ? "true" : "false", "/parametres.json");
}
void SwitchRelais2() {
    if(param.relais2) {
        param.relais2 = false;
        digitalWrite(RELAIS2, LOW);
    } else {
        param.relais2 = true;
        digitalWrite(RELAIS2, HIGH);
    }
    // modifJson("bool", "relais2", param.relais2 ? "true" : "false", "/parametres.json");
}


