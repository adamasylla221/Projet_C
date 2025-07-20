// ===== pharmacie.c =====

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h> // pour mkdir sous Linux/Mac
#include <direct.h>   // pour mkdir sous Windows
 // mkdir sous Linux
#include "pharmacie.h"
#include <dirent.h>  // Pour opendir, readdir, closedir
#include <ctype.h>   // Pour isalnum

// ANSI couleurs
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define RESET   "\033[0m"

#define MAX_UTILISATEURS 100
#define MAX_PRODUITS_VENTE 50

// Variables globales utilisateurs (en mémoire)
Utilisateur utilisateurs[MAX_UTILISATEURS];
int nbUtilisateurs = 0;

// Fonctions utilitaires

void creerDossierBills() {
#ifdef _WIN32
    _mkdir("BILLS");
#else
    mkdir("BILLS", 0777);
#endif
}

// Validation code produit 5 caractères alphanumériques
int validerCodeProduit(const char *code) {
    if (strlen(code) != 5) return 0;
    for (int i = 0; i < 5; i++) {
        if (!((code[i] >= 'A' && code[i] <= 'Z') || (code[i] >= '0' && code[i] <= '9')))
            return 0;
    }
    return 1;
}

// Validation login 5 lettres majuscules
int validerLogin(const char *login) {
    if (strlen(login) != 5) return 0;
    for (int i = 0; i < 5; i++) {
        if (!(login[i] >= 'A' && login[i] <= 'Z'))
            return 0;
    }
    return 1;
}

// Validation date AAAA-MM-JJ simple (longueur + chiffres + tirets)
int validerDate(const char *date) {
    if (strlen(date) != 10) return 0;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') return 0;
        } else {
            if (!(date[i] >= '0' && date[i] <= '9')) return 0;
        }
    }
    // TODO: Valider que date est >= date actuelle
    return 1;
}

// ===== PRODUITS =====

void ajouterProduit() {
    FILE *f = fopen("PRODUCTS.dat", "ab");
    if (!f) {
        printf(RED "[ERREUR] Impossible d'ouvrir PRODUCTS.dat.\n" RESET);
        return;
    }
    Produit p;
    do {
        printf("Code produit (5 caractères alphanumériques): ");
        scanf("%5s", p.code);
    } while (!validerCodeProduit(p.code));

    printf("Désignation : ");
    scanf(" %49[^\n]", p.designation);

    printf("Quantité : ");
    scanf("%d", &p.quantite);

    printf("Prix : ");
    scanf("%f", &p.prix);

    printf("Catégorie : ");
    scanf(" %29[^\n]", p.categorie_Produit);

    do {
        printf("Date péremption (AAAA-MM-JJ) : ");
        scanf("%10s", p.date_peremption);
    } while (!validerDate(p.date_peremption));

    fwrite(&p, sizeof(Produit), 1, f);
    fclose(f);

    printf(GREEN "[✓] Produit ajouté avec succès.\n" RESET);
}

void afficherProduits() {
    FILE *f = fopen("PRODUCTS.dat", "rb");
    if (!f) {
        printf(RED "[ERREUR] Impossible d'ouvrir PRODUCTS.dat.\n" RESET);
        return;
    }
    Produit p;
    printf(BLUE "\n=== Liste des Produits ===\n" RESET);
    while (fread(&p, sizeof(Produit), 1, f)) {
        printf("Code: %s | Désignation: %s | Qté: %d | Prix: %.2f | Catégorie: %s | Péremption: %s\n",
               p.code, p.designation, p.quantite, p.prix, p.categorie_Produit, p.date_peremption);
    }
    fclose(f);
}

Produit* rechercherProduitParCode(const char *code) {
    static Produit p;
    FILE *f = fopen("PRODUCTS.dat", "rb");
    if (!f) return NULL;
    while (fread(&p, sizeof(Produit), 1, f)) {
        if (strcmp(p.code, code) == 0) {
            fclose(f);
            return &p;
        }
    }
    fclose(f);
    return NULL;
}

