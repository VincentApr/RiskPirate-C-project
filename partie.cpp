#include "partie.h"
#include <Imagine/Graphics.h>
using namespace Imagine;
using namespace std;

const int width = 960;
const int height = 540;

const int ROUGE = 1 ;
const int BLEU = 2 ;

Partie::Partie(int id_att, int& nb_att, int id_def, int& nb_def)
    : soldats_attaquant(nb_att), soldats_defenseur(nb_def),
    id_attaquant(id_att), id_defenseur(id_def), permutation(false) {

    loadResources();

    // Permutation visuelle - rouge toujours à gauche, bleu à droite
    if (id_att == ROUGE) {
        gauche.id = id_att;
        gauche.soldats = nb_att;
        gauche.color = RED;
        droite.id = id_def;
        droite.soldats = nb_def;
        droite.color = BLUE;
        gauche.soldierImage = "assets/soldier_red.png";
        droite.soldierImage = "assets/soldier_blue.png";
        permutation = false;  // Pas de permutation
    } else {
        // Permutation pour l'affichage uniquement
        gauche.id = id_def;
        gauche.soldats = nb_def;
        gauche.color = RED;
        droite.id = id_att;
        droite.soldats = nb_att;
        droite.color = BLUE;
        gauche.soldierImage = "assets/soldier_red.png";
        droite.soldierImage = "assets/soldier_blue.png";
        permutation = true;  // Permutation effectuée
    }

    // Gestion des cas spéciaux (ID -1)
    if (id_att == -1) {
        gauche.soldierImage = "assets/soldier_red.png";
    }
    if (id_def == -1) {
        droite.soldierImage = "assets/soldier_blue.png";
    }
}

void Partie::loadResources() {
    // Charger les images
    loadColorImage("../assets/battlefield.jpg", backgroundPixels, bgWidth, bgHeight);
    loadAlphaColorImage("../assets/soldier_red.png", redSoldierPixels, soldierWidth, soldierHeight);
    loadAlphaColorImage("../assets/soldier_blue.png", blueSoldierPixels, soldierWidth, soldierHeight);
}

bool Partie::lancer_piece() {
    return intRandom(0, 1) == 1;
}

void Partie::tour(bool tour_attaque) {
    afficher_interface();
    afficher_message(tour_attaque ? "Tour de l'gauche: cliquez pour lancer la piece"
                                  : "Tour du gauche: cliquez pour lancer la piece");
    dessiner_soldats();

    bool pile = lancer_piece();
    animation_lancer_piece(pile);

    if (pile && tour_attaque) droite.soldats--;
    if (pile && !tour_attaque) gauche.soldats--;

    string result = pile ? "SUCCES! Un soldat ennemi tombe!" : "ECHEC! Aucun effet.";
    drawString(width / 2 - 300, 100, result, WHITE, 30, true, 1.0, Color(0, 0, 0));
    milliSleep(500);
}

void Partie::run() {
    afficher_interface();
    while (gauche.soldats > 0 && droite.soldats > 0) {
        tour(true);
        if (droite.soldats <= 0) break;
        tour(false);
    }

    clearWindow();
    putColorImage(0, 0, backgroundPixels, bgWidth, bgHeight);
    milliSleep(3000);
}

void Partie::afficher_interface() {
    // Dessiner le fond
    putColorImage(0, 0, backgroundPixels, width, height);
}

void Partie::afficher_message(const std::string& message) {
    // fillRect(600, 100, 720, 60, Color(0, 0, 0));
    drawString(width / 2 - 300, 140, message, WHITE, 30);
}

void Partie::dessiner_soldats() {
    const int soldierWidth = 129;
    const int soldierHeight = 161;
    const int spacingX = 30;
    const int spacingY = 100;
    const int startXAtt =20;
    const int startXDef = width - 240;
    const int startY = 250;

    // Soldats gauches
    int count = 0;
    for (int i = 0; i < gauche.soldats; ++i) {
        int row = count / 5;
        int col = count % 5;
        putAlphaColorImage(startXAtt + col * spacingX,
                           startY + row * spacingY,
                           redSoldierPixels, soldierWidth, soldierHeight);
        count++;
    }

    // Soldats défenseurs
    count = 0;
    for (int i = 0; i < droite.soldats; ++i) {
        int row = count / 5;
        int col = count % 5;
        putAlphaColorImage(startXDef + col * spacingX,
                           startY + row * spacingY,
                           blueSoldierPixels, soldierWidth, soldierHeight);
        count++;
    }
}

void Partie::animation_lancer_piece(bool resultat) {
    const int coinSize = 100;
    int centerX = width / 2 - coinSize / 2;
    int centerY = 3 * height / 4 - coinSize / 2;

    // Couleurs
    const Color goldColor(255, 215, 0);        // Couleur or pour pile
    const Color silverColor(192, 192, 192);    // Couleur argent pour face
    const Color shadowColor(50, 50, 50);  // Couleur de l'ombre
    const Color highlightColor(255, 255, 255); // Couleur pour les reflets

    // Animation du lancer
    for (int i = 0; i < 3; ++i) {
        fillEllipse(centerX, centerY, coinSize, coinSize, (i % 2 == 0) ? goldColor : silverColor);
        drawEllipse(centerX, centerY, coinSize, coinSize, BLACK, 1);
        drawString(centerX + coinSize / 2 - 20, centerY + coinSize / 2 + 5, (i % 2 == 0) ? "PILE" : "FACE", BLACK, 20);

        milliSleep(30);
    }

    putColorImage(0, 0, backgroundPixels, bgWidth, bgHeight);
    dessiner_soldats();

    // Résultat final
    for (int i = 0; i < 5; ++i) {
        fillEllipse(centerX + 2, centerY + 30, coinSize, coinSize - 50, shadowColor);
        Color coinColor = resultat ? goldColor : silverColor;
        fillEllipse(centerX, centerY + 25, coinSize, coinSize - 50, coinColor);
        drawEllipse(centerX, centerY + 25, coinSize, coinSize - 50, BLACK, 1);
        drawString(centerX + coinSize / 2 - 20, centerY + coinSize / 2 + 5, resultat ? "PILE" : "FACE", BLACK, 20);

        milliSleep(100);
    }

    // Afficher un message final
    string result = resultat ? "SUCCES! Un soldat ennemi tombe!" : "ECHEC! Aucun effet.";
    drawString(width / 2 - 300, 100, result, WHITE, 30, true, 1.0, Color(0, 0, 0));
}

void Partie::fin_combat() {
    // Mise à jour des références selon si permutation a été faite ou non
    if (permutation) {
        soldats_attaquant = droite.soldats;
        soldats_defenseur = gauche.soldats;
    } else {
        soldats_attaquant = gauche.soldats;
        soldats_defenseur = droite.soldats;
    }
}

int Partie::get_vainqueur() const {
    if (gauche.soldats > 0 && droite.soldats <= 0) {
        return permutation ? id_defenseur : id_attaquant;
    } else if (gauche.soldats <= 0 && droite.soldats > 0) {
        return permutation ? id_attaquant : id_defenseur;
    } else if (gauche.soldats > 0 && droite.soldats > 0) {
        // Cas où les deux ont encore des soldats - match nul ou inachevé
        return 0;  // Code pour "pas de vainqueur clair"
    } else {
        // Les deux ont 0 soldat - techniquement match nul
        return -1;  // Code pour "match nul"
    }
}


