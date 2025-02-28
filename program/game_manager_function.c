#include "../header/Carcassonne.h"
#include <string.h>

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
    char find = -1;
    while (find == -1 && i < NB_TOKEN_TYPE)
    {
        if (strcmp(tokenArray[i], token) == 0)
        {
            find = i;
        }
        i++;
    }
    return find;
}

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

    fgets(buff, BUFF_DEFAULT_SIZE, file); // On saute la première ligne car il s'agit de la tuile de base
    while (fgets(buff, BUFF_DEFAULT_SIZE, file) != NULL && index <= NBTILE)
    {
        token = strtok(buff, ",");
        for (char i = 0; i < NB_TOKEN_TYPE && token != NULL; i++)
        {
            token[strcspn(token, "\n")] = '\0';
            typesArray[i] = token_to_enum_types(token, tokenArray);
            token = strtok(NULL, ",");
        }

        tileArray[index] = init_tile(typesArray[0], typesArray[1], typesArray[2], typesArray[3], typesArray[4]); // créer une struct Tile et lui associer les 5 tokens
        index++;
    }
    fclose(file);
    return tileArray;
}

struct list_player *init_player_list(char nbPlayers) // Axel
/*
    Crée une liste de pointeurs qui pointe sur un Player,
    un pointer sur NULL est ajouté à la fin pour faciliter
    les iterations sur la liste (condition d'arrêt)
*/
{
    struct list_player *list_players = malloc(sizeof(struct list_player));
    list_players->player = malloc(sizeof(struct Player*) * (nbPlayers + 1));
    for( int i = 0; i < nbPlayers; i++ )
    {
        list_players->player[i] = malloc(sizeof(struct Player));
    }
    list_players->player[nbPlayers] = NULL;
    return list_players;
}

void shuffle(struct Tile **tileArray, char size) // Valentin c'est peut-être mieux si size est une macro A FAIRE
/*
    tileArray : Une liste de pointeurs sur Tile.
    size : La taille de la liste (normalement 72)
    la fonction shuffle melange la liste par repetition d'un grand nombre permutation aléatoire entre deux élément

*/
{
    srand(time(NULL)); // initialisation de la seed pour la generation de nombre aléatoire
    short i, rand1, rand2;
    struct Tile *temp;

    for (i = 0; i < size * size; i++)
    {
        rand1 = rand() % size;
        rand2 = rand() % size;
        if (rand1 != rand2)
        {
            temp = tileArray[rand1];
            tileArray[rand1] = tileArray[rand2];
            tileArray[rand2] = temp;
        }
    }
}

void array_to_stack(struct Tile **tileArray, struct Stack **stack) // Valentin A FAIRE
/*
    Prend en entrée une array et return une pile avec les éléments de l'array dedans.
    On considère que l'utilisateur donne le parametre stack non allouer
*/
{

    for (short i = 0; i < NBTILE - 1; i++)
    {
        *stack = stack_push(*stack, tileArray[i]);
    }
}
void reset_points(struct Player *player) // Fait
/*
    Réinitialise les points du joueur
*/
{
    player->points = 0;
}

void reset_meeples(struct Player *player) // Fait
/*
    Réinitialise le compteur de Meeple du joueur
*/
{
    player->nbMeeple = NBMEEPLE_DEFAULT;
}

struct Tile *rot_tile(struct Tile *tile)
/*
    Tourne la tuile de 90° dans le sens trigo si elle existe.
*/
{
    if (!tile)
    {
        return NULL;
    }
    enum types tmpRight = tile->right;
    enum types tmpTop = tile->top;
    enum types tmpLeft = tile->left;
    enum types tmpBot = tile->bot;

    tile->right = tmpBot;
    tile->top = tmpRight;
    tile->left = tmpTop;
    tile->bot = tmpLeft;

    return tile;
}

// void player_turn(char playerNumber, struct Player **PlayerArray, struct Stack *pioche, struct Grid **grid, unsigned int nb_coord) // A FAIRE

