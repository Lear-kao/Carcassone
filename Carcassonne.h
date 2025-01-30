#ifndef CARCASSONNE_H
#define CARCASSONNE_H

//#define CSV_TILE "tuiles_base_simplifiees.csv"
#define NBTILE 72

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

enum types { ROUTE, VILLE, ABBAYES, PRÉS, VILLAGE };
enum meeplePlace {NO_MEEPLE, RIGHT, TOP, LEFT, BOT, MIDDLE};

//-----------------------
// ----Data structure----
// ----------------------

// ----------
// --Stack--
// ---------

struct Stack
{
    struct Tile *data;
    struct Stack *next;
};

struct Stack *stack_push(struct Stack *stack, struct Tile *tile); // FAIT
/*
    stack : La pile
    tile : Un pointer sur un espace mémoire ou se trouve une tile à push sur la stack

    return : La stack actualisé 
*/
// Ne pas oublier de gérer la cas création de stack

struct Stack *stack_pop(struct Stack *stack, struct Tile **tileSlot); // FAIT
/*
    stack : La pile
    tile : Un pointer sur un espace mémoire ou mettre la tile pop

    return : La stack actualisé

*/

char is_stack_not_empty(struct Stack *stack); // FAIT
/*
    stack : La pile

    return : 0 si la stack est vide, 1 sinon
*/


// ------------
// ----Grid----
// ------------

struct Coord
{
    char x;
    char y;
};

struct Grid
{
    /*
        Un morceau de grille permettant de relier les tiles entre elles
    */
    struct Coord *coord;
    struct Tile *tile;
    struct Grid *right;
    struct Grid *left;
    struct Grid *bot;
    struct Grid *top;
};

// ------------
// ----Tile----
// ------------

struct Tile 
{
    /*
        Représente une tuile de carcassonne sous forme de X
          X 
        X X X
          X 
    */
    struct Player* meeple;
    enum types right;
    enum types left;
    enum types top;
    enum types bot;
    enum types middle;
};

// ----Tiles fonctions----

void init_tile(struct Tile *tile, enum types right, enum types left, enum types top, enum types bot, enum types middle); // FAIT
/*
    tile : Un pointer vers un espace mémoire déjà alloué à sizeof(struct Tile)
    right, left ... middle : les différent type des bordes de la tuile

    return : 1 en cas d'erreur, 0 sinon
*/

char is_meeple_on_tile(struct Tile *tile); // FAIT
/*
    tile : Une tile

    return :
    - 0 si aucun Meeple n'est sur la tile
    - 1 si un Meeple est sur la tile
*/

// ---------------
// ----Player-----
// ---------------
struct meeple
{
    struct tile* tuile = NULL;
    struct meeple* suiv = NULL;
}

struct Player
{
     /*
        nbMeeple : Nombre de Meeple du joueur
        points : Nombre de points du joueur
     */
    
    char nbMeeple;
    short points; 
};

// ----PLayers fonctions----

void init_player(struct Player *player); // FAIT
/*
    Innitialise l'objet player
*/

char is_meeple_on_player(struct Player *player); // FAIT
/*
    Player : L'objet Player

    return :
    - 0 si le joueur n'a aucun Meeple
    - 1 si il reste au moins un Meeple au Joueur
*/

// ------------------------------
// ----Game manager fonctions----
// ------------------------------

size_t nb_line_in_csv(char *csv_tile ); // Theo A FAIRE 

struct Tile **create_tile_array(char *csv_tile); // Theo A FAIRE
/*
    Crée la liste des tuiles à partir d'un fichier csv. AUTRE OPTION : hard code à la main la liste
*/

struct Player **create_players_array(char nbPlayers); // A FAIRE
/*
    Crée une liste de pointeurs qui pointe sur un Player,
    un pointer sur NULL est ajouté à la fin pour faciliter
    les iterations sur la liste (condition d'arrêt)
*/

void shuffle(struct Tile **tileArray, char size); // c'est peut-être mieux si size est une macro A FAIRE
/*
    tileArray : Une liste de pointeurs sur Tile.
    size : La taille de la liste (normalement 72)

    Mélange les tuiles
*/

void array_to_stack(struct Tile **tileArray, struct Stack *stack); // Fait
/*
    Prend en entrée une array et return une pile avec les éléments de l'array dedans.
*/
void reset_points(struct Player *player); // Axel
/*
    Réinitialise les points du joueur
*/

void reset_meeples(struct Player *player); // Axel
/*
    Réinitialise le compteur de Meeple du joueur
*/



void next_turn(char nbPlayers,char actualPlayerTurn); // A FAIRE
/*
    Gère les tours de jeu.
*/

void player_turn(char playerNumber); // A FAIRE
/*
    playerNumber : Le numéro du joueur

    Cette fonction pop la stack de tile
    et propose ensuite au joueur de choisir
    un emplacmement pour poser sa tuile
    avec la fonction where_i_can_play
*/

char *where_i_can_play(struct Tile *tile, struct Grid *grid); // A FAIRE
/*
    tile : La tile précedement pioché par le joueur
    grid : la grid sur laquelle doit être effectué une recherche en fonction de la tile

    return : Une liste de coordonnées sur la grille de ou il est possible de placer sa tuile
*/

void show_grid(struct Grid *grid); // A FAIRE
/*
    Affiche la grille du jeu en ascii art en minimisant l'espace occupé 
*/

void start_game(char nbPlayers, char nbBots, char *turnTraker); // arg ? // A FAIRE
/*
    Effet :
    - Réinitialise le plateau (une seule tuile au centre) (free toute les les tiles sinon par de bouton rejoué et il faut fermer et ouvrir le jeu)
    - Réinitialise les points
    - Réinitialise les Meeples 
    - écrase ou crée la liste des tuiles, les mélanges puis crée une pile
    - Réinitialise le turn tracker (le joueur 1 commence)
*/

char *end_game_points_counter(struct Grid *grid, struct Player nbPlayers); // A FAIRE
/*
    grid : la case de départ
    nbPLayers : Le nombre de joueurs

    return : Une liste de nbPLayers éléments contenant les points du joueurs 1 jusqu'à 6
*/

#endif // CARCASSONNE_H