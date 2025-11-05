#include "include/Admin.hpp"
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <sstream>  

Admin::Admin() : motDePasseAdmin("admin123") {}

bool Admin::authentifierAdmin() {
    std::string mdp;
    std::cout << "Mot de passe administrateur : ";
    std::cin >> mdp;
    return (mdp == motDePasseAdmin);
}

void Admin::afficherStatistiquesGlobales() {
    std::cout << "\n=== STATISTIQUES GLOBALES ===\n";
    
    DIR *dir;
    struct dirent *ent;
    int totalClients = 0;
    double chiffreAffairesTotal = 0.0;
    int totalConsommations = 0;
    
    if ((dir = opendir("historique_clients")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt")) {
                totalClients++;
                
                // Calculer le chiffre d'affaires par client
                std::string fichierClient = "historique_clients/" + std::string(ent->d_name);
                std::ifstream file(fichierClient.c_str());
                std::string ligne;
                
                while (std::getline(file, ligne)) {
                    if (ligne.find("PASSWORD:") == 0) continue;
                    
                    std::istringstream iss(ligne);  // ← MAINTENANT ÇA MARCHE
                    std::string date;
                    double km, litre, cout;
                    if (iss >> date >> km >> litre >> cout) {
                        chiffreAffairesTotal += cout;
                        totalConsommations++;
                    }
                }
                file.close();
            }
        }
        closedir(dir);
    }
    
    std::cout << " Nombre total de clients : " << totalClients << "\n";
    std::cout << " Chiffre d'affaires total : " << std::fixed << std::setprecision(2) << chiffreAffairesTotal << " MAD\n";
    std::cout << " Nombre total de consommations : " << totalConsommations << "\n";
    
    if (totalClients > 0) {
        std::cout << " Moyenne par client : " << (chiffreAffairesTotal / totalClients) << " MAD\n";
        std::cout << " Consommations moyennes : " << (totalConsommations / totalClients) << " par client\n";
    }
}

void Admin::modifierPrixCarburants() {
    std::cout << "\n=== MODIFICATION PRIX CARBURANTS ===\n";
    
    double prixEssence = 12.0, prixDiesel = 10.0, prixGPL = 7.0;
    
    std::ifstream fichierLecture("config_prix.txt");
    if (fichierLecture.is_open()) {
        std::string type;
        double prix;
        while (fichierLecture >> type >> prix) {
            if (type == "ESSENCE") prixEssence = prix;
            else if (type == "DIESEL") prixDiesel = prix;
            else if (type == "GPL") prixGPL = prix;
        }
        fichierLecture.close();
    }
    
    std::cout << "Prix actuels :\n";
    std::cout << "1. Essence : " << prixEssence << " MAD/L\n";
    std::cout << "2. Diesel  : " << prixDiesel << " MAD/L\n";
    std::cout << "3. GPL     : " << prixGPL << " MAD/L\n\n";
    
    int choixCarburant;
    std::cout << "Choix du carburant a modifier : ";
    std::cin >> choixCarburant;
    
    double nouveauPrix;
    std::cout << "Nouveau prix par litre : ";
    std::cin >> nouveauPrix;
    
    if (nouveauPrix <= 0) {
        std::cout << "Erreur : prix invalide.\n";
        return;
    }
    
    std::ofstream fichier("config_prix.txt");
    if (fichier.is_open()) {
        if (choixCarburant == 2) prixDiesel = nouveauPrix;
        else if (choixCarburant == 3) prixGPL = nouveauPrix;
        else prixEssence = nouveauPrix;
        
        fichier << "ESSENCE " << prixEssence << "\n";
        fichier << "DIESEL " << prixDiesel << "\n";
        fichier << "GPL " << prixGPL << "\n";
        
        fichier.close();
        std::cout << " Prix mis a jour avec succes !\n";
    } else {
        std::cout << " Erreur : impossible de sauvegarder.\n";
    }
}

void Admin::voirTousClients() {
    std::cout << "\n=== LISTE DES CLIENTS ===\n";
    
    DIR *dir;
    struct dirent *ent;
    int compteur = 0;
    
    if ((dir = opendir("historique_clients")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt")) {
                compteur++;
                std::cout << compteur << ". " << ent->d_name << "\n";
            }
        }
        closedir(dir);
        
        if (compteur == 0) {
            std::cout << "Aucun client trouve.\n";
        } else {
            std::cout << "\n Total : " << compteur << " clients\n";
        }
    } else {
        std::cout << " Erreur : dossier historique_clients introuvable.\n";
    }
}

// === NOUVELLES FONCTIONS ===

void Admin::supprimerClient() {
    std::cout << "\n=== SUPPRESSION D'UN CLIENT ===\n";
    
    DIR *dir;
    struct dirent *ent;
    std::vector<std::string> clients;
    int compteur = 0;
    
    if ((dir = opendir("historique_clients")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt")) {
                compteur++;
                clients.push_back(ent->d_name);
                std::cout << compteur << ". " << ent->d_name << "\n";
            }
        }
        closedir(dir);
    }
    
    if (clients.empty()) {
        std::cout << "Aucun client a supprimer.\n";
        return;
    }
    
    int choix;
    std::cout << "\nChoix du client a supprimer (0 pour annuler) : ";
    std::cin >> choix;
    
    // CORRECTION : convertir en unsigned int pour la comparaison
    if (choix > 0 && static_cast<size_t>(choix) <= clients.size()) {
        std::string fichierASupprimer = "historique_clients/" + clients[choix-1];
        
        std::cout << "  Confirmez la suppression de " << clients[choix-1] << " ? (o/n) : ";
        char confirmation;
        std::cin >> confirmation;
        
        if (confirmation == 'o' || confirmation == 'O') {
            if (std::remove(fichierASupprimer.c_str()) == 0) {
                std::cout << " Client supprime avec succes !\n";
            } else {
                std::cout << " Erreur lors de la suppression.\n";
            }
        } else {
            std::cout << " Suppression annulee.\n";
        }
    } else {
        std::cout << " Choix invalide.\n";
    }
}

