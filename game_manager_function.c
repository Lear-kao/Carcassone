#include "Carcassonne.h"

// ------------------------------
// ----Game manager fonctions----
// ------------------------------



char token_to_enum_types(char *token, char *tokenArray[]) // si qq veut faire un mod Carcasonne avec de nouvelles tuiles il doit modifier cette fonction
/*
    La fonction effectue une bijection entre l'enum types et tokenArray
    
    token : la chaine de charactère à annalyser
    tokenArray : La liste des différents tokens possible

    return: 
    Le type de la case associé au token.
*/
{
    char i = 0;
    char find = 0;
    while (find != 0)
    {
        if (strcmp((*tokenArray + i), token) == 0)
        {
            find = i;
        }
        i++;
    }
    return find;
};

struct Tile **create_tile_array(char *csvTile, char *tokenArray[], char maxTokenSize) // A FAIRE
/*
    Crée la liste des tuiles à partir d'un fichier csv.

    csvTile : Le nom du fichier .csv à ouvrir (utiliser la macro CSV_TILE).
    tokenArray : La liste des token correspondant 1 à 1 aux éléments de la macro types 

    return : 
    Une tileArray contenant les objets Tile innitialisé

    Attention ne pas allouez la mémoire pour struct tile **tileArray
*/
{
    char *filename = csvTile;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return NULL; // error case
    }
    char buff[BUFF_DEFAULT_SIZE];
    struct Tile **tileArray = malloc(sizeof(struct Tile *) * (NBTILE + 1));
    *(tileArray + NBTILE) = NULL;
    tileArray[NBTILE] = NULL; // Pour pouvoir itérer sur la liste en sachant quand s'arrêter (sur le NULL)
    char *token = malloc(sizeof(char) * maxTokenSize);
    size_t index = 0;
    char *typesArray = malloc(sizeof(char) * NB_TOKEN_TYPE); // liste de NB_TOKEN_TYPE token de "types enum"

    while (fgets(buff, BUFF_DEFAULT_SIZE, file) != NULL && index <= NBTILE)
    {
        token = strtok(buff, ",");
        for (char i = 0; i < NB_TOKEN_TYPE && token != NULL; i++)
        {
            token = strtok(NULL, ",");
            token_to_enum_types(token, tokenArray);

        }
        tileArray[index] = malloc(sizeof(struct Tile));
        init_tile(tileArray[index], typesArray[0], typesArray[1], typesArray[2], typesArray[3], typesArray[4]);// créer une struct Tile et lui associer les 5 tokens
        index++;
    }
    fclose(file);
    return tileArray;
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
    bPLayers : Le nombre de joueurs
                                                                       
    return : Une liste de nbPLayers éléments contenant les points du joueurs 1 jusqu'à 6
*/