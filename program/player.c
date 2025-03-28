#include "../header/Carcassonne.h"

// ---------------
// ----Player-----
// ---------------

void init_player(struct Player *player,int couleur)
{
    /*
        Innitialise l'objet player
    */
    player->coulPlayer = couleur;  
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

struct list_player *init_player_list() // Axel
/*
    Crée une liste de pointeurs qui pointe sur un Player,
    un pointer sur NULL est ajouté à la fin pour faciliter
    les iterations sur la liste (condition d'arrêt)
*/
{
    struct list_player *list_players = malloc(sizeof(struct list_player));
    list_players->player = malloc(sizeof(struct Player*) * (nbPlayers + 1));
    for( int i = 0; i < nbPlayers; i++ )
    {
        init_player((list_players)->player[i],i);
    }
    list_players->player[nbPlayers] = NULL;
    return list_players;
}


void reset_points(struct Player *player) // Fait
/*
    Réinitialise les points du joueur
*/
{
    player->points = 0;
}

void reset_meeples(struct Player *player) // Fait
/*
    Réinitialise le compteur de Meeple du joueur
*/
{
    player->nbMeeple = NBMEEPLE_DEFAULT;
}