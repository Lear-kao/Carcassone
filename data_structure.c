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
    tile : Un pointer sur un espace mémoire ou mettre la tile pop

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

// ------------
// ----Grid----
// ------------