#include <Arduino.h>
#include <mbedtls/md.h>
#include <esp_system.h>

// Génère une clé pseudo-aléatoire basée sur l'ID unique de l'ESP32
String getDeviceKey() {
    uint64_t chipid = ESP.getEfuseMac();
    String key = String((uint32_t)(chipid >> 32), HEX) + String((uint32_t)chipid, HEX);
    return key;
}

// Chiffrement XOR amélioré avec clé dérivée
String encryptPassword(const String& password) {
    if (password.length() == 0) return password;
    
    String deviceKey = getDeviceKey();
    String encrypted = "";
    
    // Utilise une clé dérivée plus complexe
    for (int i = 0; i < password.length(); i++) {
        uint8_t keyByte = deviceKey[i % deviceKey.length()] ^ (i + 1) ^ 0xAB;
        uint8_t encryptedByte = password[i] ^ keyByte;
        encrypted += char(encryptedByte);
    }
    
    // Encodage Base64 simple mais fonctionnel
    String base64 = "";
    const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    for (int i = 0; i < encrypted.length(); i += 3) {
        uint32_t octet_a = i < encrypted.length() ? (unsigned char)encrypted[i] : 0;
        uint32_t octet_b = i + 1 < encrypted.length() ? (unsigned char)encrypted[i + 1] : 0;
        uint32_t octet_c = i + 2 < encrypted.length() ? (unsigned char)encrypted[i + 2] : 0;
        
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
        
        base64 += chars[(triple >> 3 * 6) & 0x3F];
        base64 += chars[(triple >> 2 * 6) & 0x3F];
        base64 += i + 1 < encrypted.length() ? chars[(triple >> 1 * 6) & 0x3F] : '=';
        base64 += i + 2 < encrypted.length() ? chars[(triple >> 0 * 6) & 0x3F] : '=';
    }
    
    return base64;
}

String decryptPassword(const String& encryptedPassword) {
    if (encryptedPassword.length() == 0) return encryptedPassword;
    
    // Décodage Base64
    String decoded = "";
    String cleanB64 = encryptedPassword;
    
    // Supprimer les padding '='
    while (cleanB64.endsWith("=")) {
        cleanB64 = cleanB64.substring(0, cleanB64.length() - 1);
    }
    
    for (int i = 0; i < cleanB64.length(); i += 4) {
        uint32_t sextet_a = cleanB64[i] >= 'A' && cleanB64[i] <= 'Z' ? cleanB64[i] - 'A' :
                           cleanB64[i] >= 'a' && cleanB64[i] <= 'z' ? cleanB64[i] - 'a' + 26 :
                           cleanB64[i] >= '0' && cleanB64[i] <= '9' ? cleanB64[i] - '0' + 52 :
                           cleanB64[i] == '+' ? 62 : 63;
        
        uint32_t sextet_b = i + 1 < cleanB64.length() ? 
                           (cleanB64[i + 1] >= 'A' && cleanB64[i + 1] <= 'Z' ? cleanB64[i + 1] - 'A' :
                            cleanB64[i + 1] >= 'a' && cleanB64[i + 1] <= 'z' ? cleanB64[i + 1] - 'a' + 26 :
                            cleanB64[i + 1] >= '0' && cleanB64[i + 1] <= '9' ? cleanB64[i + 1] - '0' + 52 :
                            cleanB64[i + 1] == '+' ? 62 : 63) : 0;
        
        uint32_t sextet_c = i + 2 < cleanB64.length() ?
                           (cleanB64[i + 2] >= 'A' && cleanB64[i + 2] <= 'Z' ? cleanB64[i + 2] - 'A' :
                            cleanB64[i + 2] >= 'a' && cleanB64[i + 2] <= 'z' ? cleanB64[i + 2] - 'a' + 26 :
                            cleanB64[i + 2] >= '0' && cleanB64[i + 2] <= '9' ? cleanB64[i + 2] - '0' + 52 :
                            cleanB64[i + 2] == '+' ? 62 : 63) : 0;
        
        uint32_t sextet_d = i + 3 < cleanB64.length() ?
                           (cleanB64[i + 3] >= 'A' && cleanB64[i + 3] <= 'Z' ? cleanB64[i + 3] - 'A' :
                            cleanB64[i + 3] >= 'a' && cleanB64[i + 3] <= 'z' ? cleanB64[i + 3] - 'a' + 26 :
                            cleanB64[i + 3] >= '0' && cleanB64[i + 3] <= '9' ? cleanB64[i + 3] - '0' + 52 :
                            cleanB64[i + 3] == '+' ? 62 : 63) : 0;
        
        uint32_t triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);
        
        decoded += char((triple >> 2 * 8) & 0xFF);
        if (i + 2 < cleanB64.length()) decoded += char((triple >> 1 * 8) & 0xFF);
        if (i + 3 < cleanB64.length()) decoded += char((triple >> 0 * 8) & 0xFF);
    }
    
    // Déchiffrement XOR
    String deviceKey = getDeviceKey();
    String decrypted = "";
    
    for (int i = 0; i < decoded.length(); i++) {
        uint8_t keyByte = deviceKey[i % deviceKey.length()] ^ (i + 1) ^ 0xAB;
        uint8_t decryptedByte = decoded[i] ^ keyByte;
        decrypted += char(decryptedByte);
    }
    
    return decrypted;
}
  