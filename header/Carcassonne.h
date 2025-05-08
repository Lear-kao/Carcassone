#ifndef CARCASSONNE_H
#define CARCASSONNE_H

#define CSV_TILE "tuiles_base_simplifiees.csv"
#define NBTILE 72
#define BUFF_DEFAULT_SIZE 1024
#define MAX_TOKEN_SIZE 7
#define NB_TOKEN_TYPE 6
#define NBMEEPLE_DEFAULT 8  // theo 
#define NB_BOT_DIFFICULTY 1


#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

enum types { ROUTE, VILLE, ABBAYES, PRE, VILLAGE, BLASON, RIEN };
enum meeplePlace { MP_RIGHT, MP_TOP, MP_LEFT, MP_BOT, MP_MIDDLE, NO_MEEPLE};
enum places {RIGHT, TOP, LEFT, BOT, MIDDLE};

extern int turnTraker;
extern int finJeu;
extern int nbPlayers;
extern int nbBot;
extern int v_marquer;

//-----------------------
// ----Data structure----
// ----------------------


// ----------
// --DLList-- 
// ----------


// DLList : Linked list

struct DLList
{
    struct Grid *data;
    struct DLList *prev;
    struct DLList *next;
};


struct DLList *DLList_push_end(struct DLList *DLList, struct Grid *grid); //tester valider
/*
    Argument:
        DLList: un pointeur sur une liste doublement chaine
        grid: Un pointer sur un espace mémoire ou se trouve grid
    
    Retour:
        struct DLList *DLList : La liste doublement chainé actualisé

    Description:
        ajoute en fin de liste doublement chaine l'element grid
*/


void DLList_pop(struct DLList **DLList, struct Grid **gridSlot); // tester valider
/*
    Argument:
        struct DLList **DLList: un pointeur sur un pointeur sur l'element a supprimer
        struct Grid **gridSlot un pointeur sur un espace mémoire ou mettre la grid

    Retour:
        void
    
    Description:
        supprime l'element DLList de la liste doublement chaîné et met l'element dans gridSlot
    
    Note:
        La fonction s'occupe elle même de relier les bout de la liste doublement chaîne la ou l'element a ete supprimer
*/

struct Coord *init_coord(int x, int y);//tester valider
/*
    Arguments:
        int x: entier represantant la coordonnee x
        int y: entier represantant la coordonnee y

    Retour:
        struct Coord* C: la struct Coord allouer en initialisé au coordonnée x et y

    Description:
        Alloue une struct coord et l'initialise avec les coordonnee x et y
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
    Arguments:
        struct Stack *stack: Un pointeur sur le premier element de la pile
        struct Tile *tile: Un pointer sur un espace mémoire ou se trouve une tile à push sur la stack
    
    Retour:
        struct Stack *new_stack: la pile actualisé

    Description:
        alloue le nouvelle element new_stack lie sont prochain element au pointeur stack 
        et retourne new_stack
*/
// Ne pas oublier de gérer la cas création de stack

struct Stack *stack_pop(struct Stack *stack, struct Tile **tileSlot); // FAIT theo
/*
    Arguments:
        struct Stack *stack: Un pointeur sur le dernier element de la pile
        struct Tile **tileSlot: Un pointeur sur un espace mémoire ou mettre la tile pop
    
    Retour:
        struct Stack *tmp_stack: la pile actualisé
    
    Description:
        met l'element de la pile sur l'espace memoire tileSlot puis stock le pointeur sur 
        l'element suivant de la pile dans la variable tmpStack puis libere l'espace memoire
        de l'element de la pile et retourne tmpStack
*/

char is_stack_not_empty(struct Stack *stack); // FAIT
/*
    Arguments:
        struct Stack *stack: Un pointeur sur la pile

    Retour:
        char : 0 si stack est a NULL 1 sinon

    Description:
        teste si la pile est vide
*/


// ------------
// ----Grid----
// ------------

struct Coord
{
    int x;
    int y;
};

struct Grid
/*
    Un morceau de grille permettant de relier les tiles entre elles
*/
{
    int marquer;
    struct Coord *coord;
    struct Tile *tile;
    struct Grid *right;
    struct Grid *left;
    struct Grid *bot;
    struct Grid *top;
};