void modifierProduit() {
    FILE *f = fopen("PRODUCTS.dat", "rb+");
    if (!f) {
        printf(RED "[ERREUR] Impossible d'ouvrir PRODUCTS.dat.\n" RESET);
        return;
    }
    char code[6];
    printf("Code du produit à modifier : ");
    scanf("%5s", code);

    Produit p;
    int found = 0;
    while (fread(&p, sizeof(Produit), 1, f)) {
        if (strcmp(p.code, code) == 0) {
            found = 1;
            fseek(f, -sizeof(Produit), SEEK_CUR);

            printf("Nouvelle désignation : ");
            scanf(" %49[^\n]", p.designation);

            printf("Nouvelle quantité : ");
            scanf("%d", &p.quantite);

            printf("Nouveau prix : ");
            scanf("%f", &p.prix);

            printf("Nouvelle catégorie : ");
            scanf(" %29[^\n]", p.categorie_Produit);

            do {
                printf("Nouvelle date péremption (AAAA-MM-JJ) : ");
                scanf("%10s", p.date_peremption);
            } while (!validerDate(p.date_peremption));

            fwrite(&p, sizeof(Produit), 1, f);
            printf(GREEN "[✓] Produit modifié.\n" RESET);
            break;
        }
    }
    if (!found) printf(RED "[ERREUR] Produit non trouvé.\n" RESET);
    fclose(f);
}

void supprimerProduit() {
    FILE *f = fopen("PRODUCTS.dat", "rb");
    FILE *tmp = fopen("TEMP.dat", "wb");
    if (!f || !tmp) {
        printf(RED "[ERREUR] Impossible d'ouvrir fichier.\n" RESET);
        if(f) fclose(f);
        if(tmp) fclose(tmp);
        return;
    }
    char code[6];
    printf("Code du produit à supprimer : ");
    scanf("%5s", code);

    Produit p;
    int found = 0;
    while (fread(&p, sizeof(Produit), 1, f)) {
        if (strcmp(p.code, code) != 0) {
            fwrite(&p, sizeof(Produit), 1, tmp);
        } else {
            found = 1;
        }
    }
    fclose(f);
    fclose(tmp);

    remove("PRODUCTS.dat");
    rename("TEMP.dat", "PRODUCTS.dat");

    if (found) printf(GREEN "[✓] Produit supprimé.\n" RESET);
    else printf(RED "[ERREUR] Produit non trouvé.\n" RESET);
}

void verification_stock() {
    FILE *f = fopen("PRODUCTS.dat", "rb");
    if (!f) {
        printf(RED "[ERREUR] Impossible d'ouvrir PRODUCTS.dat.\n" RESET);
        return;
    }
    Produit p;
    int seuil = 5;
    printf(YELLOW "\n=== Vérification du stock (seuil %d) ===\n" RESET, seuil);
    while (fread(&p, sizeof(Produit), 1, f)) {
        if (p.quantite < seuil) {
            printf(RED "Alerte: Stock faible pour %s (%s) - Qté: %d\n" RESET, p.designation, p.code, p.quantite);
        }
    }
    fclose(f);
}

// ===== GESTION CATEGORIES =====
void ajouterCategorie() {
    FILE *f = fopen("CATEGORIES.dat", "ab");
    Categorie c;
    printf("\nID catégorie : "); scanf("%d", &c.id);
    printf("Libellé : "); scanf("%s", c.libelle);
    fwrite(&c, sizeof(Categorie), 1, f);
    fclose(f);
    printf("Catégorie ajoutée.\n");
}

void afficherCategories() {
    FILE *f = fopen("CATEGORIES.dat", "rb");
    Categorie c;
    printf("\n=== Liste des catégories ===\n");
    while (fread(&c, sizeof(Categorie), 1, f)) {
        printf("ID: %d | Libellé: %s\n", c.id, c.libelle);
    }
    fclose(f);
}

void modifierCategorie() {
    FILE *f = fopen("CATEGORIES.dat", "rb+");
    Categorie c;
    int id, found = 0;
    printf("\nID catégorie à modifier : "); scanf("%d", &id);
    while (fread(&c, sizeof(Categorie), 1, f)) {
        if (c.id == id) {
            fseek(f, -sizeof(Categorie), SEEK_CUR);
            printf("Nouveau libellé : "); scanf("%s", c.libelle);
            fwrite(&c, sizeof(Categorie), 1, f);
            found = 1;
            break;
        }
    }
    fclose(f);
    if (!found) printf("Catégorie non trouvée.\n");
}

void supprimerCategorie() {
    FILE *f = fopen("CATEGORIES.dat", "rb"), *tmp = fopen("TMP.dat", "wb");
    Categorie c;
    int id, found = 0;
    printf("\nID à supprimer : "); scanf("%d", &id);
    while (fread(&c, sizeof(Categorie), 1, f)) {
        if (c.id != id) fwrite(&c, sizeof(Categorie), 1, tmp);
        else found = 1;
    }
    fclose(f); fclose(tmp);
    remove("CATEGORIES.dat");
    rename("TMP.dat", "CATEGORIES.dat");
    if (found) printf("Catégorie supprimée.\n");
    else printf("Catégorie non trouvée.\n");
}


