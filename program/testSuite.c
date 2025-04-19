#include <criterion/criterion.h>
#include <criterion/new/assert.h> // si bug essayer de retirer le /new

#include "../header/Carcassonne.h"
#include "tile.c"
#include "func_point.c"
#include "player.c"
#include "meeple.c"
#include "game_manager_function.c"
#include "data_structure.c"


// -----------------
// ------Test-------
// ----player.c-----
// -----------------

Test(all, init_player)
{
    struct Player *player = init_player(0,0);
     // git blame O_O
    cr_expect(player->nbMeeple == 8 ,"la fonction init_player n'a pas initialisé nbMeeple\n");
    cr_expect(player->points == 0 ,"la fonction init_player n'a pas initialisé les points a 0\n");
}

Test(all, is_meeple_on_player)
{
    struct Player *player = init_player(0,0);
    cr_expect(is_meeple_on_player(player) > 0, "is_meeple_on_player(player) > 0 est faux\n");
    player->nbMeeple = 0;
    cr_expect(is_meeple_on_player(player) == 0, "is_meeple_on_player(player) == 0 est faux\n");
}

// -----------------
// ------Test-------
// -----tile.c------
// -----------------

Test(all, init_tile)
{
    struct Tile *tile = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    cr_assert(tile->right == VILLE,"tile->right == VILLE est faux\n");
    cr_assert(tile->top == ROUTE,"tile->top == ROUTE est faux\n");
    cr_assert(tile->left == ROUTE,"tile->left == ROUTE est faux\n");
    cr_assert(tile->bot == VILLE,"tile->bot == VILLE est faux\n");
    cr_assert(tile->middle == ROUTE,"tile->middle == ROUTE est faux\n");
    cr_assert(tile->meeplePlace == NO_MEEPLE,"tile->meeple == NO_MEEPLE est faux\n");
}

Test(all, is_meeple_on_tile)
{
    struct Tile *tile = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    cr_assert(tile->meeplePlace == NO_MEEPLE, "tile->meeple == NO_MEEPLE est faux\n");
}

// ------------------
// ------Test--------
// -data_structure.c-
// ------------------

//test token_to_enum_type

Test(all, create_tile_array)
{
    struct Tile **tileArray;
    char *tokenArray[MAX_TOKEN_SIZE + 1] = {"route", "ville", "abbaye", "pre", "village", "blason"};
    tileArray = create_tile_array(CSV_TILE, tokenArray, MAX_TOKEN_SIZE);
    cr_assert(tileArray[0]->right == ROUTE, "tileArray[0]->right == ROUTE est faux\n");
    cr_assert(tileArray[0]->middle == ROUTE, "tileArray[0]->middle == ROUTE est faux\n");
    cr_assert(tileArray[1]->middle == BLASON, "tileArray[1]->middle == BLASON est faux\n");
    cr_assert(tileArray[1]->right == BLASON, "tileArray[1]->right == BLASON est faux\n");
    cr_assert(tileArray[70]->middle == PRE, "tileArray[70]->middle est faux\n");
}

Test(all, init_player_list)
{
    nbPlayers=8;
    struct list_player *list_player = init_player_list(0);

    int i = 0;
    while( list_player->player[i] != NULL )
    {
        i++;
    }
    cr_expect(i == 8, "i == 8 est faux\n");

    for(int j=0;j<i;j++){
        cr_assert(list_player->player[j]->coulPlayer==j);
    }
}

Test(all, shuffle)
{
    struct Tile **tileArray;
    char *tokenArray[MAX_TOKEN_SIZE + 1] = {"route", "ville", "abbaye", "pre", "village", "blason"};
    tileArray = create_tile_array(CSV_TILE, tokenArray, MAX_TOKEN_SIZE);

    shuffle(tileArray);

    for(short i = 0;i<NBTILE-1;i++)
    {
        cr_assert(tileArray[i] != NULL, "Shuffle has broke array");
    }
}

Test(all, array_to_stack)
{
    struct Tile **tileArray;
    char *tokenArray[MAX_TOKEN_SIZE + 1] = {"route", "ville", "abbaye", "pre", "village", "blason"};
    tileArray = create_tile_array(CSV_TILE, tokenArray, MAX_TOKEN_SIZE);

    struct Stack *test=NULL;
    array_to_stack(tileArray,&test);
    
    struct Stack *parcour=test;
    short i = NBTILE-2;

    while(parcour != NULL){
        cr_assert(tileArray[i]->right == parcour->data->right, "tileArray[i]->right == parcour->data->right est faux\n");
        cr_assert(tileArray[i]->bot == parcour->data->bot, "tileArray[i]->bot == parcour->data->bot est faux\n");
        cr_assert(tileArray[i]->left == parcour->data->left, "tileArray[i]->left == parcour->data->left est faux\n");
        cr_assert(tileArray[i]->top == parcour->data->top, "tileArray[i]->top == parcour->data->top est faux\n");
        cr_assert(tileArray[i]->middle == parcour->data->middle, "tileArray[i]->middle == parcour->data->middle est faux\n");
        parcour=parcour->next;
        i--;
    }
}

Test(all, reset_points)
{
    struct Player *player=(struct Player *)malloc(sizeof(struct Player));
    player->points=380;
    reset_points(player);
    cr_assert(player->points == 0, "player->points == 0 est faux\n");
}

Test(all, reset_meeples)
{
    struct Player *player=(struct Player *)malloc(sizeof(struct Player));
    player->nbMeeple=8;
    reset_points(player);
    cr_assert(player->points == 0, "player->points == 0 est faux\n");
}

Test(all, rot_tile)
{
    struct Tile *tile_in = init_tile(VILLE, ROUTE, ABBAYES, VILLAGE, PRE); 
    struct Tile *tile_out = init_tile(VILLE, ROUTE, ABBAYES, VILLAGE, PRE);
    
    rot_tile(tile_out);

    cr_assert(tile_out->right == tile_in->bot, "tile_out->right == tile_in->bot est faux\n");
    cr_assert(tile_out->top == tile_in->right, "tile_out->top == tile_in->right est faux\n");
    cr_assert(tile_out->left == tile_in->top, "tile_out->left == tile_in->top est faux\n");
    cr_assert(tile_out->bot == tile_in->left, "tile_out->bot == tile_in->left est faux\n");
    cr_assert(tile_out->middle == tile_in->middle, "tile_out->middle == tile_in->middle est faux\n");
}

//test player_turn

//test where_i_can_play

//test first_tile

//test place_tile

/* Test(all, enum_to_char)
{
    enum types rien=RIEN;
    enum types route=ROUTE;
    enum types ville=VILLE;
    enum types abbayes=ABBAYES;
    enum types pre=PRE;
    enum types village=VILLAGE;
    enum types blason=BLASON;
    enum types errorv=45;

    cr_assert(enum_to_char(rien) == 'Z' &&
              enum_to_char(route) == 'R' &&
              enum_to_char(ville) == 'V' &&
              enum_to_char(abbayes) == 'A' &&
              enum_to_char(pre) == 'P' &&
              enum_to_char(village) == 'v' &&
              enum_to_char(blason) == 'B' &&
              enum_to_char(errorv) == 'z');
} */

/* Test(all, show_tile)
{    
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Coord *C1=init_coord(0,0);
    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);

    printf("||| TEST SHOW_TILE |||\n");
    show_tile(G->tile);
} */



Test(all ,choose_w_show)
{
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Coord *C1=init_coord(0,0);
    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);

    printf("||| TEST CHOOSE_W_SHOW |||\n");
    choose_w_show(0,G);
    choose_w_show(1,G);
    choose_w_show(2,G);
    printf("\n");
}

Test(all ,show_gridv1)
{
    printf("||| TEST SHOW_GRID V1|||\n");
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, VILLE);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, VILLE, VILLE, VILLE);
    struct Tile *tile3 = init_tile(ROUTE, VILLE, PRE, ROUTE, ROUTE);
    struct Tile *tile4 = init_tile(PRE,VILLE,VILLE,ROUTE,ROUTE);
    struct Coord *C1=init_coord(0,0);
    struct Coord *C2=init_coord(1,0);
    struct Coord *C3=init_coord(1,1);
    struct Coord *C4=init_coord(1,0);
    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,G->right,NULL);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,G,NULL);

    struct Coord **t=(struct Coord**)malloc(sizeof(struct Coord*));
    t[0]=NULL;

    show_grid(G,2,2,t);
}

Test(all ,showgridv2)
{
    printf("||| TEST SHOW_GRID V2|||\n");
    struct Tile *tile1 = init_tile(ROUTE,VILLE,ROUTE,VILLE,ROUTE); struct Tile *tile2 = init_tile(ROUTE,PRE,ROUTE,VILLE,ROUTE);
    struct Tile *tile3 = init_tile(PRE,VILLE,VILLE,PRE,VILLE);     struct Tile *tile4 = init_tile(VILLE,VILLE,VILLE,PRE,VILLE);
    struct Tile *tile5 = init_tile(VILLE,VILLE,VILLE,VILLE,VILLE); struct Tile *tile6 = init_tile(VILLE,PRE,VILLE,PRE,PRE);
    struct Tile *tile7 = init_tile(ROUTE,ROUTE,PRE,VILLE,ROUTE);   struct Tile *tile8 = init_tile(PRE,ROUTE,ROUTE,ROUTE,VILLAGE);
    struct Tile *tile9 = init_tile(VILLE,VILLE,PRE,PRE,VILLE);     struct Tile *tile10= init_tile(PRE,PRE,VILLE,VILLE,VILLE);
    struct Tile *tile11= init_tile(PRE,VILLE,ROUTE,ROUTE,ROUTE);   struct Tile *tile12= init_tile(ROUTE,ROUTE,PRE,ROUTE,ABBAYES);
    struct Tile *tile13= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile14= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);
    struct Tile *tile15= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile16= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile17= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile18= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile19= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile20= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile21= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile22= init_tile(ROUTE,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile23= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile24= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile25= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile26= init_tile(RIEN,RIEN,RIEN,ROUTE,RIEN);
    struct Tile *tile27= init_tile(RIEN,RIEN,RIEN,VILLE,RIEN);      struct Tile *tile28= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);
    struct Tile *tile29= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);      struct Tile *tile30= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Coord *C1=init_coord(0,0);   struct Coord *C2=init_coord(1,0);       struct Coord *C3=init_coord(1,-1);
    struct Coord *C4=init_coord(-1,0);  struct Coord *C5=init_coord(0,1);       struct Coord *C6=init_coord(1,1);
    struct Coord *C7=init_coord(-1,0);  struct Coord *C8=init_coord(-1,1);      struct Coord *C9=init_coord(-1,-1);
    struct Coord *C10=init_coord(2,1);  struct Coord *C11=init_coord(2,0);      struct Coord *C12=init_coord(2,-1);
    struct Coord *C13=init_coord(3,0);  struct Coord *C14=init_coord(3,1);      struct Coord *C15=init_coord(3,-1);
    struct Coord *C16=init_coord(3,-2); struct Coord *C17=init_coord(2,-2);     struct Coord *C18=init_coord(1,-2);
    struct Coord *C19=init_coord(0,-2); struct Coord *C20=init_coord(-1,-2);    struct Coord *C21=init_coord(-2,0);
    struct Coord *C22=init_coord(-2,1); struct Coord *C23=init_coord(-2,-1);    struct Coord *C24=init_coord(-2,-2);
    struct Coord *C25=init_coord(-2,2); struct Coord *C26=init_coord(-1,2);     struct Coord *C27=init_coord(0,2);
    struct Coord *C28=init_coord(1,2);  struct Coord *C29=init_coord(2,2);      struct Coord *C30=init_coord(3,2);

    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,NULL,G->right);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,NULL,G);
    G->top=init_grid(tile5,C5,NULL,NULL,G,NULL);
    G->top->right=init_grid(tile6,C6,NULL,G->top,G->right,NULL);
    G->left=init_grid(tile7,C7,G,NULL,NULL,NULL);
    G->left->top=init_grid(tile8,C8,G->top,NULL,G->left,NULL);
    G->left->bot=init_grid(tile9,C9,G->bot,NULL,NULL,G->left);
    G->right->top->right=init_grid(tile10,C10,NULL,G->right->top,NULL,NULL);
    G->right->right=init_grid(tile11,C11,NULL,G->right,NULL,G->right->top->right);
    G->right->right->bot=init_grid(tile12,C12,NULL,G->right->bot,NULL,G->right->right);
    G->right->right->right=init_grid(tile13,C13,NULL,G->right->right,NULL,NULL);
    G->right->right->right->top=init_grid(tile14,C14,NULL,G->top->right->right,G->right->right,NULL);
    G->right->right->right->bot=init_grid(tile15,C15,NULL,G->bot->right->right,NULL,G->right->right);
    G->right->right->right->bot->bot=init_grid(tile16,C16,NULL,NULL,NULL,G->right->right->right->bot);
    G->right->right->bot->bot=init_grid(tile17,C17,G->right->right->right->bot->bot,NULL,NULL,G->right->right->bot);
    G->right->bot->bot=init_grid(tile18,C18,G->right->right->bot->bot,NULL,NULL,G->right->bot);
    G->bot->bot=init_grid(tile19,C19,G->right->bot->bot,NULL,NULL,G->bot);
    G->left->bot->bot=init_grid(tile20,C20,G->bot->bot,NULL,NULL,G->left->bot);
    G->left->left=init_grid(tile21,C21,G->left,NULL,NULL,NULL);
    G->left->left->top=init_grid(tile22,C22,G->left->top,NULL,G->left->left,NULL);
    G->left->left->bot=init_grid(tile23,C23,G->left->bot,NULL,NULL,G->left->left);
    G->left->left->bot->bot=init_grid(tile24,C24,G->left->bot->bot,NULL,NULL,G->left->left->bot);
    G->left->left->top->top=init_grid(tile25,C25,NULL,NULL,G->left->left->top,NULL);
    G->left->top->top=init_grid(tile26,C26,NULL,G->left->left->top->top,G->left->top,NULL);
    G->top->top=init_grid(tile27,C27,NULL,G->left->top->top,G->top,NULL);
    G->right->top->top=init_grid(tile28,C28,NULL,G->top->top,G->right->top,NULL);
    G->right->right->top->top=init_grid(tile29,C29,NULL,G->right->top->top,G->right->right->top,NULL);
    G->right->right->right->top->top=init_grid(tile30,C30,NULL,G->right->right->top->top,G->right->right->right->top,NULL);

    struct Coord **t=(struct Coord**)malloc(sizeof(struct Coord*));
    t[0]=NULL;

    show_grid(G->left->left->top->top,6,5,t);
}

