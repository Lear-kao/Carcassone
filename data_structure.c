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

struct DLList
{
    
    struct Tile *data;
    struct DLList *prev;
    struct DLList *next;
};


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
        tmp=tmp->next;
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