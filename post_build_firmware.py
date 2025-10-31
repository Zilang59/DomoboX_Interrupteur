#!/usr/bin/env python3
"""
Script post-compilation pour renommer le firmware avec la version
Ce script s'exécute après la compilation PlatformIO
"""

import os
import re
import shutil
from pathlib import Path

# Import pour PlatformIO
Import("env")

def get_firmware_version():
    """Extrait la version du firmware depuis main.cpp"""
    main_cpp_path = Path("src/main.cpp")
    
    if not main_cpp_path.exists():
        print("❌ Fichier main.cpp non trouvé")
        return None
    
    with open(main_cpp_path, 'r', encoding='utf-8') as file:
        content = file.read()
    
    # Recherche de la ligne String Dispo_version = "x.x.x";
    version_pattern = r'String\s+Dispo_version\s*=\s*"([^"]+)"\s*;'
    match = re.search(version_pattern, content)
    
    if match:
        version = match.group(1)
        print(f"✅ Version détectée : {version}")
        return version
    else:
        print("❌ Version non trouvée dans main.cpp")
        return None

def post_build_action(source, target, env):
    """Action post-build appelée par PlatformIO"""
    print("🔧 Post-compilation : Renommage du firmware...")
    print(f"📍 Répertoire de travail : {Path.cwd()}")
    print(f"🎯 Target : {target[0]}")
    
    # Obtenir la version
    version = get_firmware_version()
    if not version:
        print("❌ Impossible d'obtenir la version")
        return
    
    # Créer le répertoire release s'il n'existe pas
    release_dir = Path("release")
    release_dir.mkdir(exist_ok=True)
    
    # Chemin du firmware source
    firmware_source = Path(str(target[0]))
    
    # Créer le nouveau nom de fichier
    new_firmware_name = f"firmware_{version}.bin"
    new_firmware_path = release_dir / new_firmware_name
    
    # Supprimer l'ancien fichier s'il existe et copier le nouveau
    try:
        # Supprimer l'ancien fichier s'il existe
        if new_firmware_path.exists():
            new_firmware_path.unlink()
            print(f"🗑️ Ancien firmware supprimé : {new_firmware_name}")
        
        # Copier le nouveau fichier
        shutil.copy2(firmware_source, new_firmware_path)
        print(f"✅ Firmware mis à jour dans release/ : {new_firmware_name}")
        
        # Afficher les informations sur le fichier
        file_size = new_firmware_path.stat().st_size
        print(f"📦 Taille du firmware : {file_size:,} bytes ({file_size / 1024:.1f} KB)")
        
        print("✅ Post-compilation terminée avec succès")
    except Exception as e:
        print(f"❌ Erreur lors de la mise à jour : {e}")

# Ajouter l'action post-build à PlatformIO
env.AddPostAction("$BUILD_DIR/firmware.bin", post_build_action)

# Compatibilité pour l'exécution directe du script
def main():
    print("🔧 Post-compilation : Renommage du firmware...")
    print(f"📍 Répertoire de travail : {Path.cwd()}")
    print(f"⏰ Heure d'exécution : {__import__('datetime').datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    
    # Simuler l'exécution pour test manuel
    build_dir = Path(".pio/build")
    build_dirs = [d for d in build_dir.iterdir() if d.is_dir()]
    
    if not build_dirs:
        print("❌ Aucun répertoire de build trouvé")
        return False
    
    target_dir = build_dirs[0]
    firmware_path = target_dir / "firmware.bin"
    
    if firmware_path.exists():
        # Simuler l'appel de post_build_action
        class MockEnv:
            pass
        
        post_build_action(None, [firmware_path], MockEnv())
    else:
        print(f"❌ Firmware non trouvé : {firmware_path}")

if __name__ == "__main__":
    main()