# 🏠 Domobox - Interrupteur Connecté

<div align="center">

![ESP32](https://img.shields.io/badge/ESP32-C3/S3-blue?style=for-the-badge&logo=espressif)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Framework-orange?style=for-the-badge&logo=platformio)
![Arduino](https://img.shields.io/badge/Arduino-Framework-teal?style=for-the-badge&logo=arduino)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

*Un interrupteur intelligent connecté avec interface web, contrôle vocal Alexa et détection de présence*

</div>

## 🌟 Présentation

Ce projet transforme un simple interrupteur en un dispositif intelligent connecté capable de contrôler l'éclairage de manière automatique et intelligente. Basé sur un ESP32, il offre une interface web moderne, une intégration Alexa et de nombreuses fonctionnalités avancées.

## ✨ Fonctionnalités

### 🌐 **Connectivité**
- **WiFi intégré** avec hotspot de configuration automatique
- **Interface web responsive** accessible depuis n'importe quel appareil
- **Intégration Alexa** pour le contrôle vocal
- **Mises à jour OTA** (Over-The-Air)

### 💡 **Contrôle d'éclairage**
- **Boutons physiques** (tactile et mécanique)
- **Contrôle via interface web**
- **Commandes vocales Alexa**
- **LED RGB/RGBW** avec effets visuels (respiration, clignotement)
- **Couleurs personnalisables** pour les états ON/OFF

### 🔍 **Détection intelligente**
- **Capteur de luminosité** pour adaptation automatique
- **Détecteur de présence LD2410** (optionnel)
- **Gestion automatique** basée sur la présence et l'heure

### ⚙️ **Configuration avancée**
- **Paramètres persistants** stockés en SPIFFS
- **Configuration modulaire** via fichiers d'en-tête
- **Mode debug intégré**
- **Gestion des relais multiples**

## 🛠️ Architecture technique

### **Plateforme**
- **Microcontrôleur** : ESP32-C3 ou ESP32-S3
- **Framework** : Arduino avec PlatformIO
- **Stockage** : SPIFFS pour les paramètres et l'interface web

### **Structure du projet**
```
├── src/                    # Code source principal
│   ├── main.cpp           # Point d'entrée
│   ├── AllFunction.h      # Fonctions générées automatiquement
│   └── web_content.h      # Contenu web embarqué
├── include/               # Fichiers d'en-tête
│   ├── Variables.h        # Définition des paramètres
│   ├── pinout.h          # Configuration des broches
│   ├── Alexa.h           # Intégration Alexa
│   ├── LED_RGB.h         # Gestion des LEDs
│   ├── LD2410.h          # Détecteur de présence
│   └── DONOTTOUCH/       # Modules système
├── data/site/            # Interface web
│   ├── Accueil/          # Page principale
│   ├── GestionWifi/      # Configuration WiFi
│   └── Informations/     # Informations système
└── lib/                  # Bibliothèques externes
```

## 🚀 Installation et configuration

### **Prérequis**
- [PlatformIO](https://platformio.org/) installé
- ESP32-C3 ou ESP32-S3
- Composants électroniques (voir schéma de câblage)

### **Étapes d'installation**

1. **Cloner le repository**
   ```bash
   git clone [URL_DU_REPO]
   cd Interrupteur_2025_09_15
   ```

2. **Configurer le matériel**
   - Modifier `include/pinout.h` selon votre câblage
   - Ajuster `include/Variables.h` pour vos besoins

3. **Compiler et téléverser**
   ```bash
   pio run -t upload
   ```

4. **Configuration initiale**
   - Se connecter au hotspot "Interrupteur_XXXXXX"
   - Accéder à l'interface web (192.168.4.1)
   - Configurer le WiFi

## 🔧 Configuration matérielle

### **Brochage par défaut (ESP32-C3)**
```cpp
#define BOUTON_UP 6           // Bouton haut (optionnel)
#define BOUTON_DOWN 7         // Bouton bas
#define BOUTON_TACTILE 0      // Bouton tactile (optionnel)
#define CAPTEUR_LUMIERE 3     // Capteur de luminosité
#define RELAIS1 4             // Relais principal
#define RELAIS2 5             // Relais secondaire (optionnel)
```

### **Composants supportés**
- **LEDs** : NeoPixel/WS2812B (RGB/RGBW)
- **Détecteur** : LD2410 (radar de présence)
- **Relais** : Compatible 3.3V/5V
- **Capteurs** : Photorésistance pour luminosité

## 📱 Interface utilisateur

### **Interface web**
- **Design moderne** avec Material Icons
- **Responsive** (mobile et desktop)
- **Contrôles intuitifs** avec retour visuel
- **Configuration en temps réel**

### **Pages disponibles**
- 🏠 **Accueil** : Contrôle principal et paramètres
- 📶 **WiFi** : Gestion des connexions réseau
- ℹ️ **Informations** : État système et diagnostics

### **Contrôle Alexa**
```
"Alexa, allume la lumière"
"Alexa, éteins la lumière"
```

## ⚡ Fonctionnalités avancées

### **Effets LED intelligents**
- **Respiration** : Effet de pulsation douce
- **Clignotement** : Signalisation d'états
- **Couleurs adaptatives** : Selon l'état et la connexion

### **Gestion d'énergie**
- **Mode veille** optimisé
- **Surveillance mémoire** en temps réel
- **Redémarrage automatique** en cas de problème

### **Sécurité**
- **Cryptage** des mots de passe WiFi
- **Mode admin** protégé
- **Réinitialisation sécurisée**

## 🔄 Mises à jour

Le système supporte les **mises à jour OTA** :
- Via l'interface web
- Téléversement automatique des nouvelles versions
- Sauvegarde des paramètres utilisateur

## 🐛 Débogage

### **Mode debug**
Décommenter dans `main.cpp` :
```cpp
#define DEBUG
```

### **Surveillance système**
- **Mémoire RAM/Flash** en temps réel
- **État des connexions** WiFi/Alexa
- **Logs détaillés** via Serial Monitor

## 📊 Spécifications techniques

| Caractéristique | Valeur |
|------------------|--------|
| **Tension** | 3.3V/5V |
| **Consommation** | < 100mA en fonctionnement |
| **WiFi** | 802.11 b/g/n |
| **Portée** | 50m en intérieur |
| **Température** | -10°C à +60°C |
| **Mémoire** | 4MB Flash, 400KB RAM |

## 🤝 Contribution

Les contributions sont les bienvenues ! N'hésitez pas à :
- 🐛 Signaler des bugs
- 💡 Proposer des améliorations
- 🔧 Soumettre des pull requests
- 📖 Améliorer la documentation

## 📄 Licence

Ce projet est sous licence MIT. Voir le fichier `LICENSE` pour plus de détails.

## 🙏 Remerciements

- **FauxmoESP** pour l'intégration Alexa
- **NeoPixelBus** pour la gestion des LEDs
- **ArduinoJson** pour la sérialisation
- **HLK-LD2410S** pour la détection de présence

---

<div align="center">

**Fait avec ❤️ pour la domotique moderne**

</div>