//test start_game

//test end_game_pointer_counter


// --------------------
// --------Test-------
// --data_structure.c--
// --------------------

Test(all, stack_push_stack_vide)
{
    struct Stack *stack=NULL;
    struct Tile *tile = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile);
    cr_assert(stack->next == NULL, "stack->next == NULL est faux \n");
    cr_assert(stack->data->right == VILLE, "stack->data->right == VILLE est faux \n");
}

Test(all, stack_push_stack_non_vide)
{
    struct Stack *stack=NULL;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile1);

    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile2);

    cr_assert(stack->next != NULL, "stack->next != NULL est faux\n");
    cr_assert(stack->next->next == NULL, "stack->next->next == NULL est faux\n");
    cr_assert(stack->data->right == ROUTE, "stack->data->right == ROUTE est faux\n");
    cr_assert(stack->next->data->right == VILLE, "stack->next->data->right == VILLE est faux\n");
}

Test(all, stack_pop)
{
    struct Stack *stack=NULL;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile1);

    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    stack = stack_push(stack, tile2);
    cr_assert(stack->data->right == ROUTE, "stack->data->right == ROUTE est faux\n");
    cr_assert(stack->next->data->right == VILLE, "stack->next->data->right == VILLE est faux\n");
    // Attention test peut-être pas assez unitaire ?

    struct Tile *tileSlot = malloc(sizeof(struct Tile));
    stack = stack_pop(stack, &tileSlot);

    cr_assert(stack->next == NULL, "stack->next == NULL est faux\n");
    cr_assert(tileSlot->right == ROUTE, "tileSlot->right == ROUTE est faux\n");
    cr_assert(stack->data->right == VILLE, "stack->data->right == VILLE est faux\n");
}

Test(all, is_stack_not_empty)
{
    struct Stack *stack=NULL;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *out;
    stack = stack_push(stack, tile1);

    cr_assert(is_stack_not_empty(stack) == 1, "is_stack_not_empty(stack) == 1 est faux\n");

    stack = stack_pop(stack,&out);
    cr_assert(is_stack_not_empty(stack) == 0, "is_stack_not_empty(stack) == 0 est faux\n");

    struct Stack *stackEmpty = NULL;
    cr_assert(is_stack_not_empty(stackEmpty) == 0, "is_stack_not_empty(stackEmpty) == 0 est faux\n");

}

Test(all, DLList_push_end_one_insert)//Test premiere insertion
{
    struct DLList *db_list=NULL;//initialisation a NULL
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Coord *coord1 = malloc(sizeof(struct Coord));
    coord1->x = 0;
    coord1->y = 0;
    struct Grid *grid1 = init_grid(tile1, coord1, NULL, NULL, NULL, NULL);


    db_list=DLList_push_end(db_list, grid1);

    cr_assert(db_list != NULL, "db_list != NULL est faux\n");
    cr_assert(db_list->data->tile->right == VILLE, "db_list->data->tile->right == VILLE est faux\n");
    cr_assert(db_list->prev==NULL && db_list->next == NULL, "db_list->prev==NULL && db_list->next == NULL est faux\n");
}

Test(all, DLList_push_end_two_insert)//Test Deuxieme insertion
{
    struct DLList *db_list=NULL;//initialisation a NULL
    struct Coord *coord = malloc(sizeof(struct Coord));
    coord->x = 0;
    coord->y = 0;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid1 = init_grid(tile1, coord, NULL, NULL, NULL, NULL);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid2 = init_grid(tile2, coord, NULL, NULL, NULL, NULL);

    db_list = DLList_push_end(db_list,grid1);
    db_list = DLList_push_end(db_list,grid2);

    cr_assert(db_list != NULL, "db_list != NULL est faux\n");
    cr_assert(db_list->prev == NULL, "db_list->prev == NULL est faux\n");
    cr_assert(db_list->next != NULL, "db_list->next != NULL est faux\n");
    cr_assert(db_list->next->next == NULL, "db_list->next->next == NULL est faux\n");
    cr_assert(db_list->next->prev == db_list, "db_list->next->prev == db_list est faux\n");
    cr_assert(db_list->next->data->tile->right == ROUTE, "db_list->next->data->tile->right == ROUTE est faux\n");
    
}

Test(all, DLList_push_end_three_insert)
{
    struct DLList *db_list=NULL;//initialisation a NULL
    struct Coord *coord = malloc(sizeof(struct Coord));
    coord->x = 0;
    coord->y = 0;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid1 = init_grid(tile1, coord, NULL, NULL, NULL, NULL);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid2 = init_grid(tile2, coord, NULL, NULL, NULL, NULL);
    struct Tile *tile3 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, PRE);
    struct Grid *grid3 = init_grid(tile3, coord, NULL, NULL, NULL, NULL);

    db_list = DLList_push_end(db_list,grid1);
    db_list = DLList_push_end(db_list,grid2);
    db_list = DLList_push_end(db_list,grid3);

    cr_assert(db_list != NULL, "db_list != NULL est faux \n");
    cr_assert(db_list->prev == NULL, "db_list->prev == NULL est faux\n");
    cr_assert(db_list->next != NULL, "db_list->next != NULL est faux\n");
    cr_assert(db_list->next->next->next == NULL, "db_list->next->next->next == NULL est faux\n");
    cr_assert(db_list->next->prev == db_list, "db_list->next->prev == db_list est faux \n");
    cr_assert(db_list->next->next != NULL, "db_list->next->next != NULL est faux\n");
    cr_assert(db_list->next->next->prev == db_list->next, "db_list->next->next->prev == db_list->next est faux\n");
    cr_assert(db_list->next->next->data->tile->middle == PRE, "db_list->next->next->data->tile->middle == PRE est faux\n");
}

Test(all, DLList_pop_vide)
{
    struct DLList *db_list=NULL;
    struct Coord *coord = malloc(sizeof(struct Coord));
    coord->x = 0;
    coord->y = 0;
    struct Tile *tile_rien = init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Grid *grid_rien = init_grid(tile_rien, coord, NULL, NULL, NULL, NULL);

    DLList_pop(&db_list,&grid_rien);

    cr_assert(db_list == NULL,"db_list == NULL est faux\n");
    cr_assert(grid_rien->tile->bot == RIEN,"grid_rien->tile->bot == RIEN est faux\n");
}

Test(all, DLList_pop_one_item)
{
    struct DLList *db_list=NULL;
    struct Coord *coord = malloc(sizeof(struct Coord));
    coord->x = 0;
    coord->y = 0;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid1 = init_grid(tile1, coord, NULL, NULL, NULL, NULL);
    struct Grid *out;

    db_list = DLList_push_end(db_list, grid1);
    DLList_pop(&db_list, &out);

    //comme il n'y y a qu'un element db_list devrait
    //être a nul
    //cr_log_info("debug db_list = %d\n", db_list->data->top);
    //cr_assert_fail("debug db_list = %d", db_list->data->top);
    //cr_fatal("debug db_list = %d\n", db_list->data->top);
    cr_assert(db_list == NULL,"db_list == NULL est faux\n");

    //test si la tuile a ete extraite
    cr_assert(out->tile->right == VILLE,"out->right == VILLE est faux\n");

}

Test(all, DLList_pop_two_item_first_item)
{
    struct DLList *db_list=NULL;
    struct Coord *coord = malloc(sizeof(struct Coord));
    coord->x = 0;
    coord->y = 0;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid1 = init_grid(tile1, coord, NULL, NULL, NULL, NULL);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid2 = init_grid(tile2, coord, NULL, NULL, NULL, NULL);
    struct Grid *out;

    db_list = DLList_push_end(db_list,grid1);
    db_list = DLList_push_end(db_list,grid2);
    DLList_pop(&db_list, &out);

    /*
    ici on veut tester le pop du premier element d'une
    liste doublement chaine composé de 2 element
    */

    /*
    on devrais avoir donc qu'un seul element restant
    d'ou ce test si il y a qu'un element il devrait avoir
    db_list->prev et db_list->next egale a NULL
    */
    cr_assert(db_list->prev == NULL, "db_list->prev == NULL est faux\n");
    cr_assert(db_list->next == NULL, "db_list->next == NULL est faux\n");
    
    //test si la tuile a ete extraite
    cr_assert(out->tile->right == VILLE,"out->right != VILLE !");
}

Test(all, DLList_pop_two_item_second_item)
{
    struct DLList *db_list=NULL;
    struct Coord *coord = malloc(sizeof(struct Coord));
    coord->x = 0;
    coord->y = 0;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid1 = init_grid(tile1, coord, NULL, NULL, NULL, NULL);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid2 = init_grid(tile1, coord, NULL, NULL, NULL, NULL);
    struct Grid *out;

    db_list = DLList_push_end(db_list,grid1);
    db_list = DLList_push_end(db_list,grid2);

    /*
    ici on veut tester le pop du deuxieme element d'une
    liste doublement chaine composé de 2 element
    */

   DLList_pop(&(db_list->next), &out);

    //meme test on va voir si il y a bien un seul element

    cr_assert(db_list->prev == NULL, "db_list->prev == NULL est faux\n");
    cr_assert(db_list->next != NULL, "db_list->next != NULL est faux\n");
    cr_assert(out->tile->right == VILLE ,"out->tile->right == VILLE est faux\n");

}

Test(all, DLList_push_three_item_pop_head)
{
    struct DLList *db_list = NULL;
    struct Coord *coord = malloc(sizeof(struct Coord));
    coord->x = 0;
    coord->y = 0;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid1 = init_grid(tile1, coord, NULL, NULL, NULL, NULL);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid2 = init_grid(tile2, coord, NULL, NULL, NULL, NULL);
    struct Tile *tile3 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, PRE);
    struct Grid *grid3 = init_grid(tile3, coord, NULL, NULL, NULL, NULL);
    struct Grid *out;

    db_list = DLList_push_end(db_list, grid1);
    db_list = DLList_push_end(db_list, grid2);
    db_list = DLList_push_end(db_list, grid3);

    DLList_pop(&db_list, &out); // db_list devient db_list->next voir description fonction DLList_pop

    // Vérification de la suppression du premier élément
    cr_assert(db_list != NULL, "db_list != NULL est faux\n");
    cr_assert(db_list->prev == NULL, "db_list->prev == NULL est faux\n");
    // Vérification que `out` contient bien l'ancienne tête
    cr_assert(out->tile->middle == ROUTE, "out->tile->middle == ROUTE est faux\n");

    // Vérification de la continuité de la liste
    cr_assert(db_list->next != NULL, "db_list->next != NULL est faux\n");
}


Test(all, DLList_pop_three_item_second_item)
{
    struct DLList *db_list = NULL;
    struct Coord *coord = malloc(sizeof(struct Coord));
    coord->x = 0;
    coord->y = 0;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid1 = init_grid(tile1, coord, NULL, NULL, NULL, NULL);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid2 = init_grid(tile2, coord, NULL, NULL, NULL, NULL);
    struct Tile *tile3 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, PRE);
    struct Grid *grid3 = init_grid(tile3, coord, NULL, NULL, NULL, NULL);
    struct Grid *out;

    db_list = DLList_push_end(db_list,grid1);
    db_list = DLList_push_end(db_list,grid2);
    db_list = DLList_push_end(db_list,grid3);
    struct DLList *db_list_parcours = db_list->next;
    DLList_pop(&db_list_parcours, &out);

    cr_assert(db_list->prev == NULL, "db_list->prev == NULL est faux\n");
    cr_assert(db_list->next != NULL, "db_list->next != NULL est faux\n");
    cr_assert(db_list->next->prev != NULL, "db_list->next->prev != NULL est faux\n");
    cr_assert(db_list->next->next == NULL, "db_list->next->next == NULL est faux\n");
    
    cr_assert(out->tile->right == ROUTE,"out->right != ROUTE est faux\n");
}

Test(all, DLList_pop_three_item_third_item)
{
    struct DLList *db_list = NULL;
    struct Coord *coord = malloc(sizeof(struct Coord));
    coord->x = 0;
    coord->y = 0;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid1 = init_grid(tile1, coord, NULL, NULL, NULL, NULL);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Grid *grid2 = init_grid(tile2, coord, NULL, NULL, NULL, NULL);
    struct Tile *tile3 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, PRE);
    struct Grid *grid3 = init_grid(tile3, coord, NULL, NULL, NULL, NULL);
    struct Grid *out;

    db_list = DLList_push_end(db_list, grid1);
    db_list = DLList_push_end(db_list, grid2);
    db_list = DLList_push_end(db_list, grid3);
    struct DLList *db_list_parcours = db_list->next->next;
    DLList_pop(&db_list_parcours, &out); // tile1 dans out normalement et db_list = db_list->prev

    cr_assert(db_list_parcours->prev != NULL, "db_list->prev != NULL est faux\n");
    cr_assert(db_list_parcours->next == NULL, "db_list->next == NULL est faux\n");
    cr_assert(out->tile->middle == PRE,"out->tile->middle == PRE est faux\n");
}

Test(all, init_coord)
{
    struct Coord *test=init_coord(14,65);

    cr_assert(test->x==14 && test->y==65);
}

