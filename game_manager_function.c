#include "Carcassonne.h"

// ------------------------------
// ----Game manager fonctions----
// ------------------------------

struct Tile **create_tile_array(FILE *stream) // A FAIRE
/*
    Crée la liste des tuiles à partir d'un fichier csv. AUTRE OPTION : hard code à la main la liste
*/
{
    char *filename = CSV_TILE;
    FILE *file = fopen(filename, "r");
    char buff[2048]; // sp plus pourquoi il faut mettre une puissance de 2 mais un ACU m'a dit que c'était mieux
    struct Tile **tileArray = malloc(sizeof(struct Tile) * NBTILE + 1);
    tileArray[NBTILE] = NULL; // Pour pouvoir itérer sur la liste en sachant quand s'arrêter (sur le NULL)
    size_t index = 0;
    size_t indexTmp = 0;

    while(fgets(buff, 2048, file) != EOF) // on utilise fgets parce que flemme d'optimiser le buffer moi même afin d'éviter trop de syscall
    {
        while(*(buff + index) != '\0') // si problème soit remplacer par un do while soit copier coller le contenu en dehors
        {
            indexTmp = index;
            while(*(buff + indexTmp) != ',')
            {
                // voir bib standard je pense
            }
            index++;
        }
    }

    fclose(CSV_TILE);
    return;
};


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

void array_to_stack(struct Tile **tileArray, struct Stack *stack); // A FAIRE
/*
    Prend en entrée une array et return une pile avec les éléments de l'array dedans.
*/
void reset_points(struct Player *player); // A FAIRE
/*
    Réinitialise les points du joueur
*/

void reset_meeples(struct Player *player); // A FAIRE
/*
    Réinitialise le compteur de Meeple du joueur
*/

void next_turn(char nbPlayers, char actualPlayerTurn); // A FAIRE
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