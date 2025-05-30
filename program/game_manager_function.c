#include "../header/Carcassonne.h"
#include <string.h>

// ------------------------------
// ----Game manager fonctions----
// ------------------------------

char token_to_enum_types(char *token, char *tokenArray[]) // pour moder le jeu modifier cette fonction
/*
    Arguments :
        token : La chaine de charactère à annalyser.
        tokenArray : La liste des token correspondant 1 à 1 aux éléments de la macro types

    Retour :
        char find : La valeur représentant le type du morceau de la tuile.

    Description :
        La fonction effectue une bijection entre l'enum types et tokenArray

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

struct Tile **create_tile_array(char *csvTile, char *tokenArray[]) // Théo
/*
    Arguments :
        csvTile : Le nom du fichier .csv à ouvrir (utiliser la macro CSV_TILE).
        tokenArray : La liste des token correspondant 1 à 1 aux éléments de la macro types

    Retour :
        struct Tile **tileArray : Une tileArray contenant les objets Tile innitialisé

    Description :
        Crée la liste des tuiles à partir d'un fichier csv.
*/
{
    char *filename = csvTile;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return NULL;
    }
    char buff[BUFF_DEFAULT_SIZE];
    struct Tile **tileArray = malloc(sizeof(struct Tile *) * (NBTILE + 1));
    *(tileArray + NBTILE) = NULL;
    tileArray[NBTILE] = NULL; // Pour pouvoir itérer sur la liste en sachant quand s'arrêter (sur le NULL)
    char *token = malloc(sizeof(char) * MAX_TOKEN_SIZE);
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


void shuffle(struct Tile **tileArray) // Valentin
/*
    Arguments :
        tileArray : Une liste de pointeurs sur Tile.

    Retour :
        void

    Description :
        Mélange une liste de struct Tile**.
*/
{
    short i, rand1, rand2;
    struct Tile *temp;

    for (i = 0; i < (NBTILE-1) * (NBTILE-1); i++)
    {
        rand1 =rand() % (NBTILE-1);
        rand2 =rand() % (NBTILE-1);
        if (rand1 != rand2)
        {
            temp = tileArray[rand1];
            tileArray[rand1] = tileArray[rand2];
            tileArray[rand2] = temp;
        }
    }
}

void array_to_stack(struct Tile **tileArray, struct Stack **stack) // Valentin
/*
    Arguments :
        tileArray : Une liste de pointeur sur des struct Tile
        stack : Une liste de pointeur sur des struct stack

    Retour :
        void

    Description :
        La fonction transforme une liste de Tile en pile de Tile.

    Note :
        On considère que l'utilisateur donne le parametre stack non allouer.    
*/
{

    for (short i = 0; i < NBTILE - 1; i++)
    {
        *stack = stack_push(*stack, tileArray[i]);
    }
}




struct Tile *rot_tile(struct Tile *tile)
/*
    Arguments :
        tile : Une struct Tile bien alloué

    Retour :
        tile : La struct Tile a était modifié dans la fonction mais elle quand même retourné à la fin.

    Description :
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


void player_turn(char playerNumber, struct list_player *p_list, struct Stack **pioche, struct Grid **leftTopGrid, struct DLList **dllist, int *hauteur, int *largeur, struct list_player *listPlayer) // A FAIRE
/*
    Arguments :
        playerNumber : L'identifiant du joueur dont c'est le tour.
        p_list : Axel ici
        leftTopGrid : La tuile la plus en haut à gauche.
        dllist : La liste doublement chaîné des tuiles pottentiels (pour where_i_can_play).
        hauteur : La heuteur de la grille.
        largeur : La largeur de la grille.
        listPlayer : La liste des joueurs dans la partie.

    Retour :
        void

    Description :
        Cette fonction gère la tout le tour d'un joueur.
*/
{
    printf("\nTour du joueur %d\n", playerNumber);
    struct Tile *turn_tile = malloc(sizeof(struct Tile));
    struct Grid **play_grid = NULL;
    unsigned int index = 0;
    char pose = 0; // bool
    unsigned int token = -1;
    struct Grid **tmpGrid;

    *pioche = stack_pop(*pioche, &turn_tile);

    if(is_possible_tile(turn_tile,dllist)) //verifie si la tuile est possible a poser 
    {
        while (pose == 0) // Continue le temps que la tuile n'est pas posé (si on tourne la tuile ça boucle)
        {
            show_tile(turn_tile);
            show_point_and_nbmeeple(*p_list);
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
            scanf("%u", &token);
            if (token == 0)
            {
                rot_tile(turn_tile);
            }
            else if(token <= index)
            {
                pose = 1;
                *leftTopGrid = place_tile(leftTopGrid, play_grid[token - 1]->coord, turn_tile, dllist, hauteur, largeur);
                put_meeple(play_grid[token - 1],p_list,playerNumber-1);
                pointPlacedTile(play_grid[token - 1], listPlayer);
            }
            else
            {
                printf("vous ne pouvez pas faire ce choix\n");
            }
        }
    }
    else
    {
        printf("La tuile piocher ne peut pas être posé\n");
    }
}

