#ifndef ADMIN_HPP
#define ADMIN_HPP

#include <vector>
#include <string>

class Admin {
private:
    std::string motDePasseAdmin;

public:
    Admin();
    bool authentifierAdmin();
    void afficherStatistiquesGlobales();
    void modifierPrixCarburants();
    void voirTousClients(); 
    void supprimerClient();
    void rechercherClient();
    void voirDetailsClient();
    void calculerChiffreAffairesTotal();
};

#endif