#include "include/Stock.hpp"
#include <iostream>
#include <fstream>

Stock::Stock() {
    initialiserStock();
}

void Stock::initialiserStock() {
    // Produits initiaux
    produits["Essence"] = 5000;
    produits["Diesel"] = 5000;
    produits["GPL"] = 2000;
    produits["Huile"] = 100;
    produits["Lave_glace"] = 50;
    
    // Seuils d'alerte
    seuilsAlertes["Essence"] = 2000;
    seuilsAlertes["Diesel"] = 2000;
    seuilsAlertes["GPL"] = 500;
    seuilsAlertes["Huile"] = 20;
    seuilsAlertes["Lave_glace"] = 10;
}

void Stock::ajouterProduit(std::string produit, int quantite) {
    if (produits.find(produit) != produits.end()) {
        produits[produit] += quantite;
        std::cout << "Verifie " << quantite << " unites de " << produit << " ajoutees au stock.\n";
    } else {
        std::cout << "Produit non reconnu.\n";
    }
}

void Stock::retirerProduit(std::string produit, int quantite) {
    if (produits.find(produit) != produits.end()) {
        if (produits[produit] >= quantite) {
            produits[produit] -= quantite;
            std::cout << "Verifie " << quantite << " unites de " << produit << " retirees du stock.\n";
            verifierStockFaible();
        } else {
            std::cout << "Stock insuffisant pour " << produit << ".\n";
        }
    } else {
        std::cout << "Produit non reconnu.\n";
    }
}

void Stock::verifierStockFaible() {
    std::cout << "\n Verification des stocks...\n";
    bool alerte = false;
    
    for (auto& produit : produits) {
        std::string nom = produit.first;
        int quantite = produit.second;
        int seuil = seuilsAlertes[nom];
        
        if (quantite <= seuil) {
            std::cout << " ALERTE: Stock faible pour " << nom 
                      << " (" << quantite << " unites, seuil: " << seuil << ")\n";
            alerte = true;
        }
    }
    
    if (!alerte) {
        std::cout << "Tous les stocks sont suffisants.\n";
    }
}

void Stock::afficherStock() {
    std::cout << "\n ETAT DES STOCKS\n";
    std::cout << "====================\n";
    
    for (auto& produit : produits) {
        std::cout << "- " << produit.first << ": " << produit.second << " unites";
        
        if (produit.second <= seuilsAlertes[produit.first]) {
            std::cout << "(Stock faible)";
        }
        std::cout << "\n";
    }
}

int Stock::getQuantite(std::string produit) {
    return (produits.find(produit) != produits.end()) ? produits[produit] : 0;
}

void Stock::setSeuilAlerte(std::string produit, int seuil) {
    seuilsAlertes[produit] = seuil;
    std::cout << "Seuil d'alerte pour " << produit << " defini a " << seuil << " unites.\n";
}