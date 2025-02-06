#include "Carcassonne.h"

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


struct DLList *DLList_push_end(struct DLList *DLList, struct Tile *tile)
/*
    DLList : La liste chainé
    tile : Un pointer sur un espace mémoire ou se trouve une tile à à la fin de la liste chainé

    return : La liste actualisé 
*/
{
    struct DLList *newDLList = malloc(sizeof(struct DLList));
    newDLList->data = tile;
    newDLList->next = NULL;
    newDLList->prev = NULL;
    struct DLList *tmp = DLList;
    if (DLList == NULL)
    {
        return newDLList;
    }
    while (tmp->next != NULL)
    {
        tmp++;
    }
    tmp->next = newDLList;
    newDLList->prev = tmp;
    
    return DLList;
}

void DLList_pop(struct DLList *DLList, struct Tile **tileSlot)
/*
    DLList : L'élément de la liste chainé à supprimer
    tileSlot : Un pointer sur un espace mémoire ou mettre la tile pop

    return : La liste actualisé 
*/
{
    struct DLList *tmpDLList;
    if (DLList == NULL)
    {
        return; // ERROR CASE
    }
    if (DLList->next == NULL)
    {
        if (DLList->prev == NULL)
        {
            *tileSlot = DLList->data;
            free(DLList);
            return;
        }
        else
        {
            // case TRUC NULL
            DLList->prev->next = NULL;
            free(DLList);
            return;
        }
    }
    else if (DLList->prev == NULL)
    {
        // case NULL TRUC
        DLList->next->prev = NULL;
        free(DLList);
        return;

    }
    else
    {
        // case TRUC TRUC
        DLList->prev->next = DLList->next;
        DLList->next->prev = DLList->prev;
        free(DLList);
        return;
    }
}

// ------------
// ----Grid----
// ------------

struct Grid *init_grid(struct Tile *tile, struct Coord *coord, struct Grid *right, struct Grid *left, struct Grid *bot, struct Grid *top)
{
    struct Grid *new_grid= malloc(sizeof(struct Grid));
    new_grid->coord = coord;
    new_grid->tile = tile;
    new_grid->right = right;
    new_grid->left = left;
    new_grid->bot = bot;
    new_grid->top = top;
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