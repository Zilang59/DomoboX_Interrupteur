#include <Arduino.h>
#include <NeoPixelBus.h>

#define ledrgbH true

#define PIN_LEDS 19
#define NUM_LEDS 1

NeoPixelBus<NeoRgbwFeature, Neo800KbpsMethod> strip(NUM_LEDS, PIN_LEDS); // RGBW avec NeoPixelBus

int brightness = 128;
int brightness_min = 10;
int brightness_max = 255;
int fadeAmount = 5;
unsigned long previousMillis = 0;
int base_interval = 10; // Intervalle de base pour la respiration
int base_brightness = 128;

void LED_setup() {
    strip.Begin();
    strip.Show(); // Éteint les LEDs au démarrage
}

void FadeSwitch(bool state, int startLED, int endLED, int steps) {
    RgbwColor startColor = state ? RgbwColor(0, 0, 0, 0) : RgbwColor(255, 255, 255, 255);
    RgbwColor endColor = state ? RgbwColor(255, 255, 255, 255) : RgbwColor(0, 0, 0, 0);

    int delayTime = 5;  // Temps entre chaque étape

    // Assurer que startLED <= endLED
    if (startLED > endLED) {
        int temp = startLED;
        startLED = endLED;
        endLED = temp;
    }

    // Boucle de transition
    for (int step = 0; step <= steps; step++) {
        float progress = (float)step / steps;
        RgbwColor currentColor = RgbwColor::LinearBlend(startColor, endColor, progress);

        for (int i = startLED; i <= endLED; i++) {
            strip.SetPixelColor(i, currentColor);
        }
        strip.Show();
        delay(delayTime);
    }
}
void FadeLight(RgbwColor Fromcolor, RgbwColor ToColor, int startLED, int endLED, int steps) {
    int delayTime = 5;  // Temps entre chaque étape

    // Assurer que startLED <= endLED
    if (startLED > endLED) {
        int temp = startLED;
        startLED = endLED;
        endLED = temp;
    }

    // Boucle de transition
    for (int step = 0; step <= steps; step++) {
        float progress = (float)step / steps;
        RgbwColor currentColor = RgbwColor::LinearBlend(Fromcolor, ToColor, progress);

        // Appliquer la couleur calculée à chaque LED
        for (int i = startLED; i <= endLED; i++) {
            strip.SetPixelColor(i, currentColor);
        }
        strip.Show();
        delay(delayTime);
    }
}
void LED_clignotement(RgbwColor color, int boucle, int rapidite, int startled, int stopled) {
    // Vérifier si startled est plus grand que stopled, sinon inverser
    if (startled > stopled) {
        int temp = startled;
        startled = stopled;
        stopled = temp;
    }

    // Clignotement des LEDs dans la plage spécifiée
    for (int i = 0; i < boucle; i++) {
        for (int led = startled; led <= stopled; led++) {
            strip.SetPixelColor(led, color); // Allume la LED avec la couleur
        }
        strip.Show();
        delay(rapidite);

        for (int led = startled; led <= stopled; led++) {
            strip.SetPixelColor(led, RgbwColor(0, 0, 0, 0)); // Éteint la LED
        }
        strip.Show();
        delay(rapidite);
    }
}
void LED_respiration(RgbwColor color, int startled, int stopled) {
    unsigned long currentMillis = millis();
    int dynamicInterval = base_interval + abs((brightness - 128) / 2);

    // Vérifier si startled est plus grand que stopled, sinon inverser
    if (startled > stopled) {
        int temp = startled;
        startled = stopled;
        stopled = temp;
    }

    if (currentMillis - previousMillis >= dynamicInterval) {
        previousMillis = currentMillis;

        brightness += fadeAmount;

        // Assure-toi que brightness reste dans les limites
        if (brightness >= brightness_max) {
            brightness = brightness_max;  // Évite de dépasser
            fadeAmount = -fadeAmount;
        } 
        if (brightness <= brightness_min) {
            brightness = brightness_min;  // Évite un passage en dessous
            fadeAmount = -fadeAmount;
        }

        // Calculer la luminosité ajustée
        float adjustedBrightness = brightness / 255.0;

        // Appliquer la luminosité à la couleur
        RgbwColor adjustedColor = color.Dim(adjustedBrightness);

        // Appliquer l'effet de respiration sur les LEDs de startLED à stopLED
        for (int i = startled; i <= stopled; i++) {
            strip.SetPixelColor(i, adjustedColor);  // Applique la couleur ajustée
        }

        strip.Show();
    }
}

RgbwColor hexToNeopixelbus(String hex) {
    if (hex.charAt(0) == '#') {
        hex.remove(0, 1);
    }

    if (hex.length() != 8) {
        return RgbwColor(0, 0, 0, 0);
    }

    long number = strtoul(hex.c_str(), NULL, 16);

    byte r = (number >> 24) & 0xFF;  // R
    byte g = (number >> 16) & 0xFF;  // G
    byte b = (number >> 8) & 0xFF;   // B
    byte w = number & 0xFF;          // W

    return RgbwColor(g, r, b, w);
}


