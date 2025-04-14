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


void shuffle(struct Tile **tileArray) // Valentin c'est peut-être mieux si size est une macro A FAIRE
/*
    tileArray : Une liste de pointeurs sur Tile.
    size : La taille de la liste (normalement 72)
    la fonction shuffle melange la liste par repetition d'un grand nombre permutation aléatoire entre deux élément
*/
{
    srand(time(NULL)); // initialisation de la seed pour la generation de nombre aléatoire
    short i, rand1, rand2;
    struct Tile *temp;

    for (i = 0; i < NBTILE * NBTILE; i++)
    {
        rand1 = rand() % NBTILE;
        rand2 = rand() % NBTILE;
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

void player_turn(char playerNumber, struct list_player *p_list, struct Stack *pioche, struct Grid **grid) // A FAIRE
/*
    playerNumber : Le numéro du joueur

    Cette fonction pop la stack de tile
    et propose ensuite au joueur de choisir
    un emplacmement pour poser sa tuile
    avec la fonction where_i_can_play
*/
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
        play_coord = where_i_can_play(turn_tile, *grid);
        show_grid( &grid, x, y ,play_coord);
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
            grid = place_tile(struct Grid grid, struct Coord * coord);
            //pointPlacedTile  besoin de la fonction de théo
            //secondary_verification()    idem
        }
    }
}

struct Grid **where_i_can_play(struct Tile *tile, struct DLList *dllist) // Théo à faire
/*
    tile : La tile précedement pioché par le joueur

    dllist : Une liste doublement chainé contenant les tuiles potentiels pour cette fonction.

    return : La liste malloc des endroit ou il est possible de jouer (position tuile fixe)
*/
{
    struct Grid **gridArrray = calloc(NBTILE + 1, sizeof(struct Grid)); // set à NULL avec calloc
    struct DLList *tmpDllist = dllist;
    int index = 0;
    for (int i = 0; i < 4; i++)
    {
        while (tmpDllist->next != NULL)
        {
            if ((tile->right == tmpDllist->data->tile->right || tile->right == RIEN) && (tile->top == tmpDllist->data->tile->top || tile->right == RIEN) && (tile->left == tmpDllist->data->tile->left || tile->right == RIEN) && (tile->bot == tmpDllist->data->tile->bot || tile->right == RIEN))
            {
                gridArrray[index] = tmpDllist->data;
                index++;
            }
            tmpDllist = tmpDllist->next;
        }
        turn_tile(tile);
    }
    return gridArrray;
}

