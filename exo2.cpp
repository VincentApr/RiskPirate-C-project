#include <Imagine/Images.h>
#include <Imagine/Graphics.h>
#include <iostream>
#include <cmath>
#include "partie.h"
#include "iles.h"
#include <QApplication>
using namespace Imagine;
typedef Image<byte> Img;


const int rayon_ile=40;
const int larg_ile = 40;
const int haut_ile = 20;


const int DEPLACER='a';
const int ATTAQUER='z';
const int PLACER='e';
const int RAPH='n';
const int TOUR_SUIVANT = 16777220;
const int RAJOUTER_TOUR = 'r';

const int DROITE =16777234;
const int GAUCHE =16777236;
const int HAUT =16777237;
const int BAS = 16777235;

const int nombre_soldats_tour = 3;
const int prix_tour = 0;


const int nb_iles = 10;
const int largeur = 800;
const int hauteur = 600;
const int largeur_panneau = 300;

const int nb_joueur=2;

const float proba_pont=0.6;
const int dist_max_pont = 6*rayon_ile;


Color GOLD_COLOR(255, 215, 0);




struct Joueur{
    int id;
    int n_soldat;
    Color couleur; // Couleur du joueur pour l'affichage
    int ressources ;
};

// Fonction pour calculer la distance entre deux points
double distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double distance(Map m, int island1, int island2){
    return distance(m.pos_x[island1],m.pos_y[island1],m.pos_x[island2], m.pos_y[island2]);
}

bool is_island_alone(Map& m, int island){
    bool alone = false;
    int sum = 0;
    for (int i=0; i<m.n +20 ;i++){
        sum= sum + m.ponts[island][i];
    }
    if (sum==2){
        return false;
    }
    return true;
}


int find_nearest_island(Map m, int island){
    double nearest = 10000;
    int iles=0;
    for (int i=0; i<m.n +20; i++){
        if (i!=island && distance(m, island, i)<nearest){
            iles = i;
            nearest = distance(m, island, i);
        }

    }
    return iles;
}


void monter_ile(Map& m){
    for(int i = 0; i < m.n; i++){
        m.pos_y[i] -= 7 ;
    }
    for(int i = 0; i < 3 * m.n; i++){
        m.trees[i].y -= 7;
    }
}
void descendre_ile(Map& m){
    for(int i = 0; i < m.n; i++){
        m.pos_y[i] += 7 ;
    }
    for(int i = 0; i < 3 * m.n; i++){
        m.trees[i].y += 7;
    }
}
void gauche_ile(Map& m){
    for(int i = 0; i < m.n; i++){
        m.pos_x[i] -= 7 ;
    }
    for(int i = 0; i < 3 * m.n; i++){
        m.trees[i].x -= 7;
    }
}
void droite_ile(Map& m){
    for(int i = 0; i < m.n; i++){
        m.pos_x[i] += 7 ;
    }
    for(int i = 0; i < 3 * m.n; i++){
        m.trees[i].x += 7;
    }
}