// ===== UTILISATEURS =====

void ajouterUtilisateur() {
    Utilisateur u;
    printf("\n=== Ajouter un utilisateur ===\n");
    printf("Login : ");
    scanf("%s", u.login);
    printf("Mot de passe : ");
    scanf("%s", u.password);
    printf("Nom : ");
    scanf("%s", u.nom);
    printf("Prénom : ");
    scanf("%s", u.prenom);
    printf("Téléphone : ");
    scanf("%s", u.telephone);

    int choix;
    do {
        printf("Rôle (1 = ADMIN, 2 = PHARMACIEN) : ");
        scanf("%d", &choix);
    } while (choix != 1 && choix != 2);
    u.role = (choix == 1) ? ADMIN : PHARMACIEN;
    u.actif = 1;

    FILE *f = fopen("USERS.dat", "ab");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier USERS.dat\n");
        return;
    }

    fwrite(&u, sizeof(Utilisateur), 1, f);
    fclose(f);
    printf("Utilisateur ajouté avec succès !\n");

}
void creerPharmacien() {
    Utilisateur u;

    printf("\n=== Création d'un compte pharmacien ===\n");

    printf("Nom complet : ");
    getchar(); // nettoyage du buffer
    fgets(u.nom, sizeof(u.nom), stdin);
    u.nom[strcspn(u.nom, "\n")] = '\0'; // enlever le \n

    printf("Login : ");
    scanf("%s", u.login);

    strcpy(u.password, "pharma123"); // mot de passe par défaut
    u.role = PHARMACIEN;
    u.actif = 1;

    FILE *f = fopen("USERS.dat", "ab");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier USERS.dat\n");
        return;
    }

    fwrite(&u, sizeof(Utilisateur), 1, f);
    fclose(f);

    printf("\033[0;32mPharmacien créé avec succès\033[0m (mot de passe = pharma123).\n");
}


void afficherUtilisateurs() {
    printf(BLUE "\n=== Liste des utilisateurs ===\n" RESET);
    for (int i = 0; i < nbUtilisateurs; i++) {
        printf("Login: %s | Nom: %s %s | Téléphone: %s | Rôle: %s | Actif: %s\n",
            utilisateurs[i].login,
            utilisateurs[i].prenom,
            utilisateurs[i].nom,
            utilisateurs[i].telephone,
            utilisateurs[i].role == ADMIN ? "Admin" : "Pharmacien",
            utilisateurs[i].actif ? "Oui" : "Non");
    }
}

void chargerUtilisateurs() {
    FILE *f = fopen("USERS.dat", "rb");
    if (!f) return;
    nbUtilisateurs = fread(utilisateurs, sizeof(Utilisateur), MAX_UTILISATEURS, f);
    fclose(f);
}

void sauvegarderUtilisateurs() {
    FILE *f = fopen("USERS.dat", "wb");
    if (!f) return;
    fwrite(utilisateurs, sizeof(Utilisateur), nbUtilisateurs, f);
    fclose(f);
}

void bloquerUtilisateur() {
    char login[6];
    printf("Login à bloquer : "); scanf("%5s", login);
    for (int i = 0; i < nbUtilisateurs; i++) {
        if (strcmp(utilisateurs[i].login, login) == 0) {
            utilisateurs[i].actif = 0;
            sauvegarderUtilisateurs();
            printf("[✓] Utilisateur bloqué.\n"); return;
        }
    }
    printf("[!] Utilisateur introuvable.\n");
}

void debloquerUtilisateur() {
    char login[6];
    printf("Login à débloquer : "); scanf("%5s", login);
    for (int i = 0; i < nbUtilisateurs; i++) {
        if (strcmp(utilisateurs[i].login, login) == 0) {
            utilisateurs[i].actif = 1;
            sauvegarderUtilisateurs();
            printf("[✓] Utilisateur débloqué.\n"); return;
        }
    }
    printf("[!] Utilisateur introuvable.\n");
}

void verifierEtCreerAdmin() {
    FILE *f = fopen("USERS.dat", "rb");
    Utilisateur user;
    int adminExiste = 0;

    if (f != NULL) {
        while (fread(&user, sizeof(Utilisateur), 1, f)) {
            if (strcmp(user.login, "admin") == 0) {
                adminExiste = 1;
                break;
            }
        }
        fclose(f);
    }

    if (!adminExiste) {
        f = fopen("USERS.dat", "ab");
        if (f == NULL) {
            printf("Erreur lors de la création du fichier d'utilisateurs.\n");
            exit(1);
        }
        Utilisateur admin = {
          //  .id = 1,
            .role = ADMIN,
          //  .statut = 1
        };
        strcpy(admin.nom, "Admin");
        strcpy(admin.login, "admin");
        strcpy(admin.password, "admin123"); // mot de passe par défaut

        fwrite(&admin, sizeof(Utilisateur), 1, f);
        fclose(f);
        printf("Compte administrateur par défaut créé (login: admin, mot de passe: admin123).\n");
    }
}


void changerMotDePasse(Utilisateur *user) {
      char ancien[20], nouveau[20];
      printf("Ancien mot de passe : "); scanf("%19s", ancien);
      if (strcmp(ancien, user->password) != 0) {
          printf("[!] Mot de passe incorrect.\n"); return;
      }
      printf("Nouveau mot de passe : "); scanf("%19s", nouveau);
      strcpy(user->password, nouveau);
      for (int i = 0; i < nbUtilisateurs; i++) {
          if (strcmp(utilisateurs[i].login, user->login) == 0) {
              strcpy(utilisateurs[i].password, nouveau);
              break;
          }
      }
      sauvegarderUtilisateurs();
      printf("[✓] Mot de passe changé.\n");
  }


int connexionUtilisateur(Utilisateur *user) {
    char login[30], password[30];
    printf("\n=== CONNEXION ===\n");
    printf("Login : ");
    scanf("%29s", login);
    printf("Mot de passe : ");
    scanf("%29s", password);

    FILE *f = fopen("USERS.dat", "rb");
    if (!f) {
        printf("Erreur ouverture fichier utilisateurs.\n");
        return 0;
    }

    Utilisateur u;
    while (fread(&u, sizeof(Utilisateur), 1, f)) {
        if (strcmp(u.login, login) == 0 && strcmp(u.password, password) == 0 && u.actif) {
            *user = u;
            fclose(f);
            return 1;  // connexion réussie
        }
    }

    fclose(f);
    return 0; // connexion échouée
}


// ===== VENTES & FACTURES =====

typedef struct {
    Produit produits[MAX_PRODUITS_VENTE];
    int quantites[MAX_PRODUITS_VENTE];
    int nbProduits;
    float total;
    char numVente[15]; // AAAAMMDDHHmmSS
    char dateHeure[20];
    char loginPharmacien[6];
} VenteComplet;

void genererNumVente(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(buffer, size, "%04d%02d%02d%02d%02d%02d",
        t->tm_year + 1900, t->tm_mon +1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
}

void venteProduits(Utilisateur userConnecte) {
    FILE *fprod = fopen("PRODUCTS.dat", "rb+");
    if (!fprod) {
        printf(RED "[ERREUR] Impossible d'ouvrir PRODUCTS.dat.\n" RESET);
        return;
    }

    VenteComplet vente;
    vente.nbProduits = 0;
    vente.total = 0.0f;
    strcpy(vente.loginPharmacien, userConnecte.login);
    genererNumVente(vente.numVente, sizeof(vente.numVente));
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(vente.dateHeure, sizeof(vente.dateHeure), "%02d-%02d-%04d %02d:%02d:%02d",
        t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min, t->tm_sec);

    int continuer = 1;
    while (continuer && vente.nbProduits < MAX_PRODUITS_VENTE) {
        char code[6];
        printf("Code produit à vendre (5 caractères) : ");
        scanf("%5s", code);

        // Recherche produit
        Produit p;
        int trouve = 0;

        rewind(fprod);
        while (fread(&p, sizeof(Produit), 1, fprod)) {
            if (strcmp(p.code, code) == 0) {
                trouve = 1;
                if (p.quantite <= 0) {
                    printf(RED "Stock insuffisant pour ce produit.\n" RESET);
                    break;
                }

                int qte;
                printf("Quantité à vendre : ");
                scanf("%d", &qte);
                if (qte <= 0 || qte > p.quantite) {
                    printf(RED "Quantité invalide ou insuffisante.\n" RESET);
                    break;
                }

                // Ajouter à la vente
                vente.produits[vente.nbProduits] = p;
                vente.quantites[vente.nbProduits] = qte;
                vente.nbProduits++;
                vente.total += p.prix * qte;

                // Mettre à jour stock
                p.quantite -= qte;
                fseek(fprod, -sizeof(Produit), SEEK_CUR);
                fwrite(&p, sizeof(Produit), 1, fprod);
                fflush(fprod);

                printf(GREEN "[✓] Produit ajouté à la vente.\n" RESET);
                break;
            }
        }
        if (!trouve) printf(RED "Produit non trouvé.\n" RESET);

        // Continuer ?
        char rep;
        printf("Ajouter un autre produit ? (O/N) : ");
        scanf(" %c", &rep);
        if (rep != 'O' && rep != 'o') continuer = 0;
    }
    fclose(fprod);

    // Générer facture
    creerDossierBills();
    char nomFichier[100];
    snprintf(nomFichier, sizeof(nomFichier), "BILLS/RECU_%s_%s.txt", vente.numVente, vente.loginPharmacien);

    FILE *facture = fopen(nomFichier, "w");
    if (!facture) {
        printf(RED "[ERREUR] Impossible de créer la facture.\n" RESET);
        return;
    }

    fprintf(facture, "════════════════════════════════════════════\n");
    fprintf(facture, "           PHARMACIE SUNUPHARMA              \n");
    fprintf(facture, "════════════════════════════════════════════\n");
    fprintf(facture, "Numero Facture : %s\n", vente.numVente);
    fprintf(facture, "Date           : %s\n", vente.dateHeure);
    fprintf(facture, "Pharmacien     : %s\n", vente.loginPharmacien);
    fprintf(facture, "--------------------------------------------\n");
    fprintf(facture, " Code   | Produit           | Qte | Total    \n");
    fprintf(facture, "--------------------------------------------\n");
    for (int i = 0; i < vente.nbProduits; i++) {
        float totalProduit = vente.produits[i].prix * vente.quantites[i];
        fprintf(facture, " %-6s | %-17s | %-3d | %-8.2f\n",
            vente.produits[i].code,
            vente.produits[i].designation,
            vente.quantites[i],
            totalProduit);
    }
    fprintf(facture, "--------------------------------------------\n");
    fprintf(facture, "TOTAL A PAYER : %.2f FCFA\n", vente.total);
    fprintf(facture, "════════════════════════════════════════════\n");

    fclose(facture);

    printf(GREEN "[✓] Vente enregistrée et facture generee : %s\n" RESET, nomFichier);
}

// Fonction état journalier : lire les factures du jour, compter total ventes et produits vendus
void etatJournalier(const char *date) {
   // DIR *rep;
    struct dirent *entry;
    struct dirent *rep;

    FILE *f;
    char path[100];
    char ligne[200];
    char today[11];
    float totalJour = 0.0;
    int totalProduits = 0, totalFactures = 0;

    // Obtenir la date du jour au format AAAA-MM-JJ
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(today, sizeof(today), "%Y-%m-%d", tm);

    rep = opendir("BILLS");
    if (!rep) {
        perror("Erreur d'ouverture du dossier BILLS");
        return;
    }

    while ((entry = readdir(rep)) != NULL) {
        // On ne garde que les fichiers FACTURE_*.txt
        if (strncmp(entry->d_name, "FACTURE_", 8) == 0 && strstr(entry->d_name, ".txt")) {
            // Construit le chemin complet
            snprintf(path, sizeof(path), "BILLS/%s", entry->d_name);

            f = fopen(path, "r");
            if (!f) continue;

            char dateFacture[11] = "";
            float totalFacture = 0.0;
            int produitsFacture = 0;

            // Lire le fichier ligne par ligne
            while (fgets(ligne, sizeof(ligne), f)) {
                // Trouver la ligne contenant "Date :"
                if (strncmp(ligne, "Date :", 6) == 0) {
                    sscanf(ligne, "Date : %10s", dateFacture);
                }

                // Trouver la ligne contenant "TOTAL A PAYER"
                if (strstr(ligne, "TOTAL A PAYER") != NULL) {
                    sscanf(ligne, "TOTAL A PAYER : %f", &totalFacture);
                }

                // Compter les lignes contenant un produit (début par espace + code)
                if (ligne[0] == ' ' && strlen(ligne) > 20 && isalnum(ligne[1])) {
                    produitsFacture++;
                }
            }

            fclose(f);

            // Si la facture est d'aujourd'hui, on l'inclut dans le bilan
            if (strcmp(today, dateFacture) == 0) {
                totalFactures++;
                totalProduits += produitsFacture;
                totalJour += totalFacture;
            }
        }
    }

    closedir(rep);

    printf(GREEN "\n===== ETAT JOURNALIER : %s =====\n" RESET, today);
    printf("Nombre de factures   : %d\n", totalFactures);
    printf("Produits vendus      : %d\n", totalProduits);
    printf("Montant total du jour: %.2f FCFA\n", totalJour);
}




