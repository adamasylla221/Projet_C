@echo off
setlocal

:: === Configuration ===
set "PROJECT_PATH=C:\Users\DELL\Desktop\L1 GL COURS\Langage C L1\sunupharma-main"
set "GIT_URL=https://github.com/adamasylla221/SUNUPHARMA.git"
set "BRANCH=main"

:: === Aller dans le dossier du projet ===
cd /d "%PROJECT_PATH%" || (
    echo [ERREUR] Impossible de trouver le dossier : %PROJECT_PATH%
    pause
    exit /b
)

:: === Initialiser Git ===
echo Initialisation du dépôt Git...
git init

:: === Ajouter le dépôt distant ===
echo Ajout du dépôt distant...
git remote remove origin 2>nul
git remote add origin %GIT_URL%

:: === Ajouter tous les fichiers ===
echo Ajout des fichiers...
git add .

:: === Créer un commit initial ===
echo Création du commit...
git commit -m "Premier commit du projet SUNUPHARMA"

:: === Créer la branche principale ===
echo Création de la branche %BRANCH%...
git branch -M %BRANCH%

:: === Pousser sur GitHub ===
echo Envoi vers GitHub...
git push -u origin %BRANCH%

echo.
echo ✅ Projet envoyé avec succès sur GitHub !
pause
