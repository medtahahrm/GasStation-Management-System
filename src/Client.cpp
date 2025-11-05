#include "include/Client.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <dirent.h>

double getPrixCarburant(const std::string& type) {
    std::ifstream fichier("config_prix.txt");
    double prixEssence = 12.0, prixDiesel = 11.0, prixGPL = 7.0;
    
    if (fichier.is_open()) {
        std::string typeFichier;
        double prix;
        while (fichier >> typeFichier >> prix) {
            if (typeFichier == "ESSENCE") prixEssence = prix;
            else if (typeFichier == "DIESEL") prixDiesel = prix;
            else if (typeFichier == "GPL") prixGPL = prix;
        }
        fichier.close();
    }
    
    if (type == "Diesel") return prixDiesel;
    if (type == "GPL") return prixGPL;
    return prixEssence;
}

const double PRIX_LITRE_BASE = 12.0;

// === Constructeur mis à jour ===
Client::Client(const std::string &n, const std::string &p, const std::string &e, const std::string &t)
    : nom(n), prenom(p), email(e), telephone(t), totalDepense(0), 
      premierKilometrage(-1), prixLitre(PRIX_LITRE_BASE),
      pointsFidelite(0), badgeActuel("Nouveau") {}

// === Fonction pour obtenir le nom du fichier ===
std::string Client::getNomFichier() const {
    return "historique_clients/" + nom + "_" + prenom + ".txt";
}

// === Fonction pour obtenir la date/heure actuelle ===
std::string Client::getCurrentDateTime() const {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::ostringstream oss;
    oss << 1900 + ltm->tm_year << "-"
        << std::setw(2) << std::setfill('0') << 1 + ltm->tm_mon << "-"
        << std::setw(2) << std::setfill('0') << ltm->tm_mday << "_"
        << std::setw(2) << std::setfill('0') << ltm->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << ltm->tm_min;
    return oss.str();
}

// === Enregistrer toutes les informations client ===
void Client::enregistrerInformationsClient() {
    std::ofstream ofs(getNomFichier().c_str(), std::ios::app);
    if (ofs) {
        ofs << "NOM:" << nom << "\n";
        ofs << "PRENOM:" << prenom << "\n";
        ofs << "EMAIL:" << email << "\n";
        ofs << "TELEPHONE:" << telephone << "\n";
        ofs << "PASSWORD:" << motDePasse << "\n";
        ofs << "=== HISTORIQUE CONSOMMATION ===\n";
        ofs.close();
    }
}

// === Vérifier mot de passe ===
bool Client::verifierMotDePasse(const std::string& mdp) {
    return (mdp == motDePasse);
}

// === Authentification par email ===
bool Client::authentifierParEmail(const std::string& email, const std::string& mdp) {
    return (this->email == email && verifierMotDePasse(mdp));
}

// === Authentification par nom/prenom ===
bool Client::authentifierParNomPrenom(const std::string& nom, const std::string& prenom, const std::string& mdp) {
    return (this->nom == nom && this->prenom == prenom && verifierMotDePasse(mdp));
}

// === Chargement des données existantes avec nouvelles infos ===
void Client::chargerDepuisFichier() {
    std::ifstream fichier(getNomFichier().c_str());
    if (!fichier.is_open()) return;

    std::string ligne;
    std::string date;
    double km, litre, cout;
    bool dansHistorique = false;

    while (std::getline(fichier, ligne)) {
        if (ligne.find("NOM:") == 0) {
            nom = ligne.substr(4);
        } else if (ligne.find("PRENOM:") == 0) {
            prenom = ligne.substr(7);
        } else if (ligne.find("EMAIL:") == 0) {
            email = ligne.substr(6);
        } else if (ligne.find("TELEPHONE:") == 0) {
            telephone = ligne.substr(10);
        } else if (ligne.find("PASSWORD:") == 0) {
            motDePasse = ligne.substr(9);
        } else if (ligne.find("=== HISTORIQUE CONSOMMATION ===") == 0) {
            dansHistorique = true;
        } else if (dansHistorique && ligne.find_first_not_of(" \t\n\r\f\v") != std::string::npos) {
            // Ligne de consommation
            std::istringstream iss(ligne);
            if (iss >> date >> km >> litre >> cout) {
                dates.push_back(date);
                kms.push_back(km);
                litres.push_back(litre);
                couts.push_back(cout);
                totalDepense += cout;
                if (premierKilometrage < 0) premierKilometrage = km;
            }
        }
    }
    fichier.close();
    
    // Calcul des points au chargement
    pointsFidelite = (int)(totalDepense);
    verifierBadges();
}

