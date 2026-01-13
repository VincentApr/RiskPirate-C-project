#include "iles.h"
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()


Imagine::AlphaColor* Map::cocotier_images[7] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
int Map::cocotier_widths[7] = {0, 492, 480, 441, 435, 483, 447};
int Map::cocotier_heights[7] = {0, 705, 805, 639, 779, 738, 744};
bool Map::images_loaded = false;


Imagine::AlphaColor* Map::flag_images[4] = {nullptr, nullptr, nullptr, nullptr};
int Map::flag_widths[4] = {0, 256, 256, 256};
int Map::flag_heights[4] = {0, 256, 256, 256};


void load_tree_images_once() {
    if (Map::images_loaded) return;  // Déjà chargées

    std::cout << "Chargement des images des cocotiers..." << std::endl;

    // Charger cocotier1
    if (!Imagine::loadAlphaColorImage(srcPath("./cocotiers/cocotier1.png"),
                                      Map::cocotier_images[1],
                                      Map::cocotier_widths[1],
                                      Map::cocotier_heights[1])) {
        std::cerr << "Erreur chargement cocotier1.png" << std::endl;
    }

    // Charger cocotier2
    if (!Imagine::loadAlphaColorImage(srcPath("./cocotiers/cocotier2.png"),
                                      Map::cocotier_images[2],
                                      Map::cocotier_widths[2],
                                      Map::cocotier_heights[2])) {
        std::cerr << "Erreur chargement cocotier2.png" << std::endl;
    }

    // Charger cocotier3
    if (!Imagine::loadAlphaColorImage(srcPath("./cocotiers/cocotier3.png"),
                                      Map::cocotier_images[3],
                                      Map::cocotier_widths[3],
                                      Map::cocotier_heights[3])) {
        std::cerr << "Erreur chargement cocotier3.png" << std::endl;
    }

    // Charger cocotier4
    if (!Imagine::loadAlphaColorImage(srcPath("./cocotiers/cocotier4.png"),
                                      Map::cocotier_images[4],
                                      Map::cocotier_widths[4],
                                      Map::cocotier_heights[4])) {
        std::cerr << "Erreur chargement cocotier4.png" << std::endl;
    }

    // Charger cocotier5
    if (!Imagine::loadAlphaColorImage(srcPath("./cocotiers/cocotier5.png"),
                                      Map::cocotier_images[5],
                                      Map::cocotier_widths[5],
                                      Map::cocotier_heights[5])) {
        std::cerr << "Erreur chargement cocotier5.png" << std::endl;
    }

    // Charger cocotier6
    if (!Imagine::loadAlphaColorImage(srcPath("./cocotiers/cocotier6.png"),
                                      Map::cocotier_images[6],
                                      Map::cocotier_widths[6],
                                      Map::cocotier_heights[6])) {
        std::cerr << "Erreur chargement cocotier6.png" << std::endl;
    }


    std::cout << "Chargement des drapeaux..." << std::endl;
    Imagine::loadAlphaColorImage(srcPath("./cocotiers/grayflag.png"), Map::flag_images[1], Map::flag_widths[1], Map::flag_heights[1]);
    Imagine::loadAlphaColorImage(srcPath("./cocotiers/blueflag.png"), Map::flag_images[2], Map::flag_widths[2], Map::flag_heights[2]);
    Imagine::loadAlphaColorImage(srcPath("./cocotiers/redflag.png"), Map::flag_images[3], Map::flag_widths[3], Map::flag_heights[3]);


    Map::images_loaded = true;
    std::cout << "Images des cocotiers chargées !" << std::endl;


}

void draw_tree(int tree_x, int tree_y, int tree_size, int tree_type){
    if (tree_type == 0 || tree_type > 6) {
        return;
    }

    // S'assurer que les images sont chargées
    if (!Map::images_loaded) {
        load_tree_images_once();
    }

    float scale_factor = 0.1;

    // Utiliser les images déjà chargées en mémoire
    if (Map::cocotier_images[tree_type] != nullptr) {
        int img_width = Map::cocotier_widths[tree_type];
        int img_height = Map::cocotier_heights[tree_type];

        float delta_x, delta_y;

        switch(tree_type) {
        case 1:
            delta_x = (scale_factor * img_width * 0.78);
            delta_y = (scale_factor * img_height);
            break;
        case 2:
            delta_x = (scale_factor * img_width * 0.42);
            delta_y = (scale_factor * img_height * 1);
            break;
        case 3:
            delta_x = (scale_factor * img_width * 0.4);
            delta_y = (scale_factor * img_height * 0.95);
            break;
        case 4:
            delta_x = (scale_factor * img_width * 0.52);
            delta_y = (scale_factor * img_height * 0.92);
            break;
        case 5:
            delta_x = (scale_factor * img_width * 0.41);
            delta_y = (scale_factor * img_height * 0.92);
            break;
        case 6:
            delta_x = (scale_factor * img_width * 0.41);
            delta_y = (scale_factor * img_height * 0.92);
            break;
        default:
            return;
        }

        Imagine::putAlphaColorImage(tree_x - delta_x, tree_y - delta_y,
                                    Map::cocotier_images[tree_type],
                                    img_width, img_height, false, scale_factor);
    }
}