Test(all, init_grid)
{
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, VILLE, VILLE, ROUTE);
    struct Tile *tile3 = init_tile(ROUTE, PRE, PRE, ROUTE, ROUTE);
    struct Tile *tile4 = init_tile(PRE,VILLE,VILLE,ROUTE,ROUTE);
    struct Coord *C1=init_coord(0,0);
    struct Coord *C2=init_coord(1,0);
    struct Coord *C3=init_coord(1,1);
    struct Coord *C4=init_coord(0,1);

    //Test sur un seul morceau de grid
    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    cr_assert(G->coord->y == 0, "G->coord->y == 0 est faux\n");
    cr_assert(G->coord->x == 0, "G->coord->x == 0 est faux\n");
    cr_assert(G->bot == NULL, "G->bot == NULL est faux\n");
    cr_assert(G->left == NULL, "G->left == NULL est faux\n");
    cr_assert(G->top == NULL, "G->top == NULL est faux\n");
    cr_assert(G->right == NULL, "G->right == NULL est faux\n");
    cr_assert(G->tile->right == VILLE, "G->tile->right == VILLE est faux\n");

    //Test sur une deuxieme insertion
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    cr_assert(G->right->coord->x == 1, "G->right->coord->x == 1 est faux\n");
    cr_assert(G->coord->y == 0, "G->coord->y == 0 est faux\n");
    cr_assert(G->right->bot == NULL, "G->right->bot == NULL est faux\n");
    cr_assert(G->right->left == G, "G->right->left == G est faux\n");
    cr_assert(G->right->top == NULL, "G->right->top == NULL est faux\n");
    cr_assert(G->right->right == NULL,"G->right->right == NULL est faux\n");
    cr_assert(G->right->tile->right == ROUTE, "G->right->tile->right == ROUTE est faux\n");
    cr_assert(G->right->tile->left == VILLE, "G->right->tile->left == VILLE est faux\n");
    
    //Test sur une troisieme insertion
    G->right->top=init_grid(tile3,C3,NULL,NULL,G->right,NULL);
    cr_assert(G->right->top->coord->x == 1,"G->right->top->coord->x == 1 est faux\n");
    cr_assert(G->right->top->coord->y == 1,"G->right->top->coord->y == 1 est faux\n");
    cr_assert(G->right->top->right == NULL,"G->right->top->right == NULL est faux\n");
    cr_assert(G->right->top->top == NULL,"G->right->top->top == NULL est faux\n");
    cr_assert(G->right->top->left == NULL,"G->right->top->left == NULL est faux\n");
    cr_assert(G->right->top->bot == G->right,"G->right->top->bot == G->right est faux\n");
    cr_assert(G->right->top->tile->top == PRE,"G->right->top->tile->top == PRE est faux\n");

    //Test sur une quatrieme insertion
    G->top=init_grid(tile4,C4,G->right->top,NULL,G,NULL);
    cr_assert(G->top->coord->x == 0, "G->top->coord->x == 0 est faux\n");
    cr_assert(G->top->coord->y == 1, "G->top->coord->y == 1 est faux\n");
    cr_assert(G->top->top == NULL, "G->top->top == NULL est faux\n");
    cr_assert(G->top->right == G->right->top, "G->top->right == G->right->top est faux\n");
    cr_assert(G->top->bot == G, "G->top->bot == G est faux\n");
    cr_assert(G->left == NULL, "G->left == NULL est faux\n");
    cr_assert(G->top->tile->right == PRE, "G->top->tile->right == PRE est faux\n");
}

Test(all, is_a_potential_tile)
{
    struct Tile *tile = init_tile(VILLE, VILLE, VILLE, VILLE, MIDDLE);
    struct Coord *coord = malloc(sizeof(struct Coord));
    coord->x = -3;
    coord->y = 42;
    struct Grid *grid = init_grid(tile, coord, NULL, NULL, NULL, NULL);
}


Test(find, find_mega_test)
{
    struct Coord *coord00 = malloc(sizeof(struct Coord));
    if (!coord00) 
    {
        perror("erreur malloc\n");
    }
    coord00->x = 0;
    coord00->y = 0;
    struct Coord *coord10 = malloc(sizeof(struct Coord));
    if (!coord10) 
    {
        perror("erreur malloc\n");
    }
    coord10->x = 1;
    coord10->y = 0;
    struct Coord *coord0m1 = malloc(sizeof(struct Coord));
    if (!coord0m1) 
    {
        perror("erreur malloc\n");
    }
    coord0m1->x = 0;
    coord0m1->y  = -1;
    struct Coord *coord1m1 = malloc(sizeof(struct Coord));
    if (!coord1m1) 
    {
        perror("erreur malloc\n");
    }
    coord1m1->x = 1;
    coord1m1->y = -1;
    struct Tile *tile00 = init_tile(VILLE, VILLE, VILLE, VILLE, VILLE);
    struct Grid *grid00 = init_grid(tile00, coord00, NULL, NULL, NULL, NULL);
    struct Tile *tile10 = init_tile(RIEN, RIEN, RIEN, RIEN, RIEN);
    struct Grid *grid10 = init_grid(tile10, coord10, NULL, NULL, NULL, NULL);
    struct Tile *tile0m1 = init_tile(ROUTE, ROUTE, ROUTE, ROUTE, ROUTE);
    struct Grid *grid0m1 = init_grid(tile0m1, coord0m1, NULL, NULL, NULL, NULL);
    struct Tile *tile1m1 = init_tile(PRE, PRE, PRE, PRE, VILLE);
    struct Grid *grid1m1 = init_grid(tile1m1, coord1m1, NULL, NULL, NULL, NULL);

    grid00->right = grid10;
    grid00->top = NULL;
    grid00->left = NULL;
    grid00->bot = grid0m1;

    grid10->right = NULL;
    grid10->top = NULL;
    grid10->left = grid00;
    grid10->bot = grid1m1; 

    grid0m1->right = grid1m1;
    grid0m1->top = grid00;
    grid0m1->left = NULL;
    grid0m1->bot = NULL;    

    grid1m1->right = NULL;
    grid1m1->top = grid10;
    grid1m1->left = grid0m1;
    grid1m1->bot = NULL;
    
    struct Coord coord1;
    coord1.x = -1;
    coord1.y = 0;
    struct Coord coord2;
    coord2.x = -1;
    coord2.y = 0;
    
    struct Grid *gridFind = find(grid00, *coord1m1);
    char isNull1 = (find(grid00, coord1) == NULL);
    char isNull2 = (find(grid00, coord2) == NULL);
    cr_assert(gridFind == grid1m1, "gridFind == grid00 est faux\n");
    cr_assert(isNull1, "NULL == find(grid00, (struct Coord)){-1,0} est faux\n");
    cr_assert(isNull2, "NULL == find(grid00, (struct Coord)){-1,0} est faux\n");
}

Test(all, count_point_city1)
{
    struct Tile *tile1 = init_tile(VILLE, PRE,PRE,PRE,PRE);
    struct Tile *tile2 = init_tile(PRE,PRE,VILLE,PRE,PRE);
    struct Coord *C1=init_coord(0,0);
    struct Coord *C2=init_coord(1,0);
    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);

    

    //char test =count_point_city(G,1,1,1); // git blame O_O
    // cr_assert(test==4);
}

Test(all, count_point_city2)
{
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, VILLE);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, VILLE, VILLE, ROUTE);
    struct Tile *tile3 = init_tile(ROUTE, PRE, PRE, ROUTE, ROUTE);
    struct Tile *tile4 = init_tile(PRE,VILLE,VILLE,ROUTE,ROUTE);
    struct Coord *C1=init_coord(0,0);
    struct Coord *C2=init_coord(1,0);
    struct Coord *C3=init_coord(1,1);
    struct Coord *C4=init_coord(1,0);
    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,G->right,NULL);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,G,NULL);

    // char test=count_point_city(G,4,1,1); // git blame O_O
    // cr_assert(test==6);
}

Test(all, count_point_city3)
{
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, VILLE);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, VILLE, VILLE, VILLE);
    struct Tile *tile3 = init_tile(ROUTE, VILLE, PRE, ROUTE, ROUTE);
    struct Tile *tile4 = init_tile(PRE,VILLE,VILLE,ROUTE,ROUTE);
    struct Coord *C1=init_coord(0,0);
    struct Coord *C2=init_coord(1,0);
    struct Coord *C3=init_coord(1,1);
    struct Coord *C4=init_coord(1,0);
    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,G->right,NULL);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,G,NULL);

    // char test=count_point_city(G,4,1,1); // git blame O_O
    //cr_assert(test==8);
}

// Test(all, nbMeepleVille)
// {
//     char test=0;
//     struct Tile *tile1 = init_tile(VILLE, ROUTE, PRE, ROUTE, ROUTE);
//     struct Tile *tile2 = init_tile(VILLE, PRE, VILLE, PRE, VILLE);
//     struct Tile *tile3 = init_tile(PRE, PRE, VILLE, PRE, PRE);
//     tile1->meeplePlace=0;
//     tile2->meeplePlace=0;
//     tile3->meeplePlace=0;
//     struct Coord *C1=init_coord(0,0);
//     struct Coord *C2=init_coord(1,0);
//     struct Coord *C3=init_coord(2,0);
//     struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
//     G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
//     G->right->right=init_grid(tile3,C2,NULL,G->right,NULL,NULL);
    
//     nbMeepleVille(G->right,1,&test);
//     cr_assert(test==0);

//     G->tile->meeplePlace=1;
//     nbMeepleVille(G->right,2,&test);
//     cr_assert(test==1);
// }

Test(all, upscale)
{
    struct Tile *tile1 = init_tile(VILLE, ROUTE, PRE, ROUTE, ROUTE);
    struct Tile *tile2 = init_tile(VILLE, PRE, VILLE, PRE, VILLE);
    struct Tile *tile3 = init_tile(PRE, PRE, VILLE, PRE, PRE);
    struct Coord *C1=init_coord(0,0);
    struct Coord *C2=init_coord(1,0);
    struct Coord *C3=init_coord(2,0);
    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->right=init_grid(tile3,C2,NULL,G->right,NULL,NULL);


    int l=3,h=1;

    
    struct Coord C4={0,1};
    cr_assert(l==3);
    cr_assert(h==1);
    
    upscale(G,&l,&h,C4);

    cr_assert(l==3);
    cr_assert(h==2);

    cr_assert(G->top != NULL);
    cr_assert(G->top->tile->top==RIEN && 
        G->top->tile->right==RIEN &&
        G->top->tile->bot==RIEN &&
        G->top->tile->left==RIEN && 
        G->top->tile->middle==RIEN);

    cr_assert(G->top->right != NULL);
    cr_assert(G->top->right->tile->top==RIEN && 
        G->top->right->tile->right==RIEN &&
        G->top->right->tile->bot==RIEN &&
        G->top->right->tile->left==RIEN && 
        G->top->right->tile->middle==RIEN);

    cr_assert(G->right->top != NULL);
    cr_assert(G->right->top->tile->top==RIEN && 
        G->right->top->tile->right==RIEN &&
        G->right->top->tile->bot==RIEN &&
        G->right->top->tile->left==RIEN && 
        G->right->top->tile->middle==RIEN);

    cr_assert(G->top->right->right != NULL);
    cr_assert(G->top->right->right->tile->top==RIEN && 
        G->top->right->right->tile->right==RIEN &&
        G->top->right->right->tile->bot==RIEN &&
        G->top->right->right->tile->left==RIEN && 
        G->top->right->right->tile->middle==RIEN);

    cr_assert(G->right->right->top != NULL);
    cr_assert(G->right->right->top->tile->top==RIEN && 
            G->right->right->top->tile->right==RIEN &&
            G->right->right->top->tile->bot==RIEN &&
            G->right->right->top->tile->left==RIEN && 
            G->right->right->top->tile->middle==RIEN);

    
    struct Coord C6={-1,0};
        struct Coord C5={3,0};
struct Coord C7={0,-1};


    //upscale(G,&l,&h,C5); //upscale vers la droite crash
    //upscale(G,&l,&h,C6); //upscale vers la gauche crash
    //upscale(G,&l,&h,C7); //upscale vers le bas crash
}

Test(all ,turn_tile)
{
    struct Tile *tile1 = init_tile(VILLE, ROUTE, PRE, ROUTE, ROUTE);
    struct Tile *rot=turn_tile(tile1);

    cr_assert(rot->middle==tile1->middle);
    cr_assert(rot->top==tile1->right);
    cr_assert(rot->left==tile1->top);
    cr_assert(rot->bot==tile1->left);
    cr_assert(rot->right==tile1->bot);
}

Test(all ,isFinishedAbbaye)
{
    char test=100; //initialisation a une valeur impossible a atteindre
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ABBAYES);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, VILLE, VILLE, VILLE);
    struct Tile *tile3 = init_tile(ROUTE, VILLE, PRE, ROUTE, ROUTE);
    struct Tile *tile4 = init_tile(PRE,VILLE,VILLE,ROUTE,ROUTE);
    struct Tile *tile5 = init_tile(PRE,PRE,PRE,PRE,PRE);
    struct Tile *tile6 = init_tile(PRE,PRE,PRE,PRE,PRE);
    struct Tile *tile7=init_tile(PRE,PRE,PRE,PRE,PRE);
    struct Tile *tile8=init_tile(PRE,PRE,PRE,PRE,PRE);
    struct Tile *tile9=init_tile(PRE,PRE,PRE,PRE,PRE);
    struct Coord *C1=init_coord(0,0);
    struct Coord *C2=init_coord(1,0);
    struct Coord *C3=init_coord(1,1);
    struct Coord *C4=init_coord(1,0);
    struct Coord *C5=init_coord(0,-1);
    struct Coord *C6=init_coord(1,-1);
    struct Coord *C7=init_coord(-1,0);
    struct Coord *C8=init_coord(-1,-1);
    struct Coord *C9=init_coord(-1,1);
    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,NULL,G->right);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,NULL,G);
    G->top=init_grid(tile5,C5,NULL,NULL,G,NULL);
    G->top->right=init_grid(tile6,C6,NULL,G->top,G->right,NULL);
    G->left=init_grid(tile7,C7,G,NULL,NULL,NULL);
    G->left->top=init_grid(tile8,C8,G->top,NULL,G->left,NULL);
    G->left->bot=init_grid(tile9,C9,G->bot,NULL,NULL,G->left);

    test=isFinishedAbbaye(G);
    cr_assert(test==8,"l'abbaye est censé etre complete");

    G->right->tile->middle=RIEN;
    test=isFinishedAbbaye(G);
    cr_assert(test==0,"l'abbaye est censé n'etre pas complete");

    finJeu=1;
    test=isFinishedAbbaye(G);
    cr_assert(test==7,"l'abbaye est censé avoir 7 case autour");
    finJeu=0;

    G->right->tile->middle=VILLE;//juste pour ne plus que sa soit considerer comme une tuile potentiel
    G->tile->middle=VILLE;
    test=isFinishedAbbaye(G);
    cr_assert(test==0,"il n'y est pas censé avoir d'abbaye");

    G->tile->left=ABBAYES;
    test=isFinishedAbbaye(G);
    cr_assert(test==8,"l'abbaye est censé etre complete");
}

