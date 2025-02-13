#include <criterion/criterion.h>
#include <criterion/new/assert.h> // si bug essayer de retirer le /new

#include "Carcassonne.h"
#include "tile.c"
#include "player.c"
#include "game_manager_function.c"
#include "data_structure.c"


// -----------------
// ------Test-------
// ----player.c-----
// -----------------

Test(all, init_player)
{
    struct Player *player = malloc(sizeof(struct Player));
    init_player(player);
    cr_expect(player->nbMeeple == 8 ,"la fonction init_player n'a pas initialisé nbMeeple");
    cr_expect(player->points == 0 ,"la fonction init_player n'a pas initialisé les points a 0");
}

Test(all, is_meeple_on_player)
{
    struct Player *player = malloc(sizeof(struct Player));
    init_player(player);
    cr_expect(is_meeple_on_player(player) > 0);
    player->nbMeeple = 0;
    cr_expect(is_meeple_on_player(player) == 0);
}

// -----------------
// ------Test-------
// -----tile.c------
// -----------------

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
        ,"init_tile na pas bien initialisé la tuile");
}

Test(all, is_meeple_on_tile)
{
    struct Tile *tile = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    cr_assert(tile->meeple == NO_MEEPLE);
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
    cr_assert(tileArray[0]->right == ROUTE);
    cr_assert(tileArray[0]->middle == ROUTE);
    cr_assert(tileArray[1]->middle == BLASON);
    cr_assert(tileArray[1]->right == BLASON);
    cr_assert(tileArray[70]->middle == PRE);
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

Test(all, shuffle)
{
    struct Tile **tileArray;
    char *tokenArray[MAX_TOKEN_SIZE + 1] = {"route", "ville", "abbaye", "pre", "village", "blason"};
    tileArray = create_tile_array(CSV_TILE, tokenArray, MAX_TOKEN_SIZE);

    shuffle(tileArray,NBTILE-1);

    for(short i = 0;i<NBTILE-1;i++)
    {
        cr_assert(tileArray[i] != NULL,"Shuffle has broke array");
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
        cr_assert(tileArray[i]->right == parcour->data->right &&
                  tileArray[i]->bot == parcour->data->bot &&
                  tileArray[i]->left == parcour->data->left && 
                  tileArray[i]->top == parcour->data->top && 
                  tileArray[i]->middle == parcour->data->middle);
        parcour=parcour->next;
        i--;
    }
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

Test(all, rot_tile)
{
    struct Tile *tile_in = init_tile(VILLE, ROUTE, ABBAYES, VILLAGE, PRE); 
    struct Tile *tile_out = init_tile(VILLE, ROUTE, ABBAYES, VILLAGE, PRE);
    
    rot_tile(tile_out);

    cr_assert(tile_out->right == tile_in->bot &&
              tile_out->top == tile_in->right &&
              tile_out->left == tile_in->top &&
              tile_out->bot == tile_in->left &&
              tile_out->middle == tile_in->middle);
 

}

//test player_turn

//test where_i_can_play

//test place_tile

Test(all, enum_to_char)
{
    enum types rien=RIEN;
    enum types route=ROUTE;
    enum types ville=VILLE;
    enum types abbayes=ABBAYES;
    enum types pre=PRE;
    enum types village=VILLAGE;
    enum types blason=BLASON;

    cr_assert(enum_to_char(rien) == 'Z' &&
              enum_to_char(route) == 'R' &&
              enum_to_char(ville) == 'V' &&
              enum_to_char(abbayes) == 'A' &&
              enum_to_char(pre) == 'P' &&
              enum_to_char(village) == 'v' &&
              enum_to_char(blason) == 'B');
}

//test show_tile (affichage)

//test show_part_tile (affichage)

//test choose_w_show

//test show_grid

//test start_game

//test end_game_pointer_counter


// --------------------
// --------Test-------
// --data_structure.c--
// --------------------

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

Test(all, DLList_push_end_one_insert)//Test premiere insertion
{
    struct DLList *db_list=NULL;//initialisation a NULL
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);

    db_list=DLList_push_end(db_list,tile1);

    cr_assert(db_list !=NULL);
    cr_assert(db_list->data->right==VILLE);
    cr_assert(db_list->prev==NULL && db_list->next==NULL);
}

Test(all, DLList_push_end_two_insert)//Test Deuxieme insertion
{
    struct DLList *db_list=NULL;//initialisation a NULL
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);

    db_list = DLList_push_end(db_list,tile1);
    db_list = DLList_push_end(db_list,tile2);

    cr_assert(db_list->next != NULL);
    cr_assert(db_list->next->prev == db_list);
    cr_assert(db_list->next->data->right == ROUTE);
    cr_assert(db_list->prev == NULL);
    cr_assert(db_list->next->next == NULL);
}

Test(all, DLList_push_end_three_insert)
{
    struct DLList *db_list=NULL;//initialisation a NULL
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *tile3 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, PRE);

    db_list = DLList_push_end(db_list,tile1);
    db_list = DLList_push_end(db_list,tile2);
    db_list = DLList_push_end(db_list,tile3);

    cr_assert(db_list->next->next != NULL);
    cr_assert(db_list->next->next->prev == db_list->next);
    cr_assert(db_list->next->next->data->middle == PRE);
    cr_assert(db_list->prev == NULL);
    cr_assert(db_list->next->next->next == NULL);
}

