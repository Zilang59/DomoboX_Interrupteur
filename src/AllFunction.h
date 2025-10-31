// Fichier généré automatiquement

// Alexa.h
    void SetupAlexa();
    void sendtoAlexa();

// API.h
    String convertTo24H(String time12h);
    void SunriseSunset();
    void getGeoInfo();
    void resetAPIDailyFlag();
    void updateAPIDatas();

// Fonctions.h
    String formatTime(uint16_t totalSeconds);
    void printMemorySummary();

// LD2410.h
    String getLD2410Info();
    void LD2410_SetCustomParams(uint8_t farthest, uint8_t nearest, uint8_t delaySec, uint8_t freqHz, uint8_t response);
    void LireLD2410();
    void Lire_FirmwareVersion();
    void Lire_Parametres();
    void Lire_SerialNumber();
    void SetupLD2410();

// LED_RGB.h
    void DefineLed(RgbColor color, int startLED, int endLED);
    void FadeLight(RgbColor FromColor, RgbColor ToColor, int startLED, int endLED, int steps);
    void LED_clignotement(RgbColor color, int boucle, int rapidite, int startled, int stopled);
    void LED_setup();
    void blinkBlocking(RgbColor color, int flashes, int speed);
    void setColor(RgbColor color);
    void setEnabled(bool state);
    void setSpeed(int ms);
    void setSpeed(int speed);
    void start(RgbColor color, int flashes, int speed);
    void start(int start, int end, RgbColor col);
    void update();

// pinout.h
    void SetupInOut();
    void SwitchRelais();
    void SwitchRelais1();
    void SwitchRelais2();
    void gestionBoutons();
    void lectureCapteurLumiere();

// setupWIFI.h
    String getUniqueESP32ID();
    bool compareVersions(String newVersion, String currentVersion);
    bool connectToWiFi();
    bool loop_Wifi();
    void accueil(WebServer* activeServer);
    void check_firmware_updates(WebServer* activeServer);
    void gestion_wifi(WebServer* activeServer);
    void informations(WebServer* activeServer);
    void ld2410_info(WebServer* activeServer);
    void option(WebServer* clientServer);
    void parameter_info(WebServer* activeServer);
    void parametres(WebServer* clientServer);
    void resetESP();
    void resetPARAMETRE();
    void setupHotspot();
    void startWebServer();
    void update_firmware(WebServer* activeServer);

// Variables.h
    void fromJson(JsonDocument& doc);
    void toJson(JsonDocument& doc);

// DONOTTOUCH/Cryptage.h
    String decryptPassword(const String& encryptedPassword);
    String encryptPassword(const String& password);

// DONOTTOUCH/OTA_Update.h
    void handleOTA();
    void setupOTA();

// DONOTTOUCH/setupSPIFFS.h
    String ReadParametreFile();
    void deleteFile(const char* fileName);
    void modifJson(String type, String key, String newValue, String fileName);
    void setupSPIFFS();
    void setupSPIFFS_parametres();
    void setupSPIFFS_wififile();