/*
    playerNumber : Le numéro du joueur

    Cette fonction pop la stack de tile
    et propose ensuite au joueur de choisir
    un emplacmement pour poser sa tuile
    avec la fonction where_i_can_play
*/
/*
{
    printf("Tour du joueur %d\n", playerNumber);
    struct Tile *turn_tile = malloc(sizeof(struct Tile *));
    pioche = stack_pop(pioche, &turn_tile);
    struct Coord **play_coord = malloc(sizeof(struct Coord *) * nb_coord);
    unsigned int index = 0;
    char pose = 0;
    unsigned int token = -1;
    while (pose == 0) // Continue le temps que la tuile n'est pas posé (si on tourne la tuile ça boucle)
    {
        show_Grid( &grid );
        play_coord = where_i_can_play(turn_tile, grid);
        index = 0;
        while (*(play_coord + index) != NULL)
        {
            printf("%u- x : %d, y : %d\n", index, play_coord[index]->x, play_coord[index]->y);
            index++;
        }
        printf("Pour tourner la tuile rentrez 0/n");
        printf("Pour poser la tuile rentrez l'une des valeurs suivante :/n");
        scanf("%u", &token);
        if (token == 0)
        {
            rot_tile(turn_tile);
        }
        else
        {
            pose = 1;
            //place_tile(struct grid grid, struct coord * coord);
        }
    }
}
*/
struct Coord **where_i_can_play(struct Tile *tile, struct Grid *grid); // A FAIRE
/*
    tile : La tile précedement pioché par le joueur

    grid : Un tableau de grid sur laquelle doit être effectué une recherche
    en fonction de la tile. Les tuiles non posé sont soit des tuiles potentiel
    soit des pointeurs sur NULL

    return : Une liste de coordonnées sur la grille de ou il est possible de placer sa tuile se terminant par NULL
    ATTENTION vous n'avez pas à gérer les rotations ici
*/

struct Grid *first_tile(struct Tile *tile)
{
    // Tile potentiel
    struct Tile *right_tile = init_tile(RIEN, RIEN, tile->right, RIEN, RIEN);
    struct Coord *right_coord = init_coord(0, 1);
    struct Grid *right_grid = init_grid(right_tile, right_coord, NULL, NULL, NULL, NULL);

    
    struct Tile *top_tile = init_tile(RIEN, RIEN, RIEN, tile->top, RIEN);
    struct Coord *top_coord = init_coord(1, 0);
    struct Grid *top_grid = init_grid(top_tile, top_coord, NULL, NULL, NULL, NULL);

    
    struct Tile *left_tile = init_tile(tile->left, RIEN, RIEN, RIEN, RIEN);
    struct Coord *left_coord = init_coord(0, -1);
    struct Grid *left_grid = init_grid(left_tile, left_coord, NULL, NULL, NULL, NULL);

    
    struct Tile *bot_tile = init_tile(RIEN, tile->bot, RIEN, RIEN, RIEN);
    struct Coord *bot_coord = init_coord(-1, 0);
    struct Grid *bot_grid = init_grid(bot_tile, bot_coord, NULL, NULL, NULL, NULL);


    // Tile NULL
    struct Coord *right_top_coord = init_coord(1, 1);
    struct Grid *right_top_grid = init_grid(NULL, right_top_coord, NULL, NULL, NULL, NULL);
   
    struct Coord *left_top_coord = init_coord(-1, 1);
    struct Grid *left_top_grid = init_grid(NULL, left_top_coord, NULL, NULL, NULL, NULL);
    
    struct Coord *right_bot_coord = init_coord(1, -1);
    struct Grid *right_bot_grid = init_grid(NULL, right_bot_coord, NULL, NULL, NULL, NULL);
   
    struct Coord *left_bot_coord = init_coord(1, -1);
    struct Grid *left_bot_grid = init_grid(NULL, right_bot_coord, NULL, NULL, NULL, NULL);

    // Tile posé
    struct Coord *first_coord = init_coord(0, 0);
    struct Grid *first_grid = init_grid(tile, first_coord, right_grid, top_grid, left_grid, bot_grid);

    //relier les Grid entre elles
    right_grid->top = right_top_grid;
    right_grid->bot = right_bot_grid;
    top_grid->right = right_top_grid;
    top_grid->left = left_top_grid;
    left_grid->top = left_top_grid;
    left_grid->bot = left_bot_grid;
    bot_grid->right = right_bot_grid;
    bot_grid->left = left_bot_grid;

    return first_grid;
}

struct Grid *place_tile(struct Grid *grid, struct Coord *coord, struct Tile *tile)
/*
    tile : La tile précedement pioché par le joueur à placer

    grid : La grid originelle TOUJOURS EN HAUT A GAUCHE (NULL si elle n'existe pas encore)

    coord : Les coordonnées de l'endroit ou placer la tuile sur ***grid

    Place la tuile à l'emplacement indiqué, actualise la grille de taille
    variable et met à jour la liste doublement chaîné les tuile ptoentiels pour les autres fonctions
*/
{
    if (grid == NULL && coord->x == 0 && coord->y == 0)
    {
        struct Grid *grid= first_tile(tile);
        
    }
    return NULL;
}

void enum_to_char(enum types type)
{
    switch(type){
        case RIEN:
            printf("Z");
            break;
            
        case ROUTE:
            printf("\x1b[48;5;130m");
            printf("  ");
            printf("\x1b[0m");
            break;

        case VILLE:
            printf("\x1b[48;5;244m");
            printf("  ");
            printf("\x1b[0m");
            break;

        case ABBAYES:
            printf("A");
            break;

        case PRE:
            printf("\x1b[48;5;2m");
            printf("  ");
            printf("\x1b[0m");
            break;

        case VILLAGE:
            printf("v");
            break;

        case BLASON:
            printf("B");
            break;

        default:
            printf("z");
            break;
    }
}

