#include <arduino.h>
#include <time.h>


void getGeoInfo() {
    if (WiFi.status() != WL_CONNECTED) {
        DEBUG_PRINTLN("WiFi non connecté");
        return;
    }
    
    HTTPClient http;
    http.begin("https://ipwho.is"); // IP publique de l'ESP automatiquement détectée
    http.setTimeout(1000); // 1s max
    int httpCode = http.GET();

    if (httpCode == 200) {
        String payload = http.getString();
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
            bool success = doc["success"];
            if (success) {
                const char* city = doc["city"];
                const char* country = doc["country"];
                float lat = doc["latitude"];
                float lon = doc["longitude"];

                float latRounded = round(lat * 1e6) / 1e6;
                float lonRounded = round(lon * 1e6) / 1e6;
                if(param.latitude != latRounded || param.longitude != lonRounded) {
                    param.latitude = latRounded;
                    param.longitude = lonRounded;
                    modifJson("float", "latitude", String(latRounded, 6), PARAMETRE_FILE);
                    modifJson("float", "longitude", String(lonRounded, 6), PARAMETRE_FILE);
                }
                

                DEBUG_PRINT("Localisation : ");
                DEBUG_PRINT(city);
                DEBUG_PRINT(", ");
                DEBUG_PRINTLN(country);

                DEBUG_PRINT("Coordonnées GPS : ");
                DEBUG_PRINT(String(lat,6));
                DEBUG_PRINT(", ");
                DEBUG_PRINTLN(String(lon,6));

                // ---- Mise à jour du RTC ----
                const char* currentTime = doc["timezone"]["current_time"]; // ex: "2025-10-20T20:43:25+02:00"
                if (currentTime) {
                    DEBUG_PRINT("Heure reçue : ");
                    DEBUG_PRINTLN(currentTime);

                    int year, month, day, hour, minute, second;
                    if (sscanf(currentTime, "%4d-%2d-%2dT%2d:%2d:%2d", &year, &month, &day, &hour, &minute, &second) == 6) {
                        struct tm timeinfo;
                        timeinfo.tm_year = year - 1900;
                        timeinfo.tm_mon  = month - 1;
                        timeinfo.tm_mday = day;
                        timeinfo.tm_hour = hour;
                        timeinfo.tm_min  = minute;
                        timeinfo.tm_sec  = second;
                        timeinfo.tm_isdst = 0;

                        time_t t = mktime(&timeinfo);
                        struct timeval now = { .tv_sec = t };
                        settimeofday(&now, nullptr);

                        DEBUG_PRINTLN("RTC mis à jour !");
                    } else {
                        DEBUG_PRINTLN("Erreur parse current_time");
                    }
                } else {
                    DEBUG_PRINTLN("Pas d'heure trouvée dans timezone.current_time");
                }
            } else {
                DEBUG_PRINTLN("Erreur API : localisation impossible");
            }
        } else {
            DEBUG_PRINTLN("Erreur parsing JSON");
        }
    } else {
        DEBUG_PRINT("Erreur HTTP : ");
        DEBUG_PRINTLN(httpCode);
    }

    http.end();
}
void SunriseSunset() {
    if (WiFi.status() != WL_CONNECTED) {
        DEBUG_PRINTLN("WiFi non connecté");
        return;
    }

    String url = "https://api.sunrisesunset.io/json?lat=" + String(param.latitude, 6) + "&lng=" + String(param.longitude, 6);
    // DEBUG_PRINT("Requête : ");
    // DEBUG_PRINTLN(url);

    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == 200) {
        String payload = http.getString();
        DynamicJsonDocument doc(2048);
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
            String sunrise = doc["results"]["sunrise"] | "";
            String sunset  = doc["results"]["sunset"]  | "";

            // Conversion 12h -> 24h
            sunrise = convertTo24H(sunrise);
            sunset  = convertTo24H(sunset);

            // DEBUG_PRINT("Lever du soleil : ");
            // DEBUG_PRINTLN(sunrise);
            // DEBUG_PRINT("Coucher du soleil : ");
            // DEBUG_PRINTLN(sunset);

            param.Sunrise = sunrise;
            param.Sunset  = sunset;

            modifJson("String", "Sunrise", sunrise, PARAMETRE_FILE);
            modifJson("String", "Sunset", sunset, PARAMETRE_FILE);
        } else {
            DEBUG_PRINTLN("Erreur JSON");
        }
    } else {
        DEBUG_PRINT("Erreur HTTP : ");
        DEBUG_PRINTLN(httpCode);
    }

    http.end();
}

void updateAPIDatas() {
    // Si déjà mis à jour aujourd'hui, on ne refait rien
    if (APIUpdated && !APIForceUpdated) return;

    // Récupère l'heure actuelle depuis le RTC
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        DEBUG_PRINTLN("RTC non initialisé, mise à zéro par défaut...");
    
        // Date/heure arbitraire (ex: 1er janvier 2025 00:00:00)
        struct tm defaultTime;
        defaultTime.tm_year = 2025 - 1900;
        defaultTime.tm_mon  = 0;
        defaultTime.tm_mday = 1;
        defaultTime.tm_hour = 0;
        defaultTime.tm_min  = 0;
        defaultTime.tm_sec  = 0;
        defaultTime.tm_isdst = 0;

        time_t t = mktime(&defaultTime);
        struct timeval now = { .tv_sec = t };
        settimeofday(&now, nullptr);
    }

    // Formate l'heure sous forme HH:MM
    char currentClock[6];
    snprintf(currentClock, sizeof(currentClock), "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);

    // Vérifie si c’est l’heure prévue (par ex. "04:00")
    if (String(currentClock) == APIUpdateClock || APIForceUpdated) {
        DEBUG_PRINTLN("Mise à jour quotidienne des API en cours...");

        getGeoInfo();      // 1️⃣ Récupère localisation + met à jour RTC
        SunriseSunset();   // 2️⃣ Récupère lever/coucher du soleil

        APIUpdated = true; // évite les répétitions jusqu’au lendemain
        APIForceUpdated = false;
        DEBUG_PRINTLN("Mise à jour quotidienne terminée !");
    }
}
void resetAPIDailyFlag() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return;

    char currentClock[6];
    snprintf(currentClock, sizeof(currentClock), "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);

    // Dès qu'on sort de 04:00, on réinitialise le flag
    if (String(currentClock) != APIUpdateClock && APIUpdated) {
        APIUpdated = false;
        DEBUG_PRINTLN("Flag APIUpdated réinitialisé pour la prochaine mise à jour.");
    }
}


String convertTo24H(String time12h) {
    time12h.trim();
    bool isPM = time12h.endsWith("PM");
    bool isAM = time12h.endsWith("AM");

    time12h.replace("AM", "");
    time12h.replace("PM", "");
    time12h.trim();

    int sep = time12h.indexOf(':');
    int hour = time12h.substring(0, sep).toInt();
    int minute = time12h.substring(sep + 1, sep + 3).toInt();

    if (isPM && hour < 12) hour += 12;
    if (isAM && hour == 12) hour = 0;

    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", hour, minute);
    return String(buffer);
}


