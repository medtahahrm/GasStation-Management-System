#include <iostream>
#include <string>
#include "include/Client.hpp"
#include "include/Admin.hpp"
#include "include/Stock.hpp"
#include "include/Facture.hpp"
#include "include/AnalyseVentes.hpp"

void menuFacturation() {
    std::cout << "\n=== SYSTEME DE FACTURATION ===" << std::endl;
    std::cout << "1. Creer nouvelle facture" << std::endl;
    std::cout << "2. Generer QR Code pour facture existante" << std::endl;
    std::cout << "0. Retour" << std::endl;
    std::cout << "Choix : ";
}

void creerNouvelleFacture() {
    std::string nomClient;
    std::cout << "\n--- NOUVELLE FACTURE ---" << std::endl;
    std::cout << "Nom du client : ";
    std::cin.ignore();
    std::getline(std::cin, nomClient);
    
    Facture facture(nomClient);
    
    int choixArticle = 1;
    while (choixArticle != 0) {
        std::cout << "\n1. Ajouter article" << std::endl;
        std::cout << "2. Generer facture" << std::endl;
        std::cout << "3. Afficher facture" << std::endl;
        std::cout << "4. Generer QR Code" << std::endl;
        std::cout << "0. Annuler" << std::endl;
        std::cout << "Choix : ";
        std::cin >> choixArticle;
        
        switch (choixArticle) {
            case 1: {
                std::string produit;
                double prix, quantite;
                
                std::cout << "Nom du produit : ";
                std::cin.ignore();
                std::getline(std::cin, produit);
                std::cout << "Prix unitaire (MAD) : ";
                std::cin >> prix;
                std::cout << "Quantite : ";
                std::cin >> quantite;
                
                facture.ajouterArticle(produit, prix, quantite);
                std::cout << "Article ajoute ! Sous-total : " << (prix * quantite) << " MAD" << std::endl;
                break;
            }
            case 2:
                facture.genererFacture();
                std::cout << "Facture generee avec succes !" << std::endl;
                break;
            case 3:
                facture.afficherFacture();
                break;
            case 4:
                facture.genererQRCode();
                break;
            case 0:
                std::cout << "Facture annulee." << std::endl;
                break;
            default:
                std::cout << "Choix invalide." << std::endl;
        }
    }
}

void genererQRCodeFactureExistante() {
    std::string numero, client, date;
    double montant;
    
    std::cout << "\n--- QR CODE FACTURE EXISTANTE ---" << std::endl;
    std::cout << "Numero de facture : ";
    std::cin.ignore();
    std::getline(std::cin, numero);
    std::cout << "Nom du client : ";
    std::getline(std::cin, client);
    std::cout << "Date (YYYY-MM-DD) : ";
    std::getline(std::cin, date);
    std::cout << "Montant total (MAD) : ";
    std::cin >> montant;
    
    Facture factureTemporaire(client);
    factureTemporaire.genererQRCodePourFactureExistante(numero, client, montant, date);
}

