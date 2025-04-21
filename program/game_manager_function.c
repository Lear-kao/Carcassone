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

    for (i = 0; i < (NBTILE-1) * (NBTILE-1); i++)
    {
        rand1 =1+ rand() % (NBTILE-1);
        rand2 =1+ rand() % (NBTILE-1);
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

void player_turn(char playerNumber, struct list_player *p_list, struct Stack *pioche, struct Grid **leftTopGrid, struct DLList **dllist, int *hauteur, int *largeur, struct list_player *listPlayer) // A FAIRE
/*
    playerNumber : Le numéro du joueur

    Cette fonction pop la stack de tile
    et propose ensuite au joueur de choisir
    un emplacemement pour poser sa tuile
    avec la fonction where_i_can_play
*/
{
    printf("Tour du joueur %d\n", playerNumber);
    struct Tile *turn_tile = malloc(sizeof(struct Tile *));
    struct Grid **play_grid = NULL;
    unsigned int index = 0;
    char pose = 0; // bool
    unsigned int token = -1;
    struct Grid **tmpGrid;


    pioche = stack_pop(pioche, &turn_tile); // désolé pour ce pop de l'enfer Axel xD

    // play_grid = where_i_can_play(turn_tile, dllist);


    while (pose == 0) // Continue le temps que la tuile n'est pas posé (si on tourne la tuile ça boucle)
    {
        play_grid = where_i_can_play(turn_tile, dllist);
        show_grid(*leftTopGrid, *largeur, *hauteur, play_grid);
        tmpGrid = play_grid;
        index = 0;
        printf("Pour tourner la tuile rentrez 0\n");
        printf("Pour poser la tuile rentrez l'une des valeurs suivante :\n");
        while(tmpGrid[index] !=NULL)
        {
            printf("%d - x = %d et y = %d\n", index + 1, tmpGrid[index]->coord->x, tmpGrid[index]->coord->y);
            index++;
        }
        //affiche un truc AXEL ICI
        scanf("%u", &token);
        if (token == 0)
        {
            rot_tile(turn_tile);
        }
        else
        {
            pose = 1;
            *leftTopGrid = place_tile(leftTopGrid, play_grid[token - 1]->coord, turn_tile, dllist, hauteur, largeur); // token -1 car 0 correspond à tourner la tuile
            pointPlacedTile(play_grid[token - 1], listPlayer); //besoin de la fonction de théo
        }
    }
}

struct Grid **where_i_can_play(struct Tile *tile, struct DLList **dllist) // Théo à faire
/*
    tile : La tile précedement pioché par le joueur

    dllist : Une liste doublement chainé contenant les tuiles potentiels pour cette fonction.

    return : La liste malloc des endroit ou il est possible de jouer (position tuile fixe)
*/
{
    struct Grid **gridArrray = calloc(NBTILE + 1, sizeof(struct Grid)); // set à NULL avec calloc de taille NBTILE -> le nombre max de tuile dans le jeu
    struct DLList *tmpDllist = *dllist;
    int index = 0;
    while (tmpDllist->next != NULL)
    {

        if ((tile->right == tmpDllist->data->tile->right || tmpDllist->data->tile->right == RIEN) 
        && (tile->top == tmpDllist->data->tile->top || tmpDllist->data->tile->top == RIEN) 
        && (tile->left == tmpDllist->data->tile->left || tmpDllist->data->tile->left == RIEN) 
        && (tile->bot == tmpDllist->data->tile->bot || tmpDllist->data->tile->bot == RIEN))
        {
            gridArrray[index] = tmpDllist->data;
            index++;
        }
        tmpDllist = tmpDllist->next;
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

void upscale(struct Grid **leftTopGrid, int *largeur, int *hauteur, struct Coord coord) // Théo A corriger
/*
    leftTopGrid : La case en haut à gauche de la Grid, elle mise à jour automatiquement.
    largeur : La largeur max de la grille.
    hauteur : La hauteur max de la grille.
    coord : La coordonnée qui dépasse de la grille

    Attention il faut appeler cette fonction avant qu'il exisite une tuile dont les coords dépassent
    xMax, yMax, xMin ou yMin.

    Attention il est impossible d'agrandire à partir d'un angle, exemple, hauteur == 1, largeur == 1 
    *(leftTopGrid)->coord == (0,0) alors si coord == (1,1) la fonction ne fonctionne pas mais si 
    coord == (1,0) alors elle fonctionne.

    L'objectif de cette fonction est de s'assurer la présence de Grid innitialiser à NULL de sorte à 
    ce que le plateau de jeu soit un rectangle afin de simplifier le parcours à la fonction d'affichage.
*/
{
    int xMin = (*leftTopGrid)->coord->x;
    int yMax = (*leftTopGrid)->coord->y;
    int xMax = xMin + *largeur - 1; // -1 car (0,0) existe (toujours).
    int yMin = yMax - *hauteur + 1; // ou géométriquement yMin = -(hauteur - 1 - yMax) 

    struct Grid *tmpGrid = *leftTopGrid;

    struct Tile *newTile;
    struct Grid *preGrid = NULL;
    struct Grid *newGrid;
    // Vérification que la coord est en dehors de la zone

    if (coord.x > xMax) // right
    {
        for (int i = 0; i < *largeur - 1; i++) // plante si largeur mal innitialisé
        {
            tmpGrid = tmpGrid->right;
        }
        for (int i = 0; i < *hauteur; i++)
        {
            newTile = init_tile(RIEN, RIEN, RIEN, RIEN, RIEN);
            struct Coord *tmpCoord = malloc(sizeof(struct Coord));
            tmpCoord->x = (*leftTopGrid)->coord->x + *largeur;
            tmpCoord->y = (*leftTopGrid)->coord->y - i;

            newGrid = init_grid(newTile, tmpCoord, NULL, NULL, NULL, NULL);
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
    else if (coord.x < xMin) // left
    {
        for (int i = 0; i < *hauteur; i++)
        {
            newTile = init_tile(RIEN, RIEN, RIEN, RIEN, RIEN);
            struct Coord *tmpCoord = malloc(sizeof(struct Coord));
            tmpCoord->x = (*leftTopGrid)->coord->x - 1;
            tmpCoord->y = (*leftTopGrid)->coord->y - i;
            newGrid = init_grid(newTile, tmpCoord, NULL, NULL, NULL, NULL);
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
        *leftTopGrid = (*leftTopGrid)->left;
    }
    else if (coord.y > yMax) // top
    {
        for (int i = 0; i < *largeur; i++)
        {
            newTile = init_tile(RIEN, RIEN, RIEN, RIEN, RIEN);
            struct Coord *tmpCoord = malloc(sizeof(struct Coord));
            tmpCoord->x = (*leftTopGrid)->coord->x + i;
            tmpCoord->y = (*leftTopGrid)->coord->y + 1;
            newGrid = init_grid(newTile, tmpCoord, NULL, NULL, NULL, NULL);
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
        *leftTopGrid = (*leftTopGrid)->top;
    }
    else if (coord.y < yMin) // bot
    {
        for (int i = 0; i < *hauteur - 1; i++) // plante si largeur mal innitialisé
        {
            tmpGrid = tmpGrid->bot;
        }
        for (int i = 0; i < *largeur; i++)
        {
            newTile = init_tile(RIEN, RIEN, RIEN, RIEN, RIEN);
            struct Coord *tmpCoord = malloc(sizeof(struct Coord));
            tmpCoord->x = (*leftTopGrid)->coord->x + i;
            tmpCoord->y = (*leftTopGrid)->coord->y - *hauteur;
            newGrid = init_grid(newTile, tmpCoord, NULL, NULL, NULL, NULL);
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
    trueGrid : La "vraie" tuile qui vient d'être posé.
    trueGrid->(right, left...) : Une grid autours d'une tuile qui vient d'être posé. 
    Grid doit être innitialisé au minimum à NULL et doit pointer sur une Tile innitialisé au minimum à NULL.
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
            potentialGrid->coord->x = (trueGrid->coord->x); // potentialGrid->coord->x crash
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

struct Grid *first_grid(struct Grid *grid, int *hauteur, int *largeur, struct DLList **dllist) // Théo A TESTER
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
    upscale(&grid, largeur, hauteur, tmpCoord); // (1,0)
    tmpCoord.x = -1;
    tmpCoord.y = 0;

    upscale(&grid, largeur, hauteur, tmpCoord); // (-1,0)
    tmpCoord.x = 0;
    tmpCoord.y = 1;

    upscale(&grid, largeur, hauteur, tmpCoord); // (0,1)
    tmpCoord.x = 0;
    tmpCoord.y = -1;
    upscale(&grid, largeur, hauteur, tmpCoord); // (0,-1)

    // Actualisation des tuiles pottentielles adjacente
    update_potential_tile(grid->right->bot, RIGHT); 
    update_potential_tile(grid->right->bot, TOP);
    update_potential_tile(grid->right->bot, LEFT);
    update_potential_tile(grid->right->bot, BOT);

    // actualisation de dllist

    *dllist = DLList_push_end(*dllist, grid->right); // (0,1)
    *dllist = DLList_push_end(*dllist, grid->right->bot->right); // (1,0)
    *dllist = DLList_push_end(*dllist, grid->bot); // (-1, 0)
    *dllist = DLList_push_end(*dllist, grid->bot->right->bot); // (0, -1)
    return grid;
}

void init_plateau(struct Grid **topLeftGrid, struct DLList **dllist, int *hauteur, int *largeur)
{
    struct Tile *startTile = init_tile(VILLE, ROUTE, PRE, ROUTE, ROUTE); // tuile de départ hard code
    struct Coord *coord = malloc(sizeof(struct Coord));
    coord->x = 0;
    coord->y = 0;

    struct Grid *firstGrid = init_grid(startTile, coord, NULL, NULL, NULL, NULL);
    *topLeftGrid = first_grid(firstGrid, hauteur, largeur, dllist);
}

struct Grid *place_tile(struct Grid **topLeftGrid, struct Coord *coord, struct Tile *tile, struct DLList **dllist, int *hauteur, int *largeur) // Théo TESTER AVEC LE GAMEMANAGER
/*
    tile : Un pointeur sur la tile précedement pioché par le joueur à placer.

    topLeftgrid : Un pointeur sur la grid la plus en haut à gauche qui s'actualise.

    coord : Les coordonnées de l'endroit ou placer la tuile sur *topLeftGrid.

    DLList : Un pointeur sur le première élément de la liste doublement chainé contenant les tuiles potentielles

    Largeur : La largeur maximale de la grille, soit la longeur de la plus grande ligne.

    Hauteur : La hauteur max de la grille, soit la hauteur de la plus grande colone.

    Place la tuile à l'emplacement indiqué, actualise la grille de taille
    variable et met à jour la liste doublement chaîné les tuile potentiels pour les autres fonctions
*/
{
    upscale(topLeftGrid, largeur, hauteur, *coord);
    struct Grid *gridFind = find(*topLeftGrid, *coord); // trouve ou poser la tuile aussi possible pour plus d'optimisation de trouver la tuile dans dllist
    gridFind->tile = tile; // pose la tuile
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
        printf("---------  ");
        return;
    }
    printf("----%d----  ",h+1);
    
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

void show_tile( struct Tile *tile )
{
    enum_to_char(PRE);
    enum_to_char(tile->top);
    enum_to_char(PRE);
    printf("\n");
    enum_to_char(tile->left);
    enum_to_char(tile->middle);
    enum_to_char(tile->right);
    printf("\n");
    enum_to_char(PRE);
    enum_to_char(tile->bot);
    enum_to_char(PRE);
}
void show_grid(struct Grid *tab, unsigned char x, unsigned char y, struct Grid **w_place)
// w_place résultat de where_i_can_place
// x *largeur
// y *hauteur
// tab grille en haut à droite
{
    char mrkr = 0;
    struct Grid *temp_x = tab, *temp_y;
    unsigned char t_x = 0, t_y;
    for (t_x = 0; t_x < y; t_x++)
    {
        printf("\n");
        for (int j = 0; j < 3; j++)
        {
            t_y = 0;
            temp_y = temp_x;
            for (; t_y < x; t_y++)
            {
                mrkr = 0;
                int h = 0;
                if (w_place == NULL) {
                    puts("Erreur : w_place est NULL");
                    return;
                }
                
                while (w_place[h] != NULL && w_place[h]->coord != NULL)
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




struct Stack *start_game(struct list_player **list_player, struct Grid **grid, struct DLList **dllist, int *hauteur, int *largeur) // en cour ( Axel )
/*
    Effet :
    - Réinitialise le plateau (une seule tuile au centre) (free toute les les tiles sinon par de bouton rejoué et il faut fermer et ouvrir le jeu)
    - Réinitialise les points
    - Réinitialise les Meeples
    - écrase ou crée la liste des tuiles, les mélanges puis crée une pile
    - Réinitialise le turn tracker (le joueur 1 commence)
*/
{
    printf("Combien de joueur : \n");
    scanf("%d", &nbPlayers);

    printf("Combien de bot: \n");
    scanf("%d", &nbBot);

    if (list_player == NULL)
    {
        *list_player = init_player_list(nbBot);
    }
        
    if ( *grid != NULL)
    {
        free_Grid( grid );
    }
    
    struct Tile **tile_array;
    char *tokenArray[MAX_TOKEN_SIZE + 1] = {"route", "ville", "abbaye", "pre", "village", "blason"};
    tile_array = create_tile_array(CSV_TILE, tokenArray, MAX_TOKEN_SIZE);
    
    struct Coord *start_coord=init_coord(0,0);
    *grid = init_grid(tile_array[0], start_coord,NULL,NULL,NULL,NULL);

    shuffle(tile_array);
    struct Stack  *stack = NULL;
    array_to_stack(tile_array, &stack);
    struct Tile *variable_crée_à_cause_dune_decision_stupide;
    stack = stack_pop(stack,&variable_crée_à_cause_dune_decision_stupide);

    init_plateau(grid, dllist, hauteur, largeur); // dllist ressort et vaut NULL erreur de pointeur ?
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



void free_Grid( struct Grid **grid) // a tester
/* 
prend en paramètre une struct grid initialisée et la free pour être réutilisée
!!! S'assurer que le pointeur vers grid sois bien en haut à gauche du graph
*/
{
    struct Grid *tmp1;
    struct Grid *tmp2;
    while(tmp2!=NULL)
    {
        tmp2=(*grid)->bot;
        while(tmp1!=NULL)
        {
            tmp1=(*grid)->right;
            free(*grid);
            *grid=tmp1;
        }
        *grid=tmp2;
    }
    return;
}