void bot_turn(char playerNumber, struct list_player *p_list, struct Stack **pioche, struct Grid **leftTopGrid, struct DLList **dllist, int *hauteur, int *largeur, struct list_player *listPlayer)
/*
    Arguments :
        playerNumber : L'indentifiant du bot dont c'est le tour.
        p_list : Axel ici
        pioche : La pioche (stack).
        leftTopGrid : Un pointeur sur la tuile la plus en haut à gauche de la grille.
        dllist : La liste doublement chaîné des tuiles pottentiels (pour where_i_can_play).
        hauteur : La hauteur de la grille.
        largeur : La largeur de la grille.
        listPlayer : La liste des joueurs.

    Retour :
        void

    Description :
        Cette fonction fait jouer un bot.
    Note :
        Copie de player_turn modifier pour que le bot joue tout seul.
*/
{
    printf("\nTour du joueur(BOT) %d\n", playerNumber);
    struct Tile *turn_tile = malloc(sizeof(struct Tile));
    struct Grid **play_grid = NULL;
    unsigned int index = 0;
    char pose = 0;
    unsigned int token = -1;
    struct Grid **tmpGrid;

    *pioche = stack_pop(*pioche, &turn_tile);

    if(is_possible_tile(turn_tile,dllist)) 
    {
        while (pose == 0)
        {
            show_tile(turn_tile);
            show_point_and_nbmeeple(*p_list);
            play_grid = where_i_can_play(turn_tile, dllist);
            show_grid(*leftTopGrid, *largeur, *hauteur, play_grid);
            tmpGrid = play_grid;
            index = 0;
            while(tmpGrid[index] !=NULL)
            {
                printf("%d - x = %d et y = %d\n", index + 1, tmpGrid[index]->coord->x, tmpGrid[index]->coord->y);
                index++;
            }

            if(index>=1)
            {
                token=1;
                pose = 1;
                *leftTopGrid = place_tile(leftTopGrid, play_grid[token - 1]->coord, turn_tile, dllist, hauteur, largeur);

                put_meeple_bot(play_grid[token - 1],p_list,playerNumber-1);

                pointPlacedTile(play_grid[token - 1], listPlayer);
            }
            else
            {
                rot_tile(turn_tile);
            }
        }
    }
    else
    {
        printf("La tuile piocher ne peut pas être posé\n");
    }
}

struct Grid **where_i_can_play(struct Tile *tile, struct DLList **dllist) // Théo à faire
/*
    Arguments :
        tile : La tile précedement pioché par le joueur.
        dllist : Une liste doublement chainé contenant les tuiles potentiels pour cette fonction.

    Retour :
        La liste malloc des endroit ou il est possible de jouer (position tuile fixe).

    Description :
        Cette fonction returne la liste de tout les pointeurs sur Grid qui valide les conditions nécessaire à la posée d'une tuile.
*/
{
    struct Grid **gridArrray = calloc(NBTILE + 1, sizeof(struct Grid*)); // set à NULL avec calloc de taille NBTILE -> le nombre max de tuile dans le jeu
    struct DLList *tmpDllist = *dllist;
    int index = 0;
    while (tmpDllist!= NULL)
    {
        if ((tile->right == tmpDllist->data->tile->right || tmpDllist->data->tile->right == RIEN || (tile->right==VILLE && tmpDllist->data->tile->right==BLASON) || (tile->right==BLASON && tmpDllist->data->tile->right==VILLE)) 
        && (tile->top == tmpDllist->data->tile->top || tmpDllist->data->tile->top == RIEN || (tile->top==VILLE && tmpDllist->data->tile->top==BLASON) || (tile->top==BLASON && tmpDllist->data->tile->top==VILLE)) 
        && (tile->left == tmpDllist->data->tile->left || tmpDllist->data->tile->left == RIEN || (tile->left==VILLE && tmpDllist->data->tile->left==BLASON) || (tile->left==BLASON && tmpDllist->data->tile->left==VILLE)) 
        && (tile->bot == tmpDllist->data->tile->bot || tmpDllist->data->tile->bot == RIEN || (tile->bot==VILLE && tmpDllist->data->tile->bot==BLASON) || (tile->bot==BLASON && tmpDllist->data->tile->bot==VILLE))
        && is_a_potential_tile(tmpDllist->data->tile))
        {
            gridArrray[index] = tmpDllist->data;
            index++;
        }
        tmpDllist = tmpDllist->next;
    }
    return gridArrray;
}