Test(all ,count_point_city)
{
    finJeu=0;
    struct Tile *tile1 = init_tile(ROUTE,VILLE,ROUTE,VILLE,ROUTE); struct Tile *tile2 = init_tile(ROUTE,PRE,ROUTE,VILLE,ROUTE);
    struct Tile *tile3 = init_tile(PRE,VILLE,VILLE,PRE,VILLE);     struct Tile *tile4 = init_tile(VILLE,VILLE,VILLE,PRE,VILLE);
    struct Tile *tile5 = init_tile(VILLE,VILLE,VILLE,VILLE,VILLE); struct Tile *tile6 = init_tile(VILLE,PRE,VILLE,PRE,PRE);
    struct Tile *tile7 = init_tile(ROUTE,ROUTE,PRE,VILLE,ROUTE);   struct Tile *tile8 = init_tile(PRE,ROUTE,ROUTE,ROUTE,VILLAGE);
    struct Tile *tile9 = init_tile(VILLE,VILLE,PRE,PRE,VILLE);     struct Tile *tile10= init_tile(PRE,PRE,VILLE,VILLE,VILLE);
    struct Tile *tile11= init_tile(PRE,VILLE,ROUTE,ROUTE,ROUTE);   struct Tile *tile12= init_tile(ROUTE,ROUTE,PRE,ROUTE,ABBAYES);
    struct Tile *tile13= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile14= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);
    struct Tile *tile15= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile16= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile17= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile18= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile19= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile20= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile21= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile22= init_tile(ROUTE,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile23= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile24= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile25= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile26= init_tile(RIEN,RIEN,RIEN,ROUTE,RIEN);
    struct Tile *tile27= init_tile(RIEN,RIEN,RIEN,VILLE,RIEN);      struct Tile *tile28= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);
    struct Tile *tile29= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);      struct Tile *tile30= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Coord *C1=init_coord(0,0);   struct Coord *C2=init_coord(1,0);       struct Coord *C3=init_coord(1,-1);
    struct Coord *C4=init_coord(-1,0);  struct Coord *C5=init_coord(0,1);       struct Coord *C6=init_coord(1,1);
    struct Coord *C7=init_coord(-1,0);  struct Coord *C8=init_coord(-1,1);      struct Coord *C9=init_coord(-1,-1);
    struct Coord *C10=init_coord(2,1);  struct Coord *C11=init_coord(2,0);      struct Coord *C12=init_coord(2,-1);
    struct Coord *C13=init_coord(3,0);  struct Coord *C14=init_coord(3,1);      struct Coord *C15=init_coord(3,-1);
    struct Coord *C16=init_coord(3,-2); struct Coord *C17=init_coord(2,-2);     struct Coord *C18=init_coord(1,-2);
    struct Coord *C19=init_coord(0,-2); struct Coord *C20=init_coord(-1,-2);    struct Coord *C21=init_coord(-2,0);
    struct Coord *C22=init_coord(-2,1); struct Coord *C23=init_coord(-2,-1);    struct Coord *C24=init_coord(-2,-2);
    struct Coord *C25=init_coord(-2,2); struct Coord *C26=init_coord(-1,2);     struct Coord *C27=init_coord(0,2);
    struct Coord *C28=init_coord(1,2);  struct Coord *C29=init_coord(2,2);      struct Coord *C30=init_coord(3,2);

    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,NULL,G->right);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,NULL,G);
    G->top=init_grid(tile5,C5,NULL,NULL,G,NULL);
    G->top->right=init_grid(tile6,C6,NULL,G->top,G->right,NULL);
    G->left=init_grid(tile7,C7,G,NULL,NULL,NULL);
    G->left->top=init_grid(tile8,C8,G->top,NULL,G->left,NULL);
    G->left->bot=init_grid(tile9,C9,G->bot,NULL,NULL,G->left);
    G->right->top->right=init_grid(tile10,C10,NULL,G->right->top,NULL,NULL);
    G->right->right=init_grid(tile11,C11,NULL,G->right,NULL,G->right->top->right);
    G->right->right->bot=init_grid(tile12,C12,NULL,G->right->bot,NULL,G->right->right);
    G->right->right->right=init_grid(tile13,C13,NULL,G->right->right,NULL,NULL);
    G->right->right->right->top=init_grid(tile14,C14,NULL,G->top->right->right,G->right->right,NULL);
    G->right->right->right->bot=init_grid(tile15,C15,NULL,G->bot->right->right,NULL,G->right->right);
    G->right->right->right->bot->bot=init_grid(tile16,C16,NULL,NULL,NULL,G->right->right->right->bot);
    G->right->right->bot->bot=init_grid(tile17,C17,G->right->right->right->bot->bot,NULL,NULL,G->right->right->bot);
    G->right->bot->bot=init_grid(tile18,C18,G->right->right->bot->bot,NULL,NULL,G->right->bot);
    G->bot->bot=init_grid(tile19,C19,G->right->bot->bot,NULL,NULL,G->bot);
    G->left->bot->bot=init_grid(tile20,C20,G->bot->bot,NULL,NULL,G->left->bot);
    G->left->left=init_grid(tile21,C21,G->left,NULL,NULL,NULL);
    G->left->left->top=init_grid(tile22,C22,G->left->top,NULL,G->left->left,NULL);
    G->left->left->bot=init_grid(tile23,C23,G->left->bot,NULL,NULL,G->left->left);
    G->left->left->bot->bot=init_grid(tile24,C24,G->left->bot->bot,NULL,NULL,G->left->left->bot);
    G->left->left->top->top=init_grid(tile25,C25,NULL,NULL,G->left->left->top,NULL);
    G->left->top->top=init_grid(tile26,C26,NULL,G->left->left->top->top,G->left->top,NULL);
    G->top->top=init_grid(tile27,C27,NULL,G->left->top->top,G->top,NULL);
    G->right->top->top=init_grid(tile28,C28,NULL,G->top->top,G->right->top,NULL);
    G->right->right->top->top=init_grid(tile29,C29,NULL,G->right->top->top,G->right->right->top,NULL);
    G->right->right->right->top->top=init_grid(tile30,C30,NULL,G->right->right->top->top,G->right->right->right->top,NULL);

    /*
        pour ce test je prend la meme grille que l'affichage
    */

    /*
          [P,V,P]
        G=[R,R,R]
          [P,V,P]
    */

    char test1=count_point_city(G,TOP); 
    //printf("count_point_city -> test1 = %d\n",test1);

    char test2=count_point_city(G->top,MIDDLE);
    //printf("count_point_city -> test2 = %d\n",test2);

    char test12=count_point_city(G->top->right,LEFT);
    //printf("count_point_city -> test12 = %d\n",test12);

    char test3=count_point_city(G->top->right,RIGHT);
    //printf("count_point_city -> test3 = %d\n",test3);
    /*
        G->top->right=[P,P,P]
                      [V,R,V]
                      [P,R,P]
    */

    char test4=count_point_city(G->top->right->right,MIDDLE);
    //printf("count_point_city -> test4 = %d\n",test4);
    /*
        G->top->right->right=[P,P,P]
                             [V,V,P]
                             [V,V,P]
    */

    char test5=count_point_city(G->right->right,TOP);
    //printf("count_point_city -> test5 = %d\n",test5);
    /*
        G->right->right=[P,V,P]
                        [P,P,P]
                        [P,P,P]
    */

    char test6=count_point_city(G->bot,MIDDLE);
    printf("count_point_city -> test6 = %d\n",test6);
    /*
        G->bot=[V,V,V]
               [V,V,V]
               [P,P,P]
    */

    char test7=count_point_city(G->bot,LEFT);
    //printf("count_point_city -> test7 = %d\n",test7);
    /*
        G->bot=[V,V,V]
               [V,V,V]
               [P,P,P]
    */

    char test8=count_point_city(G->bot,RIGHT);
    //printf("count_point_city -> test8 = %d\n",test8);
    /*
        G->bot=[V,V,V]
               [V,V,V]
               [P,P,P]
    */
   
    finJeu=1;
    char test9=count_point_city(G,TOP);
    //printf("count_point_city -> test9 = %d\n",test9);

    char test10=count_point_city(G->top,MIDDLE);
    //printf("count_point_city -> test10 = %d\n",test10);

    char test11=count_point_city(G->top->right,LEFT);
    //printf("count_point_city -> test11 = %d\n",test11);

    cr_assert(test1==0,"count_point_city : test1 faux");
    cr_assert(test2==0,"count_point_city : test2 faux");
    cr_assert(test3==6,"count_point_city : test3 faux");
    cr_assert(test4==6,"count_point_city : test4 faux");
    cr_assert(test5==6,"count_point_city : test5 faux");
    cr_assert(test6==12,"count_point_city : test6 faux");
    cr_assert(test7==12,"count_point_city : test7 faux");
    cr_assert(test8==12,"count_point_city : test8 faux");
    cr_assert(test9==3,"count_point_city : test9 faux");
    cr_assert(test10==3,"count_point_city : test10 faux");
    cr_assert(test11==3,"count_point_city : test11 faux");
    cr_assert(test12==0,"count_point_city : test12 faux");
    finJeu=0;
}

Test(all,countPointRoad)
{
    struct Tile *tile1 = init_tile(ROUTE,VILLE,ROUTE,VILLE,ROUTE); struct Tile *tile2 = init_tile(ROUTE,PRE,ROUTE,VILLE,ROUTE);
    struct Tile *tile3 = init_tile(PRE,VILLE,VILLE,PRE,VILLE);     struct Tile *tile4 = init_tile(VILLE,VILLE,VILLE,PRE,VILLE);
    struct Tile *tile5 = init_tile(VILLE,VILLE,VILLE,VILLE,VILLE); struct Tile *tile6 = init_tile(VILLE,PRE,VILLE,PRE,PRE);
    struct Tile *tile7 = init_tile(ROUTE,ROUTE,PRE,VILLE,ROUTE);   struct Tile *tile8 = init_tile(PRE,ROUTE,ROUTE,ROUTE,VILLAGE);
    struct Tile *tile9 = init_tile(VILLE,VILLE,PRE,PRE,VILLE);     struct Tile *tile10= init_tile(PRE,PRE,VILLE,VILLE,VILLE);
    struct Tile *tile11= init_tile(PRE,VILLE,ROUTE,ROUTE,ROUTE);   struct Tile *tile12= init_tile(ROUTE,ROUTE,PRE,ROUTE,ABBAYES);
    struct Tile *tile13= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile14= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);
    struct Tile *tile15= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile16= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile17= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile18= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile19= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile20= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile21= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile22= init_tile(ROUTE,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile23= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile24= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile25= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile26= init_tile(RIEN,RIEN,RIEN,ROUTE,RIEN);
    struct Tile *tile27= init_tile(RIEN,RIEN,RIEN,VILLE,RIEN);      struct Tile *tile28= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);
    struct Tile *tile29= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);      struct Tile *tile30= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Coord *C1=init_coord(0,0);   struct Coord *C2=init_coord(1,0);       struct Coord *C3=init_coord(1,-1);
    struct Coord *C4=init_coord(-1,0);  struct Coord *C5=init_coord(0,1);       struct Coord *C6=init_coord(1,1);
    struct Coord *C7=init_coord(-1,0);  struct Coord *C8=init_coord(-1,1);      struct Coord *C9=init_coord(-1,-1);
    struct Coord *C10=init_coord(2,1);  struct Coord *C11=init_coord(2,0);      struct Coord *C12=init_coord(2,-1);
    struct Coord *C13=init_coord(3,0);  struct Coord *C14=init_coord(3,1);      struct Coord *C15=init_coord(3,-1);
    struct Coord *C16=init_coord(3,-2); struct Coord *C17=init_coord(2,-2);     struct Coord *C18=init_coord(1,-2);
    struct Coord *C19=init_coord(0,-2); struct Coord *C20=init_coord(-1,-2);    struct Coord *C21=init_coord(-2,0);
    struct Coord *C22=init_coord(-2,1); struct Coord *C23=init_coord(-2,-1);    struct Coord *C24=init_coord(-2,-2);
    struct Coord *C25=init_coord(-2,2); struct Coord *C26=init_coord(-1,2);     struct Coord *C27=init_coord(0,2);
    struct Coord *C28=init_coord(1,2);  struct Coord *C29=init_coord(2,2);      struct Coord *C30=init_coord(3,2);

    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,NULL,G->right);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,NULL,G);
    G->top=init_grid(tile5,C5,NULL,NULL,G,NULL);
    G->top->right=init_grid(tile6,C6,NULL,G->top,G->right,NULL);
    G->left=init_grid(tile7,C7,G,NULL,NULL,NULL);
    G->left->top=init_grid(tile8,C8,G->top,NULL,G->left,NULL);
    G->left->bot=init_grid(tile9,C9,G->bot,NULL,NULL,G->left);
    G->right->top->right=init_grid(tile10,C10,NULL,G->right->top,NULL,NULL);
    G->right->right=init_grid(tile11,C11,NULL,G->right,NULL,G->right->top->right);
    G->right->right->bot=init_grid(tile12,C12,NULL,G->right->bot,NULL,G->right->right);
    G->right->right->right=init_grid(tile13,C13,NULL,G->right->right,NULL,NULL);
    G->right->right->right->top=init_grid(tile14,C14,NULL,G->top->right->right,G->right->right,NULL);
    G->right->right->right->bot=init_grid(tile15,C15,NULL,G->bot->right->right,NULL,G->right->right);
    G->right->right->right->bot->bot=init_grid(tile16,C16,NULL,NULL,NULL,G->right->right->right->bot);
    G->right->right->bot->bot=init_grid(tile17,C17,G->right->right->right->bot->bot,NULL,NULL,G->right->right->bot);
    G->right->bot->bot=init_grid(tile18,C18,G->right->right->bot->bot,NULL,NULL,G->right->bot);
    G->bot->bot=init_grid(tile19,C19,G->right->bot->bot,NULL,NULL,G->bot);
    G->left->bot->bot=init_grid(tile20,C20,G->bot->bot,NULL,NULL,G->left->bot);
    G->left->left=init_grid(tile21,C21,G->left,NULL,NULL,NULL);
    G->left->left->top=init_grid(tile22,C22,G->left->top,NULL,G->left->left,NULL);
    G->left->left->bot=init_grid(tile23,C23,G->left->bot,NULL,NULL,G->left->left);
    G->left->left->bot->bot=init_grid(tile24,C24,G->left->bot->bot,NULL,NULL,G->left->left->bot);
    G->left->left->top->top=init_grid(tile25,C25,NULL,NULL,G->left->left->top,NULL);
    G->left->top->top=init_grid(tile26,C26,NULL,G->left->left->top->top,G->left->top,NULL);
    G->top->top=init_grid(tile27,C27,NULL,G->left->top->top,G->top,NULL);
    G->right->top->top=init_grid(tile28,C28,NULL,G->top->top,G->right->top,NULL);
    G->right->right->top->top=init_grid(tile29,C29,NULL,G->right->top->top,G->right->right->top,NULL);
    G->right->right->right->top->top=init_grid(tile30,C30,NULL,G->right->right->top->top,G->right->right->right->top,NULL);


    /*
        pour ce test je prend la meme grille que l'affichage
    */

    /*
          [P,V,P]
        G=[R,R,R]
          [P,V,P]
    */

    
    finJeu=0;
    char unfinished;

    unfinished=1;
    char test1=countPointRoad(G,&unfinished,RIGHT);
    //printf("countPointRoad -> test1 = %d\n",test1);
    
    unfinished=1;
    char test2=countPointRoad(G,&unfinished,LEFT);
    //printf("countPointRoad -> test2 = %d\n",test2);

    unfinished=1;
    char test3=countPointRoad(G->right,&unfinished,RIGHT);
    //printf("countPointRoad -> test3 = %d\n",test3);
    
    unfinished=1;
    char test4=countPointRoad(G->right,&unfinished,LEFT);
    //printf("countPointRoad -> test4 = %d\n",test4);

    unfinished=1;
    char test5=countPointRoad(G->left->top,&unfinished,BOT);
    //printf("countPointRoad -> test5 = %d\n",test5);

    unfinished=1;
    char test6=countPointRoad(G->left->top,&unfinished,LEFT);
    //printf("countPointRoad -> test6 = %d\n",test6);

    unfinished=1;
    char test7=countPointRoad(G->left->top,&unfinished,TOP);
    //printf("countPointRoad -> test7 = %d\n",test7);

    unfinished=1;
    finJeu=1;
    char test8=countPointRoad(G->left->top,&unfinished,LEFT);
    //printf("countPointRoad -> test8 = %d\n",test8);

    unfinished=1;
    finJeu=1;
    char test9=countPointRoad(G->left->top,&unfinished,TOP);
    //printf("countPointRoad -> test9 = %d\n",test9);

    unfinished=1;
    finJeu=1;
    char test10=countPointRoad(G->right->right->bot,&unfinished,RIGHT);
    //printf("countPointRoad -> test10 = %d\n",test10);
    unfinished=1;
    finJeu=1;
    char test11=countPointRoad(G->right->right->bot,&unfinished,BOT);
    //printf("countPointRoad -> test11 = %d\n",test11);


    finJeu=0;
    cr_assert(test1==6,"countPointRoad : test1 faux");
    cr_assert(test2==6,"countPointRoad : test2 faux");
    cr_assert(test3==6,"countPointRoad : test3 faux");
    cr_assert(test4==6,"countPointRoad : test4 faux");
    cr_assert(test5==6,"countPointRoad : test5 faux");
    cr_assert(test6==0,"countPointRoad : test6 faux");
    cr_assert(test7==0,"countPointRoad : test7 faux");
    cr_assert(test8==1,"countPointRoad : test8 faux");
    cr_assert(test9==1,"countPointRoad : test9 faux");
    cr_assert(test10==1,"countPointRoad : test10 faux");
    cr_assert(test11==1,"countPointRoad : test11 faux");
}

