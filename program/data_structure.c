#include "../header/Carcassonne.h"

// ----------
// --Stack--
// ---------

struct Stack *stack_push(struct Stack *stack, struct Tile *tile)
/*
    stack : La pile
    tile : Un pointer sur un espace mémoire ou se trouve une tile à push sur la stack

    return : La stack actualisé

*/
{
    struct Stack *new_stack = malloc(sizeof(struct Stack));
    new_stack->data = tile;
    new_stack->next = stack;
    return new_stack;
}

struct Stack *stack_pop(struct Stack *stack, struct Tile **tileSlot)
/*
    stack : La pile
    tileSlot : Un pointer sur un espace mémoire ou mettre la tile pop

    return : La stack actualisé
*/
{
    *tileSlot = stack->data;
    struct Stack *tmpStack = stack->next;
    free(stack);
    return tmpStack;
}

char is_stack_not_empty(struct Stack *stack)
/*
    stack : La pile

    return : 0 si la stack est vide, 1 sinon
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
    DLList : La liste chainé
    grid : Un pointer sur un espace mémoire ou se trouve grid

    return : La liste actualisé 
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
    DLList : L'élément de la liste chainé à supprimer. Une fois l'élément supprimé le pointeur prend la valeur de l'élément précédent si il existe sinon l'élément suivant sinon NULL
    gridSlot : Un pointer sur un espace mémoire ou mettre le grid pop

    return : La liste actualisé 
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
    x: coordonnée x
    y: coordonnee y

    Cette fonction alloue une struct coord et l'initialise
    avec les coordonnée x et y

    return: un pointeur vers un struct Coord
*/
{
    struct Coord *C=malloc(sizeof(struct Coord));
    C->x=x;
    C->y=y;
    return C;
}

struct Grid *init_grid(struct Tile *tile, struct Coord *coord, struct Grid *right, struct Grid *left, struct Grid *bot, struct Grid *top)
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

/* NON UTILISE
//-------------
//-plateboard--
//-------------

struct l_ch
{
    struct Grid *place;
    struct l_ch *col; // droite
    struct l_ch *line; // bas
}

struct l_ch *l_ch_push_end_col(struct l_ch *l_ch, struct Grid *grid)
/*
    l_ch : La liste chainé
    grid : Un pointer sur un espace mémoire ou se trouve une grid 

    return : La liste actualisé 
*/
/*
{
    struct l_ch *new_lc_h = malloc(sizeof(struct l_ch));
    struct l_ch *tmp_l_ch = l_ch;
    new_l_ch->place = grid;
    new_l_ch->col = NULL;
    new_l_ch->line = NULL;

    while (tmp_l_ch->col != NULL)
    {
        tmp_l_ch = tmp_l_ch->col;
    }
    tmp_l_ch->col = new_lc_h;
}
*/
//struct l_ch *l_ch_push_end_line(struct l_ch *l_ch, struct Grid *grid)
/*
    l_ch : La liste chainé
    tile : Un pointer sur un espace mémoire ou se trouve une grid

    return : La liste actualisé 
*/
/*
{
    struct l_ch *new_lc_h = malloc(sizeof(struct l_ch));
    struct l_ch *tmp_l_ch = l_ch;
    new_l_ch->place = grid;
    new_l_ch->col = NULL;
    new_l_ch->line = NULL;

    while (tmp_l_ch->line != NULL)
    {
        tmp_l_ch = tmp_l_ch->line;
    }
    tmp_l_ch->line = new_lc_h;
}
*/