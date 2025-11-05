#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <algorithm>

class Client {
private:
    std::string nom;
    std::string prenom;
    std::string email;
    std::string telephone;
    std::string motDePasse;

    std::vector<std::string> dates;
    std::vector<double> kms;
    std::vector<double> litres;
    std::vector<double> couts;
    std::vector<std::string> typesCarburant;

    double totalDepense;
    double premierKilometrage;
    double prixLitre;
    
    int pointsFidelite;
    std::string badgeActuel;
    std::vector<std::string> badgesDebloques;

    std::string getNomFichier() const;
    std::string getCurrentDateTime() const;
    void chargerDepuisFichier();
    void appendLigneDansFichier(const std::string &dateTime, double km, double l, double cout);
    void enregistrerInformationsClient();
    bool verifierMotDePasse(const std::string& mdp);
    bool authentifierParEmail(const std::string& email, const std::string& mdp);
    bool authentifierParNomPrenom(const std::string& nom, const std::string& prenom, const std::string& mdp);

public:
    Client(const std::string &n = "", const std::string &p = "", const std::string &e = "", const std::string &t = "");
    void saisirIdentite();
    void creerNouveauClient();
    void connecterClientExistant();
    void ajouterConsommation(double kmActuel, double litresAjoutes);
    void afficherHistorique() const;
    double getTotalDepense() const;

    void setTypeCarburant(const std::string& type);
    std::string getDernierTypeCarburant() const;

    void setPrixLitre(double nouveauPrix);
    double getPrixLitre() const;
    
    void ajouterPoints(int points);
    void verifierBadges();
    void afficherBadges() const;
    int getPoints() const { return pointsFidelite; }
    std::string getBadgeActuel() const { return badgeActuel; }
    
    void ajouterConsommationAvecType(double kmActuel, double litresAjoutes, const std::string& type);
    
    std::string getEmail() const { return email; }
    std::string getTelephone() const { return telephone; }
    std::string getNom() const { return nom; }
    std::string getPrenom() const { return prenom; }
};

#endif