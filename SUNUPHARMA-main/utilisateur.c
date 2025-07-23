#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pharmacie.h"
// Vérifie si tous les caractères d'une chaîne sont des lettres MAJUSCULES
int estMajuscule(char str[]) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] < 'A' || str[i] > 'Z') {
            return 0;
        }
        i++;
    }
    return 1;
}

void creerUtilisateur() {
    Utilisateur u;

    printf("=== Creation d'un nouvel utilisateur ===\n");

    // Saisie du login
    do {
        printf("Entrez un login (5 lettres majuscules) : ");
        scanf("%s", u.login);
        viderBuffer();

        if (strlen(u.login) != 5) {
            printf("Erreur : le login doit contenir exactement 5 lettres.\n");
        } else if (!estMajuscule(u.login)) {
            printf("Erreur : le login doit contenir uniquement des lettres MAJUSCULES.\n");
        }
    } while (strlen(u.login) != 5 || !estMajuscule(u.login));

    strcpy(u.password, "pharma123");

    printf("Nom : ");
    scanf("%s", u.nom);
    viderBuffer();

    printf("Prenom : ");
    scanf("%s", u.prenom);
    viderBuffer();

    printf("Telephone : ");
    scanf("%s", u.telephone);
    viderBuffer();

    // Rôle (admin ou pharmacien)
    int choixRole;
    do {
        printf("Role (0 = admin, 1 = pharmacien) : ");
        scanf("%d", &choixRole);
        viderBuffer();
        if (choixRole != 0 && choixRole != 1) {
            printf("Choix invalide. Veuillez entrer 0 ou 1.\n");
        }
    } while (choixRole != 0 && choixRole != 1);

    u.role = (choixRole == 0) ? admin : pharmacien;
    u.actif = 1;

    // Sauvegarde dans le fichier USERS.dat
    FILE *f = fopen("USERS.dat", "ab");
    if (f == NULL) {
        printf("[ERREUR] Impossible d’ouvrir USERS.dat\n");
        return;
    }

    fwrite(&u, sizeof(Utilisateur), 1, f);
    fclose(f);

    printf("Utilisateur cree avec succes !\n");
}