char is_possible_tile(struct Tile *tile, struct DLList **dllist)
/*
    Arguments:
        struct Tile *tile, un pointeur sur une tuile
        struct DLList **dllist:un pointeur sur le début d'une liste doublement chainé
    
    Retour:
        char (0 ou 1)
    
    Description:
        cette fonction verifie si on peut poser la tuile sur ces 4 rotation 
        renvoie 1 si c'est possible , 0 sinon
*/
{
    struct Grid **gridArray[4]={NULL,NULL,NULL,NULL};

    for(int i=0;i<4;i++){
        gridArray[i]=where_i_can_play(tile,dllist);
        tile=rot_tile(tile);
    }

    int index[4]={0,0,0,0};

    for(int i=0;i<4;i++){
        while(gridArray[i][index[i]]!=NULL){
            index[i]=index[i]+1;
        }
    }
    
    free(gridArray[0]);free(gridArray[1]);free(gridArray[2]);free(gridArray[3]);
    char condition=index[0] || index[1] || index[2] || index[3] ;
    return condition;

}

char is_a_potential_tile(struct Tile *tile) // Théo FAIT
/*
    Arguments :
        tile : Une tuile à vérifier.

    Retour :
        0 si ce n'est pas une tuile potentielle
        1 si c'est une tuile potentielle

    Description :
        Permet de déterminer si il s'agit d'une tuile pottentielle.

    Note :
        Une tuile potentielle est une tuile parciellement remplie de enum qui sert pour la fonction where_i_can_place.
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
    Arguments :
        leftTopGrid : La case en haut à gauche de la Grid, elle mise à jour automatiquement par la fonction.
        largeur : La largeur max de la grille.
        hauteur : La hauteur max de la grille.
        coord : La pair de coordonnée qui dépasse de la grille (x + - 1 ou y + - 1 par rapport à la limite (le ou est exclusif dans cette phrase)).

    Retour :
        void

    Description :
        Agrandie la grille en innitialisant et reliant des nouveaux objets Grid de manère à rendre la grille renctangulaire.

    Note :
        Attention il est impossible d'agrandire à partir d'un angle, exemple, hauteur == 1, largeur == 1 
        *(leftTopGrid)->coord == (0,0) alors si coord == (1,1) la fonction ne fonctionne pas mais si 
        coord == (1,0) alors elle fonctionne.
*/
{
    int xMin = (*leftTopGrid)->coord->x;
    int yMax = (*leftTopGrid)->coord->y;
    int xMax = xMin + *largeur - 1;
    int yMin = yMax - *hauteur + 1; 

    struct Grid *tmpGrid = *leftTopGrid;

    struct Tile *newTile;
    struct Grid *preGrid = NULL;
    struct Grid *newGrid;

    if (coord.x > xMax)
    {
        for (int i = 0; i < *largeur - 1; i++)
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
    Arguments :
        trueGrid : La tuile qui vient d'être posé.    
        place : La position de la tuile potentielle par rapport à la tuile qui a était posé.
    Retour :
        void

    Description :
        Met à jour les tuiles pottentiels adjacente à la tuile qui vient d'être posé.

    Note :
        Grid doit être innitialisé au minimum à NULL et doit pointer sur une Tile innitialisé au minimum à NULL.
*/
{
    switch(place)
    {
        case RIGHT:
        {
            struct Grid *potentialGrid = trueGrid->right;

            if(is_a_potential_tile(potentialGrid->tile))
            {
                potentialGrid->coord->x = (trueGrid->coord->x) + 1;
                potentialGrid->coord->y = (trueGrid->coord->y);

                potentialGrid->tile->left = trueGrid->tile->right;
                trueGrid->right = potentialGrid;
                potentialGrid->left = trueGrid;
            }
            break;
        }
        

        case TOP:
        {
            struct Grid *potentialGrid = trueGrid->top;

            if(is_a_potential_tile(potentialGrid->tile))
            {
                potentialGrid->coord->x = (trueGrid->coord->x);
                potentialGrid->coord->y = (trueGrid->coord->y) + 1;

                potentialGrid->tile->bot = trueGrid->tile->top;
                trueGrid->top = potentialGrid;
                potentialGrid->bot = trueGrid;
            }
            break;
        }
        

        case LEFT:
        {
            struct Grid *potentialGrid = trueGrid->left;

            if(is_a_potential_tile(potentialGrid->tile))
            {
                potentialGrid->coord->x = (trueGrid->coord->x) - 1;
                potentialGrid->coord->y = (trueGrid->coord->y);

                potentialGrid->tile->right = trueGrid->tile->left;
                trueGrid->left = potentialGrid;
                potentialGrid->right = trueGrid;
            }
            break;
        }

        case BOT:
        {
            struct Grid *potentialGrid = trueGrid->bot;

            if(is_a_potential_tile(potentialGrid->tile))
            {
                potentialGrid->coord->x = (trueGrid->coord->x);
                potentialGrid->coord->y = (trueGrid->coord->y) - 1;

                potentialGrid->tile->top = trueGrid->tile->bot;
                trueGrid->bot = potentialGrid;
                potentialGrid->top = trueGrid;
            }
            break;
        }

        default: // MIDDLE et autre est une erreur.
            //ERROR  CASE
        break;
    }
    return;
}

struct Grid *find(struct Grid *leftTopGrid, struct Coord coord) // Théo FAIT
/*
    Arguments :
        leftTopGrid : La grid en haut à gauche.
        coord : Les coordonnées de la case recherchée.

    Retour :
        Grid * : Un pointeur sur la grille de coordonnées coord.
    Description :
        La fonction chercher la tuile à l'emplacement coord et renvoie le pointeur correspondant à cet emplacement.

    Note :
        Si la fonction return un pointeur sur NULL cela signifie que la case rechercher n'est pas innitialisé (pas dans le scope largeur hauteur de case innitialisé).
*/
{
    struct Grid *tmpGrid = leftTopGrid;
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
    Arguments :
        grid : La grid originelle de coord (0,0)
        hauteur : La hauteur max.
        largeur : La largeur max.

    Retour :
        struct Grid *grid : L'élément le plus en haut à gauche de la grid.

    Description :
        Place la première tuile et actualise la grille en conséquence.
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
/*
    Arguments :
        topLeftGrid : La tuile la plus en haut à gauche.
        dllist : La liste doublement chaîné des tuiles potentiels
        hauteur : La hauteur de la grille
        largeur : La largeur de la grille

    Retour :
        void

    Description :
        Pose la première tuile et innitialise ainsi le plateau.

    Note :
        topLeftGrid doit être NULL.
*/
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
    Arguments :
        tile : Un pointeur sur la tile précedement pioché par le joueur à placer.
        topLeftgrid : Un pointeur sur la grid la plus en haut à gauche qui s'actualise.
        coord : Les coordonnées de l'endroit ou placer la tuile sur *topLeftGrid.
        DLList : Un pointeur sur le première élément de la liste doublement chainé contenant les tuiles potentielles
        Hauteur : La hauteur max de la grille, soit la hauteur de la plus grande colone.
        Largeur : La largeur maximale de la grille, soit la longeur de la plus grande ligne.

    Retour :
        struct Grid *topLeftGrid : La tuile en hautla plus en haut à gauche sur la grille.

    Description :
        Place la tuile à l'emplacement indiqué, actualise la grille de taille variable et met à jour la liste doublement chaîné les tuile potentiels pour les autres fonctions 
*/
{
    
    struct Grid *gridFind = find(*topLeftGrid, *coord); // trouve ou poser la tuile aussi possible pour plus d'optimisation de trouver la tuile dans dllist

    if(gridFind->left==NULL){
        coord->x=coord->x-1;
        upscale(topLeftGrid, largeur, hauteur, *coord);
        coord->x=coord->x+1;
    }

    if(gridFind->right==NULL){
        coord->x=coord->x+1;
        upscale(topLeftGrid, largeur, hauteur, *coord);
        coord->x=coord->x-1;
    }

    if(gridFind->top==NULL){
        coord->y=coord->y+1;
        upscale(topLeftGrid, largeur, hauteur, *coord);
        coord->y=coord->y-1;
    }

    if(gridFind->bot==NULL){
        coord->y=coord->y-1;
        upscale(topLeftGrid, largeur, hauteur, *coord);
        coord->y=coord->y+1;

    }

    gridFind->tile = tile; // pose la tuile
    update_potential_tile(gridFind, RIGHT);
    update_potential_tile(gridFind, TOP);
    update_potential_tile(gridFind, LEFT);
    update_potential_tile(gridFind, BOT);

    if(is_a_potential_tile(gridFind->right->tile) && gridFind->tile->right==gridFind->right->tile->left){
        *dllist=DLList_push_end(*dllist,gridFind->right);
    }

    if(is_a_potential_tile(gridFind->left->tile) && gridFind->tile->left==gridFind->left->tile->right){
        *dllist=DLList_push_end(*dllist,gridFind->left);
    }

    if(is_a_potential_tile(gridFind->top->tile) && gridFind->tile->top==gridFind->top->tile->bot){
        *dllist=DLList_push_end(*dllist,gridFind->top);
    }

    if(is_a_potential_tile(gridFind->bot->tile) && gridFind->tile->bot==gridFind->bot->tile->top){
        *dllist=DLList_push_end(*dllist,gridFind->bot);
    }

    return *topLeftGrid;
}

void enum_to_char(enum types type, int coul)
/*
    Arguments:
        enum types type : le type de structure qu'on considere (ROUTE, VILLE, ABBAYES, PRE, VILLAGE, BLASON)
        int coul : couleur du joueur

    Description:
        affiche en fonction du type de structure une partie de la tuile 
*/
{
    switch(type){
        case RIEN:
            //printf("\x1b[48;2;%d;%d;%dm",0, 0, 0);
            if (coul == 0)
            {
                printf("   ");
            }
            else printf(" %d ",coul);
            printf("\x1b[0m");
            break;
            
        case ROUTE:
            printf("\x1b[48;2;%d;%d;%dm",168, 92, 33);
            if (coul == 0)
            {
                printf(" R ");
            }
            else printf(" %d ",coul);
            printf("\x1b[0m");
            break;

        case VILLE:
            printf("\x1b[48;2;%d;%d;%dm",110,110,110);
            if (coul == 0)
            {
            printf(" V ");
            }
            else printf(" %d ",coul);
            printf("\x1b[0m");
            break;

        case ABBAYES:
            printf("\x1b[48;2;%d;%d;%dm",241,196,15);
            if (coul == 0)
            {
            printf(" A ");
            }
            else printf(" %d ",coul);
            printf("\x1b[0m");
            break;

        case PRE:
            printf("\x1b[48;2;%d;%d;%dm",35,155,86);
            if (coul == 0)
            {
            printf("   ");
            }
            else printf(" %d ",coul);
            printf("\x1b[0m");
            break;

        case VILLAGE:
            printf("\x1b[48;2;%d;%d;%dm",102,188,186);
            if (coul == 0)
            {
            printf(" v ");
            }
            else printf(" %d ",coul);
            printf("\x1b[0m");
            break;

        case BLASON:
            printf("\x1b[48;2;%d;%d;%dm",101,112,255);
            if (coul == 0)
            {
            printf(" B ");
            }
            else printf(" %d ",coul);
            printf("\x1b[0m");
            break;
        
        
    }
}

void show_wplace(int j, int h)
/*
    Arguments:
        int j : 0 , 1 ou 2 , indique si on considere la partie haut , millieu ou basse
        int h : un nombre

    Description:
        sert a affiche un endroit on peut poser la tuile
                ---------
           Ex:  ----h----
                ---------
*/
{
    if(j == 0 || j == 2)
    {
        printf("---------  ");
        return;
    }

    if(h+1 >=10)
        printf("----%d---  ",h+1);
    else
        printf("----%d----  ",h+1);     
}

void choose_w_show(unsigned char y, struct Grid *tab)
/*
    Arguments:
        unsigned char y : 0 , 1 ou 2 , indique si on considere la partie haut , millieu ou basse de la tuile
        struct Grid *tab: un pointeur sur un element de la grid

    Description:
        affiche une partie de la tuile
*/
{
    if(is_a_potential_tile(tab->tile)){
        enum_to_char(RIEN,0);
        enum_to_char(RIEN,0);
        enum_to_char(RIEN,0);
    }

    else
    {
        switch (y)
        {
            case 0:
                //test pour savoir si  il  faut  comble le trou avec  une ville ou un pré
                if( tab->tile->top == VILLE && tab->tile->left == VILLE && tab->tile->middle==VILLE)
                {
                    enum_to_char(VILLE,0);
                }
                else enum_to_char(PRE,0);
                if(is_meeple_on_tile(tab->tile) && tab->tile->meeplePlace == TOP){
                    enum_to_char(tab->tile->top,tab->tile->meeple->coulPlayer);
                }
                else enum_to_char(tab->tile->top,0);

                //test pour savoir si  il  faut  comble le trou avec  une ville ou un pré
                if( tab->tile->top == VILLE && tab->tile->right == VILLE && tab->tile->middle==VILLE)
                {
                    enum_to_char(VILLE,0);
                }
                else enum_to_char(PRE,0);

                break;
            case 1:
                if(is_meeple_on_tile(tab->tile) && tab->tile->meeplePlace == LEFT){
                    enum_to_char(tab->tile->left,tab->tile->meeple->coulPlayer);
                }
                else enum_to_char(tab->tile->left,0);
                if(is_meeple_on_tile(tab->tile) && tab->tile->meeplePlace == MIDDLE){
                    enum_to_char(tab->tile->middle,tab->tile->meeple->coulPlayer);
                }
                else enum_to_char(tab->tile->middle,0);
                if(is_meeple_on_tile(tab->tile) && tab->tile->meeplePlace == RIGHT){
                    enum_to_char(tab->tile->right,tab->tile->meeple->coulPlayer);
                }
                else enum_to_char(tab->tile->right,0);
                break;
            case 2:
                //test pour savoir si  il  faut  comble le trou avec  une ville ou un pré
                if( tab->tile->bot == VILLE && tab->tile->left == VILLE && tab->tile->middle==VILLE)
                {                    
                    enum_to_char(VILLE,0);
                }
                else enum_to_char(PRE,0);
                if(is_meeple_on_tile(tab->tile) && tab->tile->meeplePlace == BOT){
                    enum_to_char(tab->tile->bot,tab->tile->meeple->coulPlayer);
                }
                else enum_to_char(tab->tile->bot,0);

                //teste pour savoir si  il  faut  comble le trou avec  une ville ou un pré
                if( tab->tile->bot == VILLE && tab->tile->right == VILLE && tab->tile->middle==VILLE)
                {
                    enum_to_char(VILLE,0);
                }
                else enum_to_char(PRE,0);
        }
    }
    printf("  ");//ligne temporaire pour aider a differencier les tuile
}

void show_tile(struct Tile *tile)
/*
    Arguments:
        struct Tile *tile
    
    Description:
        Affiche une tuile individuelle
*/
{
    enum_to_char(PRE,0);
    enum_to_char(tile->top,0);
    enum_to_char(PRE,0);
    printf("\n");
    enum_to_char(tile->left,0);
    enum_to_char(tile->middle,0);
    enum_to_char(tile->right,0);
    printf("\n");
    enum_to_char(PRE,0);
    enum_to_char(tile->bot,0);
    enum_to_char(PRE,0);
    printf("\n");
}

void show_grid(struct Grid *tab, unsigned char x, unsigned char y, struct Grid **w_place)
/*
    Arguments:
        struct Grid *tab : un pointeur sur le coin en haut a gauche de la grille
        unsigned char x : longueur
        unsigned char y : hauteur
        struct Grid **w_place: une liste de pointeur ou on peut poser Une tuile tuile

    Description:
        Affiche l'entiereté de la grille , avec en plus des indication sur les endroit on peut peut poser la tuile
*/
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
            if(j==1){
                printf("Y=%3d ",temp_y->coord->y);
            }
            else
            {
                printf("      ");
            }
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
                    if (w_place[h]->coord->x == temp_y->coord->x && w_place[h]->coord->y == temp_y->coord->y)
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
                        enum_to_char(PRE,0);
                        enum_to_char(PRE,0);
                        enum_to_char(PRE,0);
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

    temp_x=tab;
    printf("      ");
    for(t_x=0;t_x<x;t_x++){
        printf("  X=%3d    ",temp_x->coord->x);
        temp_x=temp_x->right;
    }
    printf("\n");

}
struct Stack *start_game(struct list_player **list_player, struct Grid **grid, struct DLList **dllist, int *hauteur, int *largeur) // en cour ( Axel )
/*
    Arguments :
        list_player : La liste des joueurs.
        grid : Un pointeur sur un pointeur grid.
        llist : Un pointeur sur une liste doublement chaîné.
        hauteur : La largeur de la grille.
        largeur : La largeur de la grille.

    Retour :
        Stack *stack : Retourne la pioche.

    Description :
        - Réinitialise le plateau (une seule tuile au centre) (free toute les les tiles sinon par de bouton rejoué et il faut fermer et ouvrir le jeu)
        - Réinitialise les points
        - Réinitialise les Meeples
        - écrase ou crée la liste des tuiles, les mélanges puis crée une pile
        - Réinitialise le turn tracker (le joueur 1 commence)
*/
{
    do{
        printf("Combien de joueur : (max:8) \n");
        scanf("%d", &nbPlayers);
    }while(nbPlayers<1 || nbPlayers>8);

    do{
        printf("Combien de bot: (max:%d) \n",nbPlayers);
        scanf("%d", &nbBot);
    }while(nbBot<0 || nbBot>nbPlayers);


    if (*list_player == NULL)
    {
        *list_player = init_player_list(nbBot);
    }
        
    if ( *grid != NULL)
    {
        free_Grid( grid );
    }
    struct Tile **tile_array;
    char *tokenArray[MAX_TOKEN_SIZE + 1] = {"route", "ville", "abbaye", "pre", "village", "blason"};
    tile_array = create_tile_array(CSV_TILE, tokenArray);
    
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
    Arguments:
        struct list_player list: la struct contenant la liste de joueur

    Retour:
        void

    Description:
        Affiche les points en fin de partie
*/
{
    for( int  i = 0; i < nbPlayers ; i++)
    {
        printf("Le joueur n°%d possède %d points\n",i+1, list.player[i]->points );
    }
}

void show_point_and_nbmeeple(struct list_player list)
/*
    Arguments:
        struct list_player list: la struct contenant la liste de joueur

    Retour:
        void

    Description:
        Affiche les points et le nombre de meeple de chaque joueurs
*/
{
    for( int  i = 0; i < nbPlayers ; i++)
    {
        printf("Le joueur n°%d possède %d points et %d meeple\n",i+1, list.player[i]->points,list.player[i]->nbMeeple);
    }
}


void free_Grid( struct Grid **grid) // a tester
/* 
    Arguments:
        struct Grid **grid : Un pointeur d'un pointeur sur l'element en haut a gauche de la grille

    Description:
        libere l'espace memoire occupé par la grille
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

void bienvenue()
/*
    Description:
        affichage de depart
*/
{
    printf("@@@@@@@@     @@@@@@@@     @@@@@@@@     @@@@@@@@     @@@@@@@@     @@@@@@@@     @@@@@@@@     @@@@@@@@     @      @     @      @     @@@@@@@@\n");
    printf("@            @      @     @      @     @            @      @     @            @            @      @     @@     @     @@     @     @       \n");
    printf("@            @      @     @      @     @            @      @     @            @            @      @     @ @    @     @ @    @     @       \n");
    printf("@            @@@@@@@@     @@@@@@@@     @            @@@@@@@@     @@@@@@@@     @@@@@@@@     @      @     @  @   @     @  @   @     @@@@@@@@\n");
    printf("@            @      @     @    @       @            @      @            @            @     @      @     @   @  @     @   @  @     @       \n");
    printf("@            @      @     @     @      @            @      @            @            @     @      @     @    @ @     @    @ @     @       \n");
    printf("@@@@@@@@     @      @     @      @     @@@@@@@@     @      @     @@@@@@@@     @@@@@@@@     @@@@@@@@     @     @@     @     @@     @@@@@@@@\n");   
}