// === Sauvegarder une nouvelle ligne ===
void Client::appendLigneDansFichier(const std::string &dateTime, double km, double l, double cout) {
    std::ofstream fichier(getNomFichier().c_str(), std::ios::app);
    if (fichier.is_open()) {
        fichier << dateTime << " " << km << " " << l << " " << cout << "\n";
        fichier.close();
    }
}

void Client::setTypeCarburant(const std::string& type) {
    typesCarburant.push_back(type);
}

std::string Client::getDernierTypeCarburant() const {
    if (typesCarburant.empty()) return "Essence";
    return typesCarburant.back();
}

// === Créer un nouveau client ===
void Client::creerNouveauClient() {
    std::cout << "\n--- INSCRIPTION NOUVEAU CLIENT ---\n";
    
    // Saisie des informations
    std::cout << "Nom : ";
    std::cin >> nom;
    std::cout << "Prenom : ";
    std::cin >> prenom;
    std::cout << "Email : ";
    std::cin >> email;
    std::cout << "Telephone : ";
    std::cin >> telephone;
    
    // Validation du mot de passe
    std::string mdp1, mdp2;
    do {
        std::cout << "Creer un mot de passe (4 chiffres) : ";
        std::cin >> mdp1;
        
        if (mdp1.size() != 4 || mdp1.find_first_not_of("0123456789") != std::string::npos) {
            std::cout << "Erreur : le mot de passe doit contenir exactement 4 chiffres.\n";
            continue;
        }
        
        std::cout << "Confirmer le mot de passe : ";
        std::cin >> mdp2;
        
        if (mdp1 != mdp2) {
            std::cout << "Les mots de passe ne correspondent pas. Veuillez reessayer.\n";
        }
    } while (mdp1 != mdp2);
    
    motDePasse = mdp1;
    enregistrerInformationsClient();
    std::cout << "Compte cree avec succes ! Bienvenue " << prenom << " " << nom << " !\n";
}

// === Connecter un client existant ===
void Client::connecterClientExistant() {
    int choixAuth;
    std::cout << "\n--- CONNEXION ---\n";
    std::cout << "1. Connexion par Nom/Prenom\n";
    std::cout << "2. Connexion par Email\n";
    std::cout << "Choix : ";
    std::cin >> choixAuth;
    
    std::string mdp;
    bool authentifie = false;
    
    if (choixAuth == 1) {
        std::string nomSaisi, prenomSaisi;
        std::cout << "Nom : ";
        std::cin >> nomSaisi;
        std::cout << "Prenom : ";
        std::cin >> prenomSaisi;
        std::cout << "Mot de passe : ";
        std::cin >> mdp;
        
        // Charger le fichier correspondant
        std::string fichierTest = "historique_clients/" + nomSaisi + "_" + prenomSaisi + ".txt";
        std::ifstream test(fichierTest.c_str());
        if (test.good()) {
            nom = nomSaisi;
            prenom = prenomSaisi;
            chargerDepuisFichier();
            authentifie = authentifierParNomPrenom(nomSaisi, prenomSaisi, mdp);
        }
        test.close();
        
    } else if (choixAuth == 2) {
        std::string emailSaisi;
        std::cout << "Email : ";
        std::cin >> emailSaisi;
        std::cout << "Mot de passe : ";
        std::cin >> mdp;
        
        // Rechercher le fichier par email (nécessite de parcourir tous les fichiers)
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir("historique_clients")) != NULL) {
            while ((ent = readdir(dir)) != NULL && !authentifie) {
                if (strstr(ent->d_name, ".txt")) {
                    std::string fichierClient = "historique_clients/" + std::string(ent->d_name);
                    std::ifstream file(fichierClient.c_str());
                    std::string ligne;
                    std::string emailFichier;
                    
                    while (std::getline(file, ligne) && emailFichier.empty()) {
                        if (ligne.find("EMAIL:") == 0) {
                            emailFichier = ligne.substr(6);
                        }
                    }
                    file.close();
                    
                    if (emailFichier == emailSaisi) {
                        // Extraire nom et prénom du nom de fichier
                        std::string nomFichier = ent->d_name;
                        size_t pos = nomFichier.find('_');
                        if (pos != std::string::npos) {
                            nom = nomFichier.substr(0, pos);
                            prenom = nomFichier.substr(pos + 1, nomFichier.length() - pos - 5); // -5 pour enlever ".txt"
                            chargerDepuisFichier();
                            authentifie = authentifierParEmail(emailSaisi, mdp);
                        }
                    }
                }
            }
            closedir(dir);
        }
    }
    
    if (authentifie) {
        std::cout << "Connexion reussie ! Bienvenue " << prenom << " " << nom << " !\n";
    } else {
        std::cout << "Echec de l'authentification. Informations incorrectes.\n";
        exit(0);
    }
}

