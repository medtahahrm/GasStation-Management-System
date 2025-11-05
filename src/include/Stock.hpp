#ifndef STOCK_HPP
#define STOCK_HPP

#include <map>
#include <string>

class Stock {
private:
    std::map<std::string, int> produits;
    std::map<std::string, int> seuilsAlertes;
    
public:
    Stock();
    void initialiserStock();
    void ajouterProduit(std::string produit, int quantite);
    void retirerProduit(std::string produit, int quantite);
    void verifierStockFaible();
    void afficherStock();
    int getQuantite(std::string produit);
    void setSeuilAlerte(std::string produit, int seuil);
};

#endif