struct Grid *init_grid(struct Tile *tile, struct Coord *coord, struct Grid *right, struct Grid *left, struct Grid *bot, struct Grid *top);//tester mais peut être a approfondir
/*
    Arguements:
        Struct Tile *tile: Un pointeur sur une struct Tile
        Struct Coord *coord: Un pointeur sur une struct Coord
        Struct Grid *right: Un pointeur sur une struct Grid qui et censé se trouver a "droite" de la nouvelle grid
        Struct Grid *left: Un pointeur sur une struct Grid qui et censé se trouver a "gauche" de la nouvelle grid
        Struct Grid *bot: Un pointeur sur une struct Grid qui et censé se trouver en "bas" de la nouvelle grid
        Struct Grid *top: Un pointeur sur une struct Grid qui et censé se trouver en "haut" de la nouvelle grid
    
    Retour:
        struct Grid *new_grid: Un pointeur sur le nouvelle element de la grid
    
    Description:
        alloue new_grid puis initialise ces valeur ainsi que ses voisin (les 4 paramametre struct Grid *) et retourne new_grid

    Note:
        il faut faire attention a ce que les voisin repointe bien vers new_grid nouvellement creer

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

struct Tile *turn_tile(struct Tile *tile); // tester valider
/*
    tile : Une tuile

    return : Une tuile malloc tourné de 90° dans le sens trigo.
*/

// ---------------
// ----Player-----
// ---------------

struct Player
{
    /*
        nbMeeple : Nombre de Meeple du joueur
        points : Nombre de points du joueur
        coulPLayer : ?????????????????????
    */
    char bot;
    int coulPlayer;
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
    char nbbot;
}; // penser a la fin a l'optimiser et l'enlever 

// ----PLayers fonctions----

struct Player *init_player(int couleur,char bot); // FAIT theo
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

void bienvenue();
/*
    fonction qui affiche l'ensemble des règle de carcasonne et les texte de bienvenue
*/

char token_to_enum_types(char *token, char *tokenArray[]); // theo si qq veut faire un mod Carcasonne avec de nouvelles tuiles il doit modifier cette fonction
/*
    La fonction effectue une bijection entre l'enum types et tokenArray
    
    token : la chaine de charactère à annalyser
    tokenArray : La liste des différents tokens possible

    return: 
    Le type de la case associé au token.
*/

struct Tile **create_tile_array(char *csvTile, char *tokenArray[]); // Théo
/*
    Crée la liste des tuiles à partir d'un fichier csv.

    csvTile : Le nom du fichier .csv à ouvrir (utiliser la macro CSV_TILE).
    tokenArray : La liste des token correspondant 1 à 1 aux éléments de la macro types 

    return : 
    Une tileArray contenant les objets Tile innitialisé

    Attention ne pas allouez la mémoire pour struct tile **tileArray
*/


struct list_player *init_player_list(char nbbot); // Fait (Axel) 
/*
    Crée une liste de pointeurs qui pointe sur un Player,
    un pointer sur NULL est ajouté à la fin pour faciliter
    les iterations sur la liste (condition d'arrêt)
*/

void shuffle(struct Tile **tileArray); // Valentin c'est peut-être mieux si size est une macro
/*S
    tileArray : Une liste de pointeurs sur Tile.

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

void enum_to_char(enum types type, int coul);
/*
    Convertie un enum en char affichable (V pour ville et v pour village).
*/

void show_tile(struct Tile *tile); // A TESTER theo
/*
    Affiche une tile.
*/

char is_a_potential_tile(struct Tile *tile); // Théo FAIT
/*
    Return 0 si ce n'est pas une tuile potentielle
    Return 1 si c'est une tuile potentielle

    Information : Une tuile potentielle est une tuile parciellement remplie de enum qui sert pour la fonction where_i_can_place.
*/

void upscale(struct Grid **leftTopGrid, int *largeur, int *hauteur, struct Coord coord); // Théo A TESTER
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