void draw_random_tree(int tree_x, int tree_y, int tree_size){
    int random_tree_type =1 + rand() % 6;
    draw_tree(tree_x,tree_y,tree_size,random_tree_type);
}


void generate_random_trees(tree* outp_list, int* isl_x, int* isl_y, int nb_isl, int isl_width, int isl_height) {
    int minDistance = 50;
    int treeX, treeY;
    bool validPosition;

    int rand_height;
    int rand_width;
    // Allocate memory for the output array of trees
    std::cout << "test" << std::endl;

    // Seed the random number generator
    srand(time(NULL));
    int treeAreaWidth = isl_width / 2;   // only use the central half
    int treeAreaHeight = isl_height / 2;


    for (int i = 0; i < nb_isl; i++) {
        for (int j = 0; j < 3; j++) {
            rand_width = rand() % treeAreaWidth - treeAreaWidth / 2;
            rand_height = rand() % treeAreaHeight - treeAreaHeight / 2;

            outp_list[3*i + j].x = isl_x[i] + rand_width;
            outp_list[3*i + j].y = isl_y[i] + rand_height;
            outp_list[3*i + j].type = rand() % 7;
        }
    }

}


void draw_island(int x, int y, int width, int height, int island_id){

    // définition des couleurs
    Imagine::Color SAND(238, 214, 173); // beige/jaune pour le sable
    Imagine::Color GREEN(34, 139, 34); // vert

    // Draw the water gradient around the island
    for (int i = 10; i > 0; --i) {
        int blue = 200 - i * 4; // Gradient from light blue to darker blue
        int green = 200 - i * 4; // Gradient from light blue to darker blue
        int red = 0; // No red component
        Imagine::fillEllipse(x - width / 2 - i, y - height / 2 - i, width + 2 * i, height + 2 * i, Imagine::Color(red, green, blue));

    }

    // Draw the sandy beach
    Imagine::fillEllipse(x - width / 2, y - height / 2, width, height, SAND);

    // Randomly decide if there is grass on the island
    bool withgrass=true;
    if (withgrass) {
        fillEllipse(x - width*0.4, y - height*0.4, width*0.8 , height*0.8 , GREEN);
    }



};




void draw_random_island(int x, int y, int width, int height, int island_id, bool withgrass=false) {

    // définition des couleurs
    Imagine::Color SAND(238, 214, 173); // beige/jaune pour le sable
    Imagine::Color GREEN(34, 139, 34); // vert

    // Draw the water gradient around the island
    for (int i = 10; i > 0; --i) {
        int blue = 200 - i * 4; // Gradient from light blue to darker blue
        int green = 200 - i * 4; // Gradient from light blue to darker blue
        int red = 0; // No red component
        Imagine::fillEllipse(x - width / 2 - i, y - height / 2 - i, width + 2 * i, height + 2 * i, Imagine::Color(red, green, blue));
    }

    // Draw the sandy beach
    Imagine::fillEllipse(x - width / 2, y - height / 2, width, height, SAND);

    // Randomly decide if there is grass on the island
    if (withgrass) {
        fillEllipse(x - width*0.4, y - height*0.4, width*0.8 , height*0.8 , GREEN);
    }

    // Draw a random number of palm trees (between 1 and 3)
    int numTrees = rand() % 4; // Generates a random number between 0 and 3

    // Minimum distance between trees
    int minDistance = 50; // Adjust this value as needed

    for (int i = 0; i < numTrees; i++) {
        int treeX, treeY;
        bool validPosition;

        do {
            treeX = x + (rand() % (width / 2)) - (width / 4); // Random x-position within the island's width
            treeY = y - (rand() % (height / 2));              // Random y-position within the island's height

            // Ensure the trees are within the bounds of the island
            treeX = std::max(x - width / 2, std::min(treeX, x + width / 2));
            treeY = std::max(y - height / 2, std::min(treeY, y + height / 2));

            // Check if the new tree is too close to any existing tree
            validPosition = true;
            for (int j = 0; j < i; j++) {
                int existingTreeX = x + (rand() % (width / 2)) - (width / 4);
                int existingTreeY = y - (rand() % (height / 2));
                int distance = std::sqrt(std::pow(treeX - existingTreeX, 2) + std::pow(treeY - existingTreeY, 2));
                if (distance < minDistance) {
                    validPosition = false;
                    break;
                }
            }
        } while (!validPosition);

        draw_random_tree(treeX, treeY+(height/3), 1);
    }
}





void draw_flag(int x, int y, int flag_color){
    if (flag_color == 0 || flag_color > 3) {
        return;
    }

    // S'assurer que les images sont chargées
    if (!Map::images_loaded) {
        load_tree_images_once();
    }

    if (Map::flag_images[flag_color] != nullptr) {
        float scale_factor = 0.17;
        int place_x = x - 22;
        int place_y = y - 35;

        Imagine::putAlphaColorImage(place_x, place_y,
                                    Map::flag_images[flag_color],
                                    Map::flag_widths[flag_color],
                                    Map::flag_heights[flag_color],
                                    false, scale_factor);
    }
}

