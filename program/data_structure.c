#include "../header/Carcassonne.h"

// ----------
// --Stack--
// ---------

struct Stack *stack_push(struct Stack *stack, struct Tile *tile)
/*
    Arguments:
        struct Stack *stack: Un pointeur sur le dernier element de la pile
        struct Tile *tile: Un pointer sur un espace mémoire ou se trouve une tile à push sur la stack
    
    Retour:
        struct Stack *new_stack: la pile actualisé

    Description:
        alloue le nouvelle element new_stack lie sont prochain element au pointeur stack 
        et retourne new_stack
*/
{
    struct Stack *new_stack = malloc(sizeof(struct Stack));
    new_stack->data = tile;
    new_stack->next = stack;
    return new_stack;
}

struct Stack *stack_pop(struct Stack *stack, struct Tile **tileSlot)
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
{
    *tileSlot = stack->data;
    struct Stack *tmpStack = stack->next;
    free(stack);
    return tmpStack;
}

char is_stack_not_empty(struct Stack *stack)
/*
    Arguments:
        struct Stack *stack: Un pointeur sur la pile

    Retour:
        char : 0 si stack est a NULL 1 sinon

    Description:
        teste si la pile est vide
*/
{
    if (stack == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// ----------
// --DLList-- 
// ----------

// DLList : Linked list


struct DLList *DLList_push_end(struct DLList *DLList, struct Grid *grid)
/*
    Argument:
        DLList: un pointeur sur une liste doublement chaine
        grid: Un pointer sur un espace mémoire ou se trouve grid
    
    Retour:
        struct DLList *DLList : La liste doublement chainé actualisé

    Description:
        ajoute en fin de liste doublement chaine l'element grid
*/
{
    struct DLList *newDLList = malloc(sizeof(struct DLList));
    newDLList->data = grid;
    newDLList->next = NULL;
    newDLList->prev = NULL;
    struct DLList *tmp = DLList;
    if (DLList == NULL)
    {
        return newDLList;
    }
    while (tmp->next != NULL)
    {
        tmp=tmp->next;
    }
    tmp->next = newDLList;
    newDLList->prev = tmp;
    
    return DLList;
}

void DLList_pop(struct DLList **DLList, struct Grid **gridSlot)
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
{
    struct DLList *tmpDLList;
    if (*DLList == NULL)
    {
        return; // ERROR CASE
    }

    *gridSlot = (*DLList)->data;
    struct DLList *DLListTmp = NULL;

    if ((*DLList)->next == NULL)
    {
        if ((*DLList)->prev == NULL)
        {
            // case NULL TO_DELETE NULL
            free(*DLList);
            *DLList = NULL;
            return;
        }
        else
        {
            // case TRUC TO_DELETE NULL
            DLListTmp = (*DLList)->prev;
            (*DLList)->prev->next = NULL;
            free(*DLList);
            *DLList = DLListTmp;
            return;
        }
    }
    else if ((*DLList)->prev == NULL)
    {
        // case NULL TO_DELETE TRUC
        DLListTmp = (*DLList)->next;
        (*DLList)->next->prev = NULL;
        free(*DLList);
        *DLList = DLListTmp;
        return;
    }
    else
    {
        // case TRUC TO_DELETE TRUC
        DLListTmp = (*DLList)->prev;
        (*DLList)->prev->next = (*DLList)->next;
        (*DLList)->next->prev = (*DLList)->prev;
        free(*DLList);
        *DLList = DLListTmp;
        return;
    }
}

// ------------
// ----Grid----
// ------------

struct Coord *init_coord(int x,int y)
/*
    Arguments:
        int x: entier represantant la coordonnee x
        int y: entier represantant la coordonnee y

    Retour:
        struct Coord* C: la struct Coord allouer en initialisé au coordonnée x et y

    Description:
        Alloue une struct coord et l'initialise avec les coordonnee x et y
*/
{
    struct Coord *C=malloc(sizeof(struct Coord));
    C->x=x;
    C->y=y;
    return C;
}

struct Grid *init_grid(struct Tile *tile, struct Coord *coord, struct Grid *right, struct Grid *left, struct Grid *bot, struct Grid *top)
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
{
    // il aurait fallut faire le malloc de Coord ici mais c'est trop tard cette fonction ne peut être modifier sans casser tout le projet
    struct Grid *new_grid= malloc(sizeof(struct Grid));
    new_grid->marquer  = 0;
    new_grid->coord = coord;
    new_grid->tile= tile;
    new_grid->right = right;
    new_grid->left = left;
    new_grid->bot = bot;
    new_grid->top = top;

    if(new_grid->right!=NULL && new_grid->right->left==NULL){
        new_grid->right->left=new_grid;
    }

    if(new_grid->top!=NULL && new_grid->top->bot==NULL){
        new_grid->top->bot=new_grid;
    }

    if(new_grid->left!=NULL && new_grid->left->right==NULL){
        new_grid->left->right=new_grid;
    }

    if(new_grid->bot!=NULL && new_grid->bot->top==NULL){
        new_grid->bot->top=new_grid;
    }
    return new_grid;
}