void update_grid_size(struct Grid *origineGrid, int *largeur, int *hauteur, struct Coord coord); // Théo A TESTER
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

void update_potential_tile(struct Grid *trueGrid, enum places place); // Théo A TESTER
/*
    trueGrid : La vraie tuile qui vient d'être posé.
    potentialGrid : Une grid autours d'une tuile qui vient d'être posé. Grid doit être innitialisé au minimum à NULL et doit pointer sur une Tile innitialisé au minimum à NULL.
    place : La position de la tuile potentielle par rapport à la tuile qui a était posé.

    Cette fonction a pour but d'actualiser une des tuiles pottentielles qui se trouve autours d'une tuile qui vient d'être posé.
*/

struct Grid *find(struct Grid *grid, struct Coord coord); // Théo FAIT
/*
    grid : La grid en haut à gauche.
    coord : Les coordonnées de la case recherchée.

    return : Un pointeur sur la grille de coordonnées coord

    Erreur : Si la fonction return un pointeur sur NULL cela
    signifie que la case rechercher n'est pas innitialisé 
    (pas de le scope largeur hauteur de case innitialisé).
*/

struct Grid *first_tile(struct Grid *grid, int *hauteur, int *largeur, struct DLList *dllist); // Théo A TESTER
/*
    Place la première tuile et actualise la grille en conséquence.
    grid : La grid originelle de coord (0,0)
    largeur : La largeur max.
    hauteur : La hauteur max.

    Return value : L'élément le plus en haut à gauche de la grid.
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

struct Grid *first_grid(struct Grid *grid, int *hauteur, int *largeur, struct DLList **dllist); // Théo A TESTER
/*
    Place la première tuile et actualise la grille en conséquence.
    grid : La grid originelle de coord (0,0)
    largeur : La largeur max.
    hauteur : La hauteur max.

    Return value : L'élément le plus en haut à gauche de la grid.
*/

struct Grid *place_tile(struct Grid **topLeftGrid, struct Coord *coord, struct Tile *tile, struct DLList **dllist, int *hauteur, int *largeur); // Théo TESTER AVEC LE GAMEMANAGER
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

void init_plateau(struct Grid **topLeftGrid, struct DLList **dllist, int *hauteur, int *largeur);


void player_turn(char playerNumber, struct list_player *p_list, struct Stack **pioche, struct Grid **leftTopGrid, struct DLList **dllist, int *hauteur, int *largeur, struct list_player *listPlayer); // A FAIRE
/*
    playerNumber : Le numéro du joueur

    Cette fonction pop la stack de tile
    et propose ensuite au joueur de choisir
    un emplacmement pour poser sa tuile
    avec la fonction where_i_can_play
*/
void bot_turn(char playerNumber, struct list_player *p_list, struct Stack **pioche, struct Grid **leftTopGrid, struct DLList **dllist, int *hauteur, int *largeur, struct list_player *listPlayer);
/*
    playerNumber : Le numéro du joueur

    Cette fonction pop la stack de tile
    et propose ensuite au joueur de choisir
    un emplacmement pour poser sa tuile
    avec la fonction where_i_can_play

    premiere fonction de tour automatique pour les bot
    il n'a aucune intelligence et ne fais que prendre le premier endroit disponible ou poser la tuile piocher
*/

struct Grid **where_i_can_play(struct Tile *tile, struct DLList **dllist); // Théo à faire
/*
    tile : La tile précedement pioché par le joueur

    dllist : Une liste doublement chainé contenant les tuiles potentiels pour cette fonction.

    return : La liste malloc des endroit ou il est possible de jouer.
*/

char is_possible_tile(struct Tile *tile, struct DLList **dllist);
/*
    verifie si avec une tuile donné elle est impossible a poser
*/


void show_grid(struct Grid *tab, unsigned char x, unsigned char y, struct Grid **w_place);
/*
    Affiche la grille du jeu en ascii art en minimisant l'espace occupé 
*/

