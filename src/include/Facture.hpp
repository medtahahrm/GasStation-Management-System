#ifndef FACTURE_HPP
#define FACTURE_HPP

#include <string>
#include <vector>
#include <utility>

class Facture {
private:
    static int compteurFactures;
    std::string numeroFacture;
    std::string clientNom;
    std::vector<std::pair<std::string, double>> articles;
    double total;
    std::string date;
    
public:
    Facture(std::string client);
    void ajouterArticle(std::string produit, double prix, double quantite = 1.0);
    void genererFacture();
    void afficherFacture();
    double getTotal() const;
    std::string getNumero() const;
    
    void genererQRCode() const;
    void genererQRCodePourFactureExistante(const std::string& numero, const std::string& client, double montant, const std::string& dateFacture) const;
};

#endif