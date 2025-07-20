#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pharmacie.h"

#define CLEAR_SCREEN() system("cls||clear")
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"

void changerMotDePasse(Utilisateur *user);
void initialiserAdmin();
void menuAdmin(Utilisateur user);
void menuPharmacien(Utilisateur user);

void initialiserAdmin() {
    // POUR TESTS UNIQUEMENT
    remove("USERS.dat");

    FILE *f = fopen("USERS.dat", "rb");
    int existe = 0;
    Utilisateur u;

    if (f != NULL) {
        while (fread(&u, sizeof(Utilisateur), 1, f)) {
            if (u.role == ADMIN) {
                existe = 1;
                break;
            }
        }
        fclose(f);
    }

    if (!existe) {
        printf(YELLOW "Aucun administrateur detecte.\n" RESET);
        Utilisateur Admin;
        strcpy(Admin.login, "ADMIN");
        strcpy(Admin.password, "admin123");
        Admin.role = ADMIN;
        Admin.actif = 1;
        strcpy(Admin.nom, "Admin");
        strcpy(Admin.prenom, "Admin");
        strcpy(Admin.telephone, "0000000000");

        FILE *fw = fopen("USERS.dat", "ab");
        fwrite(&Admin, sizeof(Utilisateur), 1, fw);
        fclose(fw);
        printf(GREEN "[OK] Compte admin cree : login = ADMIN / mot de passe = admin123\n" RESET);
    }
}

int main() {
    Utilisateur adminConnecte;
    Utilisateur userConnecte;
    int choixPrincipal;
    int connecte = 0;

    initialiserAdmin();

    // Connexion obligatoire admin
    do {
        CLEAR_SCREEN();
        printf(CYAN "=======================================\n");
        printf("     CONNEXION ADMINISTRATEUR REQUISE\n");
        printf("=======================================\n" RESET);

        connecte = connexionUtilisateur(&adminConnecte);

        if (!connecte || adminConnecte.role != ADMIN) {
            printf(RED "\n[!] Connexion refusee. Seul un administrateur peut continuer.\n" RESET);
            connecte = 0;
            printf("Appuyez sur une touche pour reessayer...");
            getchar(); getchar();
        }
    } while (!connecte);

    // MENU PRINCIPAL
    do {
        CLEAR_SCREEN();
        printf(BLUE "=======================================\n");
        printf("         PHARMACIE SUNUPHARMA\n");
        printf("=======================================\n" RESET);
        printf(YELLOW "1. Acceder au menu ADMIN\n");
        printf("2. Creer un compte Pharmacien\n");
        printf("3. Connexion Pharmacien\n");
        printf("4. Quitter\n" RESET);
        printf("Choix : ");
        scanf("%d", &choixPrincipal);

        switch (choixPrincipal) {
            case 1:
                menuAdmin(adminConnecte);
                break;
            case 2:
                creerPharmacien();
                printf(GREEN "Pharmacien cree avec succes.\n" RESET);
                printf("Appuyez sur une touche pour continuer...");
                getchar(); getchar();
                break;
            case 3:
                if (connexionUtilisateur(&userConnecte)) {
                    if (userConnecte.role == PHARMACIEN) {
                        if (strcmp(userConnecte.password, "pharma123") == 0) {
                            printf(RED "\n[!] Mot de passe par defaut detecte. Veuillez le changer.\n" RESET);
                            changerMotDePasse(&userConnecte);
                            sauvegarderUtilisateurs();
                        }
                        menuPharmacien(userConnecte);
                    } else {
                        printf(RED "[!] Ce n est pas un compte pharmacien.\n" RESET);
                        getchar(); getchar();
                    }
                } else {
                    printf(RED "[!] Connexion echouee.\n" RESET);
                    getchar(); getchar();
                }
                break;
            case 4:
                printf(GREEN "Au revoir !\n" RESET);
                return 0;
            default:
                printf(RED "[!] Choix invalide.\n" RESET);
                getchar(); getchar();
        }
    } while (1);

    return 0;
}

