#include "Carcassonne.h"

// ------------
// ----Tile----
// ------------

void init_tile(struct Tile *tile, enum types right, enum types top, enum types left, enum types bot, enum types middle)
{
    /*
    tile : Un pointer vers un espace mémoire déjà alloué à sizeof(struct Tile)
    right, left ... middle : les différent type des bordes de la tuile
     */
    tile->meeple = NO_MEEPLE;
    tile->right = right;
    tile->top = top;
    tile->left = left;
    tile->bot = bot;
    tile->middle = middle;
};

char is_meeple_on_tile(struct Tile *tile)
{
    /*
    tile : Une tile

    return :
    - 0 si aucun Meeple n'est sur la tile
    - 1 si un Meeple est sur la tile
    */
    if(tile->meeple == NO_MEEPLE)
    {
        return 0;
    }
    else
    {
        return 1;
    }
};