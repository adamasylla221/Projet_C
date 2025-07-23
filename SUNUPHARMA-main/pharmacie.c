<<<<<<< HEAD
#include <stdio.h>
#include "pharmacie.h"
=======
<<<<<<< HEAD
=======
<<<<<<< HEAD
#include <stdio.h>
#include <string.h>
#include "pharmacie.h"
#include <stdlib.h>
#include "time.h"



void infoVente(Vente *V,Produit produits[],int nbProduitsDispo,const char *loginPharmacien) {
    int choix;
    V->nbProduits = 0;
    V->total = 0.0f;
   strncpy(V->loginPharmacien, loginPharmacien, sizeof(V->loginPharmacien) - 1);
   V->loginPharmacien[sizeof(V->loginPharmacien) - 1] = '\0';


    // Générer un numéro unique pour la vente AAAAMMJJHHMMSS
     time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(V->numero, "%04d%02d%02d%02d%02d%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    sprintf(V->date, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);


    do {
        char codeProduit[10];
        int quantiteDemandee;
        int trouve = 0;

        printf("\n\033[1;34m--- Liste des produits disponibles ---\033[0m\n");
        for (int i = 0; i < nbProduitsDispo; i++) {
            printf("[%s] %s | Prix: %.2f FCFA | Stock: %d\n",
                   produits[i].code, produits[i].designation,
                   produits[i].prix, produits[i].quantite);
        }

        printf("\nEntrez le code du produit : ");
        scanf("%s", codeProduit);

        for (int i = 0; i < nbProduitsDispo; i++) {
            if (strcmp(produits[i].code, codeProduit) == 0) {
                trouve = 1;
                printf("Quantite a vendre : ");
                scanf("%d", &quantiteDemandee);

                if (quantiteDemandee <= produits[i].quantite && quantiteDemandee > 0) {
                    // Ajouter le produit à la vente
                    V->produits[V->nbProduits] = produits[i];
                    V->quantites[V->nbProduits] = quantiteDemandee;
                    V->total += produits[i].prix * quantiteDemandee;

                    // Mettre à jour le stock
                    produits[i].quantite -= quantiteDemandee;

                    V->nbProduits++;
                    printf("\033[1;32m[✓] Vente enregistree en vert).\033[0m\n");
                } else {
                    printf("\033[1;31m[ERREUR] Stock insuffisant en rouge.\033[0m\n");
                }
                break;
            }
        }

        if (!trouve) {
            printf("\033[1;31m[ERREUR] Produit introuvable.\033[0m\n");
        }

        printf("Ajouter un autre produit ? (1=Oui / 0=Non) : ");
        scanf("%d", &choix);

    } while (choix == 1);

    printf("\n\033[1;32m[✓] Vente enregistree. Total : %.2f FCFA\033[0m\n", V->total);
    printf("Numero de vente : %s\n", V->numero);
}


void generfacture(Vente V) {
    #define FACTURE_PATH "./BILLS/"
    char nomFichier[100];
    snprintf(nomFichier, sizeof(nomFichier), "%sRECU_%s_%s.txt", FACTURE_PATH, V.numero, V.loginPharmacien);

    FILE *f = fopen(nomFichier, "w");
    if (f == NULL) {
        printf("\033[1;31m[ERREUR] Impossible de creer la facture !\033[0m\n");
        return;
    }

    fprintf(f, "════════════════════════════════════════════\n");
    fprintf(f, "           PHARMACIE SUNUPHARMA              \n");
    fprintf(f, "════════════════════════════════════════════\n");
    fprintf(f, "Numero Facture : %s\n", V.numero);
    fprintf(f, "Date           : %s\n", V.date);
    fprintf(f, "Pharmacien     : %s\n", V.loginPharmacien);
    fprintf(f, "--------------------------------------------\n");
    fprintf(f, " Code   | Produit           | Qte | Total    \n");
    fprintf(f, "--------------------------------------------\n");

    for (int i = 0; i < V.nbProduits; i++) {
        float totalProduit = V.produits[i].prix * V.quantites[i];
        fprintf(f, " %-6s | %-17s | %-3d | %-8.2f\n",
                V.produits[i].code,
                V.produits[i].designation,
                V.quantites[i],
                totalProduit);
    }

    fprintf(f, "--------------------------------------------\n");
    fprintf(f, "TOTAL A PAYER : %.2f FCFA\n", V.total);
    fprintf(f, "════════════════════════════════════════════\n");

    fclose(f);
    printf("\033[1;32m[✓] Facture generee : %s\033[0m\n", nomFichier);
}

void imprimfacture(Vente V) {
    generfacture(V);

}

#define SEUIL_STOCK_CRITIQUE 10

int dateEstExpire(const char *date_peremption) {
    int mois, annee;
    int date_expire_int, date_auj_int;

    // Lire la date au format MM/YYYY
    if (sscanf(date_peremption, "%2d/%4d", &mois, &annee) != 2) {
        printf("Format de date invalide : %s\n", date_peremption);
        return 0; // Considéré non périmé par défaut
    }

    // Représenter la date comme AAAAMM
    date_expire_int = annee * 100 + mois;

    // Date actuelle
    time_t t = time(NULL);
    struct tm today = *localtime(&t);
    date_auj_int = (today.tm_year + 1900) * 100 + (today.tm_mon + 1);

    // Si la date de péremption est plus petite que aujourd'hui
    return date_expire_int < date_auj_int;
}


void consultstock(Produit produits[],int nbProduits){
     printf("\n\033[1;33m--- Consultation du stock ---\033[0m\n");
    printf("Code   | Designation                | Stock | Date peremption | Etat\n");
    printf("---------------------------------------------------------------------\n");

    for (int i = 0; i < nbProduits; i++) {
        const char *etatStock = "";
        if (produits[i].quantite <= SEUIL_STOCK_CRITIQUE) {
            etatStock = "[Stock critique]";
        }

        const char *etatDate = "";
        if (dateEstExpire(produits[i].date_peremption)) {
            etatDate = "\033[1;31m[Perime]\033[0m";
        }

        printf("%-6s | %-25s | %-5d | %-15s | %s %s\n",
               produits[i].code,
               produits[i].designation,
               produits[i].quantite,
               produits[i].date_peremption,
               etatStock,
               etatDate);
    }
    printf("---------------------------------------------------------------------\n");
}

=======
<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define RESET   "\033[0m"
typedef struct {
    char code[10];
    char designation[50];
    char categorie_Produit[30];
    char date_peremption[11]; // format : MM/YYYY
    int quantite;
    float prix;
} Produit;

void initialiserFichier() {
    FILE *f = fopen("product.dat", "ab");
    if (f == NULL) {
        printf("Erreur lors de la création de product.dat\n");
        exit(1);
    }
    fclose(f);
}


void ajouterProduit() {
    Produit p;
    FILE *f = fopen("product.dat", "ab");
    if (!f) {
        printf("Erreur ouverture fichier.\n");
        return;
    }

    fflush(stdin);
    printf(MAGENTA"\n--- Ajout d'un produit ---\n"RESET);
    printf("Code : ");
    scanf("%s", p.code);
    fflush(stdin);

    printf("Désignation : ");
    fgets(p.designation, sizeof(p.designation), stdin);
    p.designation[strcspn(p.designation, "\n")] = 0;

    printf("Catégorie : ");
    fgets(p.categorie_Produit, sizeof(p.categorie_Produit), stdin);
    p.categorie_Produit[strcspn(p.categorie_Produit, "\n")] = 0;

    printf("Date de péremption (MM/YYYY) : ");
    scanf("%s", p.date_peremption);

    printf("Quantité : ");
    scanf("%d", &p.quantite);

    printf("Prix unitaire : ");
    scanf("%f", &p.prix);

    fwrite(&p, sizeof(Produit), 1, f);
    fclose(f);

    printf(GREEN "✅Produit ajouté avec succès.\n"RESET);
}

void afficherProduits() {
    Produit p;
    FILE *f = fopen("product.dat", "rb");
    if (!f) {
        printf("Erreur ouverture fichier.\n");
        return;
    }

    printf(MAGENTA"\n=== LISTE DES PRODUITS ===\n"RESET);
    while (fread(&p, sizeof(Produit), 1, f)) {
        printf("Code : %s\n", p.code);
        printf("Désignation : %s\n", p.designation);
        printf("Catégorie : %s\n", p.categorie_Produit);
        printf("Date de péremption : %s\n", p.date_peremption);
        printf("Quantité : %d\n", p.quantite);
        printf("Prix : %.2f XOF\n", p.prix);
        printf("-------------------------------\n");
    }

    fclose(f);
}

void modifierProduit() {
    char code[10];
    int trouve = 0;
    Produit p;

    printf(MAGENTA"\n--- Modifier un produit ---\n"RESET);
    printf("Code du produit à modifier : ");
    scanf("%s", code);

    FILE *f = fopen("product.dat", "rb+");
    if (!f) {
        printf("Erreur ouverture fichier.\n");
        return;
    }

    while (fread(&p, sizeof(Produit), 1, f)) {
        if (strcmp(p.code, code) == 0) {
            printf("➤ Nouveau prix : ");
            scanf("%f", &p.prix);
            printf("➤ Nouvelle quantité : ");
            scanf("%d", &p.quantite);
            printf("➤ Nouvelle date de péremption : ");
            scanf("%s", p.date_peremption);
            printf("➤ Nouvelle categorie: ");
            scanf("%s", p.categorie_Produit);
             printf("➤ Nouveau code: ");
            scanf("%s", p.code);
            printf("➤ Nouvelle designation: ");
            scanf("%s", p.designation);
            fseek(f, -sizeof(Produit), SEEK_CUR);
            fwrite(&p, sizeof(Produit), 1, f);
            trouve = 1;
            break;
        }
    }

    fclose(f);

    if (trouve)
        printf(GREEN "✅ Produit modifié avec succès.\n" RESET);
    else
        printf(RED "❌ Produit non trouvé.\n" RESET);
}

void supprimerProduit() {
    char code[10];
    Produit p;
    int trouve = 0;

    printf(MAGENTA "\n--- Supprimer un produit ---\n"RESET);
    printf("Code du produit à supprimer : ");
    scanf("%s", code);

    FILE *f = fopen("product.dat", "rb");
    FILE *tmp = fopen("temp.dat", "wb");

    if (!f || !tmp) {
        printf("Erreur fichiers.\n");
        return;
    }

    while (fread(&p, sizeof(Produit), 1, f)) {
        if (strcmp(p.code, code) != 0) {
            fwrite(&p, sizeof(Produit), 1, tmp);
        } else {
            trouve = 1;
        }
    }

    fclose(f);
    fclose(tmp);

    remove("product.dat");
    rename("temp.dat", "product.dat");

    if (trouve)
        printf( GREEN"✅ Produit supprimé avec succès.\n" RESET);
    else
        printf( RED "❌ Produit non trouvé.\n" RESET);
}

void verification_stock() {
    char designation[50];
    int qteDemandee;
    int trouve = 0;
    Produit p;

    FILE *f = fopen("product.dat", "rb");
    if (!f) {
        printf("Erreur ouverture fichier.\n");
        return;
    }

    fflush(stdin);
    printf( MAGENTA"\n--- Vérification du stock ---\n"RESET);
    printf("Désignation du produit : ");
    fgets(designation, sizeof(designation), stdin);
    designation[strcspn(designation, "\n")] = 0;

    printf("Quantité voulue : ");
    scanf("%d", &qteDemandee);

    while (fread(&p, sizeof(Produit), 1, f)) {
        if (strcmp(p.designation, designation) == 0) {
            trouve = 1;
            if (p.quantite >= qteDemandee) {
                printf(GREEN "✅ Stock suffisant (%d disponibles).\n"RESET , p.quantite);
            } else {
                printf(RED "⚠️ Stock insuffisant (seulement %d en stock).\n"RESET, p.quantite);
            }
            break;
        }
    }

    if (!trouve) {
        printf( RED "❌ Produit non trouvé.\n");
    }

    fclose(f);
}


=======
>>>>>>> f56135b11b1caa95f49a5faafca0ce56c2312c3c
>>>>>>> 9ed36685a89b46fa6444100bafdb869af933b4b9
>>>>>>> 8085a31d8f808adaf71d70fcb524ef10c8a6895b
>>>>>>> f4499a739c63b652d77124ccfcbc509a52472b2e