// === Saisir identité (fonction principale) ===
void Client::saisirIdentite() {
#ifdef _WIN32
    system("mkdir historique_clients >nul 2>&1");
#else
    system("mkdir -p historique_clients");
#endif

    int choix;
    std::cout << "\n=== STATION SERVICE PREMIUM ===\n";
    std::cout << "1. Nouveau client\n";
    std::cout << "2. Client existant\n";
    std::cout << "Choix : ";
    std::cin >> choix;
    
    if (choix == 1) {
        creerNouveauClient();
    } else if (choix == 2) {
        connecterClientExistant();
    } else {
        std::cout << "Choix invalide.\n";
        exit(0);
    }
    
    chargerDepuisFichier();
}

// === Ajouter consommation (version originale) ===
void Client::ajouterConsommation(double kmActuel, double litresAjoutes) {
    std::string type;
    std::cout << "Type de carburant (Essence/Diesel/GPL) : ";
    std::cin >> type;
    
    prixLitre = getPrixCarburant(type);
    double cout = litresAjoutes * prixLitre;
    std::string date = getCurrentDateTime();

    dates.push_back(date);
    kms.push_back(kmActuel);
    litres.push_back(litresAjoutes);
    couts.push_back(cout);

    totalDepense += cout;

    appendLigneDansFichier(date, kmActuel, litresAjoutes, cout);
    
    // Ajout des points
    int pointsGagnes = (int)cout;
    ajouterPoints(pointsGagnes);
}

// === NOUVELLE FONCTION : Ajouter consommation sans demander le type ===
void Client::ajouterConsommationAvecType(double kmActuel, double litresAjoutes, const std::string& type) {
    setTypeCarburant(type);
    prixLitre = getPrixCarburant(type);
    double cout = litresAjoutes * prixLitre;
    std::string date = getCurrentDateTime();

    dates.push_back(date);
    kms.push_back(kmActuel);
    litres.push_back(litresAjoutes);
    couts.push_back(cout);

    totalDepense += cout;

    appendLigneDansFichier(date, kmActuel, litresAjoutes, cout);
    
    // Ajout des points
    int pointsGagnes = (int)cout;
    ajouterPoints(pointsGagnes);
}

