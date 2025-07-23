#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pharmacie.h"

// Fonction pour vider le buffer clavier
void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Afficher les informations de l'utilisateur connecté
void infoUtilisateurConnecte(Utilisateur *u) {
    printf("\n=== Informations Utilisateur ===\n");
    printf("Nom complet   : %s %s\n", u->prenom, u->nom);
    printf("Téléphone     : %s\n", u->telephone);
    printf("Login         : %s\n", u->login);
    printf("Rôle          : %s\n", u->role == admin ? "ADMIN" : "PHARMACIEN");
    printf("Statut        : %s\n", u->actif == 1 ? "Actif" : "Bloqué");
    printf("================================\n\n");
}

// Fonction pour changer le mot de passe à la première connexion
void changerMotDePasse(Utilisateur *u) {
    char nouveau[20], confirmation[20];

    do {
        printf("Entrez un nouveau mot de passe : ");
        scanf("%s", nouveau);
        viderBuffer();

        printf("Confirmez le mot de passe : ");
        scanf("%s", confirmation);
        viderBuffer();

        if (strcmp(nouveau, confirmation) != 0) {
            printf("[ERREUR] Les mots de passe ne correspondent pas. Réessayez.\n");
        }
    } while (strcmp(nouveau, confirmation) != 0);

    strcpy(u->password, nouveau);
    printf("[✓] Mot de passe mis à jour avec succès !\n");
}

// Connexion utilisateur
int connexionUtilisateur(Utilisateur *u) {
    char login[6], motDePasse[20];
    int trouve = 0;

    printf("=== Connexion ===\n");
    printf("Login : ");
    scanf("%s", login);
    viderBuffer();

    printf("Mot de passe : ");
    scanf("%s", motDePasse);
    viderBuffer();

    FILE *f = fopen("USERS.dat", "rb+"); // rb+ pour lire/écrire
    if (f == NULL) {
        printf("[ERREUR] Impossible d’ouvrir USERS.dat\n");
        return 0;
    }

    Utilisateur temp;
    while (fread(&temp, sizeof(Utilisateur), 1, f)) {
        if (strcmp(temp.login, login) == 0 && strcmp(temp.password, motDePasse) == 0) {
            trouve = 1;

            if (temp.actif == 0) {
                printf("[ERREUR] Votre compte est bloqué. Contactez l'administrateur.\n");
                fclose(f);
                return 0;
            }

            // Mot de passe par défaut
            if (strcmp(temp.password, "pharma123") == 0) {
                printf("[!] Vous utilisez le mot de passe par défaut.\n");
                changerMotDePasse(&temp);

                // Revenir à la position actuelle pour écraser
                fseek(f, -sizeof(Utilisateur), SEEK_CUR);
                fwrite(&temp, sizeof(Utilisateur), 1, f);
                fflush(f);
            }

            // Copier l'utilisateur connecté dans la variable passée
            *u = temp;

            printf("\n[✓] Connexion réussie en tant que %s.\n", u->role == admin ? "ADMIN" : "PHARMACIEN");
            infoUtilisateurConnecte(u);

            fclose(f);
            return 1;
        }
    }

    fclose(f);

    if (!trouve) {
        printf("[ERREUR] Identifiants incorrects.\n");
    }

    return 0;
}