struct Stack *start_game(struct list_player **list_player, struct Grid **grid, struct DLList **dllist, int *hauteur, int *largeur); // en cour ( Axel )
/*
    Effet :
    - Réinitialise le plateau (une seule tuile au centre) (free toute les les tiles sinon par de bouton rejoué et il faut fermer et ouvrir le jeu)
    - Réinitialise les points
    - Réinitialise les Meeples 
    - écrase ou crée la liste des tuiles, les mélanges puis crée une pile
    - Réinitialise le turn tracker (le joueur 1 commence)
    !!! a voir si il faut pas passer la  liste de  struct  tile  en paramètre
*/

void free_Grid( struct Grid **grid); // fait (Axel)
/* 
prend en paramètre une struct grid initialisée et la free pour être réutilisée
*/

void *show_point_and_nbmeeple(struct list_player list);

/* 
--------------------------------------
--------------------------------------
----Fonction de comptage de points----
--------------------------------------
--------------------------------------
*/

char isFinishedAbbaye(struct Grid *grille);// tester valider ? (verifier quand même une fois les test svp)
/* 
    Arguments:
        struct Grid *grille: Un pointeur sur l'element de la grid
    
    Retour:
        char point : le nombre de point associé represente par le nombre de tuile pose autour de l'abbaye
    
    Description:
        Compter les points abbaye.
        La fonction vérifie si l'abaye est complète avec une simple  vérification des tuiles autours.
        Elle vérifie chaque tuiles autour et  pour  chaques tuiles compte les points, si finJeu est != 0 (la partie est finie)
        la fonction envoie les points même si elle n'est pas complètement entourée, sinon elle envoie 0 si,l'abbaye n'est
        pas complètement entourée.

    Note:
        -il faut verifier si l'element de la grille (la tuile) a bien une abbaye dessus
*/

char isFinishedCity( struct Grid *grille, char *unfinished ); //tester (avec count_point_city)
/* 
    Arguments:
        struct Grid *grille : Un pointeur sur un element de la grid
        char *unfinished : un pointeur sur un char (represantant si une ville est fermer ou non)
    
    Retour:
        char point : le nombre de point
    
    Description:
        Compter les points villes
        Elle vérifie chaque tuiles ville conntecté à celle envoyé, si chacune de celles-ci sont complètes (on ne peut plus ajouter de 
        tuiles villes) la fonction renvoie les points si finJeu == 0 et la  ville est complète ou si finJeu == 1.
*/

char isFinishedRoad(struct Grid *grille, char *unfinished); //tester (avec countPointRoad)
/* 
    Arguments:
        struct Grid *grille : Un pointeur sur l'element de la grid
        char *unfinished : un pointeur sur un char (representant si une route est fermer ou non)

    Retour:
        char point : le nombre de point

    Description:
        Compter les points d'une route
        Elle vérifie chaque tuiles route conntecté à celle envoyé, si chacune de celles-ci sont complètes (on ne peut plus ajouter de 
        tuiles route) la fonction renvoie les points si finJeu == 0 et la  route est complète ou si finJeu == 1.
*/

char countPointRoad(struct Grid *grille,enum places start); //tester
/*
    Arguments:
        struct Grid *grille : Un pointeur sur un element de la Grid
        enum places start : une direction
    
    Retour:
        char point : le nombre de point

    Description:
        A appeller, elle se charge d'un cas particulier d'appel de grille avant d'appeler 'isFinishedRoad()'
        Il faut lui donner la position de la route à tester (gauche,droite,haut,bas,millieu) where = [0:4]
*/

char count_point_city(struct Grid *grille, enum places a); //tester
/*
    Arguments:
        struct Grid *grille: Un pointeur sur un element de la grid
        enum places a: une direction
    
    Retour:
        char point : le nombre de point
    
    Description:
        elle se charge d'un cas particulier d'appel de grille avant d'appeler 'isFinishedCity()'
        Il faut lui donner la position de la ville à tester (gauche,droite,haut,bas,millieu) a = [0:4]
    
    Note:
        cette fonction est a appeler en premier
*/

void pointPlacedTile(struct Grid *justPlaced, struct list_player *listPlayer);
/*
    Arguments:
        struct Grid *justPlaced : Un pointeur sur l'element de la grid qui vient juste d'être placer
        struct list_player *list : Un pointeur sur une struct list_player
    
    Description:
        Compter les points si une structure est finit , si c'est le cas il faut distribuer 
        les points au joueur concerner
*/