Test(all, DLList_pop_vide)
{
    struct DLList *db_list=NULL;
    struct Tile *tile_rien = init_tile(RIEN,RIEN,RIEN,RIEN,RIEN);

    DLList_pop(db_list,&tile_rien);

    cr_assert(db_list == NULL,"db_list != NULL !");
    cr_assert(tile_rien->bot == RIEN,"tile_rien != RIEN !");
}

Test(all, DLList_pop_one_item)
{
    struct DLList *db_list=NULL;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *out;

    db_list = DLList_push_end(db_list,tile1);
    DLList_pop(db_list,&out);

    //comme il n'y y a qu'un element db_list devrait
    //être a nul
    cr_assert(db_list==NULL,"db_list != NULL !");

    //test si la tuile a ete extraite
    cr_assert(out->right==VILLE,"out->right != VILLE");

}

Test(all, DLList_pop_two_item_first_item)
{
    struct DLList *db_list=NULL;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *out;

    db_list = DLList_push_end(db_list,tile1);
    db_list = DLList_push_end(db_list,tile2);
    DLList_pop(db_list,&out);

    /*
    ici on veut tester le pop du premier element d'une
    liste doublement chaine composé de 2 element
    */

    /*
    on devrais avoir donc qu'un seul element restant
    d'ou ce test si il y a qu'un element il devrait avoir
    db_list->prev et db_list->next egale a NULL
    */
    cr_assert(db_list->prev == NULL && db_list->next == NULL,"condition sur les element de la db_list non respecter");
    
    //test si la tuile a ete extraite
    cr_assert(out->right == VILLE,"out->right != VILLE !");
}

Test(all, DLList_pop_two_item_second_item)
{
    struct DLList *db_list=NULL;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *out;

    db_list = DLList_push_end(db_list,tile1);
    db_list = DLList_push_end(db_list,tile2);

    /*
    ici on veut tester le pop du deuxieme element d'une
    liste doublement chaine composé de 2 element
    */

   DLList_pop(db_list->next,&out);

    //meme test on va voir si il y a bien un seul element

    cr_assert(db_list->prev == NULL && db_list->next == NULL,"condition sur les element de la db_list non respecter");
    cr_assert(out->right == ROUTE ,"out->right != ROUTE !");

}

Test(all, DLList_pop_three_item_first_item)
{
    struct DLList *db_list=NULL;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *tile3 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, PRE);
    struct Tile *out;

    db_list = DLList_push_end(db_list,tile1);
    db_list = DLList_push_end(db_list,tile2);
    db_list = DLList_push_end(db_list,tile3);
    DLList_pop(db_list,&out);

    cr_assert(db_list->prev == NULL && db_list->next != NULL,"condition sur les element de la db_list non respecter");
    cr_assert(out->middle == PRE,"out->middle != PRE");

}

Test(all, DLList_pop_three_item_second_item)
{
    struct DLList *db_list=NULL;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *tile3 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, PRE);
    struct Tile *out;

    db_list = DLList_push_end(db_list,tile1);
    db_list = DLList_push_end(db_list,tile2);
    db_list = DLList_push_end(db_list,tile3);
    DLList_pop(db_list->next,&out);

    cr_assert(db_list->prev == NULL &&
              db_list->next != NULL &&
              db_list->next->prev != NULL &&
              db_list->next->next == NULL,"condition sur les element de la db_list non respecter");
    
    cr_assert(out->right == ROUTE,"out->right != ROUTE !");
}

Test(all, DLList_pop_three_item_third_item)
{
    struct DLList *db_list=NULL;
    struct Tile *tile1 = init_tile(VILLE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *tile2 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, ROUTE);
    struct Tile *tile3 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, PRE);
    struct Tile *out;

    db_list = DLList_push_end(db_list,tile1);
    db_list = DLList_push_end(db_list,tile2);
    db_list = DLList_push_end(db_list,tile3);
    DLList_pop(db_list->next->next,&out);

    cr_assert(db_list->prev == NULL &&
              db_list->next != NULL &&
              db_list->next->prev != NULL && 
              db_list->next->next == NULL,"condition sur les element de la db_list non respecter");
    
    cr_assert(out->middle == PRE,"out->middle != PRE !");
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
    struct Tile *tile3 = init_tile(ROUTE, ROUTE, ROUTE, VILLE, PRE);
    struct Coord *C1=init_coord(0,0);
    struct Coord *C2=init_coord(1,0);
    struct Coord *C3=init_coord(1,1);

    //Test sur un seul morceau de grid
    struct Grid *G=init_grid(tile1,C1,NULL,NULL,NULL,NULL);
    cr_assert(G->coord->x == 0 && G->coord->y == 0,"Coordonnee incorrect");
    cr_assert(G->bot == NULL &&
              G->left == NULL &&
               G->top == NULL &&
                G->right == NULL ,"Cellule voisine incorrect");
    cr_assert(G->tile->right == VILLE,"Propriete de la tuile incorrect");

    //Test sur une deuxieme insertion
    G->right=init_grid(tile2,C2,NULL,G,NULL,NULL);
    cr_assert(G->right->coord->x == 1 && G->coord->y == 0,"Coordonnee incorrect");
    cr_assert(G->right->bot == NULL &&
              G->right->left == G &&
              G->right->top == NULL &&
              G->right->right == NULL,"Cellule voisine incorrect");
    cr_assert(G->right->tile->right == ROUTE &&
              G->right->tile->left == VILLE ,"Propriete de la tuile incorrect");
    
    


}


//test init_grid