Test(all ,searchAbbaye)
{
    struct Tile *tile1 = init_tile(ROUTE,VILLE,ROUTE,VILLE,ROUTE); struct Tile *tile2 = init_tile(ROUTE,PRE,ROUTE,VILLE,ROUTE);
    struct Tile *tile3 = init_tile(PRE,VILLE,VILLE,PRE,VILLE);     struct Tile *tile4 = init_tile(VILLE,VILLE,VILLE,PRE,VILLE);
    struct Tile *tile5 = init_tile(VILLE,VILLE,VILLE,VILLE,VILLE); struct Tile *tile6 = init_tile(VILLE,PRE,VILLE,PRE,PRE);
    struct Tile *tile7 = init_tile(ROUTE,ROUTE,PRE,VILLE,ROUTE);   struct Tile *tile8 = init_tile(PRE,ROUTE,ROUTE,ROUTE,VILLAGE);
    struct Tile *tile9 = init_tile(VILLE,VILLE,PRE,PRE,VILLE);     struct Tile *tile10= init_tile(PRE,PRE,VILLE,VILLE,VILLE);
    struct Tile *tile11= init_tile(PRE,VILLE,ROUTE,ROUTE,ROUTE);   struct Tile *tile12= init_tile(ROUTE,ROUTE,PRE,ROUTE,ABBAYES);
    struct Tile *tile13= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile14= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);
    struct Tile *tile15= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile16= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile17= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile18= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile19= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile20= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile21= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile22= init_tile(ROUTE,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile23= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile24= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile25= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile26= init_tile(RIEN,RIEN,RIEN,ROUTE,RIEN);
    struct Tile *tile27= init_tile(RIEN,RIEN,RIEN,VILLE,RIEN);      struct Tile *tile28= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);
    struct Tile *tile29= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);      struct Tile *tile30= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Coord *C1=init_coord(0,0);   struct Coord *C2=init_coord(1,0);       struct Coord *C3=init_coord(1,-1);
    struct Coord *C4=init_coord(-1,0);  struct Coord *C5=init_coord(0,1);       struct Coord *C6=init_coord(1,1);
    struct Coord *C7=init_coord(-1,0);  struct Coord *C8=init_coord(-1,1);      struct Coord *C9=init_coord(-1,-1);
    struct Coord *C10=init_coord(2,1);  struct Coord *C11=init_coord(2,0);      struct Coord *C12=init_coord(2,-1);
    struct Coord *C13=init_coord(3,0);  struct Coord *C14=init_coord(3,1);      struct Coord *C15=init_coord(3,-1);
    struct Coord *C16=init_coord(3,-2); struct Coord *C17=init_coord(2,-2);     struct Coord *C18=init_coord(1,-2);
    struct Coord *C19=init_coord(0,-2); struct Coord *C20=init_coord(-1,-2);    struct Coord *C21=init_coord(-2,0);
    struct Coord *C22=init_coord(-2,1); struct Coord *C23=init_coord(-2,-1);    struct Coord *C24=init_coord(-2,-2);
    struct Coord *C25=init_coord(-2,2); struct Coord *C26=init_coord(-1,2);     struct Coord *C27=init_coord(0,2);
    struct Coord *C28=init_coord(1,2);  struct Coord *C29=init_coord(2,2);      struct Coord *C30=init_coord(3,2);

    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,NULL,G->right);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,NULL,G);
    G->top=init_grid(tile5,C5,NULL,NULL,G,NULL);
    G->top->right=init_grid(tile6,C6,NULL,G->top,G->right,NULL);
    G->left=init_grid(tile7,C7,G,NULL,NULL,NULL);
    G->left->top=init_grid(tile8,C8,G->top,NULL,G->left,NULL);
    G->left->bot=init_grid(tile9,C9,G->bot,NULL,NULL,G->left);
    G->right->top->right=init_grid(tile10,C10,NULL,G->right->top,NULL,NULL);
    G->right->right=init_grid(tile11,C11,NULL,G->right,NULL,G->right->top->right);
    G->right->right->bot=init_grid(tile12,C12,NULL,G->right->bot,NULL,G->right->right);
    G->right->right->right=init_grid(tile13,C13,NULL,G->right->right,NULL,NULL);
    G->right->right->right->top=init_grid(tile14,C14,NULL,G->top->right->right,G->right->right,NULL);
    G->right->right->right->bot=init_grid(tile15,C15,NULL,G->bot->right->right,NULL,G->right->right);
    G->right->right->right->bot->bot=init_grid(tile16,C16,NULL,NULL,NULL,G->right->right->right->bot);
    G->right->right->bot->bot=init_grid(tile17,C17,G->right->right->right->bot->bot,NULL,NULL,G->right->right->bot);
    G->right->bot->bot=init_grid(tile18,C18,G->right->right->bot->bot,NULL,NULL,G->right->bot);
    G->bot->bot=init_grid(tile19,C19,G->right->bot->bot,NULL,NULL,G->bot);
    G->left->bot->bot=init_grid(tile20,C20,G->bot->bot,NULL,NULL,G->left->bot);
    G->left->left=init_grid(tile21,C21,G->left,NULL,NULL,NULL);
    G->left->left->top=init_grid(tile22,C22,G->left->top,NULL,G->left->left,NULL);
    G->left->left->bot=init_grid(tile23,C23,G->left->bot,NULL,NULL,G->left->left);
    G->left->left->bot->bot=init_grid(tile24,C24,G->left->bot->bot,NULL,NULL,G->left->left->bot);
    G->left->left->top->top=init_grid(tile25,C25,NULL,NULL,G->left->left->top,NULL);
    G->left->top->top=init_grid(tile26,C26,NULL,G->left->left->top->top,G->left->top,NULL);
    G->top->top=init_grid(tile27,C27,NULL,G->left->top->top,G->top,NULL);
    G->right->top->top=init_grid(tile28,C28,NULL,G->top->top,G->right->top,NULL);
    G->right->right->top->top=init_grid(tile29,C29,NULL,G->right->top->top,G->right->right->top,NULL);
    G->right->right->right->top->top=init_grid(tile30,C30,NULL,G->right->right->top->top,G->right->right->right->top,NULL);

    struct Grid *test1=searchAbbaye(G->right->right->bot);
    struct Grid *test2=searchAbbaye(G->right->right);
    struct Grid *test3=searchAbbaye(G->right);
    struct Grid *test4=searchAbbaye(G->right->bot);

    cr_assert(test1->tile->middle==ABBAYES,"la fonction n'a pas trouver l'abbaye");
    cr_assert(test2->tile->middle==ABBAYES,"la fonction n'a pas trouver l'abbaye");
    cr_assert(test3->tile->middle==ABBAYES,"la fonction n'a pas trouver l'abbaye");
    cr_assert(test4->tile->middle==ABBAYES,"la fonction n'a pas trouver l'abbaye");
}

Test(all ,where_is_meeple)
{
    struct Tile *tile1 = init_tile(ROUTE,VILLE,ROUTE,VILLE,ROUTE);
    tile1->meeplePlace=MP_TOP;
    struct Player *player1 = init_player(0,0);
    tile1->meeple=player1;

    struct Tile *tile2 = init_tile(ROUTE,VILLE,ROUTE,VILLE,ROUTE);
    tile2->meeplePlace=MP_RIGHT;
    struct Player *player2 = init_player(0,0);
    tile2->meeple=player2;

    struct Tile *tile3= init_tile(ROUTE,ROUTE,PRE,ROUTE,ABBAYES);
    tile3->meeplePlace=MP_RIGHT;
    struct Player *player3 = init_player(0,0);
    tile3->meeple=player3;

    struct Tile *tile4= init_tile(ROUTE,ROUTE,PRE,ROUTE,ABBAYES);
    tile4->meeplePlace=MP_LEFT;
    struct Player *player4 = init_player(0,0);
    tile4->meeple=player4;

    struct Tile *tile5= init_tile(ROUTE,ROUTE,PRE,ROUTE,ABBAYES);
    tile5->meeplePlace=MP_MIDDLE;
    struct Player *player5 = init_player(0,0);
    tile5->meeple=player5;

    char test1=where_is_meeple(VILLE,*tile1);
    char test2=where_is_meeple(ROUTE,*tile1);
    char test3=where_is_meeple(VILLE,*tile2);
    char test4=where_is_meeple(ROUTE,*tile2);
    char test5=where_is_meeple(ROUTE,*tile3);
    char test6=where_is_meeple(ABBAYES,*tile3);
    char test7=where_is_meeple(VILLAGE,*tile4);
    char test8=where_is_meeple(PRE,*tile4);
    char test9=where_is_meeple(ROUTE,*tile5);
    char test10=where_is_meeple(ABBAYES,*tile5);


    cr_assert(test1==1);
    cr_assert(test2==0);
    cr_assert(test3==0);
    cr_assert(test4==1);
    cr_assert(test5==1);
    cr_assert(test6==0);
    cr_assert(test7==0);
    cr_assert(test8==1);
    cr_assert(test9==0);
    cr_assert(test10==1);
}

Test(all ,what_color_is_meeple)
{
    struct Tile *tile1 = init_tile(ROUTE,VILLE,ROUTE,VILLE,ROUTE);
    struct Player *player[10];
    char test[100];

    for(int i=0;i<10;i++){
        player[i]=init_player(i,0);
    }

    for(int i=0;i<100;i++){
        tile1->meeple=player[i%10];
        test[i]=what_color_is_meeple(i/10,*tile1);
    }

    for(int i=0;i<100;i++){
        if(i%11==0){
            cr_assert(test[i]==1);
        }
        else{
            cr_assert(test[i]==0);
        }
    }
}

