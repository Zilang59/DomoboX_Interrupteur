#include <Arduino.h>
#include <NeoPixelBus.h>

#define ledrgbH true

#define PIN_LEDS 1
#define NUM_LEDS 1

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(NUM_LEDS, PIN_LEDS); // RGB avec NeoPixelBus

RgbColor currentColor;

int brightness = 128;
int brightness_min = 10;
int brightness_max = 255;
int fadeAmount = 5;
unsigned long previousMillis = 0;
int base_interval = 10; // Intervalle de base pour la respiration
int base_brightness = 128;

RgbColor hexToNeopixelbus(String hex) {
    if (hex.charAt(0) == '#') {
        hex.remove(0, 1);
    }

    if (hex.length() == 8) {
        hex = hex.substring(0, 6);  // Ignore la composante W
    }

    if (hex.length() != 6) {
        return RgbColor(0, 0, 0);
    }

    long number = strtoul(hex.c_str(), NULL, 16);

    byte r = (number >> 16) & 0xFF;
    byte g = (number >> 8) & 0xFF;
    byte b = number & 0xFF;

    return RgbColor(r, g, b);
}
void LED_setup() {
    strip.Begin();
    strip.Show();

    // Allumer chaque LED en rouge par exemple
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.SetPixelColor(i, hexToNeopixelbus("#0000FF"));
    }

    strip.Show(); // Met à jour les LEDs
}

void DefineLed(RgbColor color, int startLED, int endLED) {
    for (int i = startLED; i <= endLED; i++) { strip.SetPixelColor(i, color); }
    strip.Show();
    DEBUG_PRINTLN("LED Color defined to #" + String(color.R, HEX) + String(color.G, HEX) + String(color.B, HEX));
}
void FadeLight(RgbColor FromColor, RgbColor ToColor, int startLED, int endLED, int steps) {
    int delayTime = 5;

    if (startLED > endLED) {
        int temp = startLED;
        startLED = endLED;
        endLED = temp;
    }

    for (int step = 0; step <= steps; step++) {
        float progress = (float)step / steps;
        RgbColor currentColor = RgbColor::LinearBlend(FromColor, ToColor, progress);

        for (int i = startLED; i <= endLED; i++) {
            strip.SetPixelColor(i, currentColor);
        }
        strip.Show();
        delay(delayTime);
    }
}
void LED_clignotement(RgbColor color, int boucle, int rapidite, int startled, int stopled) {
    if (startled > stopled) {
        int temp = startled;
        startled = stopled;
        stopled = temp;
    }

    for (int i = 0; i < boucle; i++) {
        for (int led = startled; led <= stopled; led++) {
            strip.SetPixelColor(led, color);
        }
        strip.Show();
        delay(rapidite);

        for (int led = startled; led <= stopled; led++) {
            strip.SetPixelColor(led, RgbColor(0, 0, 0));
        }
        strip.Show();
        delay(rapidite);
    }
}

struct BreathingEffect {
    int ledIndex = 0;
    RgbColor fromColor = RgbColor(0,0,0);
    RgbColor toColor = RgbColor(0,0,100);
    int steps = 100;
    int currentStep = 0;
    bool forward = true;
    unsigned long lastUpdate = 0;
    int interval = 20;  // vitesse
    bool enabled = true;

    void update() {
        if (!enabled) return;

        unsigned long now = millis();
        if (now - lastUpdate < interval) return;
        lastUpdate = now;

        float progress = (float)currentStep / steps;
        RgbColor color = RgbColor::LinearBlend(fromColor, toColor, progress);
        strip.SetPixelColor(ledIndex, color);
        strip.Show();

        if (forward) currentStep++;
        else currentStep--;

        if (currentStep >= steps) { currentStep = steps; forward = false; }
        else if (currentStep <= 0) { currentStep = 0; forward = true; }
    }

    void setColor(RgbColor color) {
        toColor = color;
    }

    void setSpeed(int speed) {
        // Plus speed est grand, plus ça va vite
        interval = max(1, 100 / speed);
    }

    void setEnabled(bool state) {
        enabled = state;
    }
};
BreathingEffect breath;

struct BlinkEffect {
    int ledIndex = 0;
    RgbColor blinkColor = RgbColor(100,0,0);
    int times = 3;       // nombre de flashes
    int interval = 200;  // ms entre étapes
    int count = 0;       // compteur d'étapes
    bool onState = false;
    bool active = false;
    unsigned long lastUpdate = 0;

    RgbColor previousColor = RgbColor(0,0,0);
    bool breathWasActive = false;

    void start(RgbColor color, int flashes, int speed) {
        if (active) return;

        blinkColor = color;
        times = flashes * 2; // chaque flash = ON + OFF
        interval = speed;
        count = 0;
        onState = false;
        active = true;

        previousColor = strip.GetPixelColor(ledIndex);
        breathWasActive = breath.enabled;

        if (breath.enabled) breath.setEnabled(false);
    }

    void update() {
        if (!active) return;

        unsigned long now = millis();
        if (now - lastUpdate < interval) return;
        lastUpdate = now;

        if (onState) {
            strip.SetPixelColor(ledIndex, blinkColor);  // couleur clignotement
        } else {
            strip.SetPixelColor(ledIndex, RgbColor(0,0,0));  // noir
        }
        strip.Show();

        onState = !onState;
        count++;

        if (count >= times * 2) {  // fin de tous les flashes
            active = false;
            strip.SetPixelColor(ledIndex, previousColor);  // restauration
            strip.Show();

            if (breathWasActive) breath.setEnabled(true);
        }
    }
};
BlinkEffect blink;
void blinkBlocking(RgbColor color, int flashes, int speed) {
    RgbColor previousColor = strip.GetPixelColor(0);
    bool breathWasActive = breath.enabled;

    if (breathWasActive) breath.setEnabled(false);

    for (int i = 0; i < flashes; i++) {
        strip.SetPixelColor(0, color);
        strip.Show();
        delay(speed);

        strip.SetPixelColor(0, RgbColor(0, 0, 0));
        strip.Show();
        delay(speed);
    }

    // Restaure la couleur précédente
    strip.SetPixelColor(0, previousColor);
    strip.Show();

    if (breathWasActive) breath.setEnabled(true);
}

struct ProgressiveOnEffect {
    /* Utilisation :
    void setup() {
        chenillard.start(0, 49, RgbColor(100, 0, 0));
        chenillard.setSpeed(30);
    }

    void loop() {
        chenillard.update();
    }
    */
    int startLED;
    int endLED;
    RgbColor color;
    unsigned long lastUpdate = 0;
    int interval = 50;
    int currentLED;

    void start(int start, int end, RgbColor col) {
        startLED = start;
        endLED = end;
        color = col;
        currentLED = startLED;
    }

    void update() {
        unsigned long now = millis();
        if (currentLED > endLED) return;

        if (now - lastUpdate >= interval) {
            lastUpdate = now;
            strip.SetPixelColor(currentLED, color);
            strip.Show();
            currentLED++;
        }
    }

    void setSpeed(int ms) {
        interval = max(1, ms); // en millisecondes
    }
};
ProgressiveOnEffect chenillard;