void give_point(char *list_meeple_player, struct list_player *list, char point);//tester
/* 
    Arguments:
        char *list_meeple_player : la liste du nombre de meeple par joueur trouvée dans le parcour de la ville/route/abbaye
        struct list_player *list : un pointeur sur la liste des joeurs
        char point : le nombre de point

    Description:
        Distribue les point au joueur concerne en fonction de la list de meeple trouver
        (les point sont distribuer au(x) joueur(s) ayant le maximum de meeple dans la zone
         si il y a egalite entre 2 ou plusieurs joueurs les points sont distribuer a chaqun)
*/

void secondary_verification(struct Grid *justPlaced, struct list_player *list, enum types middle);
/*
    Arguments:
        struct Grid *justPlaced : Un pointeur sur l'element de la grid qui vient juste d'être placer
        struct list_player *list : Un pointeur sur une struct list_player
        enum types middles : le type du millieu de la tuile dans justPlaced

    Description:
        effectue des verification secondaire dans la distribution des points

*/
void *end_game_points_counter( struct list_player list ); // à tester (axel)
/* affiche les points en fin de partie */
int max(char *list);//tester
/* calcule le maximum d'un tableau de charactère */

/* 
--------------------------------------
--------------------------------------
-----------fonction meeple------------
--------------------------------------
--------------------------------------
*/

char nbMeepleVille( struct Grid *grille,  int coul_player , enum places a); //tester
/* compte le nombre de meeple dans une ville d'une couleur précise */

char nbMeepleVilleEncap(struct Grid *grille, int coul_player, enum meeplePlace origin); //tester
/* 
    on appel cette fonction pour appeler nbMeepleVille et pouvoir 
    incrementer v_marquer
*/

char nbMeepleVille_nocolor( struct Grid *grille,enum places a); //tester

char nbMeepleVille_nocolorEncap(struct Grid *grille,enum meeplePlace origin); //tester
/* 
    Parametre:
        grille: la grille a tester
        where: ou chercher le meeple (parametre de recursion)

    on appel cette fonction pour appeler nbMeepleVille_nocolor et pouvoir 
    incrementer v_marquer
*/

char nbMeepleAbbaye( struct Grid *grille, int i); //tester
/* vérifie si un meeple d'une couleur précise est présent dans l'abbaye */
char nbMeepleAbbaye_nocolor(struct Grid *grille);//tester
/* pareil mais sans la couleur du meeple */

char where_is_meeple(  int type, struct Tile tile);//tester 
/* vérifie si le meeple posée estbien sur le type  de structure (ABBAYE/VILLE/VILLAGE/ROUTE) recherché */

char what_color_is_meeple(int color, struct Tile tile);//tester
/* vérifie si le meeple posée sur la  tuile est bien de la bonne couleur */
struct Grid* searchAbbaye(struct Grid* grille); //tester (normalement)
/* cherche et renvoie la grille d'une tuile abbaye si celle-ce est présente autour de la case en entrée et renvoie NULL sinon */

char searchMeeple(struct Tile tile ,int where);
/*verifie si il y a un meeple a l'endroit souhaiter*/

char countMeepleRoad(struct Grid *grille, enum places start, int color);//tester
char meepleRoad(struct Grid *grille, int color); //tester avec countmeepleroad

int* where_i_can_put(struct Grid *grid);
char countMeepleRoad_nocolor(struct Grid *grille, enum places start);
char meepleRoad_nocolor(struct Grid *grille ,enum meeplePlace origin);
void put_meeple(struct Grid* grid, struct list_player *p_list, char pnumber);
void put_meeple_bot(struct Grid *grid,struct list_player *p_list, char pnumber);
void remove_meeple(struct Grid* justPlaced, struct list_player *p_list);

void remove_meepleVille(struct Grid *grille,int coul_player , enum places a);
void remove_meepleVilleEncap(struct Grid *grille,int coul_player , enum meeplePlace origin);




#endif // CARCASSONNE_H