Test(all ,nbMeepleVille)
{
    
    nbPlayers=8;
    struct list_player *list_player=init_player_list(0);
    struct Tile *tile1 = init_tile(ROUTE,VILLE,ROUTE,VILLE,ROUTE); struct Tile *tile2 = init_tile(ROUTE,PRE,ROUTE,VILLE,ROUTE);
    struct Tile *tile3 = init_tile(PRE,VILLE,VILLE,PRE,VILLE);     struct Tile *tile4 = init_tile(VILLE,VILLE,VILLE,PRE,VILLE);
    struct Tile *tile5 = init_tile(VILLE,VILLE,VILLE,VILLE,VILLE); struct Tile *tile6 = init_tile(VILLE,PRE,VILLE,PRE,PRE);
    struct Tile *tile7 = init_tile(ROUTE,ROUTE,PRE,VILLE,ROUTE);   struct Tile *tile8 = init_tile(PRE,ROUTE,ROUTE,ROUTE,VILLAGE);
    struct Tile *tile9 = init_tile(VILLE,VILLE,PRE,PRE,VILLE);     struct Tile *tile10= init_tile(PRE,PRE,VILLE,VILLE,VILLE);
    struct Tile *tile11= init_tile(PRE,VILLE,ROUTE,ROUTE,ROUTE);   struct Tile *tile12= init_tile(ROUTE,ROUTE,PRE,ROUTE,ABBAYES);
    struct Tile *tile13= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile14= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);
    struct Tile *tile15= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile16= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile17= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile18= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile19= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile20= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile21= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile22= init_tile(ROUTE,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile23= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile24= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile25= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile26= init_tile(RIEN,RIEN,RIEN,ROUTE,RIEN);
    struct Tile *tile27= init_tile(RIEN,RIEN,RIEN,VILLE,RIEN);      struct Tile *tile28= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);
    struct Tile *tile29= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);      struct Tile *tile30= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Coord *C1=init_coord(0,0);   struct Coord *C2=init_coord(1,0);       struct Coord *C3=init_coord(1,-1);
    struct Coord *C4=init_coord(-1,0);  struct Coord *C5=init_coord(0,1);       struct Coord *C6=init_coord(1,1);
    struct Coord *C7=init_coord(-1,0);  struct Coord *C8=init_coord(-1,1);      struct Coord *C9=init_coord(-1,-1);
    struct Coord *C10=init_coord(2,1);  struct Coord *C11=init_coord(2,0);      struct Coord *C12=init_coord(2,-1);
    struct Coord *C13=init_coord(3,0);  struct Coord *C14=init_coord(3,1);      struct Coord *C15=init_coord(3,-1);
    struct Coord *C16=init_coord(3,-2); struct Coord *C17=init_coord(2,-2);     struct Coord *C18=init_coord(1,-2);
    struct Coord *C19=init_coord(0,-2); struct Coord *C20=init_coord(-1,-2);    struct Coord *C21=init_coord(-2,0);
    struct Coord *C22=init_coord(-2,1); struct Coord *C23=init_coord(-2,-1);    struct Coord *C24=init_coord(-2,-2);
    struct Coord *C25=init_coord(-2,2); struct Coord *C26=init_coord(-1,2);     struct Coord *C27=init_coord(0,2);
    struct Coord *C28=init_coord(1,2);  struct Coord *C29=init_coord(2,2);      struct Coord *C30=init_coord(3,2);

    tile1->meeple=list_player->player[0];
    tile1->meeplePlace=MP_TOP;
    tile6->meeple=list_player->player[1];
    tile6->meeplePlace=MP_LEFT;
    tile8->meeple=list_player->player[2];
    tile8->meeplePlace=MP_BOT;
    tile10->meeple=list_player->player[3];
    tile10->meeplePlace=MP_MIDDLE;

    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,NULL,G->right);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,NULL,G);
    G->top=init_grid(tile5,C5,NULL,NULL,G,NULL);
    G->top->right=init_grid(tile6,C6,NULL,G->top,G->right,NULL);
    G->left=init_grid(tile7,C7,G,NULL,NULL,NULL);
    G->left->top=init_grid(tile8,C8,G->top,NULL,G->left,NULL);
    G->left->bot=init_grid(tile9,C9,G->bot,NULL,NULL,G->left);
    G->right->top->right=init_grid(tile10,C10,NULL,G->right->top,NULL,NULL);
    G->right->right=init_grid(tile11,C11,NULL,G->right,NULL,G->right->top->right);
    G->right->right->bot=init_grid(tile12,C12,NULL,G->right->bot,NULL,G->right->right);
    G->right->right->right=init_grid(tile13,C13,NULL,G->right->right,NULL,NULL);
    G->right->right->right->top=init_grid(tile14,C14,NULL,G->top->right->right,G->right->right,NULL);
    G->right->right->right->bot=init_grid(tile15,C15,NULL,G->bot->right->right,NULL,G->right->right);
    G->right->right->right->bot->bot=init_grid(tile16,C16,NULL,NULL,NULL,G->right->right->right->bot);
    G->right->right->bot->bot=init_grid(tile17,C17,G->right->right->right->bot->bot,NULL,NULL,G->right->right->bot);
    G->right->bot->bot=init_grid(tile18,C18,G->right->right->bot->bot,NULL,NULL,G->right->bot);
    G->bot->bot=init_grid(tile19,C19,G->right->bot->bot,NULL,NULL,G->bot);
    G->left->bot->bot=init_grid(tile20,C20,G->bot->bot,NULL,NULL,G->left->bot);
    G->left->left=init_grid(tile21,C21,G->left,NULL,NULL,NULL);
    G->left->left->top=init_grid(tile22,C22,G->left->top,NULL,G->left->left,NULL);
    G->left->left->bot=init_grid(tile23,C23,G->left->bot,NULL,NULL,G->left->left);
    G->left->left->bot->bot=init_grid(tile24,C24,G->left->bot->bot,NULL,NULL,G->left->left->bot);
    G->left->left->top->top=init_grid(tile25,C25,NULL,NULL,G->left->left->top,NULL);
    G->left->top->top=init_grid(tile26,C26,NULL,G->left->left->top->top,G->left->top,NULL);
    G->top->top=init_grid(tile27,C27,NULL,G->left->top->top,G->top,NULL);
    G->right->top->top=init_grid(tile28,C28,NULL,G->top->top,G->right->top,NULL);
    G->right->right->top->top=init_grid(tile29,C29,NULL,G->right->top->top,G->right->right->top,NULL);
    G->right->right->right->top->top=init_grid(tile30,C30,NULL,G->right->right->top->top,G->right->right->right->top,NULL);

    char test1 =nbMeepleVille(G->top,0);
    char test2 =nbMeepleVille(G->top,1);
    char test3 =nbMeepleVille(G->top,2);
    char test4 =nbMeepleVille(G->top->right->right,0);
    char test5 =nbMeepleVille(G->top->right->right,3);


    cr_assert(test1==1,"test1 echoué");
    cr_assert(test2==1,"test2 echoué");
    cr_assert(test3==0,"test3 echoué");
    cr_assert(test4==0,"test4 echoué");
    cr_assert(test5==1,"test5 echoué");
}

Test(all ,nbMeepleVille_nocolor)
{
        
    nbPlayers=8;
    struct list_player *list_player=init_player_list(0);
    struct Tile *tile1 = init_tile(ROUTE,VILLE,ROUTE,VILLE,ROUTE); struct Tile *tile2 = init_tile(ROUTE,PRE,ROUTE,VILLE,ROUTE);
    struct Tile *tile3 = init_tile(PRE,VILLE,VILLE,PRE,VILLE);     struct Tile *tile4 = init_tile(VILLE,VILLE,VILLE,PRE,VILLE);
    struct Tile *tile5 = init_tile(VILLE,VILLE,VILLE,VILLE,VILLE); struct Tile *tile6 = init_tile(VILLE,PRE,VILLE,PRE,PRE);
    struct Tile *tile7 = init_tile(ROUTE,ROUTE,PRE,VILLE,ROUTE);   struct Tile *tile8 = init_tile(PRE,ROUTE,ROUTE,ROUTE,VILLAGE);
    struct Tile *tile9 = init_tile(VILLE,VILLE,PRE,PRE,VILLE);     struct Tile *tile10= init_tile(PRE,PRE,VILLE,VILLE,VILLE);
    struct Tile *tile11= init_tile(PRE,VILLE,ROUTE,ROUTE,ROUTE);   struct Tile *tile12= init_tile(ROUTE,ROUTE,PRE,ROUTE,ABBAYES);
    struct Tile *tile13= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile14= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);
    struct Tile *tile15= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile16= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile17= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile18= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile19= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile20= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile21= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile22= init_tile(ROUTE,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile23= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile24= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile25= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile26= init_tile(RIEN,RIEN,RIEN,ROUTE,RIEN);
    struct Tile *tile27= init_tile(RIEN,RIEN,RIEN,VILLE,RIEN);      struct Tile *tile28= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);
    struct Tile *tile29= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);      struct Tile *tile30= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Coord *C1=init_coord(0,0);   struct Coord *C2=init_coord(1,0);       struct Coord *C3=init_coord(1,-1);
    struct Coord *C4=init_coord(-1,0);  struct Coord *C5=init_coord(0,1);       struct Coord *C6=init_coord(1,1);
    struct Coord *C7=init_coord(-1,0);  struct Coord *C8=init_coord(-1,1);      struct Coord *C9=init_coord(-1,-1);
    struct Coord *C10=init_coord(2,1);  struct Coord *C11=init_coord(2,0);      struct Coord *C12=init_coord(2,-1);
    struct Coord *C13=init_coord(3,0);  struct Coord *C14=init_coord(3,1);      struct Coord *C15=init_coord(3,-1);
    struct Coord *C16=init_coord(3,-2); struct Coord *C17=init_coord(2,-2);     struct Coord *C18=init_coord(1,-2);
    struct Coord *C19=init_coord(0,-2); struct Coord *C20=init_coord(-1,-2);    struct Coord *C21=init_coord(-2,0);
    struct Coord *C22=init_coord(-2,1); struct Coord *C23=init_coord(-2,-1);    struct Coord *C24=init_coord(-2,-2);
    struct Coord *C25=init_coord(-2,2); struct Coord *C26=init_coord(-1,2);     struct Coord *C27=init_coord(0,2);
    struct Coord *C28=init_coord(1,2);  struct Coord *C29=init_coord(2,2);      struct Coord *C30=init_coord(3,2);

    tile1->meeple=list_player->player[0];
    tile1->meeplePlace=MP_TOP;

    tile6->meeple=list_player->player[1];
    tile6->meeplePlace=MP_LEFT;

    tile8->meeple=list_player->player[2];
    tile8->meeplePlace=MP_BOT;

    tile10->meeple=list_player->player[3];
    tile10->meeplePlace=MP_MIDDLE;

    tile4->meeple=list_player->player[0];
    tile4->meeplePlace=MP_MIDDLE;

    tile7->meeple=list_player->player[1];
    tile7->meeplePlace=MP_BOT;

    tile2->meeple=list_player->player[2];
    tile2->meeplePlace=MP_BOT;

    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,NULL,G->right);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,NULL,G);
    G->top=init_grid(tile5,C5,NULL,NULL,G,NULL);
    G->top->right=init_grid(tile6,C6,NULL,G->top,G->right,NULL);
    G->left=init_grid(tile7,C7,G,NULL,NULL,NULL);
    G->left->top=init_grid(tile8,C8,G->top,NULL,G->left,NULL);
    G->left->bot=init_grid(tile9,C9,G->bot,NULL,NULL,G->left);
    G->right->top->right=init_grid(tile10,C10,NULL,G->right->top,NULL,NULL);
    G->right->right=init_grid(tile11,C11,NULL,G->right,NULL,G->right->top->right);
    G->right->right->bot=init_grid(tile12,C12,NULL,G->right->bot,NULL,G->right->right);
    G->right->right->right=init_grid(tile13,C13,NULL,G->right->right,NULL,NULL);
    G->right->right->right->top=init_grid(tile14,C14,NULL,G->top->right->right,G->right->right,NULL);
    G->right->right->right->bot=init_grid(tile15,C15,NULL,G->bot->right->right,NULL,G->right->right);
    G->right->right->right->bot->bot=init_grid(tile16,C16,NULL,NULL,NULL,G->right->right->right->bot);
    G->right->right->bot->bot=init_grid(tile17,C17,G->right->right->right->bot->bot,NULL,NULL,G->right->right->bot);
    G->right->bot->bot=init_grid(tile18,C18,G->right->right->bot->bot,NULL,NULL,G->right->bot);
    G->bot->bot=init_grid(tile19,C19,G->right->bot->bot,NULL,NULL,G->bot);
    G->left->bot->bot=init_grid(tile20,C20,G->bot->bot,NULL,NULL,G->left->bot);
    G->left->left=init_grid(tile21,C21,G->left,NULL,NULL,NULL);
    G->left->left->top=init_grid(tile22,C22,G->left->top,NULL,G->left->left,NULL);
    G->left->left->bot=init_grid(tile23,C23,G->left->bot,NULL,NULL,G->left->left);
    G->left->left->bot->bot=init_grid(tile24,C24,G->left->bot->bot,NULL,NULL,G->left->left->bot);
    G->left->left->top->top=init_grid(tile25,C25,NULL,NULL,G->left->left->top,NULL);
    G->left->top->top=init_grid(tile26,C26,NULL,G->left->left->top->top,G->left->top,NULL);
    G->top->top=init_grid(tile27,C27,NULL,G->left->top->top,G->top,NULL);
    G->right->top->top=init_grid(tile28,C28,NULL,G->top->top,G->right->top,NULL);
    G->right->right->top->top=init_grid(tile29,C29,NULL,G->right->top->top,G->right->right->top,NULL);
    G->right->right->right->top->top=init_grid(tile30,C30,NULL,G->right->right->top->top,G->right->right->right->top,NULL);

    char test1=nbMeepleVille_nocolor(G->top);
    char test2=nbMeepleVille_nocolor(G->bot);
    char test3=nbMeepleVille_nocolor(G->bot->right);
    char test4=nbMeepleVille_nocolor(G->bot->left);
    char test5=nbMeepleVille_nocolor(G->top->right->right);

    cr_assert(test1==2,"test1 echoué");
    cr_assert(test2==3,"test2 echoué");
    cr_assert(test3==3,"test3 echoué");
    cr_assert(test4==3,"test4 echoué");
    cr_assert(test5==1,"test5 echoué");
}

