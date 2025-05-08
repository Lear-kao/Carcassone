#include "../header/Carcassonne.h"

// ------------
// ----Tile----
// ------------

struct Tile *init_tile(enum types right, enum types top, enum types left, enum types bot, enum types middle)
/*
    Arguments
        enum types right : le type de la tuile sur le morceau a droite de la tuile
        enum types top : le type de la tuile sur le morceau en haut de la tuile
        enum types left : le type de la tuile sur le morceau a gauce de la tuile
        enum types bot : le type de la tuile sur le morceau en bas de la tuile
        enum types middle : le type de la tuile sur le morceau au milleu de la tuile

    Retour:
        struct Tile *tile : Un pointeur sur une tuile nouvellement allouer

    Description:
        Alloue puis initialise la tuile avec nos arguments
        (evidemment par defaut il n'y a pas de meeple au debut)
        puis retourne le pointeur sur la tuile nouvellement allouer
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
/*
    Arguments:
        struct Tile *tile : Un pointeur sur une tuile
    
    Retour:
        char 0 ou 1

    Description:
        - 0 si aucun Meeple n'est sur la tile
        - 1 si un Meeple est sur la tile
*/
{
    return tile->meeplePlace != NO_MEEPLE;
}

struct Tile *turn_tile(struct Tile *tile)
/*
    Arguments:
        struct Tile *tile : Un pointeur sur une tuile

    Retour:
        struct Tile *tile : Un pointeur sur une tuile

    Description:
        effectue une rotation de 90° dans le sens trigonometrique de la tuile 
        (la fonction genere une nouvelle tuile avec la rotation appliqué et la retourne)
*/
{
    return init_tile(tile->bot, tile->right, tile->top, tile->left, tile->middle);
}