//void show_tile(struct Tile *tile)
/*
    Affiche une tile.
*/
/* {
    printf("    %s    \n", enum_to_char(tile->top));
    printf("%s   %s   %s\n", enum_to_char(tile->left), enum_to_char(tile->middle), enum_to_char(tile->right));
    printf("    %s    \n", enum_to_char(tile->bot));
} */





void choose_w_show(unsigned char y, struct Grid *tab)
{
    switch (y)
    {
        case 0:
            //test pour savoir si  il  faut  comble le trou avec  une ville ou un pré
            if( tab->tile->top == VILLE && tab->tile->left == VILLE )
            {
                enum_to_char(VILLE);
            }
            else enum_to_char(PRE);

            enum_to_char(tab->tile->top);

            //test pour savoir si  il  faut  comble le trou avec  une ville ou un pré
            if( tab->tile->top == VILLE && tab->tile->right == VILLE)
            {
                enum_to_char(VILLE);
            }
            else enum_to_char(PRE);

            break;
        case 1:
            enum_to_char(tab->tile->left);
            enum_to_char(tab->tile->middle);
            enum_to_char(tab->tile->right);
            break;
        case 2:
            //test pour savoir si  il  faut  comble le trou avec  une ville ou un pré
            if( tab->tile->bot == VILLE && tab->tile->left == VILLE )
            {                    
                enum_to_char(VILLE);
            }
            else enum_to_char(PRE);

            enum_to_char(tab->tile->bot);

            //teste pour savoir si  il  faut  comble le trou avec  une ville ou un pré
            if( tab->tile->bot == VILLE && tab->tile->right == VILLE)
            {
                enum_to_char(VILLE);
            }
            else enum_to_char(PRE);
    }
}


void show_grid(struct Grid *tab, unsigned  char x, unsigned char y)
// en cours (Axel)

    //Affiche la grille du jeu en ascii art en minimisant l'espace occupé


{
    struct Grid *temp_x = tab, *temp_y; //temp_x pas initialisé 
    unsigned char t_x = 0, t_y = 0;
    for (; t_x < x; t_x++)
    {
        for( int j = 0; j < 3; j++)
        {
            temp_y = temp_x; //la temp_y va prend un pointeur pas initialisé
            for( t_y = 0; t_y < y; t_y++)
            {
                if (temp_y->tile == NULL)
                {
                    enum_to_char(PRE);
                    enum_to_char(PRE);
                    enum_to_char(PRE);
                }
                else
                {
                    //printf("%d\n",temp_y->tile->left); //mon test de print la tile fait crash
                    choose_w_show(j, temp_y); //cette ligne semble faire crash
                }
                temp_y = temp_y->right;
            }
            printf("\n");
        }
        temp_x = temp_x->bot;
    }
    printf("\n fin affichage \n");
}

struct Tile *start_game(struct list_player **list_player, char nbPlayer, char *turnTraker, struct Grid *grid){ // en cour ( Axel )

/*
    Effet :
    - Réinitialise le plateau (une seule tuile au centre) (free toute les les tiles sinon par de bouton rejoué et il faut fermer et ouvrir le jeu)
    - Réinitialise les points
    - Réinitialise les Meeples
    - écrase ou crée la liste des tuiles, les mélanges puis crée une pile
    - Réinitialise le turn tracker (le joueur 1 commence)
*/
    if (list_player == NULL)
    {
        *list_player = malloc(sizeof(struct list_player));
        (*list_player) -> player = malloc(sizeof(struct player*) * nbPlayer);
    }
    for(  int i = 0; i < nbPlayer; i++)
    {
        init_player((*list_player)->player[i]);
    }
    if ( grid != NULL)
    {
        free_Grid( grid );
    }
    struct Tile *tile_array;
    //create_tile_array();
    //shuffle(&(tile_array))
    //init_Grid()
    turnTraker = 0;
    return tile_array;
}



char *end_game_points_counter(struct Grid *grid, struct Player nbPlayers); // A FAIRE
/*
    grid : la case de départ
    bPLayers : Le nombre de joueurs

    return : Une liste de nbPLayers éléments contenant les points du joueurs 1 jusqu'à 6
*/

void free_Grid( struct Grid *grid) // a tester
/* 
prend en paramètre une struct grid initialisée et la free pour être réutilisée
!!! S'assurer que le pointeur vers grid sois bien en haut à gauche du graph
*/
{
    if ( grid->right != NULL)
    {
        free_Grid( grid->right );
    }
    if ( grid->bot != NULL )
    {
        free_Grid( grid->bot);
    }
    free(grid);
    return;
}