void menuFidelisationClient(Client& client) {
    int choixFidelite = -1;
    while (choixFidelite != 0) {
        std::cout << "\n" << std::string(40, '=') << std::endl;
        std::cout << "    PROGRAMME DE FIDELISATION" << std::endl;
        std::cout << std::string(40, '=') << std::endl;
        std::cout << "1. Mes badges et points" << std::endl;
        std::cout << "2. Historique des depenses" << std::endl;
        std::cout << "3. Avantages selon mon niveau" << std::endl;
        std::cout << "4. Prochain palier a atteindre" << std::endl;
        std::cout << "0. Retour" << std::endl;
        std::cout << "Choix : ";
        std::cin >> choixFidelite;
        
        switch (choixFidelite) {
            case 1:
                client.afficherBadges();
                break;
            case 2: {
                double total = client.getTotalDepense();
                int points = client.getPoints();
                std::cout << "\n--- VOTRE HISTORIQUE FIDELITE ---" << std::endl;
                std::cout << "Total depense : " << total << " MAD" << std::endl;
                std::cout << "Points accumules : " << points << " points" << std::endl;
                std::cout << "Valeur des points : " << (points * 0.1) << " MAD (0.1 MAD/point)" << std::endl;
                break;
            }
            case 3: {
                std::cout << "\n--- AVANTAGES PAR BADGE ---" << std::endl;
                std::cout << "Debutant (100+ points):" << std::endl;
                std::cout << "   - 1%* de remise sur le carburant" << std::endl;
                std::cout << "   - Service prioritaire" << std::endl;
                std::cout << "Regulier (500+ points):" << std::endl;
                std::cout << "   - 3%* de remise sur le carburant" << std::endl;
                std::cout << "   - Nettoyage vitres offert" << std::endl;
                std::cout << "Expert (1000+ points):" << std::endl;
                std::cout << "   - 5%* de remise sur le carburant" << std::endl;
                std::cout << "   - Verification pression offerte" << std::endl;
                std::cout << "VIP (2000+ points):" << std::endl;
                std::cout << "   - 7%* de remise sur le carburant" << std::endl;
                std::cout << "   - Stationnement reserve" << std::endl;
                std::cout << "LEGENDE (5000+ points):" << std::endl;
                std::cout << "   - 10%* de remise permanente" << std::endl;
                std::cout << "   - Service VIP personnalise" << std::endl;
                break;
            }
            case 4: {
                int points = client.getPoints();
                std::cout << "\n--- PROGRESSION ---" << std::endl;
                std::cout << "Points actuels : " << points << std::endl;
                
                if (points < 100) {
                    std::cout << "Prochain badge : Debutant" << std::endl;
                    std::cout << "Points manquants : " << (100 - points) << std::endl;
                    std::cout << "Depense necessaire : " << (100 - points) << " MAD" << std::endl;
                } else if (points < 500) {
                    std::cout << "Prochain badge : Regulier" << std::endl;
                    std::cout << "Points manquants : " << (500 - points) << std::endl;
                    std::cout << "Depense necessaire : " << (500 - points) << " MAD" << std::endl;
                } else if (points < 1000) {
                    std::cout << "Prochain badge : Expert" << std::endl;
                    std::cout << "Points manquants : " << (1000 - points) << std::endl;
                    std::cout << "Depense necessaire : " << (1000 - points) << " MAD" << std::endl;
                } else if (points < 2000) {
                    std::cout << "Prochain badge : VIP" << std::endl;
                    std::cout << "Points manquants : " << (2000 - points) << std::endl;
                    std::cout << "Depense necessaire : " << (2000 - points) << " MAD" << std::endl;
                } else if (points < 5000) {
                    std::cout << "Prochain badge : LEGENDE" << std::endl;
                    std::cout << "Points manquants : " << (5000 - points) << std::endl;
                    std::cout << "Depense necessaire : " << (5000 - points) << " MAD" << std::endl;
                } else {
                    std::cout << "Vous avez atteint le niveau maximum !" << std::endl;
                }
                break;
            }
            case 0:
                std::cout << "Retour au menu client..." << std::endl;
                break;
            default:
                std::cout << "Choix invalide." << std::endl;
        }
    }
}

void menuGestionClients(Admin& admin) {
    int choixGestion = -1;
    while (choixGestion != 0) {
        std::cout << "\n" << std::string(40, '=') << std::endl;
        std::cout << "    GESTION DES CLIENTS" << std::endl;
        std::cout << std::string(40, '=') << std::endl;
        std::cout << "1. Voir tous les clients" << std::endl;
        std::cout << "2. Rechercher un client" << std::endl;
        std::cout << "3. Voir details client" << std::endl;
        std::cout << "4. Supprimer un client" << std::endl;
        std::cout << "5. Statistiques clients" << std::endl;
        std::cout << "0. Retour" << std::endl;
        std::cout << "Choix : ";
        std::cin >> choixGestion;
        
        switch (choixGestion) {
            case 1:
                admin.voirTousClients();
                break;
            case 2:
                admin.rechercherClient();
                break;
            case 3:
                admin.voirDetailsClient();
                break;
            case 4:
                admin.supprimerClient();
                break;
            case 5:
                admin.afficherStatistiquesGlobales();
                break;
            case 0:
                std::cout << "Retour au menu admin..." << std::endl;
                break;
            default:
                std::cout << "Choix invalide." << std::endl;
        }
    }
}

void menuGestionStock(Stock& stock) {
    int choixStock = -1;
    while (choixStock != 0) {
        std::cout << "\n" << std::string(40, '=') << std::endl;
        std::cout << "    GESTION DES STOCKS" << std::endl;
        std::cout << std::string(40, '=') << std::endl;
        std::cout << "1. Afficher le stock" << std::endl;
        std::cout << "2. Ajouter du stock" << std::endl;
        std::cout << "3. Verifier alertes stock" << std::endl;
        std::cout << "4. Modifier seuils d'alerte" << std::endl;
        std::cout << "0. Retour" << std::endl;
        std::cout << "Choix : ";
        std::cin >> choixStock;
        
        switch (choixStock) {
            case 1:
                stock.afficherStock();
                break;
            case 2: {
                std::string produit;
                int quantite;
                std::cout << "Produit a ajouter (Essence/Diesel/GPL/Huile/Lave_glace) : ";
                std::cin >> produit;
                std::cout << "Quantite : ";
                std::cin >> quantite;
                stock.ajouterProduit(produit, quantite);
                break;
            }
            case 3:
                stock.verifierStockFaible();
                break;
            case 4: {
                std::string produit;
                int nouveauSeuil;
                std::cout << "Produit : ";
                std::cin >> produit;
                std::cout << "Nouveau seuil d'alerte : ";
                std::cin >> nouveauSeuil;
                stock.setSeuilAlerte(produit, nouveauSeuil);
                break;
            }
            case 0:
                std::cout << "Retour au menu admin..." << std::endl;
                break;
            default:
                std::cout << "Choix invalide." << std::endl;
        }
    }
}

