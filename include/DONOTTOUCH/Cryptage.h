#include <Arduino.h>
int shift = 5;

String encryptPassword(const String& password) {
    String encrypted = "";
    
    for (int i = 0; i < password.length(); i++) {
        char c = password[i];
        if (isAlpha(c)) {
            // Chiffrement pour les lettres (majuscule ou minuscule)
            char base = isUpperCase(c) ? 'A' : 'a';
            encrypted += char((c - base + shift) % 26 + base);
        } else {
            // Ne change pas les caractères non alphabétiques
            encrypted += c;
        }
    }

    return encrypted;
}

String decryptPassword(const String& encryptedPassword) {
    String decrypted = "";
    int decryptshift = 26 - shift;

    for (int i = 0; i < encryptedPassword.length(); i++) {
        char c = encryptedPassword[i];
        if (isAlpha(c)) {
        // Chiffrement pour les lettres (majuscule ou minuscule)
        char base = isUpperCase(c) ? 'A' : 'a';
        decrypted += char((c - base + decryptshift) % 26 + base);
        } else {
        // Ne change pas les caractères non alphabétiques
        decrypted += c;
        }
    }

    return decrypted;
}
  