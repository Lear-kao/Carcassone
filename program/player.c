#include "../header/Carcassonne.h"

// ---------------
// ----Player-----
// ---------------

void init_player(struct Player *player,int couleur)
{
    /*
        Innitialise l'objet player
    */
    int coulPlayer = couleur;   // git blame O_O
    player->nbMeeple = NBMEEPLE_DEFAULT;
    player->points = 0;
}
char is_meeple_on_player(struct Player *player)
{/*
    Player : L'objet Player

    return :
    - 0 si le joueur n'a aucun Meeple
    - 1 si il reste au moins un Meeple au Joueur
*/
    return player->nbMeeple;

}