// === Afficher historique ===
void Client::afficherHistorique() const {
    std::cout << "\n=== Historique de consommation ===\n";
    if (dates.empty()) {
        std::cout << "Aucune consommation enregistree.\n";
        return;
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(20) << "Date"
              << std::setw(12) << "Km"
              << std::setw(10) << "Litres"
              << std::setw(12) << "Cout(MAD)" << "\n";
    std::cout << "---------------------------------------------------------\n";

    for (size_t i = 0; i < dates.size(); ++i) {
        std::cout << std::setw(20) << dates[i]
                  << std::setw(12) << kms[i]
                  << std::setw(10) << litres[i]
                  << std::setw(12) << couts[i] << "\n";
    }

    if (kms.size() > 1) {
        double distance = kms.back() - kms.front();
        double totalLitres = 0;
        for (double l : litres) totalLitres += l;

        if (distance > 0)
            std::cout << "\nTaux de consommation : "
                      << (totalLitres / distance) * 100 << " L/100km\n";
        else
            std::cout << "\nTaux de consommation : Donnees insuffisantes (meme kilometrage)\n";
    } else {
        std::cout << "\nTaux de consommation : Donnees insuffisantes.\n";
    }

    std::cout << "Total depense : " << totalDepense << " MAD\n";
}

// === NOUVELLES METHODES BADGES ===

void Client::ajouterPoints(int points) {
    pointsFidelite += points;
    verifierBadges();
}

void Client::verifierBadges() {
    std::vector<std::string> nouveauxBadges;
    
    if (pointsFidelite >= 5000 && std::find(badgesDebloques.begin(), badgesDebloques.end(), "LEGENDE") == badgesDebloques.end()) {
        badgesDebloques.push_back("LEGENDE");
        badgeActuel = "LEGENDE";
        nouveauxBadges.push_back("LEGENDE");
    }
    if (pointsFidelite >= 2000 && std::find(badgesDebloques.begin(), badgesDebloques.end(), "VIP") == badgesDebloques.end()) {
        badgesDebloques.push_back("VIP");
        if (badgeActuel != "LEGENDE") badgeActuel = "VIP";
        nouveauxBadges.push_back("VIP");
    }
    if (pointsFidelite >= 1000 && std::find(badgesDebloques.begin(), badgesDebloques.end(), "Expert") == badgesDebloques.end()) {
        badgesDebloques.push_back("Expert");
        if (badgeActuel != "LEGENDE" && badgeActuel != "VIP") badgeActuel = "Expert";
        nouveauxBadges.push_back("Expert");
    }
    if (pointsFidelite >= 500 && std::find(badgesDebloques.begin(), badgesDebloques.end(), "Regulier") == badgesDebloques.end()) {
        badgesDebloques.push_back("Regulier");
        if (badgeActuel != "LEGENDE" && badgeActuel != "VIP" && badgeActuel != "Expert") badgeActuel = "Regulier";
        nouveauxBadges.push_back("Regulier");
    }
    if (pointsFidelite >= 100 && std::find(badgesDebloques.begin(), badgesDebloques.end(), "Debutant") == badgesDebloques.end()) {
        badgesDebloques.push_back("Debutant");
        if (badgeActuel != "LEGENDE" && badgeActuel != "VIP" && badgeActuel != "Expert" && badgeActuel != "Regulier") badgeActuel = "Debutant";
        nouveauxBadges.push_back("Debutant");
    }
}

void Client::afficherBadges() const {
    std::cout << "\n" << std::string(30, '=') << "\n";
    std::cout << "     VOS BADGES\n";
    std::cout << std::string(30, '=') << "\n";
    std::cout << "  Badge actuel : " << badgeActuel << "\n";
    std::cout << "  Points fidelite : " << pointsFidelite << "\n";
    std::cout << "  Badges debloques : ";
    for (const auto& badge : badgesDebloques) {
        std::cout << badge << " ";
    }
    std::cout << "\n" << std::string(30, '=') << "\n";
    
    // Prochain palier
    if (pointsFidelite < 100) {
        std::cout << " Prochain badge : Debutant (100 points)\n";
    } else if (pointsFidelite < 500) {
        std::cout << " Prochain badge : Regulier (500 points)\n";
    } else if (pointsFidelite < 1000) {
        std::cout << " Prochain badge : Expert (1000 points)\n";
    } else if (pointsFidelite < 2000) {
        std::cout << " Prochain badge : VIP (2000 points)\n";
    } else if (pointsFidelite < 5000) {
        std::cout << " Prochain badge : LEGENDE (5000 points)\n";
    } else {
        std::cout << " Maximum atteint ! Vous etes une LEGENDE !\n";
    }
}

// === Accesseurs ===
double Client::getTotalDepense() const {
    return totalDepense;
}

void Client::setPrixLitre(double nouveauPrix) {
    prixLitre = nouveauPrix;
}

double Client::getPrixLitre() const {
    return prixLitre;
}