void instructions(){
    Color fond = GOLD_COLOR ;
    fillRect(largeur, 0, largeur_panneau, hauteur, fond);

    int x = largeur + 10;
    int y = 30;

    // Titre
    drawString(x, y, "Pirates & Risk", BLACK);
    y += 35;

    // Instructions
    drawString(x, y, "Instructions:", BLACK);
    y += 30;

    // Sélection d'île
    drawString(x, y, "Clic: Selectionner une ile", BLACK);
    y += 35;

    // Déplacement de soldats
    drawString(x, y, "a: Deplacer des soldats", BLACK);
    y += 25;
    drawString(x + 15, y, "- Cliquer ile depart", BLACK);
    y += 25;
    drawString(x + 15, y, "- Cliquer ile arrivee", BLACK);
    y += 25;
    drawString(x + 15, y, "- Taper un nombre (1-9)", BLACK);
    y += 35;

    // Attaquer
    drawString(x, y, "z: Attaquer une ile", BLACK);
    y += 25;
    drawString(x + 15, y, "- Cliquer ile attaquante", BLACK);
    y += 25;
    drawString(x + 15, y, "- Cliquer ile cible", BLACK);
    y += 35;

    // Placer nouveaux soldats
    drawString(x, y, "e: Placer nouveaux soldats", BLACK);
    y += 25;
    drawString(x + 15, y, "- Cliquer une ile", BLACK);
    y += 25;
    drawString(x + 15, y, "- Taper un nombre", BLACK);
    y += 35;

    // Construire une tour
    drawString(x, y, "r: Construire une tour", BLACK);
    y += 25;
    drawString(x + 15, y, "- Cliquer une ile", BLACK);
    y += 25;
    drawString(x + 15, y, "- Cout: " + std::to_string(prix_tour), BLACK);
    y += 35;

    // Déplacement de la carte
    drawString(x, y, "Fleches: Deplacer la carte", BLACK);
    y += 25;
    drawString(x + 15, y, "- Haut/Bas/Gauche/Droite", BLACK);
    y += 35;

    // Fin de tour
    drawString(x, y, "Entree: Fin de tour", BLACK);

}





int get_nb_ile(Map m, Joueur j){
    int i= j.id;
    int nb_ile_j = 0;
    for (int k=0; k<m.n; k++){
        if(m.iles_id[k]==i){
            nb_ile_j++;
        }
    }
    return nb_ile_j;
}



void marquer_composante(Map& m, int ile, int* composantes, int id_composante, bool* visite) {
    visite[ile] = true;
    composantes[ile] = id_composante;

    // Parcourir tous les voisins connectés
    for (int i = 0; i < m.n; i++) {
        if (m.ponts[ile][i] > 0 && !visite[i]) {
            marquer_composante(m, i, composantes, id_composante, visite);
        }
    }
}


int detecter_composantes_connexes(Map& m, int* composantes) {
    bool* visite = new bool[m.n];
    for (int i = 0; i < m.n; i++) {
        visite[i] = false;
        composantes[i] = -1;
    }

    int nb_composantes = 0;

    // Pour chaque île non visitée, lancer un parcours
    for (int i = 0; i < m.n; i++) {
        if (!visite[i]) {
            marquer_composante(m, i, composantes, nb_composantes, visite);
            nb_composantes++;
        }
    }

    delete[] visite;
    return nb_composantes;
}


void relier_composantes(Map& m) {
    int* composantes = new int[m.n];
    int nb_composantes = detecter_composantes_connexes(m, composantes);

    // Si tout est déjà connecté, on ne fait rien
    if (nb_composantes <= 1) {
        delete[] composantes;
        return;
    }

    // Pour chaque paire de composantes, trouver les deux îles les plus proches
    for (int comp1 = 0; comp1 < nb_composantes - 1; comp1++) {
        // Trouver la plus proche île d'une autre composante
        double distance_min = 999999;
        int ile1_choisie = -1;
        int ile2_choisie = -1;

        // Parcourir toutes les îles de la composante comp1
        for (int i = 0; i < m.n; i++) {
            if (composantes[i] == comp1) {
                // Chercher l'île la plus proche dans une autre composante
                for (int j = 0; j < m.n; j++) {
                    if (composantes[j] > comp1) { // Autre composante non encore reliée
                        double dist = distance(m, i, j);
                        if (dist < distance_min) {
                            distance_min = dist;
                            ile1_choisie = i;
                            ile2_choisie = j;
                        }
                    }
                }
            }
        }

        // Créer le pont entre les deux îles les plus proches
        if (ile1_choisie != -1 && ile2_choisie != -1) {
            m.ponts[ile1_choisie][ile2_choisie] = 1;
            m.ponts[ile2_choisie][ile1_choisie] = 1;

            // Mettre à jour les composantes (fusionner)
            int comp_a_fusionner = composantes[ile2_choisie];
            for (int k = 0; k < m.n; k++) {
                if (composantes[k] == comp_a_fusionner) {
                    composantes[k] = comp1;
                }
            }
        }
    }

    delete[] composantes;
}