Test(all ,nbMeepleAbbaye)
{
    nbPlayers=8;
    struct list_player *list_player=init_player_list(0);
    struct Tile *tile1 = init_tile(ROUTE,VILLE,ROUTE,VILLE,ROUTE); struct Tile *tile2 = init_tile(ROUTE,PRE,ROUTE,VILLE,ROUTE);
    struct Tile *tile3 = init_tile(PRE,VILLE,VILLE,PRE,VILLE);     struct Tile *tile4 = init_tile(VILLE,VILLE,VILLE,PRE,VILLE);
    struct Tile *tile5 = init_tile(VILLE,VILLE,VILLE,VILLE,VILLE); struct Tile *tile6 = init_tile(VILLE,PRE,VILLE,PRE,PRE);
    struct Tile *tile7 = init_tile(ROUTE,ROUTE,PRE,VILLE,ROUTE);   struct Tile *tile8 = init_tile(PRE,ROUTE,ROUTE,ROUTE,VILLAGE);
    struct Tile *tile9 = init_tile(VILLE,VILLE,PRE,PRE,VILLE);     struct Tile *tile10= init_tile(PRE,PRE,VILLE,VILLE,VILLE);
    struct Tile *tile11= init_tile(PRE,VILLE,ROUTE,ROUTE,ROUTE);   struct Tile *tile12= init_tile(ROUTE,ROUTE,PRE,ROUTE,ABBAYES);
    struct Tile *tile13= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile14= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);
    struct Tile *tile15= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile16= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile17= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile18= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile19= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile20= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile21= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile22= init_tile(ROUTE,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile23= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile24= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile25= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile26= init_tile(RIEN,RIEN,RIEN,ROUTE,RIEN);
    struct Tile *tile27= init_tile(RIEN,RIEN,RIEN,VILLE,RIEN);      struct Tile *tile28= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);
    struct Tile *tile29= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);      struct Tile *tile30= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Coord *C1=init_coord(0,0);   struct Coord *C2=init_coord(1,0);       struct Coord *C3=init_coord(1,-1);
    struct Coord *C4=init_coord(-1,0);  struct Coord *C5=init_coord(0,1);       struct Coord *C6=init_coord(1,1);
    struct Coord *C7=init_coord(-1,0);  struct Coord *C8=init_coord(-1,1);      struct Coord *C9=init_coord(-1,-1);
    struct Coord *C10=init_coord(2,1);  struct Coord *C11=init_coord(2,0);      struct Coord *C12=init_coord(2,-1);
    struct Coord *C13=init_coord(3,0);  struct Coord *C14=init_coord(3,1);      struct Coord *C15=init_coord(3,-1);
    struct Coord *C16=init_coord(3,-2); struct Coord *C17=init_coord(2,-2);     struct Coord *C18=init_coord(1,-2);
    struct Coord *C19=init_coord(0,-2); struct Coord *C20=init_coord(-1,-2);    struct Coord *C21=init_coord(-2,0);
    struct Coord *C22=init_coord(-2,1); struct Coord *C23=init_coord(-2,-1);    struct Coord *C24=init_coord(-2,-2);
    struct Coord *C25=init_coord(-2,2); struct Coord *C26=init_coord(-1,2);     struct Coord *C27=init_coord(0,2);
    struct Coord *C28=init_coord(1,2);  struct Coord *C29=init_coord(2,2);      struct Coord *C30=init_coord(3,2);

    tile12->meeple=list_player->player[3];
    tile12->meeplePlace=MP_MIDDLE;

    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,NULL,G->right);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,NULL,G);
    G->top=init_grid(tile5,C5,NULL,NULL,G,NULL);
    G->top->right=init_grid(tile6,C6,NULL,G->top,G->right,NULL);
    G->left=init_grid(tile7,C7,G,NULL,NULL,NULL);
    G->left->top=init_grid(tile8,C8,G->top,NULL,G->left,NULL);
    G->left->bot=init_grid(tile9,C9,G->bot,NULL,NULL,G->left);
    G->right->top->right=init_grid(tile10,C10,NULL,G->right->top,NULL,NULL);
    G->right->right=init_grid(tile11,C11,NULL,G->right,NULL,G->right->top->right);
    G->right->right->bot=init_grid(tile12,C12,NULL,G->right->bot,NULL,G->right->right);
    G->right->right->right=init_grid(tile13,C13,NULL,G->right->right,NULL,NULL);
    G->right->right->right->top=init_grid(tile14,C14,NULL,G->top->right->right,G->right->right,NULL);
    G->right->right->right->bot=init_grid(tile15,C15,NULL,G->bot->right->right,NULL,G->right->right);
    G->right->right->right->bot->bot=init_grid(tile16,C16,NULL,NULL,NULL,G->right->right->right->bot);
    G->right->right->bot->bot=init_grid(tile17,C17,G->right->right->right->bot->bot,NULL,NULL,G->right->right->bot);
    G->right->bot->bot=init_grid(tile18,C18,G->right->right->bot->bot,NULL,NULL,G->right->bot);
    G->bot->bot=init_grid(tile19,C19,G->right->bot->bot,NULL,NULL,G->bot);
    G->left->bot->bot=init_grid(tile20,C20,G->bot->bot,NULL,NULL,G->left->bot);
    G->left->left=init_grid(tile21,C21,G->left,NULL,NULL,NULL);
    G->left->left->top=init_grid(tile22,C22,G->left->top,NULL,G->left->left,NULL);
    G->left->left->bot=init_grid(tile23,C23,G->left->bot,NULL,NULL,G->left->left);
    G->left->left->bot->bot=init_grid(tile24,C24,G->left->bot->bot,NULL,NULL,G->left->left->bot);
    G->left->left->top->top=init_grid(tile25,C25,NULL,NULL,G->left->left->top,NULL);
    G->left->top->top=init_grid(tile26,C26,NULL,G->left->left->top->top,G->left->top,NULL);
    G->top->top=init_grid(tile27,C27,NULL,G->left->top->top,G->top,NULL);
    G->right->top->top=init_grid(tile28,C28,NULL,G->top->top,G->right->top,NULL);
    G->right->right->top->top=init_grid(tile29,C29,NULL,G->right->top->top,G->right->right->top,NULL);
    G->right->right->right->top->top=init_grid(tile30,C30,NULL,G->right->right->top->top,G->right->right->right->top,NULL);

    char test1[8],test2[8],test3[8];

    for(int i=0;i<8;i++){
        test1[i]=nbMeepleAbbaye(G->right->right->bot,i);
        test2[i]=nbMeepleAbbaye(G->right->right,i);
        test3[i]=nbMeepleAbbaye(G->right,i);
    }

    G->right->right->bot->tile->meeplePlace=MP_TOP;


    cr_assert(test1[0]==0);
    cr_assert(test1[1]==0);
    cr_assert(test1[2]==0);
    cr_assert(test1[3]==1);//joueur 3
    cr_assert(test1[4]==0);
    cr_assert(test1[5]==0);
    cr_assert(test1[6]==0);
    cr_assert(test1[7]==0);
    cr_assert(test2[0]==0);
    cr_assert(test2[1]==0);
    cr_assert(test2[2]==0);
    cr_assert(test2[3]==1);//joueur 3
    cr_assert(test2[4]==0);
    cr_assert(test2[5]==0);
    cr_assert(test2[6]==0);
    cr_assert(test2[7]==0);
    cr_assert(test3[0]==0);
    cr_assert(test3[1]==0);
    cr_assert(test3[2]==0);
    cr_assert(test3[3]==1);//joueur 3
    cr_assert(test3[4]==0);
    cr_assert(test3[5]==0);
    cr_assert(test3[6]==0);
    cr_assert(test3[7]==0);

    char test4[8],test5[8],test6[8];

    for(int i=0;i<8;i++){
        test4[i]=nbMeepleAbbaye(G->right->right->bot,i);
        test5[i]=nbMeepleAbbaye(G->right->right,i);
        test6[i]=nbMeepleAbbaye(G->right,i);
    }

    cr_assert(test4[0]==0);
    cr_assert(test4[1]==0);
    cr_assert(test4[2]==0);
    cr_assert(test4[3]==0);
    cr_assert(test4[4]==0);
    cr_assert(test4[5]==0);
    cr_assert(test4[6]==0);
    cr_assert(test4[7]==0);
    cr_assert(test5[0]==0);
    cr_assert(test5[1]==0);
    cr_assert(test5[2]==0);
    cr_assert(test5[3]==0);
    cr_assert(test5[4]==0);
    cr_assert(test5[5]==0);
    cr_assert(test5[6]==0);
    cr_assert(test5[7]==0);
    cr_assert(test6[0]==0);
    cr_assert(test6[1]==0);
    cr_assert(test6[2]==0);
    cr_assert(test6[3]==0);
    cr_assert(test6[4]==0);
    cr_assert(test6[5]==0);
    cr_assert(test6[6]==0);
    cr_assert(test6[7]==0);
}

Test(all ,nbMeepleAbbaye_nocolor)
{
    nbPlayers=8;
    struct list_player *list_player=init_player_list(0);
    struct Tile *tile1 = init_tile(ROUTE,VILLE,ROUTE,VILLE,ROUTE); struct Tile *tile2 = init_tile(ROUTE,PRE,ROUTE,VILLE,ROUTE);
    struct Tile *tile3 = init_tile(PRE,VILLE,VILLE,PRE,VILLE);     struct Tile *tile4 = init_tile(VILLE,VILLE,VILLE,PRE,VILLE);
    struct Tile *tile5 = init_tile(VILLE,VILLE,VILLE,VILLE,VILLE); struct Tile *tile6 = init_tile(VILLE,PRE,VILLE,PRE,PRE);
    struct Tile *tile7 = init_tile(ROUTE,ROUTE,PRE,VILLE,ROUTE);   struct Tile *tile8 = init_tile(PRE,ROUTE,ROUTE,ROUTE,VILLAGE);
    struct Tile *tile9 = init_tile(VILLE,VILLE,PRE,PRE,VILLE);     struct Tile *tile10= init_tile(PRE,PRE,VILLE,VILLE,VILLE);
    struct Tile *tile11= init_tile(PRE,VILLE,ROUTE,ROUTE,ROUTE);   struct Tile *tile12= init_tile(ROUTE,ROUTE,PRE,ROUTE,ABBAYES);
    struct Tile *tile13= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile14= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);
    struct Tile *tile15= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile16= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile17= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile18= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile19= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile20= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile21= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile22= init_tile(ROUTE,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile23= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile24= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile25= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile26= init_tile(RIEN,RIEN,RIEN,ROUTE,RIEN);
    struct Tile *tile27= init_tile(RIEN,RIEN,RIEN,VILLE,RIEN);      struct Tile *tile28= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);
    struct Tile *tile29= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);      struct Tile *tile30= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Coord *C1=init_coord(0,0);   struct Coord *C2=init_coord(1,0);       struct Coord *C3=init_coord(1,-1);
    struct Coord *C4=init_coord(-1,0);  struct Coord *C5=init_coord(0,1);       struct Coord *C6=init_coord(1,1);
    struct Coord *C7=init_coord(-1,0);  struct Coord *C8=init_coord(-1,1);      struct Coord *C9=init_coord(-1,-1);
    struct Coord *C10=init_coord(2,1);  struct Coord *C11=init_coord(2,0);      struct Coord *C12=init_coord(2,-1);
    struct Coord *C13=init_coord(3,0);  struct Coord *C14=init_coord(3,1);      struct Coord *C15=init_coord(3,-1);
    struct Coord *C16=init_coord(3,-2); struct Coord *C17=init_coord(2,-2);     struct Coord *C18=init_coord(1,-2);
    struct Coord *C19=init_coord(0,-2); struct Coord *C20=init_coord(-1,-2);    struct Coord *C21=init_coord(-2,0);
    struct Coord *C22=init_coord(-2,1); struct Coord *C23=init_coord(-2,-1);    struct Coord *C24=init_coord(-2,-2);
    struct Coord *C25=init_coord(-2,2); struct Coord *C26=init_coord(-1,2);     struct Coord *C27=init_coord(0,2);
    struct Coord *C28=init_coord(1,2);  struct Coord *C29=init_coord(2,2);      struct Coord *C30=init_coord(3,2);

    tile12->meeple=list_player->player[3];
    tile12->meeplePlace=MP_MIDDLE;

    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,NULL,G->right);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,NULL,G);
    G->top=init_grid(tile5,C5,NULL,NULL,G,NULL);
    G->top->right=init_grid(tile6,C6,NULL,G->top,G->right,NULL);
    G->left=init_grid(tile7,C7,G,NULL,NULL,NULL);
    G->left->top=init_grid(tile8,C8,G->top,NULL,G->left,NULL);
    G->left->bot=init_grid(tile9,C9,G->bot,NULL,NULL,G->left);
    G->right->top->right=init_grid(tile10,C10,NULL,G->right->top,NULL,NULL);
    G->right->right=init_grid(tile11,C11,NULL,G->right,NULL,G->right->top->right);
    G->right->right->bot=init_grid(tile12,C12,NULL,G->right->bot,NULL,G->right->right);
    G->right->right->right=init_grid(tile13,C13,NULL,G->right->right,NULL,NULL);
    G->right->right->right->top=init_grid(tile14,C14,NULL,G->top->right->right,G->right->right,NULL);
    G->right->right->right->bot=init_grid(tile15,C15,NULL,G->bot->right->right,NULL,G->right->right);
    G->right->right->right->bot->bot=init_grid(tile16,C16,NULL,NULL,NULL,G->right->right->right->bot);
    G->right->right->bot->bot=init_grid(tile17,C17,G->right->right->right->bot->bot,NULL,NULL,G->right->right->bot);
    G->right->bot->bot=init_grid(tile18,C18,G->right->right->bot->bot,NULL,NULL,G->right->bot);
    G->bot->bot=init_grid(tile19,C19,G->right->bot->bot,NULL,NULL,G->bot);
    G->left->bot->bot=init_grid(tile20,C20,G->bot->bot,NULL,NULL,G->left->bot);
    G->left->left=init_grid(tile21,C21,G->left,NULL,NULL,NULL);
    G->left->left->top=init_grid(tile22,C22,G->left->top,NULL,G->left->left,NULL);
    G->left->left->bot=init_grid(tile23,C23,G->left->bot,NULL,NULL,G->left->left);
    G->left->left->bot->bot=init_grid(tile24,C24,G->left->bot->bot,NULL,NULL,G->left->left->bot);
    G->left->left->top->top=init_grid(tile25,C25,NULL,NULL,G->left->left->top,NULL);
    G->left->top->top=init_grid(tile26,C26,NULL,G->left->left->top->top,G->left->top,NULL);
    G->top->top=init_grid(tile27,C27,NULL,G->left->top->top,G->top,NULL);
    G->right->top->top=init_grid(tile28,C28,NULL,G->top->top,G->right->top,NULL);
    G->right->right->top->top=init_grid(tile29,C29,NULL,G->right->top->top,G->right->right->top,NULL);
    G->right->right->right->top->top=init_grid(tile30,C30,NULL,G->right->right->top->top,G->right->right->right->top,NULL);

    char test1[8],test2[8],test3[8];

    for(int i=0;i<8;i++){
        test1[i]=nbMeepleAbbaye_nocolor(G->right->right->bot);
        test2[i]=nbMeepleAbbaye_nocolor(G->right->right);
        test3[i]=nbMeepleAbbaye_nocolor(G->right);
    }

    

    G->right->right->bot->tile->meeplePlace=MP_TOP;

    char test4[8],test5[8],test6[8];

    for(int i=0;i<8;i++){
        test4[i]=nbMeepleAbbaye_nocolor(G->right->right->bot);
        test5[i]=nbMeepleAbbaye_nocolor(G->right->right);
        test6[i]=nbMeepleAbbaye_nocolor(G->right);
    }

    for(int i=0;i<8;i++){
        cr_assert(test1[i]==1);
        cr_assert(test2[i]==1);
        cr_assert(test3[i]==1);
    }

    for(int i=0;i<8;i++){
        cr_assert(test4[i]==0);
        cr_assert(test5[i]==0);
        cr_assert(test6[i]==0);
    }

}

