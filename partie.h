#ifndef PARTIE_H
#define PARTIE_H

#include <Imagine/Graphics.h>
using namespace Imagine;

#include <string>

struct Combattant {
    int id;
    int soldats;
    Color color;
    std::string soldierImage;
};

class Partie {
private:
    Combattant gauche ;
    Combattant droite ;
    byte* backgroundPixels;
    int bgWidth, bgHeight;
    byte* redSoldierPixels;
    byte* blueSoldierPixels;
    int soldierWidth, soldierHeight;

    int& soldats_attaquant;
    int& soldats_defenseur;
    int id_attaquant;
    int id_defenseur;
    bool permutation;  // Pour suivre si les joueurs ont été permutés

public:
    void afficher_interface();
    void afficher_message(const std::string& message);
    void dessiner_soldats();
    void animation_lancer_piece(bool resultat);

    Partie(int id_attaquant, int& nb_attaquant, int id_defenseur, int& nb_defenseur);

    Combattant joueur_gauche() { return gauche; }
    Combattant joueur_droite() { return droite; }

    void afficher_soldats();
    bool lancer_piece();
    void tour(bool tour_attaque);
    void run();

    void loadResources();

    void fin_combat();
    int get_vainqueur() const;
};





#endif // PARTIE_H

