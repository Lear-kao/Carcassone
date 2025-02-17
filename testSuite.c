#include <criterion/criterion.h>
#include <criterion/new/assert.h> // si bug essayer de retirer le /new

#include "Carcassonne.h"
#include "tile.c"
#include "player.c"
#include "game_manager_function.c"
#include "data_structure.c"

Test(all, init_player)
{
    struct Player *player = malloc(sizeof(struct Player));
    init_player(player);
    cr_expect(player->nbMeeple == 8);
    cr_expect(player->points == 0);
}

Test(all, init_player_list)
{
    char nbPlayer = 8;
    struct Player **list_player = init_player_list(nbPlayer);
    int i = 0;
    while( list_player[i] != NULL )
    {
        i++;
    }
    cr_expect(i == 8);
}

Test(all, is_meeple_on_player)
{
    struct Player *player = malloc(sizeof(struct Player));
    init_player(player);
    cr_expect(is_meeple_on_player(player) > 0);
    player->nbMeeple = 0;
    cr_expect(is_meeple_on_player(player) == 0);
}

Test(all, init_tile)
{
    struct Tile *tile = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    cr_assert(
        tile->right == VILLE 
        && tile->top == ROUTE 
        && tile->left == ROUTE 
        && tile->bot == VILLE 
        && tile->middle == ROUTE 
        && tile->meeple == NO_MEEPLE
        );
}

Test(all, is_meeple_on_tile)
{
    struct Tile *tile = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    cr_assert(tile->meeple == NO_MEEPLE);
}

Test(all, stack_push_stack_vide)
{
    struct Stack *stack;
    struct Tile *tile = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile);
    cr_assert(stack->data->right == VILLE);
}

Test(all, stack_push_stack_non_vide)
{
    struct Stack *stack;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile1);

    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile2);
    cr_assert(stack->data->right == ROUTE && stack->next->data->right == VILLE);
}

Test(all, stack_pop)
{
    struct Stack *stack;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile1);

    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile2);
    cr_assert(stack->data->right == ROUTE && stack->next->data->right == VILLE);
    // Attention test peut-être pas assez unitaire ?

    struct Tile *tileSlot = malloc(sizeof(struct Tile));
    stack = stack_pop(stack, &tileSlot);
    cr_assert(tileSlot->right == ROUTE);
    cr_assert(stack->data->right == VILLE);
}

Test(all, is_stack_not_empty)
{
    struct Stack *stack;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile1);

    cr_assert(is_stack_not_empty(stack) == 1);

    struct Stack *stackEmpty = NULL;
    cr_assert(is_stack_not_empty(stackEmpty) == 0);

}


Test(all, reset_points)
{
    struct Player *player=(struct Player *)malloc(sizeof(struct Player));
    player->points=380;
    reset_points(player);
    cr_assert(player->points==0);
}

Test(all, reset_meeples)
{
    struct Player *player=(struct Player *)malloc(sizeof(struct Player));
    player->nbMeeple=8;
    reset_points(player);
    cr_assert(player->points==0);
}


Test(all, create_tile_array)
{
    struct Tile **tileArray;
    char *tokenArray[MAX_TOKEN_SIZE + 1] = {"route", "ville", "abbaye", "pre", "village", "blason"};
    tileArray = create_tile_array(CSV_TILE, tokenArray, MAX_TOKEN_SIZE);
    cr_assert(tileArray[0]->right == ROUTE);
    cr_assert(tileArray[0]->middle == ROUTE);
    cr_assert(tileArray[1]->middle == BLASON);
    cr_assert(tileArray[1]->right == BLASON);
    cr_assert(tileArray[70]->middle == PRE);
}

Test(all, DLList_push_end)
{
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);

    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);

    struct Tile *tile3 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, PRE);
    
    struct DLList *db_list=NULL;//initialisation a NULL

    //Test premiere insertion
    db_list=DLList_push_end(db_list,tile1);
    cr_assert(db_list !=NULL);
    cr_assert(db_list->data->right==VILLE);
    cr_assert(db_list->prev==NULL && db_list->next==NULL);

    //Test Deuxieme insertion
    db_list = DLList_push_end(db_list,tile2);
    cr_assert(db_list->next != NULL);
    cr_assert(db_list->next->prev == db_list);
    cr_assert(db_list->next->data->right == ROUTE);
    cr_assert(db_list->prev == NULL);
    cr_assert(db_list->next->next == NULL);

    //Test Troisième insertion
    db_list=DLList_push_end(db_list,tile3);
    cr_assert(db_list->next->next != NULL);
    cr_assert(db_list->next->next->prev == db_list->next);
    cr_assert(db_list->next->next->data->middle == PRE);
    cr_assert(db_list->prev == NULL);
    cr_assert(db_list->next->next->next == NULL);
}

Test(all, DLList_pop)
{
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);

    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);

    struct Tile *tile3 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, PRE);

    struct DLList *db_list=NULL;

    
    // db_list=DLList_push_end(db_list,tile2);
    // db_list=DLList_push_end(db_list,tile3);

    //test de supression
    struct Tile *tileSlot = malloc(sizeof(struct Tile));
    
    //cas db_list NULL
    DLList_pop(db_list,&tileSlot); 
    cr_assert(db_list == NULL);

}

