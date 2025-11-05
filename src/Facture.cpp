#include "include/Facture.hpp"
#include "include/qrcodegen.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace qrcodegen;

int Facture::compteurFactures = 1000;

Facture::Facture(std::string client) : clientNom(client), total(0) {
    // Générer numéro de facture
    compteurFactures++;
    std::ostringstream oss;
    oss << "FACT-" << compteurFactures;
    numeroFacture = oss.str();
    
    // Date actuelle
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::ostringstream dateoss;
    dateoss << 1900 + ltm->tm_year << "-" 
            << std::setw(2) << std::setfill('0') << (1 + ltm->tm_mon) << "-"
            << std::setw(2) << std::setfill('0') << ltm->tm_mday;
    date = dateoss.str();
}

void Facture::ajouterArticle(std::string produit, double prix, double quantite) {
    double sousTotal = prix * quantite;
    articles.push_back(std::make_pair(produit + " x" + std::to_string((int)quantite), sousTotal));
    total += sousTotal;
}

void Facture::genererFacture() {
    // Créer le dossier factures s'il n'existe pas
    #ifdef _WIN32
    system("mkdir factures >nul 2>&1");
    #else
    system("mkdir -p factures");
    #endif

    std::string nomFichier = "factures/" + numeroFacture + "_" + clientNom + ".txt";
    std::ofstream fichier(nomFichier);
    
    if (fichier.is_open()) {
        fichier << "========================================\n";
        fichier << "           FACTURE STATION SERVICE\n";
        fichier << "========================================\n";
        fichier << "Numero: " << numeroFacture << "\n";
        fichier << "Client: " << clientNom << "\n";
        fichier << "Date: " << date << "\n";
        fichier << "----------------------------------------\n";
        
        for (const auto& article : articles) {
            fichier << std::left << std::setw(25) << article.first 
                    << std::right << std::setw(10) << std::fixed << std::setprecision(2) 
                    << article.second << " MAD\n";
        }
        
        fichier << "----------------------------------------\n";
        fichier << std::left << std::setw(25) << "TOTAL" 
                << std::right << std::setw(10) << std::fixed << std::setprecision(2) 
                << total << " MAD\n";
        fichier << "========================================\n";
        fichier << "Merci de votre confiance !\n";
        fichier.close();
        
        std::cout << " Facture " << numeroFacture << " generee avec succes!\n";
    } else {
        std::cout << " Erreur: Impossible de créer la facture.\n";
    }
}

void Facture::afficherFacture() {
    std::cout << "\n========================================\n";
    std::cout << "           FACTURE STATION SERVICE\n";
    std::cout << "========================================\n";
    std::cout << "Numero: " << numeroFacture << "\n";
    std::cout << "Client: " << clientNom << "\n";
    std::cout << "Date: " << date << "\n";
    std::cout << "----------------------------------------\n";
    
    for (const auto& article : articles) {
        std::cout << std::left << std::setw(25) << article.first 
                  << std::right << std::setw(10) << std::fixed << std::setprecision(2) 
                  << article.second << " MAD\n";
    }
    
    std::cout << "----------------------------------------\n";
    std::cout << std::left << std::setw(25) << "TOTAL" 
              << std::right << std::setw(10) << std::fixed << std::setprecision(2) 
              << total << " MAD\n";
    std::cout << "========================================\n";
}

double Facture::getTotal() const {
    return total;
}

std::string Facture::getNumero() const {
    return numeroFacture;
}

void Facture::genererQRCode() const {
    // Lien PayPal réel avec votre compte
    std::string qrData = "https://www.paypal.com/paypalme/tahahrm123/" + std::to_string(total);
    
    try {
        QrCode qr = QrCode::encodeText(qrData.c_str(), QrCode::Ecc::MEDIUM);
        
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << "     QR CODE PAIEMENT PAYPAL\n";
        std::cout << std::string(50, '=') << "\n\n";
        
        // Afficher le VRAI QR Code
        int border = 2;
        for (int y = -border; y < qr.getSize() + border; y++) {
            for (int x = -border; x < qr.getSize() + border; x++) {
                std::cout << (qr.getModule(x, y) ? "██" : "  ");
            }
            std::cout << "\n";
        }
        
        std::cout << "\n PAIEMENT VIA PAYPAL\n";
        std::cout << " Montant: " << std::fixed << std::setprecision(2) << total << " MAD\n";
        std::cout << " Facture: " << numeroFacture << "\n";
        std::cout << " Client: " << clientNom << "\n";
        std::cout << " Compte: tahahrm123\n";
        std::cout << "\n Scannez pour payer sur PayPal\n";
        std::cout << std::string(50, '=') << "\n";
        
    } catch (const std::exception& e) {
        std::cout << "Erreur generation QR Code: " << e.what() << "\n";
    }
}

void Facture::genererQRCodePourFactureExistante(const std::string& numero, const std::string& client, double montant, const std::string& dateFacture) const {
    // Lien PayPal réel avec votre compte
    std::string qrData = "https://www.paypal.com/paypalme/tahahrm123/" + std::to_string(montant);
    
    try {
        QrCode qr = QrCode::encodeText(qrData.c_str(), QrCode::Ecc::MEDIUM);
        
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << "     QR CODE PAIEMENT PAYPAL\n";
        std::cout << std::string(50, '=') << "\n\n";
        
        // Afficher le VRAI QR Code
        int border = 2;
        for (int y = -border; y < qr.getSize() + border; y++) {
            for (int x = -border; x < qr.getSize() + border; x++) {
                std::cout << (qr.getModule(x, y) ? "██" : "  ");
            }
            std::cout << "\n";
        }
        
        std::cout << "\n💰 PAIEMENT VIA PAYPAL\n";
        std::cout << " Montant: " << std::fixed << std::setprecision(2) << montant << " MAD\n";
        std::cout << " Facture: " << numero << "\n";
        std::cout << " Client: " << client << "\n";
        std::cout << " Compte: tahahrm123\n";
        std::cout << "\n Scannez pour payer sur PayPal\n";
        std::cout << std::string(50, '=') << "\n";
        
    } catch (const std::exception& e) {
        std::cout << "Erreur generation QR Code: " << e.what() << "\n";
    }
}