Map zizi_map(int n, int ressources_max, int largeur, int hauteur){

    load_tree_images_once();
    Map m; m.n = n;
    m.nb_joueur=nb_joueur;
    m.soldat = (int*)malloc(sizeof(int) * (m.n +20));
    m.iles_id= (int*)malloc(sizeof(int) * (m.n + 20));
    m.ponts = (int**)malloc(sizeof(int*) * (m.n +20));
    m.pos_x = (int*)malloc(sizeof(int) * (m.n+20));
    m.pos_y = (int*)malloc(sizeof(int) * (m.n+20));
    m.tour = (int*)malloc(sizeof(int)*(m.n+20));

    // Taille des îles (rayon)
    int rayon_ile = 40;
    // Distance minimale entre les îles
    int dist_min = 3 * rayon_ile;

    for(int i = 0; i < n; i++){
        // Attribution des ressources
        m.soldat[i] = 5 + rand() % ressources_max;


        // Génération de position avec vérification de distance
        bool position_valide = false;
        int essais = 0;
        int max_essais = 100; // Éviter une boucle infinie

        while (!position_valide && essais < max_essais) {
            // Générer une position potentielle
            int x = rayon_ile + rand() % (largeur - 2 * rayon_ile);
            int y = rayon_ile + rand() % (hauteur - 2 * rayon_ile);

            // Vérifier la distance avec toutes les îles précédentes
            position_valide = true;
            for (int j = 0; j < i; j++) {
                if (distance(x, y, m.pos_x[j], m.pos_y[j]) < dist_min) {
                    position_valide = false;
                    break;
                }
            }

            if (position_valide) {
                m.pos_x[i] = x;
                m.pos_y[i] = y;
            }

            essais++;
        }

        // Si impossible de trouver une position valide après max_essais
        if (!position_valide) {
            // Fallback: placer l'île quelque part même si proche d'autres
            m.pos_x[i] = rayon_ile + rand() % (largeur - 2 * rayon_ile);
            m.pos_y[i] = rayon_ile + rand() % (hauteur - 2 * rayon_ile);
        }
    }




    // génération des 5 iles au dessus : de id 10 à 14

    for(int i = 10; i < 15; i++){
        // Attribution des ressources
        m.soldat[i] = 0;

        // Génération de position avec vérification de distance
        bool position_valide = false;
        int essais = 0;
        int max_essais = 100; // Éviter une boucle infinie

        while (!position_valide && essais < max_essais) {
            // Générer une position potentielle
            int x = rayon_ile + rand() % (largeur - 2 * rayon_ile);
            int y = rayon_ile + rand() % (hauteur - 2 * rayon_ile)+ hauteur;

            // Vérifier la distance avec toutes les îles précédentes
            position_valide = true;
            for (int j = 10; j < i; j++) {
                if (distance(x, y, m.pos_x[j], m.pos_y[j]) < dist_min) {
                    position_valide = false;
                    break;
                }
            }

            if (position_valide) {
                m.pos_x[i] = x ;
                m.pos_y[i] = y;
            }

            essais++;
        }

        // Si impossible de trouver une position valide après max_essais
        if (!position_valide) {
            // Fallback: placer l'île quelque part même si proche d'autres
            m.pos_x[i] = rayon_ile + rand() % (largeur - 2 * rayon_ile);
            m.pos_y[i] = rayon_ile + rand() % (hauteur - 2 * rayon_ile) + hauteur;
        }
    }


    // génération des 5 iles à droite : de id 15 à 19

    for(int i = 15; i < 20; i++){
        // Attribution des ressources
        m.soldat[i] = 0;

        // Génération de position avec vérification de distance
        bool position_valide = false;
        int essais = 0;
        int max_essais = 100; // Éviter une boucle infinie

        while (!position_valide && essais < max_essais) {
            // Générer une position potentielle
            int x = rayon_ile + rand() % (largeur - 2 * rayon_ile) + largeur;
            int y = rayon_ile + rand() % (hauteur - 2 * rayon_ile);

            // Vérifier la distance avec toutes les îles précédentes
            position_valide = true;
            for (int j = 15; j < i; j++) {
                if (distance(x, y, m.pos_x[j], m.pos_y[j]) < dist_min) {
                    position_valide = false;
                    break;
                }
            }

            if (position_valide) {
                m.pos_x[i] = x;
                m.pos_y[i] = y;
            }

            essais++;
        }

        // Si impossible de trouver une position valide après max_essais
        if (!position_valide) {
            // Fallback: placer l'île quelque part même si proche d'autres
            m.pos_x[i] = rayon_ile + rand() % (largeur - 2 * rayon_ile) + largeur;
            m.pos_y[i] = rayon_ile + rand() % (hauteur - 2 * rayon_ile);
        }
    }



    // génération des 5 iles en bas : de id 20 à 24

    for(int i = 20; i < 25; i++){
        // Attribution des ressources
        m.soldat[i] = 0;

        // Génération de position avec vérification de distance
        bool position_valide = false;
        int essais = 0;
        int max_essais = 100; // Éviter une boucle infinie

        while (!position_valide && essais < max_essais) {
            // Générer une position potentielle
            int x = rayon_ile + rand() % (largeur - 2 * rayon_ile);
            int y = rayon_ile + rand() % (hauteur - 2 * rayon_ile) - hauteur;

            // Vérifier la distance avec toutes les îles précédentes
            position_valide = true;
            for (int j = 20; j < i; j++) {
                if (distance(x, y, m.pos_x[j], m.pos_y[j]) < dist_min) {
                    position_valide = false;
                    break;
                }
            }

            if (position_valide) {
                m.pos_x[i] = x ;
                m.pos_y[i] = y ;
            }

            essais++;
        }

        // Si impossible de trouver une position valide après max_essais
        if (!position_valide) {
            // Fallback: placer l'île quelque part même si proche d'autres
            m.pos_x[i] = rayon_ile + rand() % (largeur - 2 * rayon_ile);
            m.pos_y[i] = rayon_ile + rand() % (hauteur - 2 * rayon_ile) -hauteur;
        }
    }

    // génération des 5 iles en bas : de id 25 à 29

    for(int i = 25; i < 30; i++){
        // Attribution des ressources
        m.soldat[i] = 0;

        // Génération de position avec vérification de distance
        bool position_valide = false;
        int essais = 0;
        int max_essais = 100; // Éviter une boucle infinie

        while (!position_valide && essais < max_essais) {
            // Générer une position potentielle
            int x = rayon_ile + rand() % (largeur - 2 * rayon_ile) - largeur;
            int y = rayon_ile + rand() % (hauteur - 2 * rayon_ile);

            // Vérifier la distance avec toutes les îles précédentes
            position_valide = true;
            for (int j = 25; j < i; j++) {
                if (distance(x, y, m.pos_x[j], m.pos_y[j]) < dist_min) {
                    position_valide = false;
                    break;
                }
            }

            if (position_valide) {
                m.pos_x[i] = x;
                m.pos_y[i] = y;
            }

            essais++;
        }

        // Si impossible de trouver une position valide après max_essais
        if (!position_valide) {
            // Fallback: placer l'île quelque part même si proche d'autres
            m.pos_x[i] = rayon_ile + rand() % (largeur - 2 * rayon_ile)- largeur;
            m.pos_y[i] = rayon_ile + rand() % (hauteur - 2 * rayon_ile);
        }
    }



    for (int i=0;i<30;i++){
        m.iles_id[i]=0;
    }

    for(int i = 0; i < m.n + 20; i++) {
        m.ponts[i] = (int*)malloc(sizeof(int) * (m.n + 20)) ;
        m.tour[i]=0;
        for(int j = 0; j < m.n + 20; j++) {
            m.ponts[i][j] = 0;
        }
        m.ponts[i][i] = 1; // Une île est toujours connectée à elle-même
    }

    // Créer des ponts entre les îles proches avec un facteur aléatoire
    for(int i = 0; i < m.n +20; i++) {
        for(int j = i+1; j < m.n +20; j++) {
            double dist = distance(m.pos_x[i], m.pos_y[i], m.pos_x[j], m.pos_y[j]);

            // Créer un pont seulement si les îles sont suffisamment proches ET avec une certaine probabilité
            if(dist <= dist_max_pont && (float)rand()/RAND_MAX < proba_pont) {
                m.ponts[i][j] = 1;
                m.ponts[j][i] = 1; // Symétrie
            }
        }
    }

    for (int j=0; j<m.n+20; j++){
        if (is_island_alone(m,j)){
            int nearest = find_nearest_island(m,j);
            m.ponts[j][nearest]=1;
            m.ponts[nearest][j]=1;
        }
    }



    m.trees = new tree[(m.n+20)*3];
    generate_random_trees(m.trees, m.pos_x, m.pos_y, m.n+20, larg_ile, haut_ile);


    m.n = m.n+20;

    relier_composantes(m);

    return m;
}

