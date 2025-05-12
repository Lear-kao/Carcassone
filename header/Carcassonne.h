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
    Arguments
        enum types right : le type de la tuile sur le morceau a droite de la tuile
        enum types top : le type de la tuile sur le morceau en haut de la tuile
        enum types left : le type de la tuile sur le morceau a gauce de la tuile
        enum types bot : le type de la tuile sur le morceau en bas de la tuile
        enum types middle : le type de la tuile sur le morceau au milleu de la tuile

    Retour:
        struct Tile *tile : Un pointeur sur une tuile nouvellement allouer

    Description:
        Alloue puis initialise la tuile avec nos arguments
        (evidemment par defaut il n'y a pas de meeple au debut)
        puis retourne le pointeur sur la tuile nouvellement allouer
*/

char is_meeple_on_tile(struct Tile *tile); // FAIT theo
/*
    Arguments:
        struct Tile *tile : Un pointeur sur une tuile
    
    Retour:
        char 0 ou 1

    Description:
        - 0 si aucun Meeple n'est sur la tile
        - 1 si un Meeple est sur la tile
*/

struct Tile *turn_tile(struct Tile *tile); // tester valider
/*
    Arguments:
        struct Tile *tile : Un pointeur sur une tuile

    Retour:
        struct Tile *tile : Un pointeur sur une tuile

    Description:
        effectue une rotation de 90° dans le sens trigonometrique de la tuile 
        (la fonction genere une nouvelle tuile avec la rotation appliqué et la retourne)
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
};

// ----PLayers fonctions----

struct Player *init_player(int couleur,char bot); // FAIT theo
/*
    Arguments:
        int couleur : la couleur du joueur
        char bot : (0 = vrai joueur , 1 = un bot)

    Retour:
        struct Player *player: Un pointeur sur une struct Player nouvellement allouer

    Description:
        allouer et creer un nouveau jouer 
        et l'initialise avec nos arguments et nos valeur par defaut (NBMEEPLE_DEFAULT=8)
*/

struct list_player *init_player_list(char nbbot); // Axel


char is_meeple_on_player(struct Player *player); // FAIT Theo/Axel
/*
    Player : L'objet Player

    return :
    - 0 si le joueur n'a aucun Meeple
    - 1 si )il reste au moins un Meeple au Joueur
*/

// ------------------------------
// ----Game manager fonctions----
// ------------------------------

void bienvenue();
/*
    Description:
        affichage de depart
*/

char token_to_enum_types(char *token, char *tokenArray[]); // theo si qq veut faire un mod Carcasonne avec de nouvelles tuiles il doit modifier cette fonction
/*
    Arguments :
        token : La chaine de charactère à annalyser.
        tokenArray : La liste des token correspondant 1 à 1 aux éléments de la macro types

    Retour :
        char find : La valeur représentant le type du morceau de la tuile.

    Description :
        La fonction effectue une bijection entre l'enum types et tokenArray

*/

struct Tile **create_tile_array(char *csvTile, char *tokenArray[]); // Théo
/*
    Arguments :
        csvTile : Le nom du fichier .csv à ouvrir (utiliser la macro CSV_TILE).
        tokenArray : La liste des token correspondant 1 à 1 aux éléments de la macro types

    Retour :
        struct Tile **tileArray : Une tileArray contenant les objets Tile innitialisé

    Description :
        Crée la liste des tuiles à partir d'un fichier csv.
*/

void shuffle(struct Tile **tileArray); // Valentin c'est peut-être mieux si size est une macro
/*
    Arguments :
        tileArray : Une liste de pointeurs sur Tile.

    Retour :
        void

    Description :
        Mélange une liste de struct Tile**.
*/

void array_to_stack(struct Tile **tileArray, struct Stack **stack); // Fait
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

void reset_points(struct Player *player); //Fait
/*
    Arguments:
        struct Player *player : Un pointeur sur un joueur:
    
    Description:
        reinitialiser le nombre de points du joueur a 0
*/

void reset_meeples(struct Player *player);//Fait
/*
    Arguments:
        struct Player *player : Un pointeur sur un joueur:
    
    Description:
        reinitialiser le nombre de meeple du joueur a la valeur par defaut (NBMEEPLE_DEFAULT=8)
*/

struct Tile *rot_tile(struct Tile *tile); //Fait
/*
    Arguments :
        tile : Une struct Tile bien alloué

    Retour :
        tile : La struct Tile a était modifié dans la fonction mais elle quand même retourné à la fin.

    Description :
        Tourne la tuile de 90° dans le sens trigo si elle existe.    
*/

void enum_to_char(enum types type, int coul);
/*
    Arguments:
        enum types type : le type de structure qu'on considere (ROUTE, VILLE, ABBAYES, PRE, VILLAGE, BLASON)
        int coul : couleur du joueur

    Description:
        affiche en fonction du type de structure une partie de la tuile 
*/

void show_wplace(int j, int h);
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

void choose_w_show(unsigned char y, struct Grid *tab);
/*
    Arguments:
        unsigned char y : 0 , 1 ou 2 , indique si on considere la partie haut , millieu ou basse de la tuile
        struct Grid *tab: un pointeur sur un element de la grid

    Description:
        affiche une partie de la tuile
*/

void show_tile(struct Tile *tile); // A TESTER theo
/*
    Arguments:
        struct Tile *tile
    
    Description:
        Affiche une tuile individuelle
*/

char is_a_potential_tile(struct Tile *tile); // Théo FAIT
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

void upscale(struct Grid **leftTopGrid, int *largeur, int *hauteur, struct Coord coord); // Théo A TESTER
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
    Arguments :
        trueGrid : La tuile qui vient d'être posé.    
        place : La position de la tuile potentielle par rapport à la tuile qui a était posé.
    Retour :
        <nom et type du retour> : <description de ce que retourne la fonction>

    Description :
        Met à jour les tuiles pottentiels adjacente à la tuile qui vient d'être posé.

    Note :
        Grid doit être innitialisé au minimum à NULL et doit pointer sur une Tile innitialisé au minimum à NULL.
*/

struct Grid *find(struct Grid *grid, struct Coord coord); // Théo FAIT
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
*/

short points_ville(struct Grid *grid); // Axel et blason
short points_abbayes(struct Grid *grid); // Axel
short points_pre(struct Grid *grid); // Axel

struct Grid *first_grid(struct Grid *grid, int *hauteur, int *largeur, struct DLList **dllist);
/*
    Arguments :
        grid : La grid originelle de coord (0,0)
        hauteur : La hauteur max.
        largeur : La largeur max.

    Retour :
        struct Grid *grid : L'élément le plus en haut à gauche de la grid.

    Description :
        Place la première tuile et actualise la grille en conséquence.

    Note :
        <(optionnel) message pour les développeurs sur les subtilités de la fonction>  
*/

struct Grid *place_tile(struct Grid **topLeftGrid, struct Coord *coord, struct Tile *tile, struct DLList **dllist, int *hauteur, int *largeur); 
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

void init_plateau(struct Grid **topLeftGrid, struct DLList **dllist, int *hauteur, int *largeur);
/*
    Arguments :
        topLeftGrid : La tuile la plus en haut à gauche.
        dllist : La liste doublement chaîné des tuiles potentiels
        hauteur : La hauteur de la grille
        largeur : La largeur de la grille

    Retour :
        <nom et type du retour> : <description de ce que retourne la fonction>

    Description :
        <Pose la première tuile et innitialise ainsi le plateau.

    Note :
        topLeftGrid doit être NULL.
*/

void player_turn(char playerNumber, struct list_player *p_list, struct Stack **pioche, struct Grid **leftTopGrid, struct DLList **dllist, int *hauteur, int *largeur, struct list_player *listPlayer);
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

void bot_turn(char playerNumber, struct list_player *p_list, struct Stack **pioche, struct Grid **leftTopGrid, struct DLList **dllist, int *hauteur, int *largeur, struct list_player *listPlayer);
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

struct Grid **where_i_can_play(struct Tile *tile, struct DLList **dllist); // Théo à faire
/*
    Arguments :
        tile : La tile précedement pioché par le joueur.
        dllist : Une liste doublement chainé contenant les tuiles potentiels pour cette fonction.

    Retour :
        La liste malloc des endroit ou il est possible de jouer (position tuile fixe).

    Description :
        Cette fonction returne la liste de tout les pointeurs sur Grid qui valide les conditions nécessaire à la posée d'une tuile.
*/

char is_possible_tile(struct Tile *tile, struct DLList **dllist);
/*
    verifie si avec une tuile donné elle est impossible a poser
*/


void show_grid(struct Grid *tab, unsigned char x, unsigned char y, struct Grid **w_place);
/*
    Arguments:
        struct Grid *tab : un pointeur sur le coin en haut a gauche de la grille
        unsigned char x : longueur
        unsigned char y : hauteur
        struct Grid **w_place: une liste de pointeur ou on peut poser Une tuile tuile

    Description:
        Affiche l'entiereté de la grille , avec en plus des indication sur les endroit on peut peut poser la tuile
*/

struct Stack *start_game(struct list_player **list_player, struct Grid **grid, struct DLList **dllist, int *hauteur, int *largeur); // en cour ( Axel )
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

void free_Grid( struct Grid **grid); // fait (Axel)
/* 
    Arguments:
        struct Grid **grid : Un pointeur d'un pointeur sur l'element en haut a gauche de la grille

    Description:
        libere l'espace memoire occupé par la grille
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
    Un pointeur vers  la tuile de la Grille de Jeu ou se trouve l'Abbaye.
Retour:
    Le nombre de point pour la structure Abbaye.
Description:
    La  fonction va simplement parcourir les 8  tuiles autours de la tuile Abbaye et renvoyer:
        - Les 8 points  de la structure si celle-ci est complète.
        - 0 point si la structure est incomplète et que l'on est pas en fin de partie.
        - Un nombre de point correspondant au nombre de tuile autour si c'est lafin de la partie.
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
/* 
    Arguments:
        char *list : une liste de nombre

    Retour:
        int max : le nombre maximum de la liste

    Description:
        cherche le maximum de la liste
*/

/* 
--------------------------------------
--------------------------------------
-----------fonction meeple------------
--------------------------------------
--------------------------------------
*/

char nbMeepleVille( struct Grid *grille,  int coul_player , enum places a); //tester
/*
    struct Grid *grille : Un pointeur sur un element de la grid
    int coul_player : la couleur du joueur
    enum places a : une direction

    Retour:
        char res : le nombre de meeple dans la ville

    Description:
        A appeller, elle se charge d'un cas particulier d'appel de grille avant d'appeler 'nbMeepleVilleEncap()'
        Il faut lui donner la position de la ville à tester (gauche,droite,haut,bas,millieu) where = [0:4]
*/

char nbMeepleVilleEncap(struct Grid *grille, int coul_player, enum meeplePlace origin); //tester
/*
    Arguments:
        struct Grid *grille : Un pointeur sur un element de la grid
        int coul_player : la couleur du joueur
        enum meeplePlace origin : un endroit sur la tuile

    Retour:
        char cmpt : le nombre de meeple

    Description:
        Compter les meeple d'une ville
        Elle vérifie chaque tuiles ville conntecté à celle envoyé
        et compte le nombre de meeple d'une couleur sur celui ci

    Note:
        meeplePlace origin est pour un cas particulier ou il y a 2 ville non connecteur sur une tuile 
        ex: [PPP]  ici si on vient de la gauche on ne veut pas verifier si il y a un meeple
            [VPV]  sur la ville a droite
            [PPP]
*/

char nbMeepleVille_nocolor( struct Grid *grille,enum places a); //tester
/*
    Arguments:
        struct Grid *grille : Un pointeur sur un element de la grid
        enum places a : une direction

    Retour:
        char res : le nombre de meeple dans la ville

    Description:
        A appeller, elle se charge d'un cas particulier d'appel de grille avant d'appeler 'nbMeepleVille_nocolorEncap()'
        Il faut lui donner la position de la ville à tester (gauche,droite,haut,bas,millieu) where = [0:4]
*/

char nbMeepleVille_nocolorEncap(struct Grid *grille,enum meeplePlace origin); //tester
/*
    Arguments:
        struct Grid *grille : Un pointeur sur un element de la grid
        enum meeplePlace origin : un endroit sur la tuile

    Retour:
        char cmpt : le nombre de meeple

    Description:
        Compter les meeple d'une ville
        Elle vérifie chaque tuiles ville conntecté à celle envoyé
        et compte le nombre de meeple sur celui ci

    Note:
        meeplePlace origin est pour un cas particulier ou il y a 2 ville non connecteur sur une tuile 
        ex: [PPP]  ici si on vient de la gauche on ne veut pas verifier si il y a un meeple
            [VPV]  sur la ville a droite
            [PPP]
*/

char nbMeepleAbbaye( struct Grid *grille, int i); //tester
/* 
    Arguments:
        struct Grid *grille : Un pointeur sur un element de la grille
        int color : la couleur du joueur

    Retour:
        char 0 ou 1

    Description:
        si c'est une abbaye ou si il y a une abbaye autour de *grille
        retourne le nombre de meeple d'une couleur d'une abbaye
        (par definition une abbaye ne peut qu'avoir un meeple)
*/

char nbMeepleAbbaye_nocolor(struct Grid *grille);//tester
/* 
    Arguments:
        struct Grid *grille : Un pointeur sur un element de la grille

    Retour:
        char 0 ou 1

    Description:
        si c'est une abbaye ou si il y a une abbaye autour de *grille
        retourne le nombre de meeple d'une abbaye
        (par definition une abbaye ne peut qu'avoir un meeple)
*/

char where_is_meeple(  int type, struct Tile tile);//tester 
/*
    Arguments:
        int type : le type de structure qu'on cherche (ABBAYE/VILLE/VILLAGE/ROUTE)
        struct Tile tile : une tuile
    
    Retour: 
        char 0 ou 1

    Description:
        vérifie si le meeple posée est bien sur le type  de structure (ABBAYE/VILLE/VILLAGE/ROUTE) recherché
*/

char what_color_is_meeple(int color, struct Tile tile);//tester
/*
    Arguments:
        int color : la couleur du joueur
        struct Tile *tile : une tuile

    Retour:
        char 0 ou 1

    Description:
        vérifie si le meeple posée sur la  tuile est bien de la bonne couleur
*/

struct Grid** searchAbbaye(struct Grid* grille); //tester (normalement)
/*
    Arguments:
        struct Grid *grille : Un pointeur sur un element de la grid

    Retour:
        struct Grid * : une grid ou il y a une abbaye (ou NULL)

    Description:
        cherche dans les 8 tuile autour de grid si il y a une abbaye si oui la retourne , sinon retourne NULL

    Note:
        si struct Grid *grille est deja abbayes la fonction la retourne evidemment directement
*/

char searchMeeple(struct Tile tile ,int where);
/*
    Arguments:
        struct Tile tile: une tuile
        int where: l'endroit ou on veut regarder

    Retour:
        char : 0 ou 1
    
    Description:
        regarde si il y a un meeple a l'endroit souhaité
*/

char countMeepleRoad(struct Grid *grille, enum places start, int color);//tester
/*
    Arguments:
        struct Grid *grille : Un pointeur sur un element de la Grid
        enum places start : une direction
        int color : la couleur du joueur
    
    Retour:
        char nbmeeple : le nombre de meeple d'une couleur

    Description:
        A appeller, elle se charge d'un cas particulier d'appel de grille avant d'appeler 'meepleRoad()'
        Il faut lui donner la position de la route à tester (gauche,droite,haut,bas,millieu) start = [0:4]
*/

char meepleRoad(struct Grid *grille, int color,enum meeplePlace origin); //tester avec countmeepleroad
/* 
    Arguments:
        struct Grid *grille : Un pointeur sur l'element de la grid
        int color : la couleur du joueur

    Retour:
        char cmp : le nombre de meeple

    Description:
        Compter les meeple d'une couleur d'une route
        Elle vérifie chaque tuiles route conntecté à celle envoyé,
        et compte le nombre de meeeple sur celui ci
*/

int* where_i_can_put(struct Grid *grid);
/*
    Arguments:
        struct Grid *grid : Un pointeur sur un element de la grid

    Retour:
        int *tab : un tableau de 5 entier allouer

    Description:
        appelle les differente fonction de comptage de meeple (sans couleur) 
        et verifier si on peut poser un meeple sur la tuile associé a *grid
*/

char countMeepleRoad_nocolor(struct Grid *grille, enum places start);
/*
    Arguments:
        struct Grid *grille : Un pointeur sur un element de la Grid
        enum places start : une direction
    
    Retour:
        char nbmeeple : le nombre de meeple d'une couleur

    Description:
        A appeller, elle se charge d'un cas particulier d'appel de grille avant d'appeler 'meepleRoad_nocolor()'
        Il faut lui donner la position de la route à tester (gauche,droite,haut,bas,millieu) start = [0:4]
*/

char meepleRoad_nocolor(struct Grid *grille ,enum meeplePlace origin);
/* 
    Arguments:
        struct Grid *grille : Un pointeur sur l'element de la grid

    Retour:
        char cmp : le nombre de meeple

    Description:
        Compter les meeple d'une route
        Elle vérifie chaque tuiles route conntecté à celle envoyé,
        et compte le nombre de meeeple sur celui ci
*/

void put_meeple(struct Grid* grid, struct list_player *p_list, char pnumber);
void put_meeple_bot(struct Grid *grid,struct list_player *p_list, char pnumber);

/* 
Fonction pour retirer les  meeples
*/


void removeMeepleRoadStart(struct Grid *grille,enum places a);
/* 
Argument:
    -Un pointeur sur la tuile  ou il faut retirer les  meeples.
Retour:
    -Void.
Description:
    La fonction remet le meeple dans  la réserve du  joueur puis le retire du la tuile.
*/

void remove_meeple_precice(struct Grid *grille,enum places where);
/* 
Argument:
    -Un pointeur sur la tuile  ou il faut retirer les  meeples.
    -un enum meeplePlace
Retour:
    -Void.
Description:
    La fonction remet le meeple dans  la réserve du  joueur puis le retire du la tuile.
    Elle est plus specifique que pour remove_meeple car on specifie l'endroit ou on peut poser le meeple
*/

void removeMeepleRoad(struct Grid *grille, enum meeplePlace origin);
void removeMeepleVilleStart(struct Grid *grille,enum places a);
void removeMeepleVille( struct Grid *grille, enum meeplePlace origin);

void removeMeepleAbbaye( struct Grid *grille);
/* 
Argument:
    -la grille ou se situe l'abbaye et  le meeple à retirer.
Retour :
    -Void
Description:
    Retire le  meeple de la geille si il est situé  sur une Abbaye.
*/

void remove_meeple(struct Grid *grille);
/* 
Argument:
    -Un pointeur sur la tuile  ou il faut retirer les  meeples.
Retour:
    -Void.
Description:
    La fonction remet le meeple dans  la réserve du  joueur puis le retire du la tuile.
*/

void finDuJeu(struct Grid *grille, struct list_player *list);
/*
Argument:
    Un pointeur vers la dernière tuile  posée.
    La liste des joueurs.
Retour:
    Void.
Description:
    La  fonction prend en paramètre la dernière tuile posée et compte les points que permet d'acquérir cette tuiles sur d'après la structure présente au milieu.
    (nottament si une abbaye est présente dans les alentours).
*/

#endif // CARCASSONNE_H