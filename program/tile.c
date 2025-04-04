#include "../header/Carcassonne.h"

// ------------
// ----Tile----
// ------------

struct Tile *init_tile(enum types right, enum types top, enum types left, enum types bot, enum types middle)
/*
    tile : Un pointer sur Tile
    right, left ... middle : les différent type des bordes de la tuile
*/
{
    struct Tile *tile = malloc(sizeof(struct Tile));
    tile->meeple = NULL;
    tile->meeplePlace = NO_MEEPLE;
    tile->right = right;
    tile->top = top;
    tile->left = left;
    tile->bot = bot;
    tile->middle = middle;
    return tile;
}

char is_meeple_on_tile(struct Tile *tile)
{
    /*
    tile : Une tile

    return :
    - 0 si aucun Meeple n'est sur la tile
    - 1 si un Meeple est sur la tile
    */
    return tile->meeplePlace != NO_MEEPLE;
}

struct Tile *turn_tile(struct Tile *tile)
/*
    tile : Une tuile

    return : Une tuile malloc tourné de 90° dans le sens trigo.
*/
{
    return init_tile(tile->bot, tile->right, tile->top, tile->left, tile->middle);
}