// Fonction pour libérer la mémoire allouée pour la carte
void liberer_map(Map& m) {
    for(int i = 0; i < m.n; i++) {
        free(m.ponts[i]);
    }
    free(m.ponts);
    free(m.iles_id);
    free(m.soldat);
    free(m.pos_x);
    free(m.pos_y);
    delete[] m.trees;
}

// Fonction pour dessiner la carte
void dessiner_map(Map& m, const Joueur* joueurs, int nb_joueurs) {
    clearWindow();

    // Dessiner les ponts
    fillRect(0, 0, largeur, hauteur, Imagine::Color(0, 160, 160));
    Color couleur = BLACK;
    for(int i = 0; i < m.n; i++) {
        for(int j = 0; j < m.n; j++){
            if (m.ponts[i][j]>0){
                drawLine(m.pos_x[i], m.pos_y[i],
                         m.pos_x[j], m.pos_y[j],
                         couleur, 3);
            }
        }

    }



    // Dessiner les îles
    for (int i = 0; i < m.n; i++) {
        draw_island(m.pos_x[i], m.pos_y[i], larg_ile, haut_ile, i);

        // Numéro de l'île
        //drawString(m.pos_x[i] - 5, m.pos_y[i], std::to_string(i), BLACK);

        // Ressources disponibles
        drawString(m.pos_x[i] - 40, m.pos_y[i] + 25,
                   "Soldat:" + std::to_string(m.soldat[i]), BLACK);
    }

    for (int i = 0; i < 3 * m.n; i++) {
        draw_tree(m.trees[i].x, m.trees[i].y, 1, m.trees[i].type);
    }

    // Dessiner les joueurs
    // Dessiner les drapeaux pour les joueurs
    for(int i = 0; i < nb_joueurs; i++) {
        for(int j = 0; j < nb_iles; j++){
            if (m.iles_id[j] == joueurs[i].id){
                // Choisir la couleur du drapeau selon le joueur
                int flag_color = 0; // 0 = pas de drapeau
                if (joueurs[i].id == 1) flag_color = 3; // Rouge pour joueur 1
                if (joueurs[i].id == 2) flag_color = 2; // Bleu pour joueur 2

                draw_flag(m.pos_x[j], m.pos_y[j], flag_color);
            }
        }
    }
    instructions();
}