void Admin::rechercherClient() {
    std::cout << "\n=== RECHERCHE DE CLIENT ===\n";
    
    std::string recherche;
    std::cout << "Entrez le nom ou prenom a rechercher : ";
    std::cin >> recherche;
    
    DIR *dir;
    struct dirent *ent;
    int resultats = 0;
    
    std::cout << "\n Resultats de la recherche :\n";
    
    if ((dir = opendir("historique_clients")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt")) {
                std::string nomFichier = ent->d_name;
                // Convertir en minuscules pour recherche insensible à la casse
                std::string nomMinuscule = nomFichier;
                std::transform(nomMinuscule.begin(), nomMinuscule.end(), nomMinuscule.begin(), ::tolower);
                std::string rechercheMinuscule = recherche;
                std::transform(rechercheMinuscule.begin(), rechercheMinuscule.end(), rechercheMinuscule.begin(), ::tolower);
                
                if (nomMinuscule.find(rechercheMinuscule) != std::string::npos) {
                    resultats++;
                    std::cout << " VERIFIE " << nomFichier << "\n";
                }
            }
        }
        closedir(dir);
    }
    
    if (resultats == 0) {
        std::cout << "Aucun client trouve pour : " << recherche << "\n";
    } else {
        std::cout << "\nSTATS " << resultats << " client(s) trouve(s)\n";
    }
}

void Admin::voirDetailsClient() {
    std::cout << "\n=== DETAILS D'UN CLIENT ===\n";
    
    DIR *dir;
    struct dirent *ent;
    std::vector<std::string> clients;
    int compteur = 0;
    
    if ((dir = opendir("historique_clients")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt")) {
                compteur++;
                clients.push_back(ent->d_name);
                std::cout << compteur << ". " << ent->d_name << "\n";
            }
        }
        closedir(dir);
    }
    
    if (clients.empty()) {
        std::cout << "Aucun client disponible.\n";
        return;
    }
    
    int choix;
    std::cout << "\nChoix du client : ";
    std::cin >> choix;
    
    // CORRECTION : convertir en unsigned int pour la comparaison
    if (choix > 0 && static_cast<size_t>(choix) <= clients.size()) {
        std::string fichierClient = "historique_clients/" + clients[choix-1];
        std::ifstream file(fichierClient.c_str());
        
        std::cout << "\n Fichier : " << clients[choix-1] << "\n";
        std::cout << "================================\n";
        
        std::string ligne;
        int nbConsommations = 0;
        double totalDepense = 0.0;
        
        while (std::getline(file, ligne)) {
            if (ligne.find("PASSWORD:") == 0) {
                std::cout << " Mot de passe : " << ligne.substr(9) << "\n";
                continue;
            }
            
            std::istringstream iss(ligne);  // ← MAINTENANT ÇA MARCHE
            std::string date;
            double km, litre, cout;
            if (iss >> date >> km >> litre >> cout) {
                nbConsommations++;
                totalDepense += cout;
                std::cout << " DATE " << date << " | " << km << " km | " 
                          << litre << " L | " << cout << " MAD\n";
            }
        }
        file.close();
        
        std::cout << "================================\n";
        std::cout << "Resume : " << nbConsommations << " consommations | " 
                  << std::fixed << std::setprecision(2) << totalDepense << " MAD total\n";
    } else {
        std::cout << "Choix invalide.\n";
    }
}

void Admin::calculerChiffreAffairesTotal() {
    std::cout << "\n=== CHIFFRE D'AFFAIRES TOTAL ===\n";
    
    DIR *dir;
    struct dirent *ent;
    double caTotal = 0.0;
    int nbClients = 0;
    
    if ((dir = opendir("historique_clients")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt")) {
                nbClients++;
                std::string fichierClient = "historique_clients/" + std::string(ent->d_name);
                std::ifstream file(fichierClient.c_str());
                std::string ligne;
                
                double caClient = 0.0;
                while (std::getline(file, ligne)) {
                    if (ligne.find("PASSWORD:") == 0) continue;
                    
                    std::istringstream iss(ligne);  // ← MAINTENANT ÇA MARCHE
                    std::string date;
                    double km, litre, cout;
                    if (iss >> date >> km >> litre >> cout) {
                        caClient += cout;
                    }
                }
                file.close();
                
                caTotal += caClient;
                std::cout << "COUT " << ent->d_name << " : " << std::fixed << std::setprecision(2) 
                          << caClient << " MAD\n";
            }
        }
        closedir(dir);
    }
    
    std::cout << "================================\n";
    std::cout << "CHIFFRE D'AFFAIRES TOTAL : " << std::fixed << std::setprecision(2) 
              << caTotal << " MAD\n";
    std::cout << "Nombre de clients : " << nbClients << "\n";
    
    if (nbClients > 0) {
        std::cout << "Moyenne par client : " << (caTotal / nbClients) << " MAD\n";
    }
}