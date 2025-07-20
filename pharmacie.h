// ===== pharmacie.h (corrigé) =====
#ifndef PHARMACIE_H
#define PHARMACIE_H

#include <stdio.h>

// ===== ENUM ROLES =====
typedef enum {ADMIN, PHARMACIEN} Role;

// ===== STRUCTURES =====
typedef struct {
    char code[10];
    char designation[30];
    int quantite;
    float prix;
    char categorie_Produit[30];
    char date_peremption[15];
} Produit;

typedef struct {
    int id;
    char libelle[30];
} Categorie;

typedef struct {
    char login[30];
    char password[30];
    int actif; // 1 = actif, 0 = bloqué
    Role role;
    char nom[30];
    char prenom[30];
    char telephone[20];
} Utilisateur;

typedef struct {
    int idVente;
    char codeProduit[10];
    int quantite;
    float prixTotal;
    char dateVente[20];
} Vente;


void menuProduits();
void menuCategories();
void menuUtilisateurs();
void menuVentesFactures();
void venteProduits(Utilisateur user);
void genererFacture();
void sauvegarderUtilisateurs();



// ===== FONCTIONS PRODUITS =====
void ajouterProduit();
void afficherProduits();
void recherchecode();
void modifierProduit();
void supprimerProduit();
void verification_stock();

// ===== FONCTIONS CATEGORIES =====
void ajouterCategorie();
void afficherCategories();
void modifierCategorie();
void supprimerCategorie();

// ===== FONCTIONS UTILISATEURS =====
void ajouterUtilisateur();
void creerPharmacien();
int connexionUtilisateur(Utilisateur *user);
void afficherUtilisateurs();
void bloquerUtilisateur();
void debloquerUtilisateur();
void changerMotDePasse(Utilisateur *user);

void menuAdmin(Utilisateur user);
void menuPharmacien(Utilisateur user);
void infoUtilisateurConnecte(Utilisateur *user);

// ===== FONCTIONS VENTES ET FACTURES =====
void enregistrerVente(Utilisateur user);
void afficherFactures();
void afficherFactureParDate();
void etatJournalier();

#endif