bool se_deplacer(Map& m, Joueur& j,int depart, int dest,int nb_soldat){
    if (m.iles_id[depart]==j.id && m.iles_id[dest]==j.id){
        if (m.soldat[depart]>=nb_soldat && m.ponts[depart][dest]>0){
            m.soldat[depart]-=nb_soldat;
            m.soldat[dest]+=nb_soldat;
            return true;
        }
    }
    return false;
}




int choix_action(Window& w){
    int touche = -1 ;
    bool invalid = true ;
    std::cout << "En attente d'une touche..." << std::endl;
    while(invalid){
        //anyGetKey(w,touche,true);
        touche = getKey();
        std::cout << "Touche detectee: " << touche << std::endl;
        if (touche==DEPLACER){
            invalid=false;
            return DEPLACER;
            }

        if (touche==RAPH){
            invalid=false;
            return RAPH;
        }
        if(touche>48 && touche<58){
            return (touche-48);
        }
        if (touche == TOUR_SUIVANT){
            invalid=false;
            return TOUR_SUIVANT;
        }
        if (touche == PLACER){
            invalid = false;
            return PLACER;
        }
        if (touche == ATTAQUER){
            invalid = false;
            return ATTAQUER;
        }
        if (touche == DROITE){
            invalid = false;
            return DROITE;
        }
        if (touche == GAUCHE){
            invalid = false;
            return GAUCHE;
        }

        if (touche == HAUT){
            invalid = false;
            return HAUT;
        }
        if (touche == BAS){
            invalid = false;
            return BAS;
        }
        if (touche == RAJOUTER_TOUR){
            invalid = false;
            return RAJOUTER_TOUR;
        }

    }
}

