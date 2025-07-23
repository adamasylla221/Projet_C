#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pharmacie.h"
int main() {
    int choix;
    Utilisateur u;

    do {
        printf("\n=== BIENVENUE À SUNUPHARMA ===\n");
        printf("1. S'inscrire (Créer un compte utilisateur)\n");
        printf("2. Se connecter\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        viderBuffer();

        switch (choix) {
            case 1:
                creerUtilisateur();
                break;
            case 2:
                if (connexionUtilisateur(&u)) {
                    if (u.role == admin) {
                        menuAdmin(u);
                    } else {
                        menuPharmacien(u);
                    }
                } else {
                    printf("[ERREUR] Échec de la connexion.\n");
                }
                break;
            case 0:
                printf("Merci d'avoir utilisé SUNUPHARMA. À bientôt !\n");
                break;
            default:
                printf("Choix invalide, veuillez réessayer.\n");
        }

    } while (choix != 0);

    return 0;
}
