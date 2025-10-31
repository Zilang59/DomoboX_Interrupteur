# 📦 Release Directory

Ce répertoire contient les firmwares compilés prêts pour la distribution.

## 🎯 Contenu

- `firmware_X.X.X.bin` : Firmwares compilés avec versioning automatique
- Généré automatiquement lors de chaque compilation PlatformIO
- Utilisé par GitHub Actions pour créer les releases automatiques

## 🔄 Utilisation

### **Installation manuelle**
```bash
# Via esptool
esptool.py --port COM3 write_flash 0x0 firmware_1.0.0.bin

# Via PlatformIO
pio run -t upload --upload-port COM3
```

### **Mise à jour OTA**
1. Accéder à l'interface web de l'ESP32
2. Aller dans "Informations"
3. Cliquer sur le bouton de mise à jour (si disponible)
4. Ou utiliser l'endpoint `/update_firmware?file=firmware_X.X.X.bin`

## ⚠️ Notes importantes

- **Ne pas commiter** les fichiers `.bin` dans Git
- Les fichiers sont automatiquement générés lors de la compilation
- GitHub Actions utilise ces fichiers pour créer les releases publiques
- La version est extraite automatiquement depuis `src/main.cpp`

## 🔍 Structure des noms

```
firmware_[VERSION].bin
```

Exemples :
- `firmware_1.0.0.bin`
- `firmware_1.2.3.bin`
- `firmware_2.0.0-beta.bin`