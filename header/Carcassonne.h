#ifndef CARCASSONNE_H
#define CARCASSONNE_H

#define CSV_TILE "tuiles_base_simplifiees.csv"
#define NBTILE 72
#define BUFF_DEFAULT_SIZE 1024
#define MAX_TOKEN_SIZE 7
#define NB_TOKEN_TYPE 6
#define NBMEEPLE_DEFAULT 8  // theo 


#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum types { ROUTE, VILLE, ABBAYES, PRE, VILLAGE, BLASON, RIEN };
enum meeplePlace {NO_MEEPLE, RIGHT, TOP, LEFT, BOT, MIDDLE};


//-----------------------
// ----Data structure----
// ----------------------


// ----------
// --DLList-- 
// ----------


// DLList : Linked list

struct DLList
{
    struct Tile *data;
    struct DLList *prev;
    struct DLList *next;
};


struct DLList *DLList_push_end(struct DLList *DLList, struct Tile *tile);
/*
    DLList : La liste chainé
    tile : Un pointer sur un espace mémoire ou se trouve une tile à à la fin de la liste chainé

    return : La liste actualisé 
*/


void DLList_pop(struct DLList **DLList, struct Tile **tileSlot);
/*
    DLList : L'élément de la liste chainé à supprimer
    tileSlot : Un pointer sur un espace mémoire ou mettre la tile pop

    return : La liste actualisé 
*/


// ----------
// --Stack--
// ---------

struct Stack
{
    struct Tile *data;
    struct Stack *next;
};

struct Stack *stack_push(struct Stack *stack, struct Tile *tile); // FAIT theo
/*
    stack : La pile
    tile : Un pointer sur un espace mémoire ou se trouve une tile à push sur la stack

    return : La stack actualisé 
*/
// Ne pas oublier de gérer la cas création de stack

struct Stack *stack_pop(struct Stack *stack, struct Tile **tileSlot); // FAIT theo
/*
    stack : La pile
    tileSlot : Un pointer sur un espace mémoire ou mettre la tile pop

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
    int x;
    int y;
};

struct Coord *init_coord(unsigned char x, unsigned char y);

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


struct Grid *init_grid(struct Tile *tile, struct Coord *coord, struct Grid *right, struct Grid *left, struct Grid *bot, struct Grid *top);

struct Coord *init_coord(unsigned char x, unsigned char y);
/*
    x: coordonnée x
    y: coordonnee y

    Cette fonction alloue une struct coord et l'initialise
    avec les coordonnée x et y

    return: un pointeur vers un struct Coord
*/


/* NON UTILISE
//-------------
//-plateboard--
//-------------
struct l_ch
{
    struct Grid *place;
    struct l_ch *col;
    struct l_ch *line;
}
*/
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
    enum meeplePlace meeplePlace;
    enum types right;
    enum types left;
    enum types top;
    enum types bot;
    enum types middle;
};

// ----Tiles fonctions----

struct Tile *init_tile(enum types right, enum types top, enum types left, enum types bot, enum types middle); // fait Théo
    /*
    tile : Un pointer sur Tile
    right, left ... middle : les différent type des bordes de la tuile
     */

char is_meeple_on_tile(struct Tile *tile); // FAIT theo
/*
    tile : Une tile

    return :
    - 0 si aucun Meeple n'est sur la tile
    - 1 si un Meeple est sur la tile
*/

// ---------------
// ----Player-----
// ---------------

struct Player
{
     /*
        nbMeeple : Nombre de Meeple du joueur
        points : Nombre de points du joueur
     */  
    char nbMeeple;
    short points; 
};

struct list_player
{
    /* 
    Contient la liste des joueurs, utilisé  pour lancer la partie, 
    vas  aussi  permettre d'y ajouer les bots plus tards
    */
    struct Player **player;
};

// ----PLayers fonctions----

void init_player(struct Player *player); // FAIT theo
/*
    Innitialise l'objet player
*/

char is_meeple_on_player(struct Player *player); // FAIT Theo/Axel
/*
    Player : L'objet Player

    return :
    - 0 si le joueur n'a aucun Meeple
    - 1 si il reste au moins un Meeple au Joueur
*/

// ------------------------------
// ----Game manager fonctions----
// ------------------------------

char token_to_enum_types(char *token, char *tokenArray[]); // theo si qq veut faire un mod Carcasonne avec de nouvelles tuiles il doit modifier cette fonction
/*
    La fonction effectue une bijection entre l'enum types et tokenArray
    
    token : la chaine de charactère à annalyser
    tokenArray : La liste des différents tokens possible

    return: 
    Le type de la case associé au token.
*/

struct Tile **create_tile_array(char *csvTile, char *tokenArray[], char maxTokenSize); // fait Theo
/*
    Crée la liste des tuiles à partir d'un fichier csv.

    csvTile : Le nom du fichier .csv à ouvrir (utiliser la macro CSV_TILE).
    tokenArray : La liste des token correspondant 1 à 1 aux éléments de la macro types 

    return : 
    Une tileArray contenant les objets Tile innitialisé

    Attention ne pas allouez la mémoire pour struct tile **tileArray
*/