char is_a_potential_tile(struct Tile *tile) // Théo FAIT
/*
    Return 0 si ce n'est pas une tuile potentielle
    Return 1 si c'est une tuile potentielle

    Information : Une tuile potentielle est une tuile parciellement remplie de enum qui sert pour la fonction where_i_can_place.
*/
{
    if (tile->middle == RIEN)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void upscale(struct Grid *origineGrid, int *largeur, int *hauteur, struct Coord coord) // Théo A corriger
/*
    origineGrid : La case en haut à gauche de la Grid.
    largeur : La largeur max de la grille.
    hauteur : La hauteur max de la grille.
    coord : La coordonnée qui dépasse de la grille

    Attention il faut appeler cette fonction avant qu'il exisite une tuile dont les coords dépassent
    xMax, yMax, xMin ou yMin.

    Attention il est impossible d'agrandire à partir d'un angle, exemple, hauteur == 1, largeur == 1 
    origineGrid->coord == (0,0) alors si coord == (1,1) la fonction ne fonctionne pas mais si 
    coord == (1,0) alors elle fonctionne.

    L'objectif de cette fonction est de s'assurer la présence de Grid innitialiser à NULL de sorte à 
    ce que le plateau de jeu soit un rectangle afin de simplifier le parcours à la fonction d'affichage.
*/
{
    int xMin = origineGrid->coord->x;
    
    int yMin = origineGrid->coord->y;
    int xMax = xMin + *largeur - 1; // -1 car (0,0) existe (toujours).
    int yMax = yMin + *hauteur - 1;

    struct Grid *tmpGrid = origineGrid;

    struct Tile *newTile;
    struct Grid *preGrid = NULL;
    struct Grid *newGrid;

    // Vérification que la coord est en dehors de la zone
    if (coord.x > xMax)
    {
        for (int i = 0; i < *largeur; i++) // plante si largeur mal innitialisé
        {
            tmpGrid = tmpGrid->right;
        }
        for (int i = 0; i < *hauteur; i++)
        {
            newTile = init_tile(RIEN, RIEN, RIEN, RIEN, RIEN);
            newGrid = init_grid(newTile, NULL, NULL, NULL, NULL, NULL);
            newGrid->left = tmpGrid;
            tmpGrid->right = newGrid;
            newGrid->top = preGrid;
            if (preGrid != NULL)
            {
                preGrid->bot = newGrid;
            }

            tmpGrid = tmpGrid->bot;
            preGrid = newGrid;
        }
        (*largeur)++;
    }
    else if (coord.x < xMin)
    {
        for (int i = 0; i < *hauteur; i++)
        {
            newTile = init_tile(RIEN, RIEN, RIEN, RIEN, RIEN);
            newGrid = init_grid(newTile, NULL, NULL, NULL, NULL, NULL);
            newGrid->right = tmpGrid;
            tmpGrid->left = newGrid;
            newGrid->top = preGrid;
            if (preGrid != NULL)
            {
                preGrid->bot = newGrid;
            }

            tmpGrid = tmpGrid->bot;
            preGrid = newGrid;
        }
        (*largeur)++;
    }
    else if (coord.y > yMax)
    {
        for (int i = 0; i < *largeur; i++)
        {
            newTile = init_tile(RIEN, RIEN, RIEN, RIEN, RIEN);
            newGrid = init_grid(newTile, NULL, NULL, NULL, NULL, NULL);
            newGrid->bot = tmpGrid;
            tmpGrid->top = newGrid;
            newGrid->left = preGrid;
            if (preGrid != NULL)
            {
                preGrid->right = newGrid;
            }

            tmpGrid = tmpGrid->right;
            preGrid = newGrid;

        }
        (*hauteur)++;
    }
    else if (coord.y < yMin)
    {
        for (int i = 0; i < *hauteur; i++) // plante si largeur mal innitialisé
        {
            tmpGrid = tmpGrid->bot;
        }
        for (int i = 0; i < *largeur; i++)
        {
            newTile = init_tile(RIEN, RIEN, RIEN, RIEN, RIEN);
            newGrid = init_grid(newTile, NULL, NULL, NULL, NULL, NULL);
            newGrid->top = tmpGrid;
            tmpGrid->bot = newGrid;
            newGrid->left = preGrid;
            if (preGrid != NULL)
            {
                preGrid->right = newGrid;
            }

            tmpGrid = tmpGrid->right;
            preGrid = newGrid;
        }
        (*hauteur)++;
    }
    return;
}

void update_potential_tile(struct Grid *trueGrid, enum places place) // Théo A TESTER
/*
    trueGrid : La vraie tuile qui vient d'être posé.
    potentialGrid : Une grid autours d'une tuile qui vient d'être posé. Grid doit être innitialisé au minimum à NULL et doit pointer sur une Tile innitialisé au minimum à NULL.
    place : La position de la tuile potentielle par rapport à la tuile qui a était posé.

    Cette fonction a pour but d'actualiser une des tuiles pottentielles qui se trouve autours d'une tuile qui vient d'être posé.
*/
{
    switch(place)
    {
        case RIGHT:
        {
            struct Grid *potentialGrid = trueGrid->right;
            potentialGrid->coord->x = (trueGrid->coord->x) + 1;
            potentialGrid->coord->y = (trueGrid->coord->y);

            potentialGrid->tile->left = trueGrid->tile->right;
            trueGrid->right = potentialGrid;
            potentialGrid->left = trueGrid;
            break;
        }
        

        case TOP:
        {
            struct Grid *potentialGrid = trueGrid->top;
            potentialGrid->coord->x = (trueGrid->coord->x);
            potentialGrid->coord->y = (trueGrid->coord->y) + 1;

            potentialGrid->tile->bot = trueGrid->tile->top;
            trueGrid->top = potentialGrid;
            potentialGrid->bot = trueGrid;
            break;
        }
        

        case LEFT:
        {
            struct Grid *potentialGrid = trueGrid->left;
            potentialGrid->coord->x = (trueGrid->coord->x) - 1;
            potentialGrid->coord->y = (trueGrid->coord->y);

            potentialGrid->tile->right = trueGrid->tile->left;
            trueGrid->left = potentialGrid;
            potentialGrid->right = trueGrid;
            break;
        }

        case BOT:
        {
            struct Grid *potentialGrid = trueGrid->bot;
            potentialGrid->coord->x = (trueGrid->coord->x);
            potentialGrid->coord->y = (trueGrid->coord->y) - 1;

            potentialGrid->tile->top = trueGrid->tile->bot;
            trueGrid->bot = potentialGrid;
            potentialGrid->top = trueGrid;
            break;
        }

        default: // MIDDLE et autre est une erreur.
            //ERROR  CASE
        break;
    }
    return;
}

struct Grid *find(struct Grid *grid, struct Coord coord) // Théo FAIT
/*
    grid : La grid en haut à gauche.
    coord : Les coordonnées de la case recherchée.

    return : Un pointeur sur la grille de coordonnées coord

    Erreur : Si la fonction return un pointeur sur NULL cela
    signifie que la case rechercher n'est pas innitialisé 
    (pas de le scope largeur hauteur de case innitialisé).
*/
{
    struct Grid *tmpGrid = grid;
    while ((tmpGrid != NULL) && (tmpGrid->coord->x != coord.x || tmpGrid->coord->y != coord.y))
    {
        if (tmpGrid->coord->x < coord.x)
        {
            tmpGrid = tmpGrid->right;
        }
        else if (tmpGrid->coord->y > coord.y)
        {
            tmpGrid = tmpGrid->bot;
        }
        else
        {
            return NULL;
        }
    }
    return tmpGrid;
}

struct Grid *first_grid(struct Grid *grid, int *hauteur, int *largeur, struct DLList *dllist) // Théo A TESTER
/*
    Place la première tuile et actualise la grille en conséquence.
    grid : La grid originelle de coord (0,0)
    largeur : La largeur max.
    hauteur : La hauteur max.

    Return value : L'élément le plus en haut à gauche de la grid.
*/
{
    // Adaptation de la taille de la grid
    struct Coord tmpCoord;
    tmpCoord.x = 1;
    tmpCoord.y = 0;
    upscale(grid, hauteur, largeur, tmpCoord); // (1,0)
    tmpCoord.x = -1;
    tmpCoord.y = 0;
    upscale(grid, hauteur, largeur, tmpCoord); // (-1,0)
    tmpCoord.x = 0;
    tmpCoord.y = 1;
    upscale(grid, hauteur, largeur, tmpCoord); // (0,1)
    tmpCoord.x = 0;
    tmpCoord.y = -1;
    upscale(grid, hauteur, largeur, tmpCoord); // (0,-1)

    // Actualisation des tuiles pottentielles adjacente
    struct Tile *right_tile = init_tile(RIEN, RIEN, RIEN, RIEN, RIEN);
    struct Grid *right_grid = init_grid(right_tile, NULL, NULL, NULL, NULL, NULL);
    update_potential_tile(grid, RIGHT);

    struct Tile *top_tile = init_tile(RIEN, RIEN, RIEN, RIEN, RIEN);
    struct Grid *top_grid = init_grid(top_tile, NULL, NULL, NULL, NULL, NULL);
    update_potential_tile(grid, TOP);

    struct Tile *left_tile = init_tile(RIEN, RIEN, RIEN, RIEN, RIEN);
    struct Grid *left_grid = init_grid(left_tile, NULL, NULL, NULL, NULL, NULL);
    update_potential_tile(grid, LEFT);

    struct Tile *bot_tile = init_tile(RIEN, RIEN, RIEN, RIEN, RIEN);
    struct Grid *bot_grid = init_grid(bot_tile, NULL, NULL, NULL, NULL, NULL);
    update_potential_tile(grid, BOT);

    // actualisation de dllist
    DLList_push_end(dllist, right_grid);
    DLList_push_end(dllist, top_grid);
    DLList_push_end(dllist, left_grid);
    DLList_push_end(dllist, bot_grid);

    return grid->top->left;
}

struct Grid *place_tile(struct Grid *grid, struct Coord *coord, struct Tile *tile, struct DLList *dllist, int *hauteur, int *largeur) // Théo TESTER AVEC LE GAMEMANAGER
/*
    tile : Un pointeur sur la tile précedement pioché par le joueur à placer.

    grid : Un pointeur sur la grid originelle TOUJOURS EN HAUT A GAUCHE (NULL si elle n'existe pas encore).

    coord : Les coordonnées de l'endroit ou placer la tuile sur *grid.

    DLList : Un pointeur sur le première élément de la liste doublement chainé contenant les tuiles potentielles

    Largeur : La largeur maximale de la grille, soit la longeur de la plus grande ligne.

    Hauteur : La hauteur max de la grille, soit la hauteur de la plus grande colone.

    Place la tuile à l'emplacement indiqué, actualise la grille de taille
    variable et met à jour la liste doublement chaîné les tuile potentiels pour les autres fonctions
*/
{
    if (grid == NULL && coord->x == 0 && coord->y == 0) // Cas début de partie
    {
        struct Grid *firstGrid = init_grid(tile, coord, NULL, NULL, NULL, NULL);
        struct Grid *grid= first_grid(firstGrid, hauteur, largeur, dllist);
    }
    upscale(grid, largeur, hauteur, *coord);
    struct Grid *gridFind = find(grid, *coord);
    gridFind->tile = tile;
    update_potential_tile(gridFind, RIGHT);
    update_potential_tile(gridFind, TOP);
    update_potential_tile(gridFind, LEFT);
    update_potential_tile(gridFind, BOT);
    return NULL;
}

void enum_to_char(enum types type)
{
    switch(type){
        case RIEN:
            printf("\x1b[48;2;%d;%d;%dm",0, 0, 0);
            printf(" Z ");
            printf("\x1b[0m");
            break;
            
        case ROUTE:
            printf("\x1b[48;2;%d;%d;%dm",168, 92, 33);
            printf(" R ");
            printf("\x1b[0m");
            break;

        case VILLE:
            printf("\x1b[48;2;%d;%d;%dm",110,110,110);
            printf(" V ");
            printf("\x1b[0m");
            break;

        case ABBAYES:
            printf("\x1b[48;2;%d;%d;%dm",241,196,15);
            printf(" A ");
            printf("\x1b[0m");
            break;

        case PRE:
            printf("\x1b[48;2;%d;%d;%dm",35,155,86);
            printf(" P ");
            printf("\x1b[0m");
            break;

        case VILLAGE:
            printf("\x1b[48;2;%d;%d;%dm",102,188,186);
            printf(" v ");
            printf("\x1b[0m");
            break;

        case BLASON:
            printf("\x1b[48;2;%d;%d;%dm",101,112,255);
            printf(" B ");
            printf("\x1b[0m");
            break;

        default:
            printf("z");
            break;
    }
}

void show_wplace(int j, int h)
{
    if(j == 0 || j == 2)
    {
        printf("---");
        return;
    }
    printf("-%d-",h);
    
}

void choose_w_show(unsigned char y, struct Grid *tab)
{
    switch (y)
    {
        case 0:
            //test pour savoir si  il  faut  comble le trou avec  une ville ou un pré
            if( tab->tile->top == VILLE && tab->tile->left == VILLE && tab->tile->middle==VILLE)
            {
                enum_to_char(VILLE);
            }
            else enum_to_char(PRE);

            enum_to_char(tab->tile->top);

            //test pour savoir si  il  faut  comble le trou avec  une ville ou un pré
            if( tab->tile->top == VILLE && tab->tile->right == VILLE && tab->tile->middle==VILLE)
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
            if( tab->tile->bot == VILLE && tab->tile->left == VILLE && tab->tile->middle==VILLE)
            {                    
                enum_to_char(VILLE);
            }
            else enum_to_char(PRE);

            enum_to_char(tab->tile->bot);

            //teste pour savoir si  il  faut  comble le trou avec  une ville ou un pré
            if( tab->tile->bot == VILLE && tab->tile->right == VILLE && tab->tile->middle==VILLE)
            {
                enum_to_char(VILLE);
            }
            else enum_to_char(PRE);
    }
    printf("  ");//ligne temporaire pour aider a differencier les tuile
}


void show_grid(struct Grid *tab, unsigned char x, unsigned char y, struct Grid **w_place)
{
    char mrkr = 0;
    struct Grid *temp_x = tab, *temp_y;
    unsigned char t_x = 0, t_y;
    for (t_x = 0; t_x < x; t_x++)
    {
        for (int j = 0; j < 3; j++)
        {
            t_y = 0;
            temp_y = temp_x;
            for (; t_y < y; t_y++)
            {
                mrkr = 0;
                int h = 0;

                while (w_place[h] != NULL)
                {
                    if (w_place[h]->coord->x == t_x && w_place[h]->coord->y == t_y)
                    {
                        show_wplace(j, h);
                        mrkr = 1;
                        break;
                    }
                    h++;
                }

                if (temp_y == NULL || temp_y->tile == NULL)
                {
                    if (mrkr == 0)
                    {
                        enum_to_char(PRE);
                        enum_to_char(PRE);
                        enum_to_char(PRE);
                    }
                }
                else if (mrkr == 0)
                {
                    choose_w_show(j, temp_y);
                }

                if (temp_y != NULL)
                    temp_y = temp_y->right;
            }
            printf("\n");
        }
        if (temp_x != NULL)
            temp_x = temp_x->bot;
    }
}




struct Stack *start_game(struct list_player **list_player, struct Grid *grid) // en cour ( Axel )
/*
    Effet :
    - Réinitialise le plateau (une seule tuile au centre) (free toute les les tiles sinon par de bouton rejoué et il faut fermer et ouvrir le jeu)
    - Réinitialise les points
    - Réinitialise les Meeples
    - écrase ou crée la liste des tuiles, les mélanges puis crée une pile
    - Réinitialise le turn tracker (le joueur 1 commence)
*/
{
    printf("combien  de joueur : ");
    scanf("%d",&nbPlayers);

    printf("combien  de bot: ");
    scanf("%d",&nbBot);

    if (list_player == NULL)
    {
        *list_player = init_player_list(nbBot);
    }
        
    if ( grid != NULL)
    {
        free_Grid( grid );
    }
    struct Tile **tile_array;
    char *tokenArray[MAX_TOKEN_SIZE + 1] = {"route", "ville", "abbaye", "pre", "village", "blason"};
    tile_array = create_tile_array("tuiles_base_csv_simplifiees.csv",tokenArray, MAX_TOKEN_SIZE);
    shuffle(tile_array   );
    struct Stack  *stack;
    array_to_stack(tile_array,&stack);
    struct Tile *variable_crée_à_cause_dune_decision_stupide;
    stack = stack_pop(stack,&variable_crée_à_cause_dune_decision_stupide);
    struct Coord *start_coord=init_coord(0,0);
    grid = init_grid(variable_crée_à_cause_dune_decision_stupide,start_coord,NULL,NULL,NULL,NULL);
    return stack;
}


void *end_game_points_counter( struct list_player list ) // à tester (Axel)
/*
    return : Une liste de nbPLayers éléments contenant les points du joueurs 1 jusqu'à 6
*/
{
    for( int  i = 0; i < nbPlayers ; i++)
    {
        printf("Le joueur n°%d possède %d points",i, list.player[i]->points );
    }
}

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