Test(all ,max)
{
    nbPlayers=8;
    char test[8]={4,1,5,6,101,118,99,41};
    char restest=max(test);
    cr_assert(restest==118);
}

Test(all ,countMeepleRoad)
{
    nbPlayers=8;
    struct list_player *list_player=init_player_list(0);
    struct Tile *tile1 = init_tile(ROUTE,VILLE,ROUTE,VILLE,ROUTE); struct Tile *tile2 = init_tile(ROUTE,PRE,ROUTE,VILLE,ROUTE);
    struct Tile *tile3 = init_tile(PRE,VILLE,VILLE,PRE,VILLE);     struct Tile *tile4 = init_tile(VILLE,VILLE,VILLE,PRE,VILLE);
    struct Tile *tile5 = init_tile(VILLE,VILLE,VILLE,VILLE,VILLE); struct Tile *tile6 = init_tile(VILLE,PRE,VILLE,PRE,PRE);
    struct Tile *tile7 = init_tile(ROUTE,ROUTE,PRE,VILLE,ROUTE);   struct Tile *tile8 = init_tile(PRE,ROUTE,ROUTE,ROUTE,VILLAGE);
    struct Tile *tile9 = init_tile(VILLE,VILLE,PRE,PRE,VILLE);     struct Tile *tile10= init_tile(PRE,PRE,VILLE,VILLE,VILLE);
    struct Tile *tile11= init_tile(PRE,VILLE,ROUTE,ROUTE,ROUTE);   struct Tile *tile12= init_tile(ROUTE,ROUTE,PRE,ROUTE,ABBAYES);
    struct Tile *tile13= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile14= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);
    struct Tile *tile15= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile16= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile17= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile18= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile19= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile20= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile21= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile22= init_tile(ROUTE,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile23= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile24= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile25= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile26= init_tile(RIEN,RIEN,RIEN,ROUTE,RIEN);
    struct Tile *tile27= init_tile(RIEN,RIEN,RIEN,VILLE,RIEN);      struct Tile *tile28= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);
    struct Tile *tile29= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);      struct Tile *tile30= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Coord *C1=init_coord(0,0);   struct Coord *C2=init_coord(1,0);       struct Coord *C3=init_coord(1,-1);
    struct Coord *C4=init_coord(-1,0);  struct Coord *C5=init_coord(0,1);       struct Coord *C6=init_coord(1,1);
    struct Coord *C7=init_coord(-1,0);  struct Coord *C8=init_coord(-1,1);      struct Coord *C9=init_coord(-1,-1);
    struct Coord *C10=init_coord(2,1);  struct Coord *C11=init_coord(2,0);      struct Coord *C12=init_coord(2,-1);
    struct Coord *C13=init_coord(3,0);  struct Coord *C14=init_coord(3,1);      struct Coord *C15=init_coord(3,-1);
    struct Coord *C16=init_coord(3,-2); struct Coord *C17=init_coord(2,-2);     struct Coord *C18=init_coord(1,-2);
    struct Coord *C19=init_coord(0,-2); struct Coord *C20=init_coord(-1,-2);    struct Coord *C21=init_coord(-2,0);
    struct Coord *C22=init_coord(-2,1); struct Coord *C23=init_coord(-2,-1);    struct Coord *C24=init_coord(-2,-2);
    struct Coord *C25=init_coord(-2,2); struct Coord *C26=init_coord(-1,2);     struct Coord *C27=init_coord(0,2);
    struct Coord *C28=init_coord(1,2);  struct Coord *C29=init_coord(2,2);      struct Coord *C30=init_coord(3,2);

    tile7->meeple=list_player->player[0];
    tile7->meeplePlace=MP_RIGHT;
    tile11->meeple=list_player->player[1];
    tile11->meeplePlace=MP_LEFT;
    tile1->meeple=list_player->player[0];
    tile1->meeplePlace=MP_TOP;
    tile2->meeple=list_player->player[0];
    tile2->meeplePlace=MP_MIDDLE;

    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,NULL,G->right);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,NULL,G);
    G->top=init_grid(tile5,C5,NULL,NULL,G,NULL);
    G->top->right=init_grid(tile6,C6,NULL,G->top,G->right,NULL);
    G->left=init_grid(tile7,C7,G,NULL,NULL,NULL);
    G->left->top=init_grid(tile8,C8,G->top,NULL,G->left,NULL);
    G->left->bot=init_grid(tile9,C9,G->bot,NULL,NULL,G->left);
    G->right->top->right=init_grid(tile10,C10,NULL,G->right->top,NULL,NULL);
    G->right->right=init_grid(tile11,C11,NULL,G->right,NULL,G->right->top->right);
    G->right->right->bot=init_grid(tile12,C12,NULL,G->right->bot,NULL,G->right->right);
    G->right->right->right=init_grid(tile13,C13,NULL,G->right->right,NULL,NULL);
    G->right->right->right->top=init_grid(tile14,C14,NULL,G->top->right->right,G->right->right,NULL);
    G->right->right->right->bot=init_grid(tile15,C15,NULL,G->bot->right->right,NULL,G->right->right);
    G->right->right->right->bot->bot=init_grid(tile16,C16,NULL,NULL,NULL,G->right->right->right->bot);
    G->right->right->bot->bot=init_grid(tile17,C17,G->right->right->right->bot->bot,NULL,NULL,G->right->right->bot);
    G->right->bot->bot=init_grid(tile18,C18,G->right->right->bot->bot,NULL,NULL,G->right->bot);
    G->bot->bot=init_grid(tile19,C19,G->right->bot->bot,NULL,NULL,G->bot);
    G->left->bot->bot=init_grid(tile20,C20,G->bot->bot,NULL,NULL,G->left->bot);
    G->left->left=init_grid(tile21,C21,G->left,NULL,NULL,NULL);
    G->left->left->top=init_grid(tile22,C22,G->left->top,NULL,G->left->left,NULL);
    G->left->left->bot=init_grid(tile23,C23,G->left->bot,NULL,NULL,G->left->left);
    G->left->left->bot->bot=init_grid(tile24,C24,G->left->bot->bot,NULL,NULL,G->left->left->bot);
    G->left->left->top->top=init_grid(tile25,C25,NULL,NULL,G->left->left->top,NULL);
    G->left->top->top=init_grid(tile26,C26,NULL,G->left->left->top->top,G->left->top,NULL);
    G->top->top=init_grid(tile27,C27,NULL,G->left->top->top,G->top,NULL);
    G->right->top->top=init_grid(tile28,C28,NULL,G->top->top,G->right->top,NULL);
    G->right->right->top->top=init_grid(tile29,C29,NULL,G->right->top->top,G->right->right->top,NULL);
    G->right->right->right->top->top=init_grid(tile30,C30,NULL,G->right->right->top->top,G->right->right->right->top,NULL);

    char test1=countMeepleRoad(G,RIGHT,0);
    printf("test1=%d\n",test1);
    char test2=countMeepleRoad(G,RIGHT,1);
    printf("test2=%d\n",test2);

    cr_assert(test1==2,"countmeepleroad test1 faux");
    cr_assert(test2==1,"countmeepleroad test2 faux");
}

Test(all,free_Grid)
{
    struct Tile *tile1 = init_tile(ROUTE,VILLE,ROUTE,VILLE,ROUTE); struct Tile *tile2 = init_tile(ROUTE,PRE,ROUTE,VILLE,ROUTE);
    struct Tile *tile3 = init_tile(PRE,VILLE,VILLE,PRE,VILLE);     struct Tile *tile4 = init_tile(VILLE,VILLE,VILLE,PRE,VILLE);
    struct Tile *tile5 = init_tile(VILLE,VILLE,VILLE,VILLE,VILLE); struct Tile *tile6 = init_tile(VILLE,PRE,VILLE,PRE,PRE);
    struct Tile *tile7 = init_tile(ROUTE,ROUTE,PRE,VILLE,ROUTE);   struct Tile *tile8 = init_tile(PRE,ROUTE,ROUTE,ROUTE,VILLAGE);
    struct Tile *tile9 = init_tile(VILLE,VILLE,PRE,PRE,VILLE);     struct Tile *tile10= init_tile(PRE,PRE,VILLE,VILLE,VILLE);
    struct Tile *tile11= init_tile(PRE,VILLE,ROUTE,ROUTE,ROUTE);   struct Tile *tile12= init_tile(ROUTE,ROUTE,PRE,ROUTE,ABBAYES);
    struct Tile *tile13= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile14= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);
    struct Tile *tile15= init_tile(RIEN,RIEN,PRE,RIEN,RIEN);      struct Tile *tile16= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile17= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile18= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile19= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);      struct Tile *tile20= init_tile(RIEN,PRE,RIEN,RIEN,RIEN);
    struct Tile *tile21= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile22= init_tile(ROUTE,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile23= init_tile(PRE,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile24= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Tile *tile25= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);      struct Tile *tile26= init_tile(RIEN,RIEN,RIEN,ROUTE,RIEN);
    struct Tile *tile27= init_tile(RIEN,RIEN,RIEN,VILLE,RIEN);      struct Tile *tile28= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);
    struct Tile *tile29= init_tile(RIEN,RIEN,RIEN,PRE,RIEN);      struct Tile *tile30= init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);
    struct Coord *C1=init_coord(0,0);   struct Coord *C2=init_coord(1,0);       struct Coord *C3=init_coord(1,-1);
    struct Coord *C4=init_coord(-1,0);  struct Coord *C5=init_coord(0,1);       struct Coord *C6=init_coord(1,1);
    struct Coord *C7=init_coord(-1,0);  struct Coord *C8=init_coord(-1,1);      struct Coord *C9=init_coord(-1,-1);
    struct Coord *C10=init_coord(2,1);  struct Coord *C11=init_coord(2,0);      struct Coord *C12=init_coord(2,-1);
    struct Coord *C13=init_coord(3,0);  struct Coord *C14=init_coord(3,1);      struct Coord *C15=init_coord(3,-1);
    struct Coord *C16=init_coord(3,-2); struct Coord *C17=init_coord(2,-2);     struct Coord *C18=init_coord(1,-2);
    struct Coord *C19=init_coord(0,-2); struct Coord *C20=init_coord(-1,-2);    struct Coord *C21=init_coord(-2,0);
    struct Coord *C22=init_coord(-2,1); struct Coord *C23=init_coord(-2,-1);    struct Coord *C24=init_coord(-2,-2);
    struct Coord *C25=init_coord(-2,2); struct Coord *C26=init_coord(-1,2);     struct Coord *C27=init_coord(0,2);
    struct Coord *C28=init_coord(1,2);  struct Coord *C29=init_coord(2,2);      struct Coord *C30=init_coord(3,2);

    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    G->right->bot=init_grid(tile3,C3,NULL,NULL,NULL,G->right);
    G->bot=init_grid(tile4,C4,G->right->bot,NULL,NULL,G);
    G->top=init_grid(tile5,C5,NULL,NULL,G,NULL);
    G->top->right=init_grid(tile6,C6,NULL,G->top,G->right,NULL);
    G->left=init_grid(tile7,C7,G,NULL,NULL,NULL);
    G->left->top=init_grid(tile8,C8,G->top,NULL,G->left,NULL);
    G->left->bot=init_grid(tile9,C9,G->bot,NULL,NULL,G->left);
    G->right->top->right=init_grid(tile10,C10,NULL,G->right->top,NULL,NULL);
    G->right->right=init_grid(tile11,C11,NULL,G->right,NULL,G->right->top->right);
    G->right->right->bot=init_grid(tile12,C12,NULL,G->right->bot,NULL,G->right->right);
    G->right->right->right=init_grid(tile13,C13,NULL,G->right->right,NULL,NULL);
    G->right->right->right->top=init_grid(tile14,C14,NULL,G->top->right->right,G->right->right,NULL);
    G->right->right->right->bot=init_grid(tile15,C15,NULL,G->bot->right->right,NULL,G->right->right);
    G->right->right->right->bot->bot=init_grid(tile16,C16,NULL,NULL,NULL,G->right->right->right->bot);
    G->right->right->bot->bot=init_grid(tile17,C17,G->right->right->right->bot->bot,NULL,NULL,G->right->right->bot);
    G->right->bot->bot=init_grid(tile18,C18,G->right->right->bot->bot,NULL,NULL,G->right->bot);
    G->bot->bot=init_grid(tile19,C19,G->right->bot->bot,NULL,NULL,G->bot);
    G->left->bot->bot=init_grid(tile20,C20,G->bot->bot,NULL,NULL,G->left->bot);
    G->left->left=init_grid(tile21,C21,G->left,NULL,NULL,NULL);
    G->left->left->top=init_grid(tile22,C22,G->left->top,NULL,G->left->left,NULL);
    G->left->left->bot=init_grid(tile23,C23,G->left->bot,NULL,NULL,G->left->left);
    G->left->left->bot->bot=init_grid(tile24,C24,G->left->bot->bot,NULL,NULL,G->left->left->bot);
    G->left->left->top->top=init_grid(tile25,C25,NULL,NULL,G->left->left->top,NULL);
    G->left->top->top=init_grid(tile26,C26,NULL,G->left->left->top->top,G->left->top,NULL);
    G->top->top=init_grid(tile27,C27,NULL,G->left->top->top,G->top,NULL);
    G->right->top->top=init_grid(tile28,C28,NULL,G->top->top,G->right->top,NULL);
    G->right->right->top->top=init_grid(tile29,C29,NULL,G->right->top->top,G->right->right->top,NULL);
    G->right->right->right->top->top=init_grid(tile30,C30,NULL,G->right->right->top->top,G->right->right->right->top,NULL);

    struct Grid *coin=G->top->top->left->left;
    printf("%d\n",coin->right->tile->left);
    free_Grid(&coin);

}