void ajouterConsommationClient(Client& client, Stock& stock, Facture& factureClient) {
    double km, litres;
    std::string typeCarburant;
    
    std::cout << "\n--- AJOUTER CONSOMMATION ---" << std::endl;
    std::cout << "Kilometrage actuel : ";
    std::cin >> km;
    std::cout << "Quantite (L) : ";
    std::cin >> litres;
    std::cout << "Type de carburant (Essence/Diesel/GPL) : ";
    std::cin >> typeCarburant;
    
    // Calcul du prix selon le type de carburant
    double prixLitre = 0;
    if (typeCarburant == "Diesel") prixLitre = 10.0;
    else if (typeCarburant == "GPL") prixLitre = 7.0;
    else prixLitre = 12.0; // Essence par défaut
    
    double coutTotal = litres * prixLitre;
    
    std::cout << "\n" << std::string(40, '-') << std::endl;
    std::cout << "  RECAPITULATIF DE LA CONSOMMATION" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    std::cout << "Type carburant : " << typeCarburant << std::endl;
    std::cout << "Quantite : " << litres << " L" << std::endl;
    std::cout << "Prix au litre : " << prixLitre << " MAD" << std::endl;
    std::cout << "Cout total : " << coutTotal << " MAD" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    
    std::cout << "\nConfirmer l'ajout ? (o/n) : ";
    char confirmation;
    std::cin >> confirmation;
    
    if (confirmation == 'o' || confirmation == 'O') {
        // Ajouter la consommation SANS redemander le type
        client.ajouterConsommationAvecType(km, litres, typeCarburant);
        
        // Mettre à jour le stock
        stock.retirerProduit(typeCarburant, (int)litres);
        
        // Ajouter à la facture
        factureClient.ajouterArticle(typeCarburant + " (" + std::to_string((int)litres) + "L)", prixLitre, litres);
        
        std::cout << "\n Consommation enregistree avec succes !" << std::endl;
        std::cout << " " << (int)coutTotal << " points de fidelite ajoutes" << std::endl;
    } else {
        std::cout << "Operation annulee." << std::endl;
    }
}

