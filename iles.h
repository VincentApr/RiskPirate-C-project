#pragma once

#include <Imagine/Images.h>
#include <Imagine/Graphics.h>
#include <iostream>
#include <cmath>

const int nmax=30;
const int ressources_max=100;

struct tree{
    int x;
    int y;
    int type;
};


struct Map{
    int nb_joueur;
    int n; //nombre iles
    int* soldat;// pour chaque ile le nombre de soldat
    int* iles_id; // pour chaque ile, l'id du joueur qui la possède : commence à 1
    int** ponts; //matrice d'adj avec 1 si il y a un pont 0 sinon
    int* pos_x; // Position x de chaque île
    int* pos_y; // Position y de chaque île
    int * tour;
    tree* trees;


    static Imagine::AlphaColor* cocotier_images[7];  // Cache pour les images
    static int cocotier_widths[7];                   // Largeurs des images
    static int cocotier_heights[7];                  // Hauteurs des images

    static Imagine::AlphaColor* flag_images[4];  // 0=null, 1=gris, 2=bleu, 3=rouge
    static int flag_widths[4];
    static int flag_heights[4];

    static bool images_loaded;                       // Flag pour savoir si chargées
};



void draw_tree(int tree_x, int tree_y, int tree_size, int tree_type);

void draw_random_tree(int tree_x, int tree_y, int tree_size);

void generate_random_trees(tree* outp_list, int* isl_x, int* isl_y, int nb_isl, int isl_width, int isl_height);

void draw_island(int x, int y, int width, int height, int island_id);

void draw_random_island(int x, int y, int width, int height, int island_id, bool withgrass);

void draw_flag(int x, int y, int flag_color);

void load_tree_images_once();