int choix_nombre(Window& w){
    int touche = -1 ;
    bool invalid = true ;
    std::cout << "En attente d'un nombre..." << std::endl;
    while(invalid){
        //anyGetKey(w,touche,true);
        touche = getKey();
        std::cout << "Touche detectee: " << touche << std::endl;
        if(touche>48 && touche<58){
            return (touche-48);
        }
        return 0;
    }

}

int get_destination(const Map& m) {
    // Attendre un clic de l'utilisateur
    int x, y;
    getMouse(x, y);

    // Vérifier si le clic est sur une île
    for (int i = 0; i < m.n; i++) {
        // Calculer la distance entre le clic et le centre de l'île
        double dist = distance(x, y, m.pos_x[i], m.pos_y[i]);

        // Si la distance est inférieure au rayon de l'île, le clic est sur cette île
        if (dist <= rayon_ile) {
            return i; // Retourner l'indice de l'île cliquée
        }
    }

    // Si aucune île n'a été cliquée,
    return get_destination(m);
}



void nouveau_soldat(Map& m , Joueur* j, Window& w, int id_joueur){

    int new_soldat = get_nb_ile(m,j[id_joueur]);
    std::cout<<new_soldat<<std::endl;

    while (new_soldat>0){
        std::cout<<"Attente d'un clique"<<std::endl;
        int ile = get_destination(m);
        std::cout<<"Attente d'un nombre"<<std::endl;
        int nb = choix_nombre(w);
        if (nb<=new_soldat){
            new_soldat-=nb;
            m.soldat[ile]+=nb;
        dessiner_map(m,j,nb_joueur);
        }
    }
}


int attaquer(Map& m, Joueur* j, int depart, int dest) {
    // Paramètres initiaux du jeu
    int id_att = m.iles_id[depart];
    int id_def = m.iles_id[dest];

    // Gérer les tours
    if (m.soldat[dest] == 0 && m.tour[dest] == id_def) {
        m.soldat[dest] = nombre_soldats_tour;
        m.tour[dest] = 0;
    }
    clearWindow();

    // Créer et lancer le jeu
    Partie combat(id_att, m.soldat[depart], id_def, m.soldat[dest]);
    combat.run();
    combat.fin_combat();  // Remplacer nb_soldat par fin_combat

    clearWindow();

    // Logique de conquête d'île simplifiée
    if (m.soldat[dest] == 0 && m.tour[dest] == 0) {
        m.iles_id[dest] = id_att;  // L'attaquant conquiert l'île
    }

    return 0;
}


