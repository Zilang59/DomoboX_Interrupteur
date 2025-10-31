#!/usr/bin/env python3
"""
Script pour créer automatiquement une release GitHub
basée sur la version dans main.cpp
"""

import re
import subprocess
import sys
from pathlib import Path

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

def check_git_status():
    """Vérifie que le dépôt git est propre"""
    try:
        result = subprocess.run(['git', 'status', '--porcelain'], 
                              capture_output=True, text=True, check=True)
        if result.stdout.strip():
            print("❌ Il y a des modifications non commitées :")
            print(result.stdout)
            return False
        return True
    except subprocess.CalledProcessError:
        print("❌ Erreur lors de la vérification du statut git")
        return False

def tag_exists(tag):
    """Vérifie si un tag existe déjà"""
    try:
        result = subprocess.run(['git', 'tag', '-l', tag], 
                              capture_output=True, text=True, check=True)
        return bool(result.stdout.strip())
    except subprocess.CalledProcessError:
        return False

def create_tag_and_push(version):
    """Crée un tag et le pousse vers GitHub"""
    tag = f"v{version}"
    
    if tag_exists(tag):
        print(f"❌ Le tag {tag} existe déjà")
        return False
    
    try:
        # Créer le tag
        subprocess.run(['git', 'tag', '-a', tag, '-m', f'Release {tag}'], check=True)
        print(f"✅ Tag {tag} créé")
        
        # Pousser le tag
        subprocess.run(['git', 'push', 'origin', tag], check=True)
        print(f"✅ Tag {tag} poussé vers GitHub")
        
        return True
    except subprocess.CalledProcessError as e:
        print(f"❌ Erreur lors de la création/push du tag : {e}")
        return False

def main():
    print("🚀 Création d'une nouvelle release...")
    
    # Vérifier que le dépôt est propre
    if not check_git_status():
        print("💡 Commitez vos modifications avant de créer une release")
        sys.exit(1)
    
    # Obtenir la version
    version = get_firmware_version()
    if not version:
        sys.exit(1)
    
    # Demander confirmation
    response = input(f"Créer une release pour la version {version} ? (y/N): ")
    if response.lower() not in ['y', 'yes']:
        print("❌ Release annulée")
        sys.exit(0)
    
    # Créer et pousser le tag
    if create_tag_and_push(version):
        print(f"""
✅ Release créée avec succès !

🔗 Le workflow GitHub Actions va maintenant :
   1. Compiler le firmware
   2. Créer la release v{version}
   3. Attacher le fichier firmware_{version}.bin

📍 Suivez le progrès sur : https://github.com/Zilang59/DomoboX_Interrupteur/actions
""")
    else:
        sys.exit(1)

if __name__ == "__main__":
    main()