int main() {
    // Créer les dossiers nécessaires
    #ifdef _WIN32
    system("mkdir historique_clients >nul 2>&1");
    system("mkdir factures >nul 2>&1");
    #else
    system("mkdir -p historique_clients");
    system("mkdir -p factures");
    #endif

    int choixPrincipal;
    
    do {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "       STATION SERVICE PREMIUM" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Espace Client" << std::endl;
        std::cout << "2. Espace Admin" << std::endl;
        std::cout << "3. Facturation & Paiement" << std::endl;
        std::cout << "0. Quitter" << std::endl;
        std::cout << "Choix : ";
        std::cin >> choixPrincipal;
        
        switch (choixPrincipal) {
            case 1: {
                Client client;
                client.saisirIdentite();  // Cette fonction gère maintenant l'inscription et la connexion
                
                Stock stock;
                Facture factureClient(client.getNom() + " " + client.getPrenom());
                int choixClient = -1;
                
                while (choixClient != 0) {
                    std::cout << "\n" << std::string(40, '=') << std::endl;
                    std::cout << "   MENU CLIENT - " << client.getPrenom() << " " << client.getNom() << std::endl;
                    std::cout << std::string(40, '=') << std::endl;
                    std::cout << "1. Ajouter consommation" << std::endl;
                    std::cout << "2. Afficher historique" << std::endl;
                    std::cout << "3. Programme de fidelisation" << std::endl;
                    std::cout << "4. Generer facture" << std::endl;
                    std::cout << "5. Paiement QR Code" << std::endl;
                    std::cout << "6. Mes informations" << std::endl;
                    std::cout << "0. Deconnexion" << std::endl;
                    std::cout << "Choix : ";
                    std::cin >> choixClient;
                    
                    switch (choixClient) {
                        case 1:
                            ajouterConsommationClient(client, stock, factureClient);
                            break;
                        case 2:
                            client.afficherHistorique();
                            break;
                        case 3:
                            menuFidelisationClient(client);
                            break;
                        case 4:
                            factureClient.afficherFacture();
                            std::cout << "\nVoulez-vous generer la facture ? (o/n) : ";
                            char confirmation;
                            std::cin >> confirmation;
                            if (confirmation == 'o' || confirmation == 'O') {
                                factureClient.genererFacture();
                            }
                            break;
                        case 5:
                            if (factureClient.getTotal() > 0) {
                                factureClient.genererQRCode();
                                
                                std::cout << "\nVoulez-vous reinitialiser la facture apres paiement ? (o/n) : ";
                                char reset;
                                std::cin >> reset;
                                if (reset == 'o' || reset == 'O') {
                                    // Recréer une nouvelle facture vide
                                    factureClient = Facture(client.getNom() + " " + client.getPrenom());
                                    std::cout << " Facture reinitialisee." << std::endl;
                                }
                            } else {
                                std::cout << " Aucun article dans la facture." << std::endl;
                            }
                            break;
                        case 6:
                            std::cout << "\n--- MES INFORMATIONS ---" << std::endl;
                            std::cout << "Nom : " << client.getNom() << std::endl;
                            std::cout << "Prenom : " << client.getPrenom() << std::endl;
                            std::cout << "Email : " << client.getEmail() << std::endl;
                            std::cout << "Telephone : " << client.getTelephone() << std::endl;
                            break;
                        case 0:
                            std::cout << " Deconnexion..." << std::endl;
                            break;
                        default:
                            std::cout << " Choix invalide." << std::endl;
                    }
                }
                break;
            }
            case 2: {
                Admin admin;
                if (admin.authentifierAdmin()) {
                    Stock stock;
                    AnalyseVentes analyse;
                    
                    int choixAdmin = -1;
                    while (choixAdmin != 0) {
                        std::cout << "\n" << std::string(40, '=') << std::endl;
                        std::cout << "       MENU ADMIN" << std::endl;
                        std::cout << std::string(40, '=') << std::endl;
                        std::cout << "1. Tableau de bord" << std::endl;
                        std::cout << "2. Gestion des clients" << std::endl;
                        std::cout << "3. Gestion des stocks" << std::endl;
                        std::cout << "4. Analyse des ventes" << std::endl;
                        std::cout << "5. Modifier prix carburants" << std::endl;
                        std::cout << "6. Voir toutes les factures" << std::endl;
                        std::cout << "0. Deconnexion" << std::endl;
                        std::cout << "Choix : ";
                        std::cin >> choixAdmin;
                        
                        switch (choixAdmin) {
                            case 1:
                                admin.afficherStatistiquesGlobales();
                                break;
                            case 2:
                                menuGestionClients(admin);
                                break;
                            case 3:
                                menuGestionStock(stock);
                                break;
                            case 4: {
                                int choixAnalyse;
                                std::cout << "\n1. Meilleurs clients" << std::endl;
                                std::cout << "2. Produits plus vendus" << std::endl;
                                std::cout << "3. Previsions ventes" << std::endl;
                                std::cout << "Choix : ";
                                std::cin >> choixAnalyse;
                                switch (choixAnalyse) {
                                    case 1: analyse.meilleursClients(); break;
                                    case 2: analyse.produitsPlusVendus(); break;
                                    case 3: analyse.previsionsVentes(); break;
                                }
                                break;
                            }
                            case 5:
                                admin.modifierPrixCarburants();
                                break;
                            case 6: {
                                std::cout << "\n--- LISTE DES FACTURES ---" << std::endl;
                                #ifdef _WIN32
                                system("dir factures /B");
                                #else
                                system("ls factures/");
                                #endif
                                break;
                            }
                            case 0:
                                std::cout << " Deconnexion admin..." << std::endl;
                                break;
                            default:
                                std::cout << " Choix invalide." << std::endl;
                        }
                    }
                } else {
                    std::cout << " Acces refuse." << std::endl;
                }
                break;
            }
            case 3: {
                int choixFacturation = -1;
                while (choixFacturation != 0) {
                    menuFacturation();
                    std::cin >> choixFacturation;
                    
                    switch (choixFacturation) {
                        case 1:
                            creerNouvelleFacture();
                            break;
                        case 2:
                            genererQRCodeFactureExistante();
                            break;
                        case 0:
                            std::cout << "Retour au menu principal..." << std::endl;
                            break;
                        default:
                            std::cout << "Choix invalide." << std::endl;
                    }
                }
                break;
            }
            case 0:
                std::cout << "\nMerci d'avoir utilise Station Service Premium !" << std::endl;
                std::cout << " Au revoir !" << std::endl;
                break;
            default:
                std::cout << "Choix invalide." << std::endl;
        }
    } while (choixPrincipal != 0);
    
    return 0;
}