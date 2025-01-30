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

Test(all, is_meeple_on_player)
{
    struct Player *player = malloc(sizeof(struct Player));
    init_player(player);
    cr_expect(is_meeple_on_player(player) == 1);
    player->nbMeeple = 0;
    cr_expect(is_meeple_on_player(player) == 0);
}

Test(all, init_tile)
{
    struct Tile *tile = malloc(sizeof(struct Tile));
    init_tile(tile, VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    cr_assert(tile->right == VILLE && tile->top == ROUTE && tile->left == ROUTE && tile->bot == VILLE && tile->middle == ROUTE && tile->meeple == NO_MEEPLE);
}

Test(all, is_meeple_on_tile)
{
    struct Tile *tile = malloc(sizeof(struct Tile));
    init_tile(tile, VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    cr_assert(tile->meeple == NO_MEEPLE);
}

Test(all, stack_push_stack_vide)
{
    struct Stack *stack;
    struct Tile *tile = malloc(sizeof(struct Tile));
    init_tile(tile, VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile);
    cr_assert(stack->data->right == VILLE);
}

Test(all, stack_push_stack_non_vide)
{
    struct Stack *stack;
    struct Tile *tile1 = malloc(sizeof(struct Tile));
    init_tile(tile1, VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile1);

    struct Tile *tile2 = malloc(sizeof(struct Tile));
    init_tile(tile2, ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile2);
    cr_assert(stack->data->right == ROUTE && stack->next->data->right == VILLE);
}

Test(all, stack_pop)
{
    struct Stack *stack;
    struct Tile *tile1 = malloc(sizeof(struct Tile));
    init_tile(tile1, VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile1);

    struct Tile *tile2 = malloc(sizeof(struct Tile));
    init_tile(tile2, ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
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
    struct Tile *tile1 = malloc(sizeof(struct Tile));
    init_tile(tile1, VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile1);

    cr_assert(is_stack_not_empty(stack) == 1);

    struct Stack *stackEmpty = NULL;
    cr_assert(is_stack_not_empty(stackEmpty) == 0);

}

Test(all, create_tile_array)
{
    struct Tile **tileArray;
    char *tokenArray[MAX_TOKEN_SIZE + 1] = {"route", "ville", "abbaye", "pre", "village", ""};
    tileArray = create_tile_array(CSV_TILE, tokenArray, MAX_TOKEN_SIZE);
    printf("debug : %d\n", tileArray[0]->right);
    cr_assert(tileArray[71]->middle == PRE);
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
