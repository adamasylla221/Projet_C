#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pharmacie.h"

void bloquerUtilisateur() {
    char login[6];
    int trouve = 0;

    printf("=== Blocage d’un compte pharmacien ===\n");
    printf("Login du pharmacien à bloquer : ");
    scanf("%5s", login);
    viderBuffer();

    FILE *f = fopen("USERS.dat", "rb+");
    if (f == NULL) {
        printf("[ERREUR] Impossible d’ouvrir USERS.dat\n");
        return;
    }

    Utilisateur u;
    while (fread(&u, sizeof(Utilisateur), 1, f)) {
        if (strcmp(u.login, login) == 0 && u.role == pharmacien) {
            trouve = 1;
            if (u.actif == 0) {
                printf("Ce compte est déjà bloqué.\n");
                break;
            }
            u.actif = 0;
            fseek(f, -sizeof(Utilisateur), SEEK_CUR);
            fwrite(&u, sizeof(Utilisateur), 1, f);
            printf("[✓] Compte pharmacien bloqué avec succès.\n");
            break;
        }
    }

    fclose(f);
    if (!trouve) {
        printf("[ERREUR] Compte pharmacien introuvable.\n");
    }
}

void debloquerUtilisateur() {
    char login[6];
    int trouve = 0;

    printf("=== Déblocage d’un compte pharmacien ===\n");
    printf("Login du pharmacien à débloquer : ");
    scanf("%5s", login);
    viderBuffer();

    FILE *f = fopen("USERS.dat", "rb+");
    if (f == NULL) {
        printf("[ERREUR] Impossible d’ouvrir USERS.dat\n");
        return;
    }

    Utilisateur u;
    while (fread(&u, sizeof(Utilisateur), 1, f)) {
        if (strcmp(u.login, login) == 0 && u.role == pharmacien) {
            trouve = 1;
            if (u.actif == 1) {
                printf("Ce compte est déjà actif.\n");
                break;
            }
            u.actif = 1;
            fseek(f, -sizeof(Utilisateur), SEEK_CUR);
            fwrite(&u, sizeof(Utilisateur), 1, f);
            printf("[✓] Compte pharmacien débloqué avec succès.\n");
            break;
        }
    }

    fclose(f);
    if (!trouve) {
        printf("[ERREUR] Compte pharmacien introuvable.\n");
    }
}

