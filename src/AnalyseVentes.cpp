#include "include/AnalyseVentes.hpp"
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <cstring>  // ← AJOUTER

// Fonction de comparaison pour le tri
bool comparerClients(const std::pair<std::string, double>& a, const std::pair<std::string, double>& b) {
    return a.second > b.second;
}

void AnalyseVentes::meilleursClients() {
    std::cout << "\n TOP 5 MEILLEURS CLIENTS\n";
    std::cout << "==========================\n";
    
    std::map<std::string, double> clientsCA;
    
    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir("historique_clients")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt")) {
                std::string fichierClient = "historique_clients/" + std::string(ent->d_name);
                std::ifstream file(fichierClient.c_str());
                std::string ligne;
                double caClient = 0.0;
                
                while (std::getline(file, ligne)) {
                    if (ligne.find("PASSWORD:") == 0) continue;
                    
                    std::istringstream iss(ligne);
                    std::string date;
                    double km, litre, cout;
                    if (iss >> date >> km >> litre >> cout) {
                        caClient += cout;
                    }
                }
                file.close();
                
                clientsCA[ent->d_name] = caClient;
            }
        }
        closedir(dir);
    }
    
    // Trier par CA décroissant (version C++11 compatible)
    std::vector<std::pair<std::string, double>> clientsTries(clientsCA.begin(), clientsCA.end());
    std::sort(clientsTries.begin(), clientsTries.end(), comparerClients);
    
    int rang = 1;
    for (const auto& client : clientsTries) {
        if (rang > 5) break;
        std::cout << rang << ". " << client.first << " - " 
                  << std::fixed << std::setprecision(2) << client.second << " MAD\n";
        rang++;
    }
}

void AnalyseVentes::produitsPlusVendus() {
    std::cout << "\n PRODUITS LES PLUS VENDUS\n";
    std::cout << "===========================\n";
    
    std::map<std::string, int> ventesProduits;
    ventesProduits["Essence"] = 0;
    ventesProduits["Diesel"] = 0;
    ventesProduits["GPL"] = 0;
    
    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir("historique_clients")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt")) {
                std::string fichierClient = "historique_clients/" + std::string(ent->d_name);
                std::ifstream file(fichierClient.c_str());
                std::string ligne;
                
                while (std::getline(file, ligne)) {
                    if (ligne.find("PASSWORD:") == 0) continue;
                    
                    std::istringstream iss(ligne);
                    std::string date;
                    double km, litre, cout;
                    if (iss >> date >> km >> litre >> cout) {
                        // Estimation basée sur le prix
                        if (cout/litre >= 11.5) ventesProduits["Essence"]++;
                        else if (cout/litre >= 10.5) ventesProduits["Diesel"]++;
                        else ventesProduits["GPL"]++;
                    }
                }
                file.close();
            }
        }
        closedir(dir);
    }
    
    for (const auto& produit : ventesProduits) {
        std::cout << "- " << produit.first << ": " << produit.second << " ventes\n";
    }
}

void AnalyseVentes::heuresAffluence() {
    std::cout << "\n HEURES D'AFFLUENCE\n";
    std::cout << "====================\n";
    std::cout << "Matin (8h-12h):  Forte affluence\n";
    std::cout << "Midi (12h-14h):  Moyenne affluence\n";
    std::cout << "Soir (17h-20h):  Tres forte affluence\n";
}

void AnalyseVentes::tendancesMensuelles() {
    std::cout << "\n TENDANCES MENSUELLES\n";
    std::cout << "======================\n";
    std::cout << "Ete:  Hausse des ventes (+15%)\n";
    std::cout << "Hiver:  Baisse moderee (-5%)\n";
    std::cout << "Vacances:  Stabilite\n";
}

void AnalyseVentes::previsionsVentes() {
    std::cout << "\n PREVISIONS DES VENTES\n";
    std::cout << "=======================\n";
    
    DIR *dir;
    struct dirent *ent;
    int totalClients = 0;
    
    if ((dir = opendir("historique_clients")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt")) totalClients++;
        }
        closedir(dir);
    }
    
    double prevision = totalClients * 1.1; // +10%
    std::cout << "Clients actuels: " << totalClients << "\n";
    std::cout << "Prevision mois prochain: " << (int)prevision << " clients\n";
    std::cout << "Croissance estimee: +10%\n";
}