#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pharmacie.h"
void menuAdmin(Utilisateur u) {
    int choix;
    do {
        printf("\n=== MENU ADMIN (%s %s) ===\n", u.nom, u.prenom);
        printf("1. Ajouter un produit\n");
        printf("2. Modifier un produit\n");
        printf("3. Supprimer un produit\n");
        printf("4. Gérer les catégories\n");
        printf("5. Créer un compte utilisateur (pharmacien)\n");
        printf("6. Bloquer un compte pharmacien\n");
        printf("7. Débloquer un compte pharmacien\n");
        printf("8. Afficher la liste des utilisateurs\n");
        printf("0. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);
        viderBuffer();

        switch (choix) {
            case 1: ajouterProduit(); break;
            case 2: modifierProduit(); break;
            case 3: supprimerProduit(); break;
            case 4: infoCategorie(); break;
            case 5:
                creerUtilisateur();
                break;
            case 6:
                bloquerUtilisateur();
                break;
            case 7:
                debloquerUtilisateur();
                break;
            case 8:
                infoUtilisateurConnecte(&u);
                break;
            case 0:
                printf("Déconnexion...\n");
                break;
            default: printf("Choix invalide.\n");
        }
    } while (choix != 0);
}
void menuPharmacien(Utilisateur u) {
    int choix;
    do {
        printf("\n=== MENU PHARMACIEN ===\n");
        printf("1. Vendre un médicament (fonction à implémenter)\n");
        printf("2. Consulter le stock (fonction à implémenter)\n");
        printf("3. Générer facture (fonction à implémenter)\n");
        printf("0. Déconnexion\n");

        printf("Choix : ");
        scanf("%d", &choix);
        viderBuffer();

        switch (choix) {
            case 1:
                printf("[INFO] Fonction vente à coder.\n");
                break;
            case 2:
                printf("[INFO] Fonction consultation stock à coder.\n");
                break;
            case 3:
                printf("[INFO] Fonction génération facture à coder.\n");
                break;
            case 0:
                printf("Déconnexion...\n");
                break;
            default:
                printf("Choix invalide. Réessayez.\n");
        }
    } while (choix != 0);
}