void action(Map& m, Joueur* j, Window& w, int id_joueur){

    int action = choix_action(w) ;
    bool already_placed = true;

    while (action != TOUR_SUIVANT){
        bool is_action_done=false;
        if(action == DEPLACER){
            int depart=get_destination(m) ;
            int dest = get_destination(m) ;
            int nb_soldat=choix_nombre(w);
            bool is_action_done=se_deplacer(m, j[id_joueur], depart, dest, nb_soldat) ;
        }

        if (action == PLACER && already_placed){
            nouveau_soldat(m,j,w, id_joueur);
            already_placed=false;
        }
        if (action == ATTAQUER){
            int depart=get_destination(m) ;
            int dest = get_destination(m) ;
            int ok = attaquer(m,j, depart, dest);
        }

        if (action == DROITE){
            droite_ile(m);
        }
        if (action == GAUCHE){
            gauche_ile(m);
        }
        if (action == HAUT){
            monter_ile(m);
        }
        if (action == BAS){
            descendre_ile(m);
        }
        if (action == RAJOUTER_TOUR && (j->ressources)>=prix_tour){
            int dest = get_destination(m);
            m.tour[dest]=id_joueur;
            (j->ressources) -= prix_tour;
        }

        dessiner_map(m, j,nb_joueur);
        action = choix_action(w);
    }
}


void choix_iles_depart(Map&m, Joueur joueurs[2]){
    for (int i=0; i<nb_iles/2; i++){
        for (int j=0; j<m.nb_joueur;j++){
            int ile = -1;

            std::cout << "en attente d'un clique" << std::endl;
            while (ile==-1){
                ile=get_destination(m);
                milliSleep(1);
                if (m.iles_id[ile]==0){
                    m.iles_id[ile]=j+1;
                }
                else{
                        ile=-1;
                    }

            }
            dessiner_map(m,joueurs,nb_joueur);
        }

    }
}

int get_nb_soldat(Map m, Joueur j){
    int id = j.id;
    int nb_soldat = 0;
    for (int i=0; i<m.n ; i++){
        if (m.iles_id[i]==id){
            nb_soldat+= m.soldat[i];
        }
    }
    return nb_soldat;
}


int game1(){
    srand(time(NULL)); // Initialisation de la graine aléatoire


    // Créer la fenêtre
    Window w = openWindow(largeur+largeur_panneau, hauteur, "Jeu des Îles");

        // Initialiser la carte
        Map m = zizi_map(nb_iles, 5, largeur, hauteur);

    // Initialiser les joueurs
    Joueur joueurs[2];
    joueurs[0] = {1, 0,RED};
    joueurs[1] = {2, 0, BLUE};

    // Afficher la carte
    dessiner_map(m, joueurs, nb_joueur);
    choix_iles_depart(m,joueurs);

    // Attendre un clic pour quitter
    //click();

    while (true){
        for (int id_joueur = 0; id_joueur< nb_joueur; id_joueur++){
            action(m,joueurs, w, id_joueur);
            dessiner_map(m,joueurs, nb_joueur);
        }
    }


    // Nettoyer la mémoire avant de quitter
    liberer_map(m);
    closeWindow(w);
    int x=0; //suppr
    return x;


}




int main() {


    std::cout << "Début du programme" << std::endl;
    srand(time(NULL));

    std::cout << "Création de la fenêtre" << std::endl;
    Window w = openWindow(largeur+largeur_panneau, hauteur, "Jeu des Îles");

    std::cout << "Création de la carte" << std::endl;
    Map m = zizi_map(nb_iles, 5, largeur, hauteur);


    // Initialiser les joueurs
    Joueur joueurs[2];
    joueurs[0] = {1, 0,RED,0};
    joueurs[1] = {2, 0, BLUE,0};

    // Afficher la carte
    dessiner_map(m, joueurs, nb_joueur);
    choix_iles_depart(m,joueurs);

    // Attendre un clic pour quitter
    //click();

    while (true){
        for (int id_joueur = 0; id_joueur< nb_joueur; id_joueur++){
            action(m,joueurs, w, id_joueur);
            dessiner_map(m,joueurs, nb_joueur);
        }
    }


    // Nettoyer la mémoire avant de quitter
    liberer_map(m);
    closeWindow(w);
    int x=0; //suppr
    return x;


}
