@echo off
setlocal ENABLEDELAYEDEXPANSION

:: ==== Présentation ====
echo ============================================
echo       🚀 PUSH TO GITHUB - by SUNUPHARMA
echo ============================================
echo.

:: === Demander le chemin du dossier local ===
set /p PROJECT_PATH=📁 Entrez le chemin complet du dossier projet (ex: C:\monprojet) : 

:: Vérifie si le dossier existe
if not exist "!PROJECT_PATH!" (
    echo ❌ Dossier introuvable. Vérifie le chemin.
    pause
    exit /b
)

cd /d "!PROJECT_PATH!"

:: === Demander l'URL GitHub ===
set /p GIT_URL=🌐 Entrez l'URL du dépôt GitHub (ex: https://github.com/monnom/projet.git) : 

:: === Initialiser Git si besoin ===
if not exist ".git" (
    echo 🔧 Initialisation de Git...
    git init
)

:: === Ajouter ou remplacer le remote ===
echo 🔗 Connexion au dépôt distant...
git remote remove origin 2>nul
git remote add origin "!GIT_URL!"

:: === Récupérer les données distantes s'il y en a ===
echo 📥 Récupération du contenu distant...
git pull origin main --allow-unrelated-histories

:: === Ajouter et commit local ===
echo 📦 Ajout des fichiers locaux...
git add .
git commit -m "Mise à jour initiale du projet" || echo (aucun changement à valider)

:: === Créer et pousser sur la branche main ===
echo 🚀 Envoi du projet sur GitHub...
git branch -M main
git push -u origin main

echo.
echo ✅ Projet synchronisé avec GitHub !
pause