#!/usr/bin/env python3
"""
Script de configuration Git/GitHub LOCAL AVANCÉ
Ce script ne doit JAMAIS être ajouté au repository !
Il configure automatiquement votre projet local pour GitHub avec GitHub CLI.

Usage: python git_setup_local.py
"""

# =============================================================================
# CONFIGURATION - Modifiez ces valeurs selon vos besoins
# =============================================================================

# Votre token GitHub personnel (créé sur GitHub.com → Settings → Developer settings → Personal access tokens)
# Permissions requises : "repo" (Full control of private repositories)
# Laissez vide ("") si vous voulez le saisir à chaque fois
GITHUB_TOKEN = "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"

# Votre nom d'utilisateur GitHub par défaut
GITHUB_USERNAME = "Zilang59"

# =============================================================================
# NE MODIFIEZ RIEN EN DESSOUS DE CETTE LIGNE
# =============================================================================

import os
import subprocess
import sys
import json
import getpass
from pathlib import Path

# Vérifier et installer requests si nécessaire
try:
    import requests
except ImportError:
    print("📦 Installation de la bibliothèque 'requests' nécessaire...")
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "install", "requests"])
        import requests
        print("   ✅ 'requests' installé avec succès")
    except Exception as e:
        print(f"   ❌ Erreur lors de l'installation de 'requests': {e}")
        print("   💡 Installez manuellement avec: pip install requests")
        sys.exit(1)

class LocalGitSetup:
    def __init__(self):
        self.project_name = ""
        self.github_username = GITHUB_USERNAME  # Utilise la valeur configurée
        self.repo_url = ""
        self.use_ssh = False
        self.gh_available = False
        self.gh_authenticated = False
        self.github_token = GITHUB_TOKEN if GITHUB_TOKEN else None  # Utilise le token configuré
        
    def check_git_installation(self):
        """Vérifie que Git est installé et accessible"""
        print("\n🔍 Vérification de Git...")
        
        try:
            result = subprocess.run(["git", "--version"], capture_output=True, text=True, check=True)
            git_version = result.stdout.strip()
            print(f"   ✅ Git installé : {git_version}")
            return True
        except (subprocess.CalledProcessError, FileNotFoundError):
            print("   ❌ Git n'est pas installé ou pas accessible")
            print("   📝 Installez Git depuis : https://git-scm.com/download/win")
            print("   💡 Redémarrez ce script après l'installation")
            return False
        
    def check_github_cli(self):
        """Vérifie si GitHub CLI est disponible et configuré (optionnel)"""
        print("\n🔍 Vérification GitHub CLI (optionnel)...")
        
        # Vérifier si gh est installé
        success, _ = self.run_command("gh --version", "Test installation GitHub CLI", check_error=False)
        if not success:
            print("   ⚠️  GitHub CLI (gh) n'est pas installé")
            print("   ✅ Pas de problème ! Le workflow classique fonctionne parfaitement")
            print("   💡 GitHub CLI est juste un bonus pour créer automatiquement les repos")
            return False
        
        self.gh_available = True
        print("   ✅ GitHub CLI installé")
        
        # Vérifier l'authentification
        success, output = self.run_command("gh auth status", "Vérification authentification GitHub", check_error=False)
        if "Logged in to github.com" in output:
            self.gh_authenticated = True
            print("   ✅ Authentifié sur GitHub")
            
            # Récupérer le username automatiquement
            success, user_info = self.run_command("gh api user", "Récupération infos utilisateur", check_error=False)
            if success:
                try:
                    user_data = json.loads(user_info)
                    self.github_username = user_data.get('login', self.github_username)
                    print(f"   👤 Utilisateur GitHub: {self.github_username}")
                except:
                    pass
        else:
            print("   ⚠️  Non authentifié sur GitHub")
            print("   ✅ Pas de problème ! Vous pouvez utiliser le workflow classique")
        
        return True
    
    def choose_connection_method(self):
        """Choisit la méthode de connexion (HTTPS/SSH)"""
        if self.gh_authenticated:
            print("\n🔐 Méthode de connexion:")
            print("1. HTTPS (recommandé avec GitHub CLI)")
            print("2. SSH (clés SSH requises)")
            choice = input("Votre choix [1]: ") or "1"
            
            if choice == "2":
                # Vérifier les clés SSH
                success, _ = self.run_command("gh ssh-key list", "Vérification clés SSH", check_error=False)
                if success:
                    self.use_ssh = True
                    print("   ✅ SSH configuré")
                else:
                    print("   ⚠️  Aucune clé SSH trouvée, utilisation HTTPS")
        
    def get_project_info(self):
        """Récupère les infos du projet et propose la création automatique si possible"""
        print("🔧 Configuration Git/GitHub LOCAL AVANCÉ")
        print("=" * 40)
        
        # Nom du projet (dossier actuel)
        current_folder = os.path.basename(os.getcwd())
        self.project_name = input(f"Nom du repo GitHub [{current_folder}]: ") or current_folder
        
        # Username (possiblement récupéré automatiquement)
        username = input(f"Username GitHub [{self.github_username}]: ") or self.github_username
        self.github_username = username
        
        # URL du repo selon la méthode choisie
        if self.use_ssh:
            self.repo_url = f"git@github.com:{self.github_username}/{self.project_name}.git"
        else:
            self.repo_url = f"https://github.com/{self.github_username}/{self.project_name}.git"
        
        print(f"📡 URL du repo: {self.repo_url}")
        
        # Vérifier si le repository existe déjà
        repo_exists = self.check_repo_exists_on_github()
        
        if repo_exists:
            print(f"\n✅ Le repository existe déjà sur GitHub !")
            print(f"🔧 Configuration locale uniquement (pas de création de repo)")
            return True
        
        # Repository n'existe pas, proposer création automatique
        print(f"\n🤖 Repository inexistant - Création automatique disponible !")
        
        if self.gh_authenticated:
            print("   ✅ Méthode 1: GitHub CLI (disponible)")
        else:
            print("   ⚠️  Méthode 1: GitHub CLI (non disponible)")
        
        if GITHUB_TOKEN:
            print("   ✅ Méthode 2: API GitHub (token configuré - RECOMMANDÉE)")
            default_choice = "2"
        else:
            print("   ⚠️  Méthode 2: API GitHub (token requis)")
            default_choice = "3"
            
        print("   ✅ Méthode 3: Création manuelle")
        
        choice = input(f"\nChoisissez (1=CLI, 2=API, 3=Manuel) [{default_choice}]: ") or default_choice
        
        if choice == "1" and self.gh_authenticated:
            return self.create_github_repo()
        elif choice == "2":
            return self.create_repo_with_api()
        else:
            # Workflow manuel
            print(f"\n📝 Workflow manuel :")
            print(f"1. Allez sur GitHub.com")
            print(f"2. Cliquez sur 'New repository'")
            print(f"3. Nommez-le '{self.project_name}'")
            print(f"4. Laissez-le VIDE (pas de README, pas de .gitignore)")
            print(f"5. Cliquez 'Create repository'")
            
            confirm = input(f"\nRepository '{self.project_name}' créé sur GitHub ? (y/n): ")
            if confirm.lower() != 'y':
                print("❌ Créez d'abord le repo sur GitHub, puis relancez ce script.")
                print("💡 C'est rapide : github.com -> New repository -> Create")
                sys.exit(1)
        
        return True
    
    def create_github_repo(self):
        """Crée le repository sur GitHub avec GitHub CLI"""
        print(f"\n🚀 Création du repository '{self.project_name}' sur GitHub...")
        
        # Options pour la création du repo
        visibility = input("Repository public ou privé ? (public/private) [private]: ") or "private"
        description = input(f"Description du projet [Projet {self.project_name}]: ") or f"Projet {self.project_name}"
        
        # Commande de création
        visibility_flag = "--public" if visibility.lower() == "public" else "--private"
        cmd = f'gh repo create "{self.project_name}" {visibility_flag} --description "{description}" --confirm'
        
        success, output = self.run_command(cmd, "Création repository GitHub")
        
        if success:
            print(f"   🎉 Repository créé avec succès !")
            print(f"   🌐 Accessible sur: https://github.com/{self.github_username}/{self.project_name}")
            return True
        else:
            # Le repo existe peut-être déjà
            if "already exists" in output.lower():
                print("   ⚠️  Le repository existe déjà")
                use_existing = input("   Utiliser le repository existant ? (y/n): ")
                return use_existing.lower() == 'y'
            else:
                print(f"   ❌ Erreur lors de la création: {output}")
                manual = input("   Continuer avec création manuelle ? (y/n): ")
                if manual.lower() == 'y':
                    print("   📝 Créez le repository manuellement sur GitHub puis continuez")
                    input("   Appuyez sur Entrée quand c'est fait...")
                    return True
                return False
    
    def get_github_token(self):
        """Récupère le token GitHub configuré ou le demande"""
        # Si le token est configuré dans le fichier, l'utiliser directement
        if GITHUB_TOKEN:
            print("   ✅ Token GitHub configuré dans le script")
            return GITHUB_TOKEN
        
        # Vérifier si un token est dans les variables d'environnement
        token = os.environ.get('GITHUB_TOKEN')
        if token:
            print("   ✅ Token GitHub trouvé dans les variables d'environnement")
            return token
        
        print("\n🔑 Aucun token GitHub configuré")
        print("📝 Comment configurer le token :")
        print("   1. Allez sur GitHub.com → Settings → Developer settings → Personal access tokens → Tokens (classic)")
        print("   2. Cliquez 'Generate new token (classic)'")
        print("   3. Donnez un nom (ex: 'Git Setup Script')")
        print("   4. Cochez au minimum : 'repo' (Full control of private repositories)")
        print("   5. Cliquez 'Generate token' et copiez-le")
        print("   6. Collez-le dans la variable GITHUB_TOKEN au début de ce fichier")
        
        choice = input("\nVoulez-vous saisir temporairement un token ? (y/n): ")
        if choice.lower() == 'y':
            token = getpass.getpass("Collez votre token GitHub (ne s'affichera pas): ").strip()
            if token:
                print("💡 Pour éviter de le ressaisir, ajoutez-le dans la variable GITHUB_TOKEN du script")
                return token
        
        return None
    
    def create_repo_with_api(self):
        """Crée le repository via l'API GitHub REST"""
        print(f"\n🤖 Création automatique du repository via l'API GitHub...")
        
        if not self.github_token:
            self.github_token = self.get_github_token()
            if not self.github_token:
                print("   ❌ Token requis pour la création automatique")
                return False
        
        # Options pour la création du repo
        visibility = input("Repository public ou privé ? (public/private) [private]: ") or "private"
        description = input(f"Description du projet [Projet {self.project_name}]: ") or f"Projet {self.project_name}"
        
        # Préparer les données pour l'API
        data = {
            "name": self.project_name,
            "description": description,
            "private": visibility.lower() == "private",
            "auto_init": False,  # Pas de README automatique
            "gitignore_template": None,
            "license_template": None
        }
        
        # Appel à l'API GitHub
        headers = {
            "Authorization": f"token {self.github_token}",
            "Accept": "application/vnd.github.v3+json",
            "User-Agent": "Git-Setup-Script"
        }
        
        try:
            response = requests.post(
                "https://api.github.com/user/repos",
                headers=headers,
                json=data,
                timeout=10
            )
            
            if response.status_code == 201:
                repo_info = response.json()
                print(f"   🎉 Repository créé avec succès !")
                print(f"   🌐 URL: {repo_info['html_url']}")
                return True
            
            elif response.status_code == 422:
                # Repository existe déjà
                print("   ⚠️  Le repository existe déjà")
                use_existing = input("   Utiliser le repository existant ? (y/n): ")
                return use_existing.lower() == 'y'
            
            else:
                error_info = response.json() if response.content else {}
                print(f"   ❌ Erreur {response.status_code}: {error_info.get('message', 'Erreur inconnue')}")
                return False
                
        except requests.exceptions.RequestException as e:
            print(f"   ❌ Erreur de connexion: {e}")
            print("   💡 Vérifiez votre connexion internet")
            return False
        except Exception as e:
            print(f"   ❌ Erreur: {e}")
            return False
    
    def check_repo_exists_on_github(self):
        """Vérifie si le repository existe déjà sur GitHub"""
        print(f"\n🔍 Vérification de l'existence du repository sur GitHub...")
        
        # Méthode 1: Avec token API (plus fiable)
        if self.github_token:
            try:
                headers = {
                    "Authorization": f"token {self.github_token}",
                    "Accept": "application/vnd.github.v3+json",
                    "User-Agent": "Git-Setup-Script"
                }
                
                response = requests.get(
                    f"https://api.github.com/repos/{self.github_username}/{self.project_name}",
                    headers=headers,
                    timeout=10
                )
                
                if response.status_code == 200:
                    repo_info = response.json()
                    print(f"   ✅ Repository existe : {repo_info['html_url']}")
                    print(f"   📝 Description : {repo_info.get('description', 'Aucune description')}")
                    print(f"   🔒 Visibilité : {'Privé' if repo_info['private'] else 'Public'}")
                    return True
                elif response.status_code == 404:
                    print(f"   ❌ Repository n'existe pas encore")
                    return False
                else:
                    print(f"   ⚠️  Impossible de vérifier (erreur {response.status_code})")
                    
            except Exception as e:
                print(f"   ⚠️  Erreur lors de la vérification : {e}")
        
        # Méthode 2: Tentative de clone léger (fallback)
        try:
            result = subprocess.run(
                ["git", "ls-remote", self.repo_url],
                capture_output=True,
                text=True,
                timeout=10
            )
            if result.returncode == 0:
                print(f"   ✅ Repository accessible")
                return True
            else:
                print(f"   ❌ Repository non accessible ou inexistant")
                return False
        except Exception:
            print(f"   ⚠️  Impossible de vérifier l'existence du repository")
            return False
    
    def run_command(self, cmd, description, check_error=True):
        """Exécute une commande et gère les erreurs"""
        print(f"🔄 {description}...")
        try:
            result = subprocess.run(cmd, shell=True, check=True, capture_output=True, text=True)
            print(f"   ✅ {description}")
            return True, result.stdout
        except subprocess.CalledProcessError as e:
            if check_error:
                print(f"   ❌ Erreur: {e.stderr.strip()}")
                return False, e.stderr
            else:
                # Certaines commandes peuvent "échouer" normalement
                print(f"   ⚠️  {description} (ignoré)")
                return True, ""
    
    def setup_git_config(self):
        """Configure Git si nécessaire - OBLIGATOIRE pour les commits"""
        print("\n⚙️  Configuration Git...")
        
        # Vérifier le nom d'utilisateur
        success, name_output = self.run_command("git config user.name", "Vérification nom Git", check_error=False)
        user_name = name_output.strip() if success else ""
        
        # Vérifier l'email
        success, email_output = self.run_command("git config user.email", "Vérification email Git", check_error=False)
        user_email = email_output.strip() if success else ""
        
        # Configuration du nom si manquant
        if not user_name:
            print("   ⚠️  Nom d'utilisateur Git non configuré")
            name = input("   👤 Entrez votre nom complet (ex: Jean Dupont): ").strip()
            if name:
                self.run_command(f'git config --global user.name "{name}"', "Configuration nom Git")
            else:
                print("   ❌ Nom requis pour Git")
                return False
        else:
            print(f"   ✅ Nom Git configuré : {user_name}")
        
        # Configuration de l'email si manquant
        if not user_email:
            print("   ⚠️  Email Git non configuré")
            email = input("   📧 Entrez votre email GitHub (ex: votre@email.com): ").strip()
            if email and "@" in email:
                self.run_command(f'git config --global user.email "{email}"', "Configuration email Git")
            else:
                print("   ❌ Email valide requis pour Git")
                return False
        else:
            print(f"   ✅ Email Git configuré : {user_email}")
        
        print("   🎉 Git correctement configuré pour les commits !")
        return True
    
    def init_git_if_needed(self):
        """Initialise Git si le dossier n'est pas déjà un repo"""
        print("\n🌿 Initialisation Git...")
        
        if Path(".git").exists():
            print("   ✅ Repository Git déjà initialisé")
            return True
        
        success, _ = self.run_command("git init", "Initialisation Git")
        if success:
            self.run_command("git branch -M main", "Création branche main")
        return success
    
    def setup_gitignore_if_needed(self):
        """Crée un .gitignore s'il n'existe pas"""
        print("\n📝 Vérification .gitignore...")
        
        if Path(".gitignore").exists():
            print("   ✅ .gitignore existe déjà")
            return
        
        # Ajouter notre script à .gitignore !
        gitignore_content = """# Scripts de configuration locale (ne pas pousser)
git_setup_local.py
git_setup_local.bat
setup_*.py
*_local.*

# Python
__pycache__/
*.py[cod]
*$py.class
.env
.venv
venv/
env/
ENV/

# IDEs  
.vscode/settings.json
.vscode/launch.json
.idea/
*.swp
*.swo

# OS
.DS_Store
Thumbs.db
*.log
*.tmp
desktop.ini

# GitHub CLI
.gh/

# Temporary files
*.bak
*.orig
*.rej
"""
        
        with open(".gitignore", "w", encoding="utf-8") as f:
            f.write(gitignore_content)
        print("   ✅ .gitignore créé (avec exclusion des scripts de setup)")
    
    def setup_remote(self):
        """Configure le remote origin"""
        print(f"\n🔗 Configuration remote GitHub...")
        
        # Vérifier si origin existe déjà
        success, output = self.run_command("git remote get-url origin", "Vérification remote", check_error=False)
        
        if success and output.strip():
            print(f"   ✅ Remote origin existe: {output.strip()}")
            # Vérifier s'il correspond
            if self.repo_url not in output:
                change = input(f"Changer pour {self.repo_url} ? (y/n): ")
                if change.lower() == 'y':
                    self.run_command("git remote remove origin", "Suppression ancien remote")
                    self.run_command(f"git remote add origin {self.repo_url}", "Ajout nouveau remote")
        else:
            # Ajouter le remote
            self.run_command(f"git remote add origin {self.repo_url}", "Ajout remote origin")
    
    def prepare_initial_commit(self):
        """Prépare le commit initial"""
        print(f"\n📦 Préparation commit initial...")
        
        # Vérifier s'il y a déjà des commits
        success, output = self.run_command("git log --oneline -1", "Vérification commits", check_error=False)
        
        if success and output.strip():
            print("   ✅ Des commits existent déjà")
            return True
        
        # Ajouter tous les fichiers (sauf ceux dans .gitignore)
        self.run_command("git add .", "Ajout des fichiers")
        
        # Vérifier qu'il y a quelque chose à committer
        success, output = self.run_command("git status --porcelain", "Vérification fichiers", check_error=False)
        
        if not output.strip():
            print("   ⚠️  Aucun fichier à committer")
            return False
        
        # Commit initial
        commit_msg = f"Initial commit: {self.project_name}"
        success, _ = self.run_command(f'git commit -m "{commit_msg}"', "Commit initial")
        return success
    
    def push_to_github(self):
        """Pousse le commit initial vers GitHub"""
        print(f"\n📤 Push initial vers GitHub...")
        
        auto_push = input("Pousser automatiquement vers GitHub maintenant ? (y/n): ")
        if auto_push.lower() == 'y':
            success, _ = self.run_command("git push -u origin main", "Push initial vers GitHub")
            if success:
                print("   🎉 Code poussé avec succès vers GitHub !")
                return True
            else:
                print("   ⚠️  Erreur lors du push")
                print("   💡 Vous pourrez faire 'git push' manuellement dans VS Code")
        else:
            print("   📝 Vous pourrez faire le push plus tard avec 'git push' ou dans VS Code")
        
        return False
    
    def final_instructions(self, pushed=False):
        """Affiche les instructions finales"""
        print(f"\n🎉 CONFIGURATION TERMINÉE !")
        print("=" * 50)
        print(f"📁 Projet: {self.project_name}")
        print(f"🐙 GitHub: {self.repo_url}")
        
        if pushed:
            print(f"✅ Code déjà poussé vers GitHub !")
        
        print(f"\n🎯 PROCHAINES ÉTAPES:")
        print("1. Ouvrez VS Code dans ce dossier")
        print("2. Utilisez Source Control (Ctrl+Shift+G)")
        print("3. Faites vos commits/push normalement")
        
        if self.gh_available and not pushed:
            print(f"\n🚀 Commandes GitHub CLI utiles (optionnelles):")
            print("   gh repo view           - Voir le repo sur GitHub")
            print("   gh issue create        - Créer une issue")
            print("   gh pr create           - Créer une pull request")
        
        print(f"\n📋 Commandes Git classiques:")
        print("   git status             - Voir l'état")
        print("   git add .              - Ajouter tous les fichiers")  
        print("   git commit -m '...'    - Committer")
        
        if not pushed:
            print("   git push               - Pousser vers GitHub")
        else:
            print("   git push               - Pousser les futurs commits")
            
        print(f"\n🔒 SÉCURITÉ:")
        print("   ✅ Ce script est dans .gitignore (ne sera jamais poussé)")
        if self.use_ssh:
            print("   🔑 Connexion SSH configurée")
        else:
            print("   🔐 Connexion HTTPS sécurisée")
            
        print(f"\n🌐 Accès web: https://github.com/{self.github_username}/{self.project_name}")
    
    def run(self):
        """Lance le processus complet avec toutes les vérifications"""
        try:
            # 1. Vérifications système
            if not self.check_git_installation():
                return False
            
            # 2. Vérifications GitHub CLI (optionnelles)
            self.check_github_cli()
            self.choose_connection_method()
            
            # 3. Configuration du projet
            if not self.get_project_info():
                return False
            
            # 4. Configuration Git OBLIGATOIRE
            if not self.setup_git_config():
                print("❌ Configuration Git requise pour continuer")
                return False
                
            # 5. Initialisation locale
            if not self.init_git_if_needed():
                return False
            
            # 6. Préparation des fichiers
            self.setup_gitignore_if_needed()
            self.setup_remote()
            
            # 7. Commits et push
            if self.prepare_initial_commit():
                pushed = self.push_to_github()
                self.final_instructions(pushed)
            else:
                self.final_instructions(False)
            
            return True
            
        except KeyboardInterrupt:
            print("\n\n⏹️  Processus annulé")
            return False
        except Exception as e:
            print(f"\n❌ Erreur: {e}")
            print("💡 Vérifiez votre configuration Git et GitHub")
            return False

if __name__ == "__main__":
    setup = LocalGitSetup()
    setup.run()
    input("\nAppuyez sur Entrée pour fermer...")