void menuAdmin(Utilisateur user) {
    int choix;
    do {
       // CLEAR_SCREEN();
        printf(CYAN "================================\n");
        printf("  MENU ADMINISTRATEUR - %s\n", user.login);
        printf("================================\n" RESET);
        printf(YELLOW "1. Gestion des produits\n");
        printf("2. Gestion des categories\n");
        printf("3. Gestion des utilisateurs\n");
        printf("4. Verification des stocks\n");
        printf("5. Deconnexion\n" RESET);
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: {
                int souschoix;
                do {
                    CLEAR_SCREEN();
                    printf(BLUE "=== Gestion Produits ===\n" RESET);
                    printf(YELLOW "1. Ajouter produit\n");
                    printf("2. Modifier produit\n");
                    printf("3. Supprimer produit\n");
                    printf("4. Afficher produits\n");
                    printf("5. Retour\n" RESET);
                    printf("Choix : ");
                    scanf("%d", &souschoix);
                    switch (souschoix) {
                        case 1: ajouterProduit(); break;
                        case 2: modifierProduit(); break;
                        case 3: supprimerProduit(); break;
                        case 4: afficherProduits(); break;
                    }
                    if (souschoix != 5) {
                        printf("Appuyez sur une touche pour continuer...");
                        getchar(); getchar();
                    }
                } while (souschoix != 5);
                break;
            }
            case 2: {
                int souschoix;
                do {
                  //  CLEAR_SCREEN();
                    printf(BLUE "=== Gestion Categories ===\n" RESET);
                    printf(YELLOW "1. Ajouter categorie\n");
                    printf("2. Modifier categorie\n");
                    printf("3. Supprimer categorie\n");
                    printf("4. Afficher categories\n");
                    printf("5. Retour\n" RESET);
                    printf("Choix : ");
                    scanf("%d", &souschoix);
                    switch (souschoix) {
                        case 1: ajouterCategorie(); break;
                        case 2: modifierCategorie(); break;
                        case 3: supprimerCategorie(); break;
                        case 4: afficherCategories(); break;
                    }
                    if (souschoix != 5) {
                        printf("Appuyez sur une touche pour continuer...");
                        getchar(); getchar();
                    }
                } while (souschoix != 5);
                break;
            }
            case 3: {
                int souschoix;
                do {
                  //  CLEAR_SCREEN();
                    printf(BLUE "=== Gestion Utilisateurs ===\n" RESET);
                    printf(YELLOW "1. Creer utilisateur\n");
                    printf("2. Afficher utilisateurs\n");
                    printf("3. Bloquer utilisateur\n");
                    printf("4. Debloquer utilisateur\n");
                    printf("5. Retour\n" RESET);
                    printf("Choix : ");
                    scanf("%d", &souschoix);
                    switch (souschoix) {
                        case 1: ajouterUtilisateur(); break;
                        case 2: afficherUtilisateurs(); break;
                        case 3: bloquerUtilisateur(); break;
                        case 4: debloquerUtilisateur(); break;
                    }
                    if (souschoix != 5) {
                        printf("Appuyez sur une touche pour continuer...");
                        getchar(); getchar();
                    }
                } while (souschoix != 5);
                break;
            }
            case 4:
                verification_stock();
                printf("Appuyez sur une touche pour continuer...");
                getchar(); getchar();
                break;
            case 5:
                printf(GREEN "Deconnexion...\n" RESET);
                break;
            default:
                printf(RED "Choix invalide.\n" RESET);
                getchar(); getchar();
        }
    } while (choix != 5);
}

void menuPharmacien(Utilisateur user) {
    int choix;
    do {
      //  CLEAR_SCREEN();
        printf(CYAN "================================\n");
        printf("  MENU PHARMACIEN - %s\n", user.login);
        printf("================================\n" RESET);
        printf(YELLOW "1. Vendre un produit\n");
        printf("2. Consulter le stock\n");
        printf("3. Etat journalier\n");
        printf("4. Deconnexion\n" RESET);
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                venteProduits(user);
                break;
            case 2:
                afficherProduits();
                break;
            case 3:
                etatJournalier(NULL);
                break;
            case 4:
                printf(GREEN "Deconnexion...\n" RESET);
                break;
            default:
                printf(RED "Choix invalide.\n" RESET);
        }
        if (choix != 4) {
            printf("Appuyez sur une touche pour continuer...");
            getchar(); getchar();
        }
    } while (choix != 4);
}