struct list_player *init_player_list(char nbPlayers); // Fait (Axel) 
/*
    Crée une liste de pointeurs qui pointe sur un Player,
    un pointer sur NULL est ajouté à la fin pour faciliter
    les iterations sur la liste (condition d'arrêt)
*/

void shuffle(struct Tile **tileArray, char size); // Valentin c'est peut-être mieux si size est une macro A FAIRE
/*S
    tileArray : Une liste de pointeurs sur Tile.
    size : La taille de la liste (normalement 72)

    Mélange les tuiles
*/

void array_to_stack(struct Tile **tileArray, struct Stack **stack); // Fait
/*
    Prend en entrée une array et return une pile avec les éléments de l'array dedans. ya pas de return !!!
*/

void reset_points(struct Player *player); //Fait

/*
    Réinitialise les points du joueur
*/

void reset_meeples(struct Player *player);//Fait

/*
    Réinitialise le compteur de Meeple du joueur
*/

struct Tile *rot_tile(struct Tile *tile); //Fait
/*
    Tourne la tuile de 90° dans le sens trigo.
*/

void enum_to_char(enum types type); //Fait
/*
    Convertie un enum en char affichable (V pour ville et v pour village).
*/

void show_tile(struct Tile *tile); // A TESTER theo
/*
    Affiche une tile.
*/

char is_a_potential_tile(struct Tile *tile) // Axel peut-être rajouter en argument la liste des tuiles potentiels utilisé dans where_i_can_place
/*
    Return 0 si ce n'est pas une tuile potentielle
    Return 1 si c'est une tuile potentielle

    Information : Une tuile potentielle est une tuile parciellement remplie de enum qui sert pour la fonction where_i_can_place.
*/

short points_route(struct Grid *grid); // Axel
/*
    grid : Une grid qui pointe sur une tuile complète (pas une tuile potentiel) donc oublie pas de vérifier si c'est une tuile potentiel avant d'aller dessus.

    return value :
    Les conditions pour que les points soit rajouté aux joueurs ne sont pas remplie
    return 0
    ne pas retirer de meeple

    Les conditions pour que les points soit rajouté aux joueurs sont remplie
    return le nombre de point gagné par ce joueurs. 

    ATTENTION : Que faire si plusieurs joueurs gagne des points ? Comment les différencier ? return une liste de couple joueurs points peut-être ?
*/
short points_ville(struct Grid *grid); // Axel et blason
short points_abbayes(struct Grid *grid); // Axel
short points_pre(struct Grid *grid); // Axel





ROUTE, VILLE, ABBAYES, PRE, VILLAGE, BLASON, RIEN
struct Grid *place_tile(struct Grid *grid, struct Coord *coord, struct Tile *tile); // A TESTER
/*
    tile : La tile précedement pioché par le joueur à placer

    grid : Un tableau de grid sur laquelle doit être effectué une recherche
    en fonction de la tile. Les tuiles non posé sont soit des tuiles potentiel 
    soit des pointeurs sur NULL

    coord : Les coordonnées de l'endroit ou placer la tuile sur ***grid

    Place la tuile à l'emplacement indiqué.
*/

void player_turn(char playerNumber, struct Player **PlayerArray, struct Stack *pioche, struct Grid **grid, unsigned int nb_coord); // A FAIRE
/*
    playerNumber : Le numéro du joueur

    Cette fonction pop la stack de tile
    et propose ensuite au joueur de choisir
    un emplacmement pour poser sa tuile
    avec la fonction where_i_can_play
*/

struct Coord **where_i_can_play(struct Tile *tile, struct Grid *grid); // A FAIRE theo
/*
    tile : La tile précedement pioché par le joueur

    grid : Un tableau de grid sur laquelle doit être effectué une recherche
    en fonction de la tile. Les tuiles non posé sont soit des tuiles potentiel 
    soit des pointeurs sur NULL

    return : Une liste de coordonnées sur la grille de ou il est possible de placer sa tuile se terminant par NULL
    ATTENTION vous n'avez pas à gérer les rotations ici
*/


void show_grid( struct Grid *tab, unsigned char x, unsigned char  y ); // A tester ( Valentin )
/*
    Affiche la grille du jeu en ascii art en minimisant l'espace occupé 
*/

struct Tile *start_game(struct list_player **list_player, char nbPlayerchar, char *turnTraker,  struct Grid *grid); // fait (Axel)
/*
    Effet :
    - Réinitialise le plateau (une seule tuile au centre) (free toute les les tiles sinon par de bouton rejoué et il faut fermer et ouvrir le jeu)
    - Réinitialise les points
    - Réinitialise les Meeples 
    - écrase ou crée la liste des tuiles, les mélanges puis crée une pile
    - Réinitialise le turn tracker (le joueur 1 commence)
    !!! a voir si il faut pas passer la  liste de  struct  tile  en paramètre
*/


char *end_game_points_counter(struct Grid *grid, struct Player nbPlayers); // A FAIRE
/*
    grid : la case de départ
    nbPLayers : Le nombre de joueurs

    return : Une liste de nbPLayers éléments contenant les points du joueurs 1 jusqu'à 6
*/

void free_Grid( struct Grid *grid); // fait (Axel)
/* 
prend en paramètre une struct grid initialisée et la free pour être réutilisée
*/

